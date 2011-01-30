#ifndef CONTACTMODELFILTER_H
#define CONTACTMODELFILTER_H

#include <QSortFilterProxyModel>

class ContactModelFilter : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit ContactModelFilter(QObject *parent = 0);

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;

signals:

public slots:

};

#endif // CONTACTMODELFILTER_H
