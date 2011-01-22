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

private:
    Ui::MainWindow *ui;
    Tp::AccountManagerPtr m_accountManager;
    AccountsListModel *m_accountsListModel;



private Q_SLOTS:
    void onAccountManagerReady(Tp::PendingOperation *op);
};

#endif // MAINWINDOW_H
