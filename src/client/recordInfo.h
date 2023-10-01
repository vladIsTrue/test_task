#ifndef RECORDINFO_H
#define RECORDINFO_H

#include <QString>

enum ColumnEmployee {
    FULL_NAME = 0,
    POST,
    PHONE_NUMBER,
    ALL_TASKS,
    EXPIRED_TASKS,
    LAST_E
};

enum ColumnTask {
    TASK_ID = 0,
    TASK_NAME,
    DEADLINE,
    STATUS,
    LAST_T
};

struct RecordInfoEmployee
{
    QString fullName;
    QString post;
    QString phoneNumber;
    QString allTasks = {"->"};
    QString expiredTasks = {"->"};
};

struct RecordInfoTask
{
    QString taskId;
    QString taskName;
    QString deadline;
    QString status;
};

#endif // RECORDINFO_H
