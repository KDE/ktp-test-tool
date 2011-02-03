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

#include "contactmodelfilter.h"

#include "contactlistmodel.h"

ContactModelFilter::ContactModelFilter(QObject *parent) :
    QSortFilterProxyModel(parent)
{
}

bool ContactModelFilter::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    QModelIndex target = sourceModel()->index(source_row, 0, source_parent);
    Tp::ConnectionPresenceType presence = sourceModel()->data(target, ContactListModel::PresenceRole).value<Tp::ConnectionPresenceType>();

    switch (presence) {
    case Tp::ConnectionPresenceTypeUnset:
    case Tp::ConnectionPresenceTypeOffline:
    case Tp::ConnectionPresenceTypeHidden :
    case Tp::ConnectionPresenceTypeUnknown:
        return false;
    default:
        return true;
    }
}
