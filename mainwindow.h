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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

#include <TelepathyQt4/AccountManager>

class AccountsListModel;
class ContactListModel;
class ContactModelFilter;

namespace Ui {
    class MainWindow;
}

namespace Tp {
    class PendingOperation;
}

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


private Q_SLOTS:
    void onAccountManagerReady(Tp::PendingOperation *op);
    void onAccountSelectionChanaged();
    void onChannelJoined(Tp::PendingOperation *op);
    void onJoinRoomClicked();
    void startTextChannel();
    void startAudioChannel();
    void startVideoChannel();



private:
    Ui::MainWindow *ui;
    Tp::AccountManagerPtr m_accountManager;

    AccountsListModel *m_accountsListModel;
    ContactListModel *m_contactListModel;
    ContactModelFilter *m_contactModelFilter;

};

#endif // MAINWINDOW_H
