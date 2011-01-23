#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

#include <TelepathyQt4/AccountManager>

class AccountsListModel;

namespace Ui {
    class MainWindow;
}

namespace Tp {
    class PendingOperation;
}

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


private Q_SLOTS:
    void onAccountManagerReady(Tp::PendingOperation *op);
    void onChannelJoined(Tp::PendingOperation *op);
    void onConnectClicked();

private:
    Ui::MainWindow *ui;
    Tp::AccountManagerPtr m_accountManager;
    AccountsListModel *m_accountsListModel;

};

#endif // MAINWINDOW_H
