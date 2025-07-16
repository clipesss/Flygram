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

        User *user = new User(splitedData[0],splitedData[1]);
        users.push_back(user);
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

