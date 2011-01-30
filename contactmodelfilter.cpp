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
