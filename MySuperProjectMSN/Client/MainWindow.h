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
#include <QStandardItemModel>
#include <QJsonArray>

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
    void onMessageReceived(const QJsonObject &object);
    void onConnectionEstablished();
    void onConnectionClosed();
    void onErrorOccurred(const QString &error);
    void onWizzClicked();
    void connectButtonToPage(QAbstractButton* button, int pageIndex);
    void onLoginClicked();
    void onSigninClicked();
    void onEyePasswordReleased(QToolButton* eyeButton);
    void onEyePasswordPressed(QToolButton* eyeButton);
    void onEyePen();
    void editUserInfo(QPushButton *penButton);
    void displayInvalidPasswordEmail();
    void displayAllUserInfo(const QJsonObject &object);
private:
    Ui::MainWindow *ui;
    ClientSocket *clientSocket;
    QStackedWidget *stackedWidget;
    void connectButtonToChatInterface();
    QString username;
    bool isPasswordVisible = false;
    void displayInfoUserProfile();
    QStandardItem* createMessageItem(const QString &message, bool sent);
};

#endif // MAINWINDOW_H
