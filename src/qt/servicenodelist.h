// Copyright (c) 2016-2021 Duality Blockchain Solutions Developers
// Copyright (c) 2014-2021 The Dash Core Developers
// Copyright (c) 2009-2021 The Bitcoin Developers
// Copyright (c) 2009-2021 Satoshi Nakamoto
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DYNAMIC_QT_SERVICENODELIST_H
#define DYNAMIC_QT_SERVICENODELIST_H

#include "platformstyle.h"

#include "primitives/transaction.h"
#include "sync.h"
#include "util.h"

#include <QMenu>
#include <QTimer>
#include <QWidget>

#define MY_SERVICENODELIST_UPDATE_SECONDS 60
#define SERVICENODELIST_UPDATE_SECONDS 15
#define SERVICENODELIST_FILTER_COOLDOWN_SECONDS 3

namespace Ui
{
class ServiceNodeList;
}

class ClientModel;
class WalletModel;

QT_BEGIN_NAMESPACE
class QModelIndex;
QT_END_NAMESPACE

/** ServiceNode Manager page widget */
class ServiceNodeList : public QWidget
{
    Q_OBJECT

public:
    explicit ServiceNodeList(const PlatformStyle* platformStyle, QWidget* parent = 0);
    ~ServiceNodeList();

    void setClientModel(ClientModel* clientModel);
    void setWalletModel(WalletModel* walletModel);
    void StartAlias(std::string strAlias);
    void StartAll(std::string strCommand = "start-all");

private:
    QMenu* contextMenu;
    int64_t nTimeFilterUpdated;
    bool fFilterUpdated;

public Q_SLOTS:
    void updateMyServiceNodeInfo(QString strAlias, QString strAddr, const COutPoint& outpoint);
    void updateMyNodeList(bool fForce = false);
    void updateNodeList();

Q_SIGNALS:

private:
    QTimer* timer;
    Ui::ServiceNodeList* ui;
    ClientModel* clientModel;
    WalletModel* walletModel;
    // Protects tableWidgetServiceNodes
    CCriticalSection cs_dnlist;

    // Protects tableWidgetMyServiceNodes
    CCriticalSection cs_mydnlist;
    QString strCurrentFilter;

private Q_SLOTS:
    void showContextMenu(const QPoint&);
    void on_filterLineEdit_textChanged(const QString& strFilterIn);
    void on_startButton_clicked();
    void on_startAllButton_clicked();
    void on_startMissingButton_clicked();
    void on_tableWidgetMyServiceNodes_itemSelectionChanged();
    void on_UpdateButton_clicked();
};
#endif // DYNAMIC_QT_SERVICENODELIST_H
