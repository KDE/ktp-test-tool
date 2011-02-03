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

#ifndef CONTACTLISTMODEL_H
#define CONTACTLISTMODEL_H

#include <QAbstractListModel>

//FIXME find out which include I really need for the ptrs
#include <TelepathyQt4/Account>
#include <TelepathyQt4/Contact>
#include <TelepathyQt4/PendingContacts>

Q_DECLARE_METATYPE(Tp::ConnectionPresenceType);

class ContactListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum Roles {
        PresenceRole = Qt::UserRole+1,
    };


    explicit ContactListModel(QObject *parent = 0);

    Tp::AccountPtr account() const;
    void setAccount(Tp::AccountPtr account);

    Tp::ContactPtr contact(const QModelIndex &index) const;

protected:
    QVariant data(const QModelIndex &index, int role) const;
    int rowCount(const QModelIndex &parent) const;

signals:

public slots:

private slots:
    void onContactUpdated();
    void clear();

private:
    QList<Tp::ContactPtr> m_contacts;
    Tp::AccountPtr m_account;
};

#endif // CONTACTLISTMODEL_H
