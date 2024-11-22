#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ClientSocket.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onSendButtonClicked();
    void onMessageReceived(const QString &message);
    void onConnectionEstablished();
    void onConnectionClosed();
    void onErrorOccurred(const QString &error);

private:
    Ui::MainWindow *ui;
    ClientSocket *clientSocket;
};

#endif // MAINWINDOW_H
