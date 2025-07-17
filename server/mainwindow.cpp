#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    startup();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::startup()
{
    server = new QTcpServer(this);

    connect(server,&QTcpServer::newConnection,this,&MainWindow::newConnection);

    server->listen(QHostAddress::Any, 4444);
}

void MainWindow::newReadyRead()
{
    QByteArray data = socket->readAll();

    if(data.startsWith("/reg"))
    {
        QString data_text = data.mid(5);
        QStringList splitedData = data_text.split("#");
        int stat = 0;

        User *user = new User(splitedData[0], splitedData[1], socket, "");

        for(auto it : users)
        {
            if(it->GetId() == splitedData[0])
            {
                socket->write("/regStatus#Id");
                stat++;
                break;
            }
        }

        if(stat == 0)
        {
            users.push_back(user);
            socket->write("/regStatus#true");
        }
    }
    if(data.startsWith("/auth"))
    {
        QString data_text = data.mid(6);
        QStringList splitedData = data_text.split("#");
        int stat = 0;

        for(auto &it : users)
        {
            if(it->GetId() == splitedData[0])
            {
                if(it->GetPass() == splitedData[1])
                {
                    User *user = new User(splitedData[0], splitedData[1], socket, "");
                    it = user;
                    socket->write("/authStatus#true");
                    stat = 1;
                    break;
                }
            }
        }
        if(stat == 1)
        {
            stat = 0;
        }
        else
        {
            socket->write("/authStatus#false");
        }
    }
}

void MainWindow::newConnection()
{
    socket = server->nextPendingConnection();

    connect(socket,&QTcpSocket::readyRead,this, &MainWindow::newReadyRead);

    qDebug() << "Connected: " << socket->peerAddress();
}

void MainWindow::on_pushButton_clicked()
{
    for(auto it : users)
    {
        it->Get();
    }
}

