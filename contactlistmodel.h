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
    void setAccount(Tp::AccountPtr account);

protected:
    QVariant data(const QModelIndex &index, int role) const;
    int rowCount(const QModelIndex &parent) const;

signals:

public slots:

private slots:
    void onConnectionReady(Tp::PendingOperation *op);
    void onContactsUpgraded(Tp::PendingOperation *op);
    void onContactUpdated();

private:
    QList<Tp::ContactPtr> m_contacts;
};

#endif // CONTACTLISTMODEL_H
