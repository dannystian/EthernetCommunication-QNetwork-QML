#ifndef TCPHANDLER_H
#define TCPHANDLER_H

#include <QObject>
#include <QTcpSocket>
#include <QUdpSocket>
#include <QHostAddress>

class TCPHandler : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isConnected READ isConnected NOTIFY connectionChanged)
    Q_PROPERTY(QString ipAddress READ ipAddress WRITE setIpAddress NOTIFY ipAddressChanged)
    Q_PROPERTY(int port READ port WRITE setPort NOTIFY portChanged)
    Q_PROPERTY(bool useTcp READ useTcp WRITE setUseTcp NOTIFY useTcpChanged)

public:
    explicit TCPHandler(QObject *parent = nullptr);
    ~TCPHandler();

    Q_INVOKABLE bool connectToDevice();
    Q_INVOKABLE void disconnectFromDevice();
    Q_INVOKABLE void sendCommand(const QString &command);
    bool isConnected() const;

    QString ipAddress() const;
    void setIpAddress(const QString &ipAddress);

    int port() const;
    void setPort(int port);

    bool useTcp() const;
    void setUseTcp(bool useTcp);

signals:
    void connectionChanged();
    void dataSent(const QString &data);
    void dataReceived(const QString &data);
    void errorOccurred(const QString &error);
    void ipAddressChanged();
    void portChanged();
    void useTcpChanged();

private slots:
    void onTcpConnected();
    void onTcpDisconnected();
    void onTcpReadyRead();
    void onTcpError(QAbstractSocket::SocketError error);

private:
    QTcpSocket *m_tcpSocket;
    QUdpSocket *m_udpSocket;
    bool m_connected;
    QString m_ipAddress;
    int m_port;
    bool m_useTcp;
};

#endif // TCPHANDLER_H
