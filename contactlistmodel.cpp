/*
 * This file is part of kde-telepathy-test-tool
 *
 * Copyright (C) 2011 David Edmundson <kde@davidedmundson.co.uk>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

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

Tp::AccountPtr ContactListModel::account() const
{
    return m_account;
}

void ContactListModel::setAccount(Tp::AccountPtr account)
{
    m_account = account;
    clear();

    //FIXME move this to a "loadContacts" method, which is also run if accountConnectionState changes
    if (account->connectionStatus() == Tp::ConnectionStatusConnected && account->connection()) {
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
    if (! index.isValid()) {
        return Tp::ContactPtr();
    }
    return m_contacts.at(index.row());
}

void ContactListModel::clear()
{
    beginRemoveRows(QModelIndex(), 0, m_contacts.size());
    m_contacts.clear();
    endRemoveRows();
}



