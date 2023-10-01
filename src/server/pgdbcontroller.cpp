#include "pgdbcontroller.h"

PgDBController::PgDBController()
{
    m_db = QSqlDatabase::addDatabase(m_dbDriver);
    m_db.setHostName(m_dbHost);
    m_db.setDatabaseName(m_dbName);
    m_db.setUserName(m_dbUserName);
    m_db.setPassword(m_dbPassword);
}

PgDBController::PgDBController(const QString &dbHost
                               , const QString &dbName
                               , const QString &dbUserName
                               , const QString &dbPassword)
    : m_dbHost(dbHost)
    , m_dbUserName(dbUserName)
    , m_dbName(dbName)
    , m_dbPassword(dbPassword)
{
    m_db = QSqlDatabase::addDatabase(m_dbDriver);
    m_db.setHostName(m_dbHost);
    m_db.setDatabaseName(m_dbName);
    m_db.setUserName(m_dbUserName);
    m_db.setPassword(m_dbPassword);
}

PgDBController::~PgDBController()
{
    if (m_db.isOpen()) {
        m_db.close();
    }
}

bool PgDBController::connectToDB()
{
    return m_db.open();
}

void PgDBController::disconnectFromDB()
{
    return m_db.close();
}

bool PgDBController::checkConnection()
{
    return m_db.isOpen();
}
