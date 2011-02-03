#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "accounts-list-model.h"
#include "account-item.h"
#include "contactlistmodel.h"
#include "contactmodelfilter.h"

#include <TelepathyQt4/PendingReady>
#include <TelepathyQt4/PendingChannelRequest>

#include <KDebug>

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    Tp::registerTypes();


    kDebug() << "This is NOT a fully feature complete contact list, or a product that should ever be shipped. It is for development testing only, use at your own risk.";

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
    m_contactModelFilter = new ContactModelFilter(this);
    m_contactModelFilter->setSourceModel(m_contactListModel);
    m_contactModelFilter->sort(0, Qt::AscendingOrder);
    m_contactModelFilter->setDynamicSortFilter(true);
    ui->listView->setModel(m_contactModelFilter);

    connect(ui->connectButton, SIGNAL(released()), SLOT(onConnectClicked()));
    connect(ui->accountCombo, SIGNAL(currentIndexChanged(int)),SLOT(onAccountSelectionChanaged()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onAccountManagerReady(Tp::PendingOperation *op)
{
    qDebug() << "account manager ready";
    // Add all the accounts to the Accounts Model.
    QList<Tp::AccountPtr> accounts = m_accountManager->allAccounts();
    foreach (Tp::AccountPtr account, accounts) {
        m_accountsListModel->addAccount(account);
    }
    onAccountSelectionChanaged();
}


void MainWindow::onAccountSelectionChanaged()
{
    QModelIndex index = m_accountsListModel->index(ui->accountCombo->currentIndex(),0);
    Tp::AccountPtr account = m_accountsListModel->itemForIndex(index)->account();
    m_contactListModel->setAccount(account);
}


//FIXME this will be a menu button and have a slot for start text chat, audio video, tubes and stuff.
void MainWindow::onConnectClicked()
{
    QModelIndex index = ui->listView->currentIndex();
    Tp::ContactPtr contact = m_contactListModel->contact(m_contactModelFilter->mapToSource(index));
    Tp::AccountPtr account = m_contactListModel->account();

    Tp::PendingChannelRequest* channelRequest = account->ensureTextChat(contact);
    connect(channelRequest, SIGNAL(finished(Tp::PendingOperation*)), SLOT(onChannelJoined(Tp::PendingOperation*)));
}

void MainWindow::onChannelJoined(Tp::PendingOperation *op)
{
    if (op->isError()) {
        qDebug() << op->errorName();
        qDebug() << op->errorMessage();
    }
}
