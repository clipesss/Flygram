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
    socket = new QTcpSocket(this);

    QObject::connect(socket,&QTcpSocket::readyRead, this, &MainWindow::newReadyRead);

    socket->connectToHost("192.168.0.193",4444);
}

void MainWindow::newReadyRead()
{

}

void MainWindow::sendData()
{
    QByteArray data = "/reg#1337#pass133777";
    socket->write(data);
}

void MainWindow::on_pushButton_clicked()
{
    sendData();
}

