#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ClientSocket.h"
#include "QHBoxLayout"

#include <QStackedWidget>
#include <QPushButton>
#include <QToolButton>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>

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
    void onWizzClicked();
    void connectButtonToPage(QAbstractButton* button, int pageIndex);
    void onLoginClicked();
    void onSigninClicked();

private:
    Ui::MainWindow *ui;
    ClientSocket *clientSocket;
    QStackedWidget *stackedWidget;
    void connectButtonToChatInterface();
    QString username;
};

#endif // MAINWINDOW_H
