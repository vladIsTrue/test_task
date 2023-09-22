#include "server.h"

#include "utils.h"

Server::Server()
{}

Server::Server(int port
       , const QHostAddress &address
       , const QString &dbDriver
       , const QString &dbHost
       , const QString &dbName
       , const QString &dbUserName
       , const QString &dbPassword
       )
    : _port(port)
    , _address(address)
    , _dbDriver(dbDriver)
    , _dbHost(dbHost)
    , _dbUserName(dbUserName)
    , _dbName(dbName)
    , _dbPassword(dbPassword)
{}

Server::~Server()
{}

void Server::run()
{
}
