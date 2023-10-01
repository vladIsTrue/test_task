#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QString>
#include <QTcpSocket>
#include <QMainWindow>

#include "tablemodeltask.h"
#include "socketconnection.h"
#include "tablemodelemployee.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void initialization();

private slots:
    void handleOnTableClicked(QModelIndex index);
    void on_pushButtonInsertTask_clicked();
    void on_tableViewTask_clicked(const QModelIndex &index);

private:
    void createUI();
    void initializeWindow();

    Ui::MainWindow *ui;

    SocketConnection *m_connectionToServer;
    TableModelEmployee *m_modelEmployee;
    TableModelTask *m_modelTask;

    int _countRows;
};
#endif // MAINWINDOW_H
