// Copyright (c) 2016-2021 Duality Blockchain Solutions Developers

#include "fluid.h"

#include "bdap/domainentry.h"
#include "bdap/domainentrydb.h"
#include "bdap/utils.h"
#include "chain.h"
#include "core_io.h"
#include "keepass.h"
#include "net.h"
#include "netbase.h"
#include "timedata.h"
#include "txmempool.h"
#include "util.h"
#include "utilmoneystr.h"
#include "utiltime.h"
#include "validation.h"

#include "wallet/wallet.h"
#include "wallet/walletdb.h"

CFluid fluid;

#ifdef ENABLE_WALLET
extern CWallet* pwalletMain;
#endif //ENABLE_WALLET

bool IsTransactionFluid(const CScript& txOut)
{
    return (txOut.IsProtocolInstruction(MINT_TX) || txOut.IsProtocolInstruction(MASTERNODE_MODFIY_TX) || txOut.IsProtocolInstruction(MINING_MODIFY_TX) ||
                txOut.IsProtocolInstruction(BDAP_REVOKE_TX));
}

bool IsTransactionFluid(const CTransaction& tx, CScript& fluidScript)
{
    for (const CTxOut& txout : tx.vout) {
        CScript txOut = txout.scriptPubKey;
        if (IsTransactionFluid(txOut)) {
            fluidScript = txOut;
            return true;
        }
    }
    return false;
}

int GetFluidOpCode(const CScript& fluidScript)
{
    if (fluidScript.IsProtocolInstruction(MINT_TX)) {
        return OP_MINT;
    } else if (fluidScript.IsProtocolInstruction(MASTERNODE_MODFIY_TX)) {
        return OP_REWARD_MASTERNODE;
    } else if (fluidScript.IsProtocolInstruction(MINING_MODIFY_TX)) {
        return OP_REWARD_MINING;
    } else if (fluidScript.IsProtocolInstruction(BDAP_REVOKE_TX)) {
        return OP_BDAP_REVOKE;
    }
    return 0;
}

/** Initialise sovereign identities that are able to run fluid commands */
std::vector<std::pair<std::string, CDebitAddress> > CFluidParameters::InitialiseSovereignIdentities()
{
    std::vector<std::pair<std::string, CDebitAddress> > x;
    if (Params().NetworkIDString() == CBaseChainParams::MAIN) {
        x.push_back(std::make_pair("Main01", CDebitAddress("CMRbmYckH5TKhkFev8y4gCrpMo7gZ96xoD")));
        x.push_back(std::make_pair("Main02", CDebitAddress("CVrMHUfNfFUnzbTM2m7ZpCjtXSuzyKfyG6")));
        x.push_back(std::make_pair("Main03", CDebitAddress("CeCbHRu59fqW6a3RoZfcz148mrAmyQjXCU")));
        x.push_back(std::make_pair("Main04", CDebitAddress("CHt97fgnG9LXzQNyWaqYNW3EhRumLCENqS")));
        x.push_back(std::make_pair("Main05", CDebitAddress("CaJQVSM8A9ZVfeJq9S8q1v7AxzE1XVHzo8")));
    } else if (Params().NetworkIDString() == CBaseChainParams::TESTNET) {
        x.push_back(std::make_pair("Test01", CDebitAddress("cFWG3ehFrwnmGjtg1NJJyPE72fHkWu5Sfe"))); //importprivkey aboX2GQU61mLKkSM95keNAhvbgcpN5Rdaq9ZbouAiVfmkWanzmjW
        x.push_back(std::make_pair("Test02", CDebitAddress("c2xbM7uqFaagjCsqK6bNKHu1QeCr9PtHkp"))); //importprivkey aejV3C4nunDUyM67J7MjC6sGN8vvMSDkzYzzscCvhLg7yqtMHWPp
        x.push_back(std::make_pair("Test03", CDebitAddress("cBFo7y2Z1poM954HLvCneWGwPi6Jh1xsPd"))); //importprivkey ae4orEKkQv9za7mzAZVqxuymgJ3KZUXfu2fQKAzpG4ENWp4po3Z9
        x.push_back(std::make_pair("Test04", CDebitAddress("cMDjfnWnADPxMas9eHEP4TwEEELgXMd4nQ"))); //importprivkey aevGrNXvZ4JYWHvDc5kCpWjdNN59vDxD6j3WUqYdvTau7PyZYfR9
        x.push_back(std::make_pair("Test05", CDebitAddress("c927QBgjEtWJB9BhgbDxhQzPq68Nw9hWyf"))); //importprivkey afdsuYrhAr75p2pCqFjkN7XqEFARix1r5XoFoNrQK1ov5qYCd7jS
    } else if (Params().NetworkIDString() == CBaseChainParams::REGTEST) {
        x.push_back(std::make_pair("RegTest01", CDebitAddress("yVTakBsj3x1deP3wjuKr7YetnAB4hH7HaD"))); //importprivkey agqNZR7pQJsZwrYmMEGCsUfgDcVerdfsZq73DJe8qFzM6Y3QonXd
        x.push_back(std::make_pair("RegTest02", CDebitAddress("yeCMR9T3Ps9VESzmdk5ohTeLiTbnbaBZRz"))); //importprivkey agHopWBGyoa78jSNajt1fM2S5ZjvUBbCDAtRD44VxZTdWzXRnsDZ
        x.push_back(std::make_pair("RegTest03", CDebitAddress("ya8DWb1kvsamWoDgyE7DF6qoR6BqrYKP1f"))); //importprivkey ae2UGRep6n2rUAwvzYZyHrru25tDA1DJycaL7PpcEbcmP1u14N1e
        x.push_back(std::make_pair("RegTest04", CDebitAddress("yLdaHs5E4zWfuFgzgtqGPx6UeRtB5DfmVi"))); //importprivkey ahJn46JnVrGq1raQdoEVuUi5RYa2NSjUNtCXUgvfUfKkNcbhUmyf
        x.push_back(std::make_pair("RegTest05", CDebitAddress("yPbK1Kkq7q8WxVL5gAmScUZawigHt9vfTF"))); //importprivkey agfro3kMD8kLSmkgb9zKGpc2uaNfFtnJLWo4QJjPPAoFtG33df3h
    }
    else if (Params().NetworkIDString() == CBaseChainParams::PRIVATENET) {
        x.push_back(std::make_pair("Priv01", CDebitAddress("zXxBSLdnM37xNRNupWzoXuHZ8Y5E9qG15Y"))); //importprivkey ahbXGXSgonHhJe5gNU5zGQbQ1pTQiCCQK99S7PdJdqUb8MtoyGey
        x.push_back(std::make_pair("Priv02", CDebitAddress("zDqX4cYz94K4xNYjicvKGvMHdspsXZqN3j"))); //importprivkey aebYzznt5oh1SGAtzDZSbVFTfLo7uzEFg29iSscA5YauhaQBf54S
        x.push_back(std::make_pair("Priv03", CDebitAddress("zF6KFaNUX9VgZ7zZj8uWAoGCNKYCnbb9eS"))); //importprivkey agUzkeSkYi9oJND6mwhXNAVteHiu7RAp8NLVYHJybrSGKgphR44U
        x.push_back(std::make_pair("Priv04", CDebitAddress("zCiLScE3RRommWgWCkSygtDkdvtuyzowuj"))); //importprivkey afMC8dtcoE1n13mSBpKjRCJL2LYxaaP4XZrCYMPfrCUanDtuwVSn
        x.push_back(std::make_pair("Priv05", CDebitAddress("z92TwCnVLxQRNxsCyQnz4Qvwh4xqsE9WxV"))); //importprivkey aeZomS8gkLK9ehj9fVKnjr36D1kRMVTQa4dyEUiBqz7z7DVi9r3p
    }
    return x;
}

std::vector<std::string> InitialiseAddresses()
{
    CFluidParameters params;
    return params.InitialiseAddresses();
}

std::vector<std::string> CFluidParameters::InitialiseAddresses()
{
    std::vector<std::string> initialSovereignAddresses;
    std::vector<std::pair<std::string, CDebitAddress> > fluidIdentities = InitialiseSovereignIdentities();
    for (const std::pair<std::string, CDebitAddress>& sovereignId : fluidIdentities) {
        initialSovereignAddresses.push_back(sovereignId.second.ToString());
    }
    return initialSovereignAddresses;
}

std::vector<std::vector<unsigned char> > CFluidParameters::InitialiseAddressCharVector()
{
    std::vector<std::vector<unsigned char> > initialSovereignAddresses;
    std::vector<std::pair<std::string, CDebitAddress> > fluidIdentities = InitialiseSovereignIdentities();
    for (const std::pair<std::string, CDebitAddress>& sovereignId : fluidIdentities) {
        initialSovereignAddresses.push_back(CharVectorFromString(sovereignId.second.ToString()));
    }
    return initialSovereignAddresses;
}

/** Checks fluid transactoin operation script amount for invalid values. */
bool CFluid::CheckFluidOperationScript(const CScript& fluidScriptPubKey, const int64_t& timeStamp, std::string& errorMessage, const bool fSkipTimeStampCheck)
{
    std::string strFluidOpScript = ScriptToAsmStr(fluidScriptPubKey);
    std::string verificationWithoutOpCode = GetRidOfScriptStatement(strFluidOpScript);
    std::string strOperationCode = GetRidOfScriptStatement(strFluidOpScript, 0);
    if (!fSkipTimeStampCheck) {
        if (!ExtractCheckTimestamp(strOperationCode, strFluidOpScript, timeStamp)) {
            errorMessage = "CheckFluidOperationScript fluid timestamp is too old.";
            return false;
        }
    }
    if (IsHex(verificationWithoutOpCode)) {
        std::string strUnHexedFluidOpScript = HexToString(verificationWithoutOpCode);
        std::vector<std::string> vecSplitScript;
        SeparateString(strUnHexedFluidOpScript, vecSplitScript, "$");
        if (strOperationCode == "OP_MINT" || strOperationCode == "OP_REWARD_MINING" || strOperationCode == "OP_REWARD_MASTERNODE") {
            if (vecSplitScript.size() > 1) {
                std::string strAmount = vecSplitScript[0];
                CAmount fluidAmount;
                if (ParseFixedPoint(strAmount, 8, &fluidAmount)) {
                    if ((strOperationCode == "OP_REWARD_MINING" || strOperationCode == "OP_REWARD_MASTERNODE") && fluidAmount < 0) {
                        errorMessage = "CheckFluidOperationScript fluid reward amount is less than zero: " + strAmount;
                        return false;
                    } else if (strOperationCode == "OP_MINT" && (fluidAmount > FLUID_MAX_FOR_MINT)) {
                        errorMessage = "CheckFluidOperationScript fluid OP_MINT amount exceeds maximum: " + strAmount;
                        return false;
                    } else if (strOperationCode == "OP_REWARD_MINING" && (fluidAmount > FLUID_MAX_REWARD_FOR_MINING)) {
                        errorMessage = "CheckFluidOperationScript fluid OP_REWARD_MINING amount exceeds maximum: " + strAmount;
                        return false;
                    } else if (strOperationCode == "OP_REWARD_MASTERNODE" && (fluidAmount > FLUID_MAX_REWARD_FOR_MASTERNODE)) {
                        errorMessage = "CheckFluidOperationScript fluid OP_REWARD_MASTERNODE amount exceeds maximum: " + strAmount;
                        return false;
                    }
                }
            } else {
                errorMessage = "CheckFluidOperationScript fluid token invalid. " + strUnHexedFluidOpScript;
                return false;
            }
        }
        else if (strOperationCode == "OP_BDAP_REVOKE") {
            if (vecSplitScript.size() > 1) {
                if (!fSkipTimeStampCheck) {
                    for (uint32_t iter = 1; iter != vecSplitScript.size(); iter++) {
                        CDomainEntry entry;
                        std::string strBanAccountFQDN = DecodeBase64(vecSplitScript[iter]);
                        std::vector<unsigned char> vchBanAccountFQDN = vchFromString(strBanAccountFQDN);
                        if (!DomainEntryExists(vchBanAccountFQDN)) {
                            LogPrintf("%s -- Can't ban %s account because it was not found.\n", __func__, strBanAccountFQDN);
                            errorMessage = strprintf("Skipping... Can't ban %s account because it was not found.", strBanAccountFQDN);
                        }
                    }
                }
            }
            else {
                errorMessage = strprintf("CheckFluidOperationScript fluid OP_BDAP_REVOKE incorrect paramaters %d", vecSplitScript.size());
                return false;
            }
        }
        else {
            errorMessage = strprintf("%s -- %s is an unknown fluid operation", __func__, strOperationCode);
            return false;
        }
    } else {
        errorMessage = "CheckFluidOperationScript fluid token is not hex. " + verificationWithoutOpCode;
        return false;
    }

    return true;
}

/** Checks whether fluid transaction is in the memory pool already */
bool CFluid::CheckIfExistsInMemPool(const CTxMemPool& pool, const CScript& fluidScriptPubKey, std::string& errorMessage)
{
    for (const CTxMemPoolEntry& e : pool.mapTx) {
        const CTransaction& tx = e.GetTx();
        for (const CTxOut& txOut : tx.vout) {
            if (IsTransactionFluid(txOut.scriptPubKey)) {
                std::string strNewFluidScript = ScriptToAsmStr(fluidScriptPubKey);
                std::string strMemPoolFluidScript = ScriptToAsmStr(txOut.scriptPubKey);
                std::string strNewTxWithoutOpCode = GetRidOfScriptStatement(strNewFluidScript);
                std::string strMemPoolTxWithoutOpCode = GetRidOfScriptStatement(strMemPoolFluidScript);
                if (strNewTxWithoutOpCode == strMemPoolTxWithoutOpCode) {
                    errorMessage = "CheckIfExistsInMemPool: fluid transaction is already in the memory pool!";
                    LogPrintf("CheckIfExistsInMemPool: fluid transaction, %s is already in the memory pool! %s\n", tx.GetHash().ToString(), strNewTxWithoutOpCode);
                    return true;
                }
            }
        }
    }

    return false;
}

bool CFluid::CheckAccountBanScript(const CScript& fluidScript, const uint256& txHashId, const unsigned int& nHeight, std::vector<CDomainEntry>& vBanAccounts, std::string& strErrorMessage)
{
    std::string strFluidOpScript = ScriptToAsmStr(fluidScript);
    std::string verificationWithoutOpCode = GetRidOfScriptStatement(strFluidOpScript);
    if (!IsHex(verificationWithoutOpCode)) {
        strErrorMessage = "Fluid token is not a valid hexidecimal value.";
        return false;
    }
    std::string strUnHexedFluidOpScript = HexToString(verificationWithoutOpCode);
    std::vector<std::string> vecSplitScript;
    SeparateString(strUnHexedFluidOpScript, vecSplitScript, "$");
    if (vecSplitScript.size() == 0) {
        strErrorMessage = "Could not split fluid command script.";
        return false;
    }

    for (uint32_t iter = 1; iter != vecSplitScript.size(); iter++) {
        CDomainEntry entry;
        std::string strBanAccountFQDN = DecodeBase64(vecSplitScript[iter]);
        std::vector<unsigned char> vchBanAccountFQDN = vchFromString(strBanAccountFQDN);
        if (GetDomainEntry(vchBanAccountFQDN, entry)) {
            vBanAccounts.push_back(entry);
        }
        else {
            LogPrintf("%s -- Skipping... Can't ban %s account because it was not found.\n", __func__, strBanAccountFQDN);
        }
    }

    return true;
}

/** Checks whether as to parties have actually signed it - please use this with ones with the OP_CODE */
bool CFluid::CheckIfQuorumExists(const std::string& consentToken, std::string& message, const bool individual)
{
    std::vector<std::string> fluidSovereigns;
    std::pair<CDebitAddress, bool> keyOne;
    std::pair<CDebitAddress, bool> keyTwo;
    std::pair<CDebitAddress, bool> keyThree;
    keyOne.second = false, keyTwo.second = false;
    keyThree.second = false;

    GetLastBlockIndex(chainActive.Tip());
    CBlockIndex* pindex = chainActive.Tip();

    if (pindex != NULL) {
        //TODO fluid
        fluidSovereigns = InitialiseAddresses();
    } else
        fluidSovereigns = InitialiseAddresses();

    for (const std::string& address : fluidSovereigns) {
        CDebitAddress attemptKey, xAddress(address);

        if (!xAddress.IsValid())
            return false;

        if (GenericVerifyInstruction(consentToken, attemptKey, message, 1) && xAddress == attemptKey) {
            keyOne = std::make_pair(attemptKey.ToString(), true);
        }

        if (GenericVerifyInstruction(consentToken, attemptKey, message, 2) && xAddress == attemptKey) {
            keyTwo = std::make_pair(attemptKey.ToString(), true);
        }

        if (GenericVerifyInstruction(consentToken, attemptKey, message, 3) && xAddress == attemptKey) {
            keyThree = std::make_pair(attemptKey.ToString(), true);
        }
    }

    bool fValid = (keyOne.first.ToString() != keyTwo.first.ToString() && keyTwo.first.ToString() != keyThree.first.ToString() && keyOne.first.ToString() != keyThree.first.ToString());

    LogPrintf("CheckIfQuorumExists(): Addresses validating this consent token are: %s, %s and %s\n", keyOne.first.ToString(), keyTwo.first.ToString(), keyThree.first.ToString());

    if (individual)
        return (keyOne.second || keyTwo.second || keyThree.second);
    else if (fValid)
        return (keyOne.second && keyTwo.second && keyThree.second);

    return false;
}


/** Checks whether as to parties have actually signed it - please use this with ones **without** the OP_CODE */
bool CFluid::CheckNonScriptQuorum(const std::string& consentToken, std::string& message, const bool individual)
{
    std::string result = "12345 " + consentToken;
    return CheckIfQuorumExists(result, message, individual);
}

/** It will append a signature of the new information */
bool CFluid::GenericConsentMessage(const std::string& message, std::string& signedString, const CDebitAddress& signer)
{
    std::string token, digest;

    if (!IsHex(message))
        return false;

    if (!CheckNonScriptQuorum(message, token, true))
        return false;

    if (token == "")
        return false;

    if (!SignTokenMessage(signer, token, digest, false))
        return false;

    std::string strConvertedMessage = message;
    ConvertToString(strConvertedMessage);
    signedString = StitchString(strConvertedMessage, digest, false);

    ConvertToHex(signedString);

    return true;
}

/** Extract timestamp from a Fluid Transaction */
bool CFluid::ExtractCheckTimestamp(const std::string& strOpCode, const std::string& consentToken, const int64_t& timeStamp)
{
    std::string consentTokenNoScript = GetRidOfScriptStatement(consentToken);
    std::string dehexString = HexToString(consentTokenNoScript);
    std::vector<std::string> strs, ptrs;
    SeparateString(dehexString, strs, false);
    if (strs.size() == 0)
        return false;

    SeparateString(strs.at(0), ptrs, true);
    if (1 >= (int)strs.size())
        return false;

    std::string ls;
    if (strOpCode == "OP_MINT" || strOpCode == "OP_REWARD_MINING" || strOpCode == "OP_REWARD_MASTERNODE") {
        ls = ptrs.at(1);
    }
    else if (strOpCode == "OP_BDAP_REVOKE") {
        ls = ptrs.at(0);
    }
    else {
        return false;
    }
    ScrubString(ls, true);
    int64_t tokenTimeStamp;
    ParseInt64(ls, &tokenTimeStamp);
    if (timeStamp > tokenTimeStamp + fluid.MAX_FLUID_TIME_DISTORT)
        return false;

    return true;
}

bool CFluid::ProcessFluidToken(const std::string& consentToken, std::vector<std::string>& ptrs, const int& strVecNo)
{
    std::string consentTokenNoScript = GetRidOfScriptStatement(consentToken);

    std::string message;
    if (!CheckNonScriptQuorum(consentTokenNoScript, message))
        return false;

    std::string dehexString = HexToString(consentTokenNoScript);

    std::vector<std::string> strs;
    SeparateString(dehexString, strs, false);
    SeparateString(strs.at(0), ptrs, true);

    if (strVecNo >= (int)strs.size())
        return false;

    return true;
}

/** It gets a number from the ASM of an OP_CODE without signature verification */
bool CFluid::GenericParseNumber(const std::string consentToken, const int64_t timeStamp, CAmount& coinAmount, bool txCheckPurpose)
{
    std::vector<std::string> ptrs;

    if (!ProcessFluidToken(consentToken, ptrs, 1))
        return false;

    std::string lr = ptrs.at(0);
    ScrubString(lr, true);
    std::string ls = ptrs.at(1);
    ScrubString(ls, true);
    int64_t tokenTimeStamp;
    ParseInt64(ls, &tokenTimeStamp);

    if (timeStamp > tokenTimeStamp + fluid.MAX_FLUID_TIME_DISTORT && !txCheckPurpose)
        return false;

    ParseFixedPoint(lr, 8, &coinAmount);

    return true;
}

CDebitAddress CFluid::GetAddressFromDigestSignature(const std::string& digestSignature, const std::string& messageTokenKey)
{
    bool fInvalid = false;
    std::vector<unsigned char> vchSig = DecodeBase64(digestSignature.c_str(), &fInvalid);

    if (fInvalid) {
        LogPrintf("GetAddressFromDigestSignature(): Digest Signature Found Invalid, Signature: %s \n", digestSignature);
        return nullptr;
    }

    CHashWriter ss(SER_GETHASH, 0);
    ss << strMessageMagic;
    ss << messageTokenKey;

    CPubKey pubkey;

    if (!pubkey.RecoverCompact(ss.GetHash(), vchSig)) {
        LogPrintf("GetAddressFromDigestSignature(): Public Key Recovery Failed! Hash: %s\n", ss.GetHash().ToString());
        return nullptr;
    }
    CDebitAddress newAddress;
    newAddress.Set(pubkey.GetID());
    return newAddress;
}

/** Individually checks the validity of an instruction */
bool CFluid::GenericVerifyInstruction(const std::string& consentToken, CDebitAddress& signer, std::string& messageTokenKey, const int& whereToLook)
{
    std::string consentTokenNoScript = GetRidOfScriptStatement(consentToken);
    messageTokenKey = "";
    std::vector<std::string> strs;

    ConvertToString(consentTokenNoScript);
    SeparateString(consentTokenNoScript, strs, false);

    messageTokenKey = strs.at(0);

    /* Don't even bother looking there there aren't enough digest keys or we are checking in the wrong place */
    if (whereToLook >= (int)strs.size() || whereToLook == 0)
        return false;

    std::string digestSignature = strs.at(whereToLook);

    signer = GetAddressFromDigestSignature(digestSignature, messageTokenKey);

    return true;
}

bool CFluid::ParseMintKey(const int64_t& nTime, CDebitAddress& destination, CAmount& coinAmount, const std::string& uniqueIdentifier, const bool txCheckPurpose)
{
    std::vector<std::string> ptrs;

    if (!ProcessFluidToken(uniqueIdentifier, ptrs, 1))
        return false;

    if (2 >= (int)ptrs.size())
        return false;

    std::string lr = ptrs.at(0);
    ScrubString(lr, true);
    std::string ls = ptrs.at(1);
    ScrubString(ls, true);
    int64_t tokenTimeStamp;
    ParseInt64(ls, &tokenTimeStamp);

    if (nTime > tokenTimeStamp + fluid.MAX_FLUID_TIME_DISTORT && !txCheckPurpose)
        return false;

    ParseFixedPoint(lr, 8, &coinAmount);

    std::string recipientAddress = ptrs.at(2);
    destination.SetString(recipientAddress);

    if (!destination.IsValid())
        return false;

    LogPrintf("ParseMintKey(): Token Data -- Address %s | Coins to be minted: %s | Time: %s\n", ptrs.at(2), coinAmount / COIN, ls);

    return true;
}

bool GetFluidBlock(const CBlockIndex* pblockindex, CBlock& block)
{
    if (pblockindex != nullptr) {
        // Check for invalid block position and file.
        const CDiskBlockPos pos = pblockindex->GetBlockPos();
        if (pos.nFile > -1 && pos.nPos > 0) {
            if (!ReadBlockFromDisk(block, pblockindex, Params().GetConsensus())) {
                LogPrintf("Unable to read from disk! Highly unlikely but has occured, may be bug or damaged blockchain copy!\n");
                return false;
            }
        } else {
            return false;
        }
    } else {
        return false;
    }
    return true;
}

bool CFluid::GetMintingInstructions(const CBlockIndex* pblockindex, CDebitAddress& toMintAddress, CAmount& mintAmount)
{
    CBlock block;
    if (GetFluidBlock(pblockindex, block)) {
        for (const CTransactionRef& tx : block.vtx) {
            for (const CTxOut& txout : tx->vout) {
                if (txout.scriptPubKey.IsProtocolInstruction(MINT_TX)) {
                    std::string message;
                    if (CheckIfQuorumExists(ScriptToAsmStr(txout.scriptPubKey), message))
                        return ParseMintKey(block.nTime, toMintAddress, mintAmount, ScriptToAsmStr(txout.scriptPubKey));
                }
            }
        }
    }
    return false;
}

/* Check if transaction exists in record */
bool CFluid::CheckTransactionInRecord(const CScript& fluidInstruction, CBlockIndex* pindex)
{
    if (IsTransactionFluid(fluidInstruction)) {
        std::string verificationString = ScriptToAsmStr(fluidInstruction);
        std::vector<std::string> transactionRecord;
        std::string verificationWithoutOpCode = GetRidOfScriptStatement(verificationString);
        std::string message;
        if (CheckIfQuorumExists(verificationString, message)) {
            for (const std::string& existingRecord : transactionRecord) {
                std::string existingWithoutOpCode = GetRidOfScriptStatement(existingRecord);
                LogPrint("fluid", "CheckTransactionInRecord(): operation code removed. existingRecord  = %s verificationString = %s\n", existingWithoutOpCode, verificationWithoutOpCode);
                if (existingWithoutOpCode == verificationWithoutOpCode) {
                    LogPrintf("CheckTransactionInRecord(): Attempt to repeat Fluid Transaction: %s\n", existingRecord);
                    return true;
                }
            }
        }
    }

    return false;
}

CAmount GetStandardPoWBlockPayment(const int& nHeight)
{
    if (nHeight == 1) {
        CAmount nSubsidy = INITIAL_SUPERBLOCK_PAYMENT;
        LogPrint("superblock creation", "GetStandardPoWBlockPayment() : create=%s nSubsidy=%d\n", FormatMoney(nSubsidy), nSubsidy);
        return nSubsidy;
    } else if (nHeight > 1 && nHeight <= Params().GetConsensus().nRewardsStart) {
        LogPrint("zero-reward block creation", "GetStandardPoWBlockPayment() : create=%s nSubsidy=%d\n", FormatMoney(BLOCKCHAIN_INIT_REWARD), BLOCKCHAIN_INIT_REWARD);
        return BLOCKCHAIN_INIT_REWARD; // Burn transaction fees
    } else if (nHeight > Params().GetConsensus().nRewardsStart) {
        LogPrint("creation", "GetStandardPoWBlockPayment() : create=%s PoW Reward=%d\n", FormatMoney(PHASE_1_POW_REWARD), PHASE_1_POW_REWARD);
        return PHASE_1_POW_REWARD; // .1 0DYNC
    } else
        return BLOCKCHAIN_INIT_REWARD;
}

CAmount GetStandardMasternodePayment(const int& nHeight)
{
    if (nHeight > Params().GetConsensus().nMasternodePaymentsStartBlock) {
        LogPrint("fluid", "GetStandardMasternodePayment() : create=%s MN Payment=%d\n", FormatMoney(PHASE_2_MASTERNODE_PAYMENT), PHASE_2_MASTERNODE_PAYMENT);
        return PHASE_2_MASTERNODE_PAYMENT; // 0.4 0DYNC
    } else {
        return BLOCKCHAIN_INIT_REWARD; // 0 0DYNC
    }
}

bool CFluid::ValidationProcesses(CValidationState& state, const CScript& txOut, const CAmount& txValue)
{
    std::string message;
    CAmount mintAmount;
    CDebitAddress toMintAddress;

    if (IsTransactionFluid(txOut)) {
        if (!CheckIfQuorumExists(ScriptToAsmStr(txOut), message)) {
            return state.DoS(100, false, REJECT_INVALID, "bad-txns-fluid-auth-failure");
        }

        if (txOut.IsProtocolInstruction(MINT_TX) &&
            !ParseMintKey(0, toMintAddress, mintAmount, ScriptToAsmStr(txOut), true)) {
            return state.DoS(100, false, REJECT_INVALID, "bad-txns-fluid-mint-auth-failure");
        }

        if ((txOut.IsProtocolInstruction(MASTERNODE_MODFIY_TX) ||
                txOut.IsProtocolInstruction(MINING_MODIFY_TX))  &&
            !GenericParseNumber(ScriptToAsmStr(txOut), 0, mintAmount, true)) {
            return state.DoS(100, false, REJECT_INVALID, "bad-txns-fluid-modify-parse-failure");
        }
    }

    return true;
}

bool CFluid::ProvisionalCheckTransaction(const CTransaction& transaction)
{
    for (const CTxOut& txout : transaction.vout) {
        CScript txOut = txout.scriptPubKey;

        if (IsTransactionFluid(txOut) && CheckTransactionInRecord(txOut)) {
            LogPrintf("ProvisionalCheckTransaction(): Fluid Transaction %s has already been executed!\n", transaction.GetHash().ToString());
            return false;
        }
    }

    return true;
}

bool CFluid::CheckTransactionToBlock(const CTransaction& transaction, const CBlockHeader& blockHeader)
{
    uint256 hash = blockHeader.GetHash();
    return CheckTransactionToBlock(transaction, hash);
}

bool CFluid::CheckTransactionToBlock(const CTransaction& transaction, const uint256 hash)
{
    if (mapBlockIndex.count(hash) == 0)
        return true;

    CBlockIndex* pblockindex = mapBlockIndex[hash];

    for (const CTxOut& txout : transaction.vout) {
        CScript txOut = txout.scriptPubKey;

        if (IsTransactionFluid(txOut) && CheckTransactionInRecord(txOut, pblockindex)) {
            LogPrintf("CheckTransactionToBlock(): Fluid Transaction %s has already been executed!\n", transaction.GetHash().ToString());
            return false;
        }
    }

    return true;
}

std::vector<unsigned char> CharVectorFromString(const std::string& str)
{
    return std::vector<unsigned char>(str.begin(), str.end());
}

std::string StringFromCharVector(const std::vector<unsigned char>& vch)
{
    std::string strReturn;
    std::vector<unsigned char>::const_iterator vi = vch.begin();
    while (vi != vch.end()) {
        strReturn += (char)(*vi);
        vi++;
    }
    return strReturn;
}

std::vector<unsigned char> FluidScriptToCharVector(const CScript& fluidScript)
{
    std::string fluidOperationString = ScriptToAsmStr(fluidScript);
    return vchFromString(fluidOperationString);
}

bool CFluid::ExtractTimestampWithAddresses(const std::string& strOpCode, const CScript& fluidScript, int64_t& nTimeStamp, std::vector<std::vector<unsigned char>>& vSovereignAddresses)
{
    std::string fluidOperationString = ScriptToAsmStr(fluidScript);
    std::string consentTokenNoScript = GetRidOfScriptStatement(fluidOperationString);
    std::string strDehexedToken = HexToString(consentTokenNoScript);
    std::vector<std::string> strs, ptrs;
    SeparateString(strDehexedToken, strs, false);
    if (strs.size() == 0)
        return false;

    SeparateString(strs.at(0), ptrs, true);
    if (1 >= (int)strs.size())
        return false;

    std::string strTimeStamp;
    if (strOpCode == "OP_MINT" || strOpCode == "OP_REWARD_MINING" || strOpCode == "OP_REWARD_MASTERNODE") {
        strTimeStamp = ptrs.at(1);
    } else if (strOpCode == "OP_BDAP_REVOKE") {
        strTimeStamp = ptrs.at(0);
    } else {
        return false;
    }

    ScrubString(strTimeStamp, true);
    ParseInt64(strTimeStamp, &nTimeStamp);
    if (strs.size() > 1) {
        std::string strToken = strs.at(0);
        for (uint32_t iter = 1; iter != strs.size(); iter++) {
            std::string strSignature = strs.at(iter);
            CDebitAddress address = GetAddressFromDigestSignature(strSignature, strToken);
            vSovereignAddresses.push_back(CharVectorFromString(address.ToString()));
        }
    }
    else {
        return false;
    }
    return true;
}
