#include "TCPHandler.h"
#include <QDebug>

TCPHandler::TCPHandler(QObject *parent) : QObject(parent),
    m_tcpSocket(new QTcpSocket(this)),
    m_udpSocket(new QUdpSocket(this)),
    m_connected(false),
    m_ipAddress("192.168.1.100"), 
    m_port(8888),                 
    m_useTcp(true)                
{
    connect(m_tcpSocket, &QTcpSocket::connected, this, &TCPHandler::onTcpConnected);
    connect(m_tcpSocket, &QTcpSocket::disconnected, this, &TCPHandler::onTcpDisconnected);
    connect(m_tcpSocket, &QTcpSocket::readyRead, this, &TCPHandler::onTcpReadyRead);
    connect(m_tcpSocket, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::errorOccurred),
            this, &TCPHandler::onTcpError);
}

TCPHandler::~TCPHandler()
{
    disconnectFromDevice();
}

bool TCPHandler::connectToDevice()
{
    if (m_useTcp) {
        
        m_tcpSocket->abort();
        m_tcpSocket->connectToHost(m_ipAddress, m_port);
        return true;
    } else {
        
        m_connected = true;
        emit connectionChanged();
        qDebug() << "UDP socket ready for sending to" << m_ipAddress << ":" << m_port;
        return true;
    }
}

void TCPHandler::disconnectFromDevice()
{
    if (m_useTcp) {
        m_tcpSocket->disconnectFromHost();
    } else {
        m_connected = false;
        emit connectionChanged();
        qDebug() << "UDP disconnected";
    }
}

void TCPHandler::sendCommand(const QString &command)
{
    if (m_useTcp) {
        
        if (!m_tcpSocket->isOpen() || !m_tcpSocket->isWritable()) {
            qDebug() << "TCP socket not ready for writing";
            emit errorOccurred("TCP socket not ready");
            return;
        }

        QByteArray data = command.toUtf8() + "\n";
        qint64 bytesWritten = m_tcpSocket->write(data);

        if (bytesWritten == -1) {
            qDebug() << "Failed to write to TCP socket:" << m_tcpSocket->errorString();
            emit errorOccurred(m_tcpSocket->errorString());
        } else {
            m_tcpSocket->flush();
            emit dataSent(command);
            qDebug() << "Command sent via TCP:" << command;
        }
    } else {
        
        QByteArray data = command.toUtf8() + "\n";
        QHostAddress address(m_ipAddress);
        qint64 bytesWritten = m_udpSocket->writeDatagram(data, address, m_port);

        if (bytesWritten == -1) {
            qDebug() << "Failed to send UDP datagram:" << m_udpSocket->errorString();
            emit errorOccurred(m_udpSocket->errorString());
        } else {
            emit dataSent(command);
            qDebug() << "Command sent via UDP:" << command;
        }
    }
}


QString TCPHandler::ipAddress() const { return m_ipAddress; }
void TCPHandler::setIpAddress(const QString &ipAddress) {
    if (m_ipAddress != ipAddress) {
        m_ipAddress = ipAddress;
        emit ipAddressChanged();
    }
}

int TCPHandler::port() const { return m_port; }
void TCPHandler::setPort(int port) {
    if (m_port != port) {
        m_port = port;
        emit portChanged();
    }
}

bool TCPHandler::useTcp() const { return m_useTcp; }
void TCPHandler::setUseTcp(bool useTcp) {
    if (m_useTcp != useTcp) {
        m_useTcp = useTcp;
        emit useTcpChanged();
    }
}

bool TCPHandler::isConnected() const {
    if (m_useTcp) {
        return m_tcpSocket->state() == QAbstractSocket::ConnectedState;
    }
    return m_connected;
}

void TCPHandler::onTcpConnected()
{
    m_connected = true;
    emit connectionChanged();
    qDebug() << "TCP Connected to" << m_ipAddress << ":" << m_port;
}

void TCPHandler::onTcpDisconnected()
{
    m_connected = false;
    emit connectionChanged();
    qDebug() << "TCP Disconnected";
}

void TCPHandler::onTcpReadyRead()
{
    QByteArray data = m_tcpSocket->readAll();
    QString response = QString::fromUtf8(data).trimmed();
    emit dataReceived(response);
    qDebug() << "TCP Received:" << response;
}

void TCPHandler::onTcpError(QAbstractSocket::SocketError error)
{
    QString errorString = m_tcpSocket->errorString();
    qDebug() << "TCP Error:" << errorString;
    emit errorOccurred(errorString);
}
