#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "accounts-list-model.h"
#include "account-item.h"
#include "contactlistmodel.h"
#include "contactmodelfilter.h"

#include <TelepathyQt4/PendingReady>
#include <TelepathyQt4/PendingChannelRequest>

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    Tp::registerTypes();

    ui->setupUi(this);
    ui->cancelButton->setIcon(KIcon("dialog-cancel"));
    ui->connectButton->setIcon(KIcon("dialog-ok"));

    // Start setting up the Telepathy AccountManager.
    Tp::AccountFactoryPtr  accountFactory = Tp::AccountFactory::create(QDBusConnection::sessionBus(),
                                                                        Tp::Features() << Tp::Account::FeatureCore
                                                                        << Tp::Account::FeatureAvatar
                                                                        << Tp::Account::FeatureProtocolInfo
                                                                        << Tp::Account::FeatureProfile);

    Tp::ConnectionFactoryPtr connectionFactory = Tp::ConnectionFactory::create(QDBusConnection::sessionBus(),
                                                                              Tp::Features() << Tp::Connection::FeatureCore
                                                                              <<  Tp::Connection::FeatureRosterGroups
                                                                              << Tp::Connection::FeatureRoster);

    Tp::ContactFactoryPtr contactFactory = Tp::ContactFactory::create(Tp::Features()  << Tp::Contact::FeatureAlias
                                                                      << Tp::Contact::FeatureAvatarData
                                                                      << Tp::Contact::FeatureSimplePresence);

    Tp::ChannelFactoryPtr channelFactory = Tp::ChannelFactory::create(QDBusConnection::sessionBus());

    m_accountManager = Tp::AccountManager::create(QDBusConnection::sessionBus(), accountFactory, connectionFactory, channelFactory, contactFactory);

    connect(m_accountManager->becomeReady(),
             SIGNAL(finished(Tp::PendingOperation*)),
             SLOT(onAccountManagerReady(Tp::PendingOperation*)));

    m_accountsListModel = new AccountsListModel(this);
    ui->accountCombo->setModel(m_accountsListModel);


    m_contactListModel = new ContactListModel(this);
    ContactModelFilter* filter = new ContactModelFilter(this);
    filter->setSourceModel(m_contactListModel);
    filter->sort(0, Qt::AscendingOrder);
    filter->setDynamicSortFilter(true);
    ui->listView->setModel(filter);

    connect(ui->connectButton, SIGNAL(released()), SLOT(onConnectClicked()));
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
    m_contactListModel->setAccount(accounts[0]);
}

void MainWindow::onConnectClicked()
{
    QModelIndex index = ui->listView->currentIndex();
    Tp::ContactPtr contact = m_contactListModel->contact(index);
    Tp::Account account =
            Tp::PendingChannelRequest* channelRequest = account->ensureTextChat(contact);


//    int currentIndex = ui->accountCombo->currentIndex();
//    AccountItem* accountItem = m_accountsListModel->itemForIndex(m_accountsListModel->index(currentIndex, 0));
//    if (accountItem) {
//        Tp::PendingChannelRequest* channelRequest = accountItem->account()->ensureTextChatroom(ui->channelEdit->text());
//        connect(channelRequest, SIGNAL(finished(Tp::PendingOperation*)), SLOT(onChannelJoined(Tp::PendingOperation*)));
//    }
}

void MainWindow::onChannelJoined(Tp::PendingOperation *op)
{
    if (op->isError()) {
        qDebug() << op->errorName();
        qDebug() << op->errorMessage();
    }
    else {
        close();
    }
}
