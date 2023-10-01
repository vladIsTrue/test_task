#ifndef IDBCONTROLLER_H
#define IDBCONTROLLER_H

#include <QtSql>
#include <QString>

class IDBController
{
public:
    explicit IDBController()
    {}
    virtual ~IDBController() = default;
    virtual bool connectToDB() = 0;
    virtual void disconnectFromDB() = 0;
    virtual bool checkConnection() = 0;
};

#endif // IDBCONTROLLER_H
