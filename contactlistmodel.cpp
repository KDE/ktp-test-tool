#include "contactlistmodel.h"

#include <KIcon>

#include <TelepathyQt4/ContactManager>
#include <TelepathyQt4/Contact>
#include <TelepathyQt4/PendingReady>
#include <TelepathyQt4/PendingContacts>
#include <TelepathyQt4/AvatarData>


ContactListModel::ContactListModel(QObject *parent) :
    QAbstractListModel(parent)
{
}

void ContactListModel::setAccount(Tp::AccountPtr account)
{
    qDebug() << account->displayName();
    if (account->connection()) {
        Tp::PendingReady* op = account->connection()->becomeReady(Tp::Features() << Tp::Connection::FeatureCore
                                                                  <<  Tp::Connection::FeatureRosterGroups
                                                                  << Tp::Connection::FeatureRoster);

        connect(op, SIGNAL(finished(Tp::PendingOperation*)), SLOT(onConnectionReady(Tp::PendingOperation*)));
    }
}

void ContactListModel::onConnectionReady(Tp::PendingOperation *op)
{
    Tp::ConnectionPtr connection = Tp::ConnectionPtr::dynamicCast(op->object());

    Tp::ContactManagerPtr contactManager = connection->contactManager();
    Tp::PendingContacts *pendingContacts = contactManager->upgradeContacts(contactManager->allKnownContacts().toList(),
                                                                           Tp::Features() << Tp::Contact::FeatureAlias << Tp::Contact::FeatureAvatarData << Tp::Contact::FeatureSimplePresence);
    connect(pendingContacts, SIGNAL(finished(Tp::PendingOperation*)), SLOT(onContactsUpgraded(Tp::PendingOperation*)));
}

void ContactListModel::onContactsUpgraded(Tp::PendingOperation *po)
{
    Tp::PendingContacts* pendingContacts = qobject_cast<Tp::PendingContacts*>(po);

    QList<Tp::ContactPtr> newContacts;

    foreach(Tp::ContactPtr contact, pendingContacts->contacts()) {
        newContacts.append(contact);
    }

    beginInsertRows(QModelIndex(), 0, newContacts.size());
    m_contacts.append(newContacts);
    endInsertRows();

}


int ContactListModel::rowCount(const QModelIndex &parent) const
{
    if (parent == QModelIndex()) {
        return m_contacts.size();
    }
    return 0;
}

QVariant ContactListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        qDebug() << "invalid index";
        return QVariant();
    }

    Tp::ContactPtr contact = m_contacts[index.row()];

    switch (role)
    {
    case Qt::DecorationRole:
        if(! contact->isAvatarTokenKnown()) {
            return QVariant(KIcon("im-user"));
        }
        else {
          return QVariant(QIcon(m_contacts[index.row()]->avatarData().fileName));
        }
    case Qt::DisplayRole:
      return QVariant(m_contacts[index.row()]->alias());
    case ContactListModel::PresenceRole:
        return QVariant::fromValue(m_contacts[index.row()]->presence().type());
    }

    return QVariant();
}

void ContactListModel::onContactUpdated()
{
    //get contact from sender
    //emit modelupdated..
}





