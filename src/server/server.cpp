#include "server.h"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QTcpSocket>
#include <QDataStream>
#include <QSqlQueryModel>

#include "pgdbcontroller.h"
#include "../utils/utils.h"
#include "../utils/requests.h"

Server::Server(QObject *parent)
    : QObject(parent)
    , m_dbController(new PgDBController())
    , m_server(new QTcpServer())
{}

Server::Server(int port
       , const QHostAddress &address
       , const QString &dbHost
       , const QString &dbName
       , const QString &dbUserName
       , const QString &dbPassword
       , QObject *parent
       )
    : QObject(parent)
    , m_port(port)
    , m_address(address)
    , m_dbController(new PgDBController(dbHost,
                                       dbName,
                                       dbUserName,
                                       dbPassword)
                    )
    , m_server(new QTcpServer())
{}

Server::~Server()
{
    foreach (QTcpSocket* socket, m_sockets) {
        socket->close();
        socket->deleteLater();
    }

    m_server->close();
    m_server->deleteLater();

    delete m_dbController;
    delete m_server;
}

void Server::run()
{
    if (m_server->listen(m_address, m_port)) {
        connect(m_server, SIGNAL(newConnection()), this, SLOT(newConnection()));
        LOGI("Server listening "
             << m_address.toString() << " address, "
             << m_port << " port");
    }
    else {
        LOGE("Socket listening error.");
    }

    if (m_dbController->connectToDB()) {
        LOGI("Server opens database.");
    }
    else {
        LOGE("Database open error.");
    }
}

void Server::newConnection()
{
    while (m_server->hasPendingConnections()) {
        appendToSocketList(m_server->nextPendingConnection());
    }
}

void Server::appendToSocketList(QTcpSocket* socket)
{
    m_sockets.insert(socket);

    connect(socket, SIGNAL (readyRead()), this, SLOT (readSocket()));
    connect(socket, SIGNAL (disconnected()), this, SLOT (discardSocket()));

    LOGI("Socket " << socket->socketDescriptor() << " connected");
}

void Server::readSocket()
{
    QTcpSocket* socket = reinterpret_cast<QTcpSocket*>(sender());

    QByteArray buffer;

    QDataStream socketStream(socket);
    socketStream.setVersion(QDataStream::Qt_5_15);

    socketStream.startTransaction();
    socketStream >> buffer;

    if (!socketStream.commitTransaction()) {
        return;
    }

    QString response = execute(buffer);

    sendResponse(socket, response);
}

void Server::discardSocket()
{
    QTcpSocket* socket = reinterpret_cast<QTcpSocket*>(sender());
    QSet<QTcpSocket*>::iterator it = m_sockets.find(socket);
    if (it != m_sockets.end()) {
        LOGI("Client disconnected.");
        m_sockets.remove(*it);
    }

    socket->deleteLater();
}

void Server::sendResponse(QTcpSocket *socket, const QString &data)
{
    if (socket) {
        if (socket->isOpen()) {
            QDataStream socketStream(socket);
            socketStream.setVersion(QDataStream::Qt_5_15);

            socketStream << createResponse(data);

            LOGI("Send data to socket " << socket->socketDescriptor());
        }
        else {
            LOGE("Socket doesn't seem to be opened");
        }
    } else {
        LOGE("Socket not connected");
    }
}

QString Server::execute(QByteArray buffer)
{
    QString header = buffer.mid(0,128);
    int code = header.split(",")[0].split(":")[1].toInt();

    buffer = buffer.mid(128);

    switch (code)
    {
        case requestsCode::GET_EMPLOYEE_DATA:
            return getEmployeeData();
            break;
        case requestsCode::GET_ALL_TASKS: {
            QString DataAsString = QString(buffer);
            return getAllTasksData(DataAsString);
        }
            break;
        case requestsCode::GET_EXPIRED_TASKS: {
            QString DataAsString = QString(buffer);
            return getExpiredTasksData(DataAsString);
        }
        break;
        case requestsCode::INSERT_TASK: {
            return insertTask(buffer);
        }
            break;
        case requestsCode::REMOVE_TASK: {
            return removeTask(buffer);
        }
            break;
    }

    return {};
}

QByteArray Server::createResponse(QString data)
{
    QByteArray header;

    auto code = data.split(":")[0];
    data.remove(0, 2);

    header.prepend(QString("fileType:%1,fileName:null,fileSize:%2;").arg(code).arg(data.size()).toUtf8());
    header.resize(128);

    QByteArray buffer = data.toUtf8();
    buffer.prepend(header);

    return buffer;
}

QString Server::getEmployeeData()
{
    QString response = "0:";
    QSqlQueryModel employees;
    employees.setQuery("SELECT full_name, post, phone_number FROM employees");
    for (int i = 0; i < employees.rowCount(); ++i) {
        response += employees.record(i).value("full_name").toString().trimmed() + ':';
        response += employees.record(i).value("post").toString().trimmed() + ':';
        response += employees.record(i).value("phone_number").toString().trimmed() + ':';
    }
    return response;
}

QString Server::getAllTasksData(QString buffer)
{
    QString response = "1:";
    QSqlQueryModel employees;
    employees.setQuery("SELECT task_id, task_name, deadline, status FROM tasks WHERE executor = '" + buffer + "'");

    for (int i = 0; i < employees.rowCount(); ++i) {
        response += employees.record(i).value("task_id").toString().trimmed() + ':';
        response += employees.record(i).value("task_name").toString().trimmed() + ':';
        response += employees.record(i).value("deadline").toString().trimmed() + ':';
        response += employees.record(i).value("status").toString().trimmed() + ':';
    }

    return response;
}

QString Server::getExpiredTasksData(QString buffer)
{
    QString response = "2:";
    QSqlQueryModel employees;

    const QString dateTime = QDateTime::currentDateTime().toString("dd.MM.yyyy");

    QStringList date = dateTime.split('.');

    employees.setQuery("SELECT task_id, task_name, deadline, status FROM tasks WHERE executor = '" + buffer + "' "
                        "AND deadline < '" + date.at(2) + "-" + date.at(1) + "-" + date.at(0) + "'");

    for (int i = 0; i < employees.rowCount(); ++i) {
        response += employees.record(i).value("task_id").toString().trimmed() + ':';
        response += employees.record(i).value("task_name").toString().trimmed() + ':';
        response += employees.record(i).value("deadline").toString().trimmed() + ':';
        response += employees.record(i).value("status").toString().trimmed() + ':';
    }
    return response;
}

QString Server::insertTask(QByteArray buffer)
{
    QString response = "3:";
    QString status = "open";

    QStringList tasksInformation = QString(buffer).split(':');
    QSqlQuery insertQuery;

    insertQuery.prepare(
        "INSERT INTO tasks (executor, task_name, deadline, status) "
        "VALUES (:executor, :task_name, :deadline, :status)"
        );
    insertQuery.bindValue(":executor", tasksInformation.at(0));
    insertQuery.bindValue(":task_name", tasksInformation.at(1));

    if (tasksInformation.at(2) == "NULL") {
        insertQuery.bindValue(":parent_task",  QVariant());
    } else {
        insertQuery.bindValue(":parent_task", tasksInformation.at(2));
    }

    insertQuery.bindValue(":deadline", tasksInformation.at(3));
    insertQuery.bindValue(":status", status);
    insertQuery.exec();

    response += "200";

    return response;
}

QString Server::removeTask(QByteArray buffer)
{
    QString response = "4:";

    QString tasksInformation = QString(buffer);

    QSqlQuery deleteQuery;
    deleteQuery.prepare("DELETE FROM tasks WHERE task_id = :id");
    deleteQuery.bindValue(":id", tasksInformation);
    deleteQuery.exec();

    response += "200";

    return response;
}
