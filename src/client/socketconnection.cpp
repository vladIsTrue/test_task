#include "socketconnection.h"

#include <QEventLoop>

#include "../utils/requests.h"
#include "../utils/utils.h"

SocketConnection::SocketConnection(QObject *parent, const QString &serverHostName, quint16 serverPort)
    : QObject(parent)
    , m_serverHostName(serverHostName)
    , m_serverPort(serverPort)
    , m_socket(new QTcpSocket(this))
{
    connect(m_socket, SIGNAL(readyRead()), this, SLOT(readSocket()));
    connect(m_socket, SIGNAL(disconnected()), this, SLOT(discardSocket()));

    m_socket->connectToHost(m_serverHostName, m_serverPort);
}

SocketConnection::~SocketConnection()
{
    if (m_socket->isOpen()) {
        m_socket->close();
    }

    delete m_socket;
}

QStringList SocketConnection::getEmployeeData()
{
    sendRequest(requestsCode::GET_EMPLOYEE_DATA, "");

    QEventLoop event_loop;
    connect(this, SIGNAL(done()), &event_loop, SLOT(quit()));
    event_loop.exec();

    return m_responseData;
}

QStringList SocketConnection::getAllTasks(QString employeeName)
{
    sendRequest(requestsCode::GET_ALL_TASKS, employeeName);

    QEventLoop event_loop;
    connect(this, SIGNAL(done()), &event_loop, SLOT(quit()));
    event_loop.exec();

    return m_responseData;
}

QStringList SocketConnection::getExpiredTasks (QString employeeName)
{
    sendRequest(requestsCode::GET_EXPIRED_TASKS, employeeName);

    QEventLoop event_loop;
    connect(this, SIGNAL(done()), &event_loop, SLOT(quit()));
    event_loop.exec();

    return m_responseData;
}

void SocketConnection::insertTask(QString insertData)
{
    sendRequest(requestsCode::INSERT_TASK, insertData);
}

void SocketConnection::removeTask(QString removeId)
{
    sendRequest(requestsCode::REMOVE_TASK, removeId);
}

void SocketConnection::readSocket()
{
    QByteArray buffer;

    QDataStream socketStream(m_socket);
    socketStream.setVersion(QDataStream::Qt_5_15);

    socketStream.startTransaction();
    socketStream >> buffer;

    if (!socketStream.commitTransaction()) {
        return;
    }

    QString header = buffer.mid(0,128);
    buffer = buffer.mid(128);
    int code = header.split(",")[0].split(":")[1].toInt();

    handleResponse(code, buffer);
}

void SocketConnection::discardSocket()
{
    m_socket->deleteLater();
    m_socket = nullptr;
}

void SocketConnection::sendRequest(qint16 code, const QString &data)
{
    if (m_socket) {
        if (m_socket->isOpen()) {
            QDataStream socketStream(m_socket);
            socketStream.setVersion(QDataStream::Qt_5_15);

            socketStream << createRequest(code, data);
        }
    }
}

QByteArray SocketConnection::createRequest(qint16 code, QString data)
{
    QByteArray header;

    header.prepend(QString("fileType:%1,fileName:null,fileSize:%2;").arg(code).arg(data.size()).toUtf8());
    header.resize(128);

    QByteArray buffer = data.toUtf8();
    buffer.prepend(header);

    return buffer;
}

void SocketConnection::handleResponse(qint16 code, QByteArray buffer)
{
    switch (code) {
        case 0:
        case 1:
        case 2: {
            m_responseData.clear();
            QString BufferAsString = QString(buffer);
            m_responseData = BufferAsString.split(':');

            emit done();
        }
            break;
        case 3: {
            LOGI("Insert status: " << QString(buffer));
        }
            break;
        case 4: {
            LOGI("Remove status: " << QString(buffer));
        }
            break;
    }
}
