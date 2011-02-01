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
        Tp::ContactManagerPtr contactManager = account->connection()->contactManager();

        QList<Tp::ContactPtr> newContacts = contactManager->allKnownContacts().toList();

        //need to add a connection to each contact to emit updated when applicable.

        beginInsertRows(QModelIndex(), 0, newContacts.size());
        m_contacts.append(newContacts);
        endInsertRows();

    }
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
    Tp::Contact* c = qobject_cast<Tp::Contact*>(sender());
    Tp::ContactPtr contact = Tp::ContactPtr(c);

    QModelIndex index = createIndex(m_contacts.lastIndexOf(contact), 0);
    emit dataChanged(index, index);
}

Tp::ContactPtr ContactListModel::contact(const QModelIndex &index) const
{
    return m_contacts.at(index.row());
}





