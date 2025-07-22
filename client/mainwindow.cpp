#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // startup();
}

MainWindow::~MainWindow()
{
    delete ui;
}

//reg#id#pass
//auth#id#pass

void MainWindow::startup()
{
    socket = new QTcpSocket(this);

    QObject::connect(socket,&QTcpSocket::readyRead, this, &MainWindow::newReadyRead);

    socket->connectToHost("193.233.254.72",4444);
}

void MainWindow::newReadyRead()
{
    QByteArray data = socket->readAll();

    if(data.startsWith("/authStatus"))
    {
        QString text_data = data.mid(12);
        auth(text_data);
    }
    if(data.startsWith("/regStatus"))
    {
        QString text_data = data.mid(11);

        if(text_data == "Id")
        {
            qDebug() << "This id is already taken";
        }
        else if(text_data == "true")
        {
            qDebug() << "Successful reg";
            reg();
        }
    }

}

void MainWindow::sendData(QString data)
{
    QByteArray sendData = data.toUtf8();
    socket->write(sendData);
}

void MainWindow::on_registration_clicked()
{
    if(socketConnected == 0)
    {
        startup();
        socketConnected++;
    }

    sendData("/reg#"+ui->idText->text()+"#"+ui->passText->text());
    my_id = ui->idText->text();

}


void MainWindow::on_authButton_clicked()
{
    if(socketConnected == 0)
    {
        startup();
        socketConnected++;
    }
    my_id = ui->idText2->text();
    sendData("/auth#"+ui->idText2->text()+"#"+ui->passText2->text());
}

void MainWindow::reg()
{

    ui->stackedWidget->setCurrentIndex(1);

}

void MainWindow::auth(QString data)
{
    if(data == "true")
    {
        qDebug() << "Successful Auth";
        ui->stackedWidget->setCurrentIndex(2);
    }
    else if(data == "false")
    {
        qDebug() << "Error Auth";
    }
}
//"/sendMsg~ОтКого~Кому~Время~Сообщение
void MainWindow::on_send_clicked()
{
    QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm");
    QString data = "/sendMsg~" + my_id + "~" + ui->to_user->text() + "~" + time + "~" + ui->message_2->text();
    qDebug() << "Message sended";
    sendData(data);
}

