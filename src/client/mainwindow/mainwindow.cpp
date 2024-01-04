#include "../mainwindow/mainwindow.h"
#include "./ui_mainwindow.h"

#include "../info/recordinfo.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_connectionToServer(new SocketConnection(this, "localhost", 2323))
{
    ui->setupUi(this);
    this->setWindowTitle("Task tracker");
    this->createUI();
    initializeWindow();

    connect(ui->tableViewEmployee, SIGNAL(clicked(QModelIndex)), this, SLOT(handleOnTableClicked(QModelIndex)));
}

MainWindow::~MainWindow()
{
    delete m_connectionToServer;
    delete m_modelEmployee;
    delete m_modelTask;
    delete ui;
}

void MainWindow::createUI()
{
    ui->tableViewEmployee->setModel(m_modelEmployee = new TableModelEmployee);
    ui->tableViewEmployee->resizeColumnsToContents();
    ui->tableViewEmployee->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->tableViewEmployee->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->tableViewEmployee->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->tableViewEmployee->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->tableViewEmployee->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);

    ui->tableViewTask->setModel(m_modelTask= new TableModelTask);
    ui->tableViewTask->resizeColumnsToContents();
    ui->tableViewTask->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->tableViewTask->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->tableViewTask->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->tableViewTask->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->tableViewTask->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);

}

void MainWindow::initializeWindow()
{
    auto buff = m_connectionToServer->getEmployeeData();

    for (int i = 0; i < buff.size() - 1; i +=3) {
        RecordInfoEmployee record{buff.at(i), buff.at(i + 1), buff.at(i + 2)};
        m_modelEmployee->addRecord(record);
    }
}

void MainWindow::handleOnTableClicked(QModelIndex index)
{
    if (index.isValid()) {
        QStringList buff{};
        QModelIndex current = ui->tableViewEmployee->currentIndex();
        QString fullName = ui->
                           tableViewEmployee->
                           model()->
                           data(ui->tableViewEmployee->model()->index(current.row(),0),0).toString();

        m_modelTask->removeRows(0, m_countRows);

        switch (index.column()) {
            case  ColumnEmployee::ALL_TASKS:
                buff = m_connectionToServer->getAllTasks(fullName);
                break;
            case  ColumnEmployee::EXPIRED_TASKS:
                buff = m_connectionToServer->getExpiredTasks(fullName);
                break;
        }

        m_countRows = (buff.size() - 1) / 4;

        for (int i = 0; i < buff.size() - 1; i +=4) {
            RecordInfoTask record{buff.at(i), buff.at(i + 1), buff.at(i + 2), buff.at(i + 3)};
            m_modelTask->addRecord(record);
        }
    }
}

void MainWindow::on_pushButtonInsertTask_clicked()
{
    QString insertData = "";
    insertData += ui->lineEdiExecutor->text() + ":";
    insertData += ui->lineEditTaskName->text() + ":";
    insertData += ui->lineEditParentTask->text() + ":";
    insertData += ui->lineEditDeadline->text();

    ui->lineEdiExecutor->clear();
    ui->lineEditTaskName->clear();
    ui->lineEditParentTask->clear();
    ui->lineEditDeadline->clear();

    m_connectionToServer->insertTask(insertData);
}

void MainWindow::on_tableViewTask_clicked(const QModelIndex &index)
{
    if (index.isValid()) {
        if (index.column() == ColumnTask::TASK_ID) {
            QModelIndex current = ui->tableViewTask->currentIndex();
            QString id = ui->tableViewTask->model()->data(ui->tableViewTask->model()->index(current.row(),0),0).toString();

            m_connectionToServer->removeTask(id);
        }
    }
}

