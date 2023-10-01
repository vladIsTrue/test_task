#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QSqlDatabase>

#include "idbcontroller.h"

class QSettings;

class Server : public QObject
{
    Q_OBJECT

public:
    Server(QObject *parent = nullptr);
    Server(int port
           , const QHostAddress &address
           , const QString &dbHost
           , const QString &dbName
           , const QString &dbUserName
           , const QString &dbPassword
           , QObject *parent = nullptr
           );

    ~Server();

    void run();
private slots:
    void newConnection();
    void appendToSocketList(QTcpSocket *socket);

    void readSocket();
    void discardSocket();
private:
    void sendResponse(QTcpSocket *socket, const QString &data);
    QString execute(QByteArray buffer);
    QByteArray createResponse(QString data);

    QString getEmployeeData();
    QString getAllTasksData(QString buffer);
    QString getExpiredTasksData(QString buffer);
    QString insertTask(QByteArray buffer);
    QString removeTask(QByteArray buffer);

    QHostAddress m_address        {QHostAddress::Any};
    int m_port                    {2323};

    IDBController*      m_dbController;
    QSet<QTcpSocket*>   m_sockets;
    QTcpServer*         m_server;
};

#endif // SERVER_H
