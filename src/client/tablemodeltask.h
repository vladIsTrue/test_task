#ifndef TABLEMODELTASK_H
#define TABLEMODELTASK_H

#include <QAbstractTableModel>
#include <QStringList>

#include "recordInfo.h"

class TableModelTask : public QAbstractTableModel
{
    Q_OBJECT
public:
    TableModelTask(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QVariant data(const QModelIndex &index, int role) const;

    Qt::ItemFlags flags(const QModelIndex &index) const;

    void addRecord(const RecordInfoTask &data);
    bool setData(const QModelIndex &index, const QVariant &value, int role);

    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());

private:
    typedef QHash<ColumnTask, QVariant> RecordData;
    typedef QList<RecordData> Records;
    Records m_records;
};

#endif // TABLEMODELTASK_H
