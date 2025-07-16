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

        User(QString id, QString pass)
        {
            this->id = id;
            this->pass = pass;

        }

        void Get()
        {
            qDebug() << "Id: " << id << " Pass: " << pass;
        }

        QString id;
        QString pass;
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
