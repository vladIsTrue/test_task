#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QSqlDatabase>

class QSettings;

class Server : public QTcpServer
{
    Q_OBJECT

public:
    Server();
    Server(int port
           , const QHostAddress &address
           , const QString &dbDriver
           , const QString &dbHost
           , const QString &dbName
           , const QString &dbUserName
           , const QString &dbPassword
           );

    ~Server();

    void run();
private:
    QHostAddress _address        {QHostAddress::Any};
    int _port                    {5432};
    QVector <QTcpSocket *> _sockets{};

    QString _dbDriver            {"QPSQL"};
    QString _dbHost              {"localhost"};
    QString _dbName              {"server"};
    QString _dbUserName          {"server"};
    QString _dbPassword          {"server"};
    QSqlDatabase _db             {};
};

#endif // SERVER_H
