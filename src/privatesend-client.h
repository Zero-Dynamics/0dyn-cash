// Copyright (c) 2016-2021 Duality Blockchain Solutions Developers
// Copyright (c) 2014-2017 The Dash Core Developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef PRIVATESENDCLIENT_H
#define PRIVATESENDCLIENT_H

#include "masternode.h"
#include "privatesend-util.h"
#include "privatesend.h"
#include "wallet/wallet.h"

class CPrivateSendClientManager;
class CConnman;

static const int DENOMS_COUNT_MAX = 1000;

static const int MIN_PRIVATESEND_SESSIONS = 1;
static const int MIN_PRIVATESEND_ROUNDS = 2;
static const int MIN_PRIVATESEND_AMOUNT = 2;
static const int MIN_PRIVATESEND_LIQUIDITY = 0;
static const int MAX_PRIVATESEND_SESSIONS = 20;
static const int MAX_PRIVATESEND_ROUNDS = 32;
static const int MAX_PRIVATESEND_AMOUNT = MAX_MONEY / COIN;
static const int MAX_PRIVATESEND_LIQUIDITY = 1000;
static const int DEFAULT_PRIVATESEND_SESSIONS = 8;
static const int DEFAULT_PRIVATESEND_ROUNDS = 8;
static const int DEFAULT_PRIVATESEND_AMOUNT = 1000;
static const int DEFAULT_PRIVATESEND_LIQUIDITY = 0;

static const bool DEFAULT_PRIVATESEND_MULTISESSION = false;

// Warn user if mixing in gui or try to create backup if mixing in daemon mode
// when we have only this many keys left
static const int PRIVATESEND_KEYS_THRESHOLD_WARNING = 500;
// Stop mixing completely, it's too dangerous to continue when we have only this many keys left
static const int PRIVATESEND_KEYS_THRESHOLD_STOP = 250;

// The main object for accessing mixing
extern CPrivateSendClientManager privateSendClient;

class CPendingPsaRequest
{
private:
    static const int TIMEOUT = 15;

    CService addr;
    CPrivateSendAccept psa;
    int64_t nTimeCreated;

public:
    CPendingPsaRequest() : addr(CService()),
                           psa(CPrivateSendAccept()),
                           nTimeCreated(0)
    {
    }

    CPendingPsaRequest(const CService& addr_, const CPrivateSendAccept& psa_) : addr(addr_),
                                                                                psa(psa_),
                                                                                nTimeCreated(GetTime())
    {
    }

    CService GetAddr() { return addr; }
    CPrivateSendAccept GetPSA() { return psa; }
    bool IsExpired() { return GetTime() - nTimeCreated > TIMEOUT; }

    friend bool operator==(const CPendingPsaRequest& a, const CPendingPsaRequest& b)
    {
        return a.addr == b.addr && a.psa == b.psa;
    }
    friend bool operator!=(const CPendingPsaRequest& a, const CPendingPsaRequest& b)
    {
        return !(a == b);
    }
    explicit operator bool() const
    {
        return *this != CPendingPsaRequest();
    }
};

class CPrivateSendClientSession : public CPrivateSendBaseSession
{
private:
    std::vector<COutPoint> vecOutPointLocked;

    int nEntriesCount;
    bool fLastEntryAccepted;

    std::string strLastMessage;
    std::string strAutoDenomResult;

    masternode_info_t infoMixingMasternode;
    CMutableTransaction txMyCollateral; // client side collateral
    CPendingPsaRequest pendingPsaRequest;

    CKeyHolderStorage keyHolderStorage; // storage for keys used in PrepareDenominate

    /// Create denominations
    bool CreateDenominated(CConnman& connman);
    bool CreateDenominated(const CompactTallyItem& tallyItem, bool fCreateMixingCollaterals, CConnman& connman);

    /// Split up large inputs or make fee sized inputs
    bool MakeCollateralAmounts(CConnman& connman);
    bool MakeCollateralAmounts(const CompactTallyItem& tallyItem, bool fTryDenominated, CConnman& connman);

    bool JoinExistingQueue(CAmount nBalanceNeedsAnonymized, CConnman& connman);
    bool StartNewQueue(CAmount nValueMin, CAmount nBalanceNeedsAnonymized, CConnman& connman);

    /// step 0: select denominated inputs and txouts
    bool SelectDenominate(std::string& strErrorRet, std::vector<std::pair<CTxPSIn, CTxOut> >& vecPSInOutPairsRet);
    /// step 1: prepare denominated inputs and outputs
    bool PrepareDenominate(int nMinRounds, int nMaxRounds, std::string& strErrorRet, const std::vector<std::pair<CTxPSIn, CTxOut> >& vecPSInOutPairsIn, std::vector<std::pair<CTxPSIn, CTxOut> >& vecPSInOutPairsRet, bool fDryRun = false);
    /// step 2: send denominated inputs and outputs prepared in step 1
    bool SendDenominate(const std::vector<std::pair<CTxPSIn, CTxOut> >& vecPSInOutPairsIn, CConnman& connman);

    /// Get Masternodes updates about the progress of mixing
    bool CheckPoolStateUpdate(PoolState nStateNew, int nEntriesCountNew, PoolStatusUpdate nStatusUpdate, PoolMessage nMessageID, int nSessionIDNew = 0);
    // Set the 'state' value, with some logging and capturing when the state changed
    void SetState(PoolState nStateNew);

    /// Check for process
    void CheckPool();
    void CompletedTransaction(PoolMessage nMessageID);

    /// As a client, check and sign the final transaction
    bool SignFinalTransaction(const CTransaction& finalTransactionNew, CNode* pnode, CConnman& connman);

    void RelayIn(const CPrivateSendEntry& entry, CConnman& connman);

    void SetNull();

public:
    CPrivateSendClientSession() : vecOutPointLocked(),
                                  nEntriesCount(0),
                                  fLastEntryAccepted(false),
                                  strLastMessage(),
                                  strAutoDenomResult(),
                                  infoMixingMasternode(),
                                  txMyCollateral(),
                                  pendingPsaRequest(),
                                  keyHolderStorage()
    {
    }

    void ProcessMessage(CNode* pfrom, const std::string& strCommand, CDataStream& vRecv, CConnman& connman);

    void UnlockCoins();

    void ResetPool();

    std::string GetStatus(bool fWaitForBlock);

    bool GetMixingMasternodeInfo(masternode_info_t& mnInfoRet) const;

    /// Passively run mixing in the background according to the configuration in settings
    bool DoAutomaticDenominating(CConnman& connman, bool fDryRun = false);

    /// As a client, submit part of a future mixing transaction to a Masternode to start the process
    bool SubmitDenominate(CConnman& connman);

    bool ProcessPendingPsaRequest(CConnman& connman);

    bool CheckTimeout();
};

/** Used to keep track of current status of mixing pool
 */
class CPrivateSendClientManager : public CPrivateSendBaseManager
{
private:
    // Keep track of the used Masternodes
    std::vector<COutPoint> vecMasternodesUsed;

    std::vector<CAmount> vecDenominationsSkipped;

    // TODO: or map<denom, CPrivateSendClientSession> ??
    std::deque<CPrivateSendClientSession> peqSessions;
    mutable CCriticalSection cs_peqsessions;

    int nCachedLastSuccessBlock;
    int nMinBlocksToWait; // how many blocks to wait after one successful mixing tx in non-multisession mode
    std::string strAutoDenomResult;

    // Keep track of current block height
    int nCachedBlockHeight;

    bool WaitForAnotherBlock();

    // Make sure we have enough keys since last backup
    bool CheckAutomaticBackup();

public:
    int nPrivateSendSessions;
    int nPrivateSendRounds;
    int nPrivateSendAmount;
    int nLiquidityProvider;
    bool fEnablePrivateSend;
    bool fPrivateSendMultiSession;

    int nCachedNumBlocks;    //used for the overview screen
    bool fCreateAutoBackups; //builtin support for automatic backups

    CPrivateSendClientManager() : vecMasternodesUsed(),
                                  vecDenominationsSkipped(),
                                  peqSessions(),
                                  nCachedLastSuccessBlock(0),
                                  nMinBlocksToWait(1),
                                  strAutoDenomResult(),
                                  nCachedBlockHeight(0),
                                  nPrivateSendRounds(DEFAULT_PRIVATESEND_ROUNDS),
                                  nPrivateSendAmount(DEFAULT_PRIVATESEND_AMOUNT),
                                  nLiquidityProvider(DEFAULT_PRIVATESEND_LIQUIDITY),
                                  fEnablePrivateSend(false),
                                  fPrivateSendMultiSession(DEFAULT_PRIVATESEND_MULTISESSION),
                                  nCachedNumBlocks(std::numeric_limits<int>::max()),
                                  fCreateAutoBackups(true)
    {
    }

    void ProcessMessage(CNode* pfrom, const std::string& strCommand, CDataStream& vRecv, CConnman& connman);

    bool IsDenomSkipped(const CAmount& nDenomValue);
    void AddSkippedDenom(const CAmount& nDenomValue);
    void ClearSkippedDenominations() { vecDenominationsSkipped.clear(); }

    void SetMinBlocksToWait(int nMinBlocksToWaitIn) { nMinBlocksToWait = nMinBlocksToWaitIn; }

    void ResetPool();

    std::string GetStatuses();
    std::string GetSessionDenoms();

    bool GetMixingMasternodesInfo(std::vector<masternode_info_t>& vecMnInfoRet) const;

    /// Passively run mixing in the background according to the configuration in settings
    bool DoAutomaticDenominating(CConnman& connman, bool fDryRun = false);

    void CheckTimeout();

    void ProcessPendingPsaRequest(CConnman& connman);

    void AddUsedMasternode(const COutPoint& outpointMn);
    masternode_info_t GetNotUsedMasternode();

    void UpdatedSuccessBlock();

    void UpdatedBlockTip(const CBlockIndex* pindex);

    void DoMaintenance(CConnman& connman);
};

#endif
