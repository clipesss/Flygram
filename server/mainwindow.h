#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    class Messages
    {
    public:
        Messages(QString string, QString chatWith)
        {
            this->allHistory += string;
            this->chatWith = chatWith;
        }

        QString GetChatHistory()
        {
            return allHistory;
        }

        void ReWriteChat(QString messages)
        {
            allHistory+=messages;
        }

        QString allHistory;
        QString chatWith;
    };

    class User
    {
    public:

        User(QString id, QString pass, QTcpSocket *socket)
        {
            this->id = id;
            this->pass = pass;
            this->socket = socket;
        }

        void Get()
        {
            qDebug() << "Id: " << id << " Pass: " << pass << " Socket Ip: " << socket->peerAddress();
        }

        QString GetId()
        {
            return id;
        }

        QString GetPass()
        {
            return pass;
        }

        bool isExistChat(QString idUser)
        {
            for(auto it : chats)
            {
                if(it->chatWith == idUser)
                {
                    return true;
                }
            }
            return false;
        }

        void addChat(Messages *chat)
        {
            chats.push_back(chat);
        }

        bool ReWriteChat(QString chat, QString idUser)
        {
            for(auto it : chats)
            {
                if(it->chatWith == id)
                {
                    it->ReWriteChat(chat);
                    return true;
                }
            }
            return false;
        }

        QString GetChatHistory(QString idUser)
        {
            for(auto it : chats)
            {
                if(it->chatWith == idUser)
                {
                    return it->GetChatHistory();
                }
            }
        }

        QString id;
        QString pass;
        QTcpSocket *socket;
        QVector<Messages*> chats;
    };

private slots:
    void startup();
    void newReadyRead();
    void newConnection();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
    QTcpServer *server;
    //QTcpSocket *socket;
    QVector<User*> users;
    QList<QTcpSocket*> clients;

    int chatCheck = 0;
};

#endif // MAINWINDOW_H
