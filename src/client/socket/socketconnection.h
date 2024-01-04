#ifndef SOCKETCONNECTION_H
#define SOCKETCONNECTION_H

#include <QObject>
#include <QTcpSocket>
#include <QStringList>

class SocketConnection : public QObject
{
    Q_OBJECT
public:
    explicit SocketConnection(QObject *parent = nullptr
                              , const QString &serverHostName = "localhost"
                              , quint16 serverPort = 2323
                              );
    ~SocketConnection();

    QStringList getEmployeeData();
    QStringList getAllTasks(QString employeeName);
    QStringList getExpiredTasks(QString employeeName);

    void insertTask(QString insertData);
    void removeTask(QString removeId);

signals:
    bool done();

private slots:
    void readSocket();
    void discardSocket();

    void sendRequest(qint16 code, const QString &data);
    QByteArray createRequest(qint16 code, const QString &data);
    void handleResponse(qint16 code, QByteArray buffer);

private:
    const QString m_serverHostName;
    const quint16 m_serverPort;
    QStringList   m_responseData;
    QTcpSocket*   m_socket;
};

#endif // SOCKETCONNECTION_H
