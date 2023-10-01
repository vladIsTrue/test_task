#include "tablemodelemployee.h"

TableModelEmployee::TableModelEmployee(QObject *parent)
    : QAbstractTableModel(parent)
{}

int TableModelEmployee::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent)
    return m_records.count();
}

int TableModelEmployee::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent)
    return LAST_E;
}

QVariant TableModelEmployee::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role != Qt::DisplayRole) {
        return QVariant();
    }

    if (orientation == Qt::Vertical) {
        return section;
    }

    switch(section) {
        case FULL_NAME:
            return "Full name";
        case POST:
            return "Post";
        case PHONE_NUMBER:
            return "Phone number";
        case ALL_TASKS:
            return "All tasks";
        case EXPIRED_TASKS:
            return "Expired tasks";
    }

    return QVariant();
}

QVariant TableModelEmployee::data(const QModelIndex &index, int role) const {
    if (!index.isValid()
        || m_records.count() <= index.row()
        || (role != Qt::DisplayRole && role != Qt::EditRole)
        ) {
        return QVariant();
    }

    return m_records[index.row()][ColumnEmployee(index.column())];
}

Qt::ItemFlags TableModelEmployee::flags(const QModelIndex &index) const {
    Qt::ItemFlags flags = QAbstractTableModel::flags(index);
    return flags;
}

void TableModelEmployee::addRecord(const RecordInfoEmployee &data) {
    RecordData record;

    record[FULL_NAME] =     data.fullName;
    record[PHONE_NUMBER] =  data.phoneNumber;
    record[POST] =          data.post;
    record[ALL_TASKS] =     data.allTasks;
    record[EXPIRED_TASKS] = data.expiredTasks;

    int row = m_records.count();
    beginInsertRows(QModelIndex(), row, row);
    m_records.append(record);
    endInsertRows();
}

bool TableModelEmployee::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (!index.isValid() || role != Qt::EditRole || m_records.count() <= index.row()) {
        return false;
    }

    m_records[index.row()][ColumnEmployee(index.column())] = value;
    emit dataChanged(index, index);

    return true;
}
