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
    class User
    {
    public:

        User(QString id, QString pass, QTcpSocket *socket, QString messages)
        {
            this->id = id;
            this->pass = pass;
            this->socket = socket;
            this->messages = messages;
        }

        void Get()
        {
            qDebug() << "Id: " << id << " Pass: " << pass << " Socket Ip: " << socket->peerAddress() << " Messages size: " << messages.size();
        }

        QString GetId()
        {
            return id;
        }

        QString GetPass()
        {
            return pass;
        }

        QString id;
        QString pass;
        QTcpSocket *socket;
        QString messages;
    };

private slots:
    void startup();
    void newReadyRead();
    void newConnection();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    QTcpServer *server;
    QTcpSocket *socket;
    QVector<User*> users;

};

#endif // MAINWINDOW_H
