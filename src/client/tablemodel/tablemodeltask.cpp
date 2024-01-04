#include "tablemodeltask.h"

#include "../../utils/utils.h"

TableModelTask::TableModelTask(QObject* parent)
    : QAbstractTableModel(parent)
{}

int TableModelTask::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent)
    return m_records.count();
}

int TableModelTask::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent)
    return LAST_T;
}

QVariant TableModelTask::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role != Qt::DisplayRole) {
        return QVariant();
    }

    if (orientation == Qt::Vertical) {
        return section;
    }

    switch(section) {
        case TASK_ID:
            return "ID";
        case TASK_NAME:
            return "Task name";
        case DEADLINE:
            return "Deadline";
        case STATUS:
            return "Status";
    }

    return QVariant();
}

QVariant TableModelTask::data(const QModelIndex &index, int role) const {
    if (!index.isValid()
        || m_records.count() <= index.row()
        || ( role != Qt::DisplayRole && role != Qt::EditRole )
        ) {
        return QVariant();
    }

    return m_records[index.row()][ColumnTask(index.column())];
}

Qt::ItemFlags TableModelTask::flags(const QModelIndex &index) const {
    Qt::ItemFlags flags = QAbstractTableModel::flags(index);
    return flags;
}

void TableModelTask::addRecord(const RecordInfoTask &data) {
    RecordData record;

    record[TASK_ID] =   data.taskId;
    record[TASK_NAME] = data.taskName;
    record[DEADLINE] =  data.deadline;
    record[STATUS] =    data.status;

    int row = m_records.count();
    beginInsertRows(QModelIndex(), row, row);
    m_records.append(record);
    endInsertRows();
}

bool TableModelTask::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (!index.isValid() || role != Qt::EditRole || m_records.count() <= index.row()) {
        return false;
    }

    m_records[index.row()][ColumnTask(index.column())] = value;
    emit dataChanged(index, index);

    return true;
}

bool TableModelTask::removeRows(int row, int count, const QModelIndex &parent)
{
    if ((m_records.isEmpty())
        || (row >= m_records.size())
        || ((row + count) > m_records.size())) {
        return false;
    }

    beginRemoveRows(parent, row, row + count - 1);
    while (count-- > 0) {
        m_records.removeAt(row);
    }
    endRemoveRows();

    return true;
}
