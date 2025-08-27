import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Window 2.12
import communication 1.0 

ApplicationWindow
{
    visible: true
    width: 600
    height: 400
    title: "Ethernet Communication"
    TCPHandler 
    {
        id: networkHandler
        ipAddress: "192.168.1.100"  
        port: 8888
        useTcp: true  
        onConnectionChanged: 
        {
            console.log("Network status: " + (isConnected ? "Connected" : "Disconnected"))
        }
        onDataSent: 
        {
            console.log("Data sent: " + data)
        }
        onDataReceived: 
        {
            console.log("Data received: " + data)
        }
        onErrorOccurred: 
        {
            console.log("Network error: " + error)
        }
    }
    function sendNetworkCommand(command) 
    {
        console.log("Mengirim perintah: " + command)
        if (networkHandler.isConnected) 
        {
            networkHandler.sendCommand(command)
        } 
        else 
        {
            console.log("Network tidak terkoneksi, mencoba menghubungkan...")
            if (networkHandler.connectToDevice()) 
            {
                timerSendCommand.command = command
                timerSendCommand.start()
            } 
            else 
            {
                console.log("Gagal menghubungkan ke network")
            }
        }
    }
    Component.onCompleted: 
    {
        console.log("Menghubungkan ke " + networkHandler.ipAddress + ":" + networkHandler.port)
        networkHandler.connectToDevice()
    }
    Timer 
    {
        id: timerSendCommand
        interval: 500
        property string command
        onTriggered: 
        {
            if (networkHandler.isConnected) 
            {
                networkHandler.sendCommand(command)
            }
        }
    }
    Item
    {
        id: buttonGroup2
        width: 120
        height: 30
        property int activeButton: 1  
        Rectangle
        {
            id: toggleStateButton1
            y: 121
            x: 8
            radius: 4
            width: 32
            height: 20
            color: buttonGroup2.activeButton === 1 ? "#01579b" : "#0277bd"
            Text
            {
                anchors.centerIn: parent
                text: "Low"
                color: "white"
                font.pixelSize: 10
            }
            MouseArea
            {
                anchors.fill: parent
                onClicked:
                {
                     buttonGroup2.activeButton = 1
                     sendNetworkCommand("MODE:Low")
                     console.log("Low")
                }
            }
        }
        Rectangle
        {
            id: toggleStateButton2
            y: 121
            x: 60
            radius: 4
            width: 32
            height: 20
            color: buttonGroup2.activeButton === 2 ? "#01579b" : "#0277bd"
            Text
            {
                 anchors.centerIn: parent
                 text: "High"
                 color: "white"
                 font.pixelSize: 10
            }
            MouseArea
            {
                anchors.fill: parent
                onClicked:
                {
                    buttonGroup2.activeButton = 2
                    console.log("High")
                }
            }
        }                       
        Rectangle
        {
            id: toggleStateButton3
            y: 121
            x: 36
            width: 28
            height: 20
            color: buttonGroup2.activeButton === 3 ? "#01579b" : "#0277bd"
            Text
            {
                anchors.centerIn: parent
                text: "Mid"
                color: "white"
                font.pixelSize: 10
            }
            MouseArea
            {
                anchors.fill: parent
                onClicked:
                {
                    buttonGroup2.activeButton = 3
                    console.log("Mid")
                }
            }
        }
    }
