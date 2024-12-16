#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>

#include <QLineEdit>
#include <QPushButton>
#include<QWidget>
#include<QVBoxLayout>
#include <QScrollBar>


#include "header/Client.h"

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
    void onReadyRead();
    void displayMessage(const QString &message);
    void sendMessage();

private:
    Ui::MainWindow *ui;
    QTcpSocket *socket;
    Client *client;
    QLineEdit *messageLineEdit;
    QPushButton *sendButton;
    QVBoxLayout *ConversationFrameLayout;
    QFrame *ConversationFrame;
    QScrollBar ScrollBar;
};

#endif // MAINWINDOW_H
