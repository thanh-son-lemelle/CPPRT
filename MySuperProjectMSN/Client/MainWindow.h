#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QTimer>
#include <QLabel>

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

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void onSendButtonClicked();
    void onReadyRead();
    void onDisconnected();
    void onErrorOccurred(QAbstractSocket::SocketError socketError);
    void attemptReconnect();


private:
    Ui::MainWindow *ui;
    QTcpSocket *socket;
    QTimer *reconnectTimer;
    int reconnectInterval =6000; // this is ms
    QLabel *statusLabel;
    void setupStatusBar();
};

#endif // MAINWINDOW_H
