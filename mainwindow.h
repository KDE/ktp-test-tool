#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

#include <TelepathyQt4/AccountManager>

class AccountsListModel;
class ContactListModel;
class ContactModelFilter;

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
    void onAccountSelectionChanaged();
    void onChannelJoined(Tp::PendingOperation *op);
    void startTextChannel();
    void startAudioChannel();
    void startVideoChannel();


private:
    Ui::MainWindow *ui;
    Tp::AccountManagerPtr m_accountManager;

    AccountsListModel *m_accountsListModel;
    ContactListModel *m_contactListModel;
    ContactModelFilter *m_contactModelFilter;

};

#endif // MAINWINDOW_H
