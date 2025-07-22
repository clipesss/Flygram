#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QDateTime>

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

private slots:
    void startup();
    void newReadyRead();
    void sendData(QString data);
    void auth(QString data);
    void reg();

    void on_registration_clicked();

    void on_authButton_clicked();

    void on_send_clicked();

private:
    Ui::MainWindow *ui;
    QTcpSocket *socket;
    QString my_id;
    int socketConnected = 0;

};
#endif // MAINWINDOW_H
