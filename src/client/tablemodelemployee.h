#ifndef TABLEMODELEMPLOYEE_H
#define TABLEMODELEMPLOYEE_H

#include <QAbstractTableModel>
#include <QStringList>

#include "recordInfo.h"

class TableModelEmployee : public QAbstractTableModel
{
    Q_OBJECT
public:
    TableModelEmployee(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QVariant data(const QModelIndex &index, int role) const;

    Qt::ItemFlags flags(const QModelIndex &index) const;

    void addRecord(const RecordInfoEmployee &data);
    bool setData(const QModelIndex &index, const QVariant &value, int role);

private:
    typedef QHash<ColumnEmployee, QVariant> RecordData;
    typedef QList<RecordData> Records;
    Records m_records;
};

#endif // TABLEMODELEMPLOYEE_H
