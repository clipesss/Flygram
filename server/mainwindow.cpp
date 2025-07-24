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
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());

    QByteArray data = socket->readAll();

    if(data.startsWith("/reg"))
    {
        QString data_text = data.mid(5);
        QStringList splitedData = data_text.split("#");
        int stat = 0;

        User *user = new User(splitedData[0], splitedData[1], socket);

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
                    it->setNewData(splitedData[0], splitedData[1], socket);
                    QString chats_users;

                    qDebug() << it->chats.size() << "777" << it->GetId();
                    for(auto it : it->chats)
                    {
                        qDebug() << 777;
                        qDebug() << it->chatWith;
                        chats_users += "~" + it->chatWith;
                        //qDebug() << chats_users;
                    }


                    QString temp = "/authStatus#true" + chats_users;
                    qDebug() << temp;
                    socket->write(temp.toUtf8());
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
    if(data.startsWith("/sendMsg"))
    {
        QString text_data = data.mid(9);
        QStringList listed_data = text_data.split("~");
        QString constructed_data = "~" + listed_data[0] + "~" + listed_data[2] + "~" + listed_data[3] + "~" + listed_data[1];
        qDebug() << "Constr Data: " << constructed_data;
        for(auto it : users)
        {
            if(it->GetId() == listed_data[1])
            {
                if(it->isExistChat(listed_data[0]))
                {
                    //Messages *message = new Messages(constructed_data, listed_data[0]);
                    it->ReWriteChat(constructed_data,listed_data[0]);

                    for(auto it : users)
                    {
                        if(it->GetId() == listed_data[0])
                        {
                            if(it->isExistChat(listed_data[1]))
                            {
                                //Messages *message = new Messages(constructed_data, listed_data[1]);
                                it->ReWriteChat(constructed_data,listed_data[1]);
                                qDebug() << "successfuly";
                                chatCheck = 1;
                            }
                            break;
                        }
                    }
                }
                break;
            }
        }

        if(chatCheck == 0)
        {
            qDebug() << "Chat Check = 0";
            for(auto it : users)
            {
                if(it->GetId() == listed_data[0])
                {
                    Messages *messages = new Messages(constructed_data, listed_data[1]);
                    it->addChat(messages);
                    qDebug() << "Created chat for " << listed_data[0] << " With " << listed_data[1];

                }
            }
            for(auto it : users)
            {
                if(it->GetId() == listed_data[1])
                {
                    Messages *messages = new Messages(constructed_data, listed_data[0]);
                    it->addChat(messages);
                    qDebug() << "Created chat for " << listed_data[1] << " With " << listed_data[0];

                }
            }
        }

        chatCheck = 0;
    }
}

void MainWindow::newConnection()
{
    QTcpSocket* clientSocket = server->nextPendingConnection();

    clients.append(clientSocket);

    connect(clientSocket, &QTcpSocket::readyRead, this, &MainWindow::newReadyRead);
    connect(clientSocket, &QTcpSocket::disconnected, this, [this, clientSocket]() {
        clientSocket->deleteLater();
    });
    qDebug() << "Connected: " << clientSocket->peerAddress();
}

void MainWindow::on_pushButton_clicked()
{
    for(auto it : users)
    {
        it->Get();
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    for(auto it : users)
    {
        if(it->GetId() == "vipuser1337")
        {
            if(it->isExistChat("vipuser228"))
            {
                qDebug() << 1;
                qDebug() << it->GetChatHistory("vipuser228");
            }
        }
    }

    for(auto it : users)
    {
        if(it->GetId() == "vipuser228")
        {
            if(it->isExistChat("vipuser1337"))
            {
                qDebug() << 2;
                qDebug() << it->GetChatHistory("vipuser1337");
            }
        }
    }
}


void MainWindow::on_pushButton_3_clicked()
{
    for(auto it : users)
    {
        if(it->GetId() == "vipuser1337")
        {
            qDebug() << it->chats.size();
            for(auto it : it->chats)
            {
                qDebug() << it->chatWith;
            }
        }
    }

    for(auto it : users)
    {
        if(it->GetId() == "vipuser228")
        {
            qDebug() << it->chats.size();
            for(auto it : it->chats)
            {
                qDebug() << it->chatWith;
            }
        }
    }
}

