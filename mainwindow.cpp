#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "accounts-list-model.h"

#include <TelepathyQt4/PendingReady>


MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->cancelButton->setIcon(KIcon("dialog-cancel"));
    ui->connectButton->setIcon(KIcon("dialog-ok"));

    // Start setting up the Telepathy AccountManager.
     Tp::AccountFactoryPtr  accountFactory = Tp::AccountFactory::create(QDBusConnection::sessionBus(),
                                                                        Tp::Features() << Tp::Account::FeatureCore
                                                                        << Tp::Account::FeatureAvatar
                                                                        << Tp::Account::FeatureProtocolInfo
                                                                        << Tp::Account::FeatureProfile);

     m_accountManager = Tp::AccountManager::create(accountFactory);

     connect(m_accountManager->becomeReady(),
             SIGNAL(finished(Tp::PendingOperation*)),
             SLOT(onAccountManagerReady(Tp::PendingOperation*)));

     m_accountsListModel = new AccountsListModel(this);
     ui->accountCombo->setModel(m_accountsListModel);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onAccountManagerReady(Tp::PendingOperation *op)
{
    // Add all the accounts to the Accounts Model.
    QList<Tp::AccountPtr> accounts = m_accountManager->allAccounts();
    foreach (Tp::AccountPtr account, accounts) {
        m_accountsListModel->addAccount(account);
    }
}
