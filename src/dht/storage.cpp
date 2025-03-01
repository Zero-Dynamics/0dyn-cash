// Copyright (c) 2019-2021 Duality Blockchain Solutions Developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "dht/storage.h"

#include "bdap/utils.h"
#include "dht/ed25519.h"
#include "dht/limits.h"
#include "dht/mutable.h"
#include "dht/mutabledb.h"
#include "util.h"
#include "validation.h"

#include <libtorrent/aux_/numeric_cast.hpp>
#include <libtorrent/broadcast_socket.hpp> // for ip_v4

#include <libtorrent/config.hpp>
#include <libtorrent/hex.hpp>
#include <libtorrent/random.hpp>
#include <libtorrent/packet_buffer.hpp>
#include <libtorrent/span.hpp>
#include <libtorrent/socket_io.hpp>

#include <array>
#include <string>

using namespace libtorrent;
using namespace libtorrent::dht;

size_t CDHTStorage::num_torrents() const
{ 
    LogPrint("dht", "CDHTStorage -- num_torrents\n");
    return pDefaultStorage->num_torrents(); 
}

size_t CDHTStorage::num_peers() const
{
    LogPrint("dht", "CDHTStorage -- num_peers\n");
    return pDefaultStorage->num_peers(); 
}

void CDHTStorage::update_node_ids(std::vector<libtorrent::sha1_hash> const& ids)
{
    if (!fMasternodeMode) // Only update node ids if Masternode
        return;
    LogPrint("dht", "CDHTStorage -- update_node_ids\n");
    pDefaultStorage->update_node_ids(ids);
}

bool CDHTStorage::get_peers(sha1_hash const& info_hash, bool const noseed, bool const scrape, address const& requester, entry& peers) const
{
    if (!fMasternodeMode) // Only get peers if Masternode
        return false;
    bool ret = pDefaultStorage->get_peers(info_hash, noseed, scrape, requester, peers);
    //LogPrint("dht", "CDHTStorage -- get_peers peers = %s **********\n", peers.to_string());
    return ret;
}

void CDHTStorage::announce_peer(sha1_hash const& info_hash, tcp::endpoint const& endp, string_view name, bool const seed)
{
    if (!fMasternodeMode) // Only announce peers if Masternode
        return;
    LogPrint("dht", "CDHTStorage -- announce_peer\n");
    pDefaultStorage->announce_peer(info_hash, endp, name, seed);
}

bool CDHTStorage::get_immutable_item(sha1_hash const& target, entry& item) const
{
    LogPrint("dht", "CDHTStorage -- get_immutable_item\n");
    return false;
}

void CDHTStorage::put_immutable_item(sha1_hash const& target, span<char const> buf, address const& addr)
{
    //TODO: ban nodes that try to put immutable entries.
    LogPrint("dht", "CDHTStorage -- put_immutable_item target = %s, buf = %s, addr = %s\n", aux::to_hex(target.to_string()), std::string(buf.data()), addr.to_string());
}

bool CDHTStorage::get_mutable_item_seq(sha1_hash const& target, sequence_number& seq) const
{
    if (!fMasternodeMode) // Only try to get DHT data if Masternode
        return false;
    //bool ret = pDefaultStorage->get_mutable_item_seq(target, seq);
    //return ret;
    // TODO (DHT): Try to find entry in memory before searching leveldb
    CMutableData mutableData;
    std::string strInfoHash = aux::to_hex(target.to_string());
    CharString vchInfoHash = vchFromString(strInfoHash);
    LogPrint("dht", "CDHTStorage -- get_mutable_item_seq infohash = %s\n", strInfoHash);
    if (!GetLocalMutableData(vchInfoHash, mutableData)) {
        LogPrintf("********** CDHTStorage -- get_mutable_item_seq failed to get mutable entry sequence_number for infohash = %s.\n", strInfoHash);
        return false;
    }
    seq = dht::sequence_number(mutableData.SequenceNumber);
    LogPrint("dht", "CDHTStorage -- get_mutable_item_seq found seq = %u\n", mutableData.SequenceNumber);
    return true;
}

template<class T>
static entry get_bdecode(T start, T end)
{
    entry e;
    bool err = false;
    detail::bdecode_recursive(start, end, e, err, 0);
    if (err) return entry();
    return e;
}

bool CDHTStorage::get_mutable_item(sha1_hash const& target, sequence_number const seq, bool const force_fill, entry& item) const
{
    if (!fMasternodeMode) // Only try to get DHT data if Masternode
        return false;
    //bool ret = pDefaultStorage->get_mutable_item(target, seq, force_fill, item);
    //return ret;
    // TODO (DHT): Try to find entry in memory before searching leveldb
    CMutableData mutableData;
    std::string strInfoHash = aux::to_hex(target.to_string());
    CharString vchInfoHash = vchFromString(strInfoHash);
    if (!GetLocalMutableData(vchInfoHash, mutableData)) {
        LogPrintf("********** CDHTStorage -- get_mutable_item failed to get mutable entry for infohash = %s.\n", strInfoHash);
        return false;
    }
    item["seq"] = mutableData.SequenceNumber;
    if (force_fill || (sequence_number(0) <= seq && seq < sequence_number(mutableData.SequenceNumber)))
    {
        LogPrint("dht", "********** CDHTStorage -- get_mutable_item data found.\n");
        item["v"] = get_bdecode(mutableData.vchValue.begin(), mutableData.vchValue.end());
        std::array<char, 64> sig;
        aux::from_hex(mutableData.Signature(), sig.data());
        item["sig"] = sig;
        std::array<char, 32> pubKey;
        aux::from_hex(mutableData.PublicKey(), pubKey.data());
        item["k"] = pubKey;
    }
    LogPrint("dht", "CDHTStorage -- get_mutable_item target = %s, item = %s\n", aux::to_hex(target.to_string()), item.to_string());
    return true;
}

void ExtractValueFromSpan(std::unique_ptr<char[]>& value, const span<char const>& buf)
{
    int const size = int(buf.size());
    value.reset(new char[std::size_t(size)]);
    std::memcpy(value.get(), buf.data(), buf.size());
}

void CDHTStorage::put_mutable_item(sha1_hash const& target
    , span<char const> buf
    , signature const& sig
    , sequence_number const seq
    , public_key const& pk
    , span<char const> salt
    , address const& addr)
{
    if (!fMasternodeMode) // Do not store DHT data if not a Masternode
        return;
    // TODO (DHT): Store entries in memory as well
    //pDefaultStorage->put_mutable_item(target, buf, sig, seq, pk, salt, addr);

    std::string strInfoHash = aux::to_hex(target.to_string());
    CharString vchInfoHash = vchFromString(strInfoHash);

    std::unique_ptr<char[]> value;
    ExtractValueFromSpan(value, buf);
    std::string strPutValue = std::string(value.get(), buf.size());
    CharString vchPutValue = vchFromString(strPutValue);

    std::string strSignature = aux::to_hex(std::string(sig.bytes.data(), ED25519_SIGTATURE_BYTE_LENGTH));
    CharString vchSignature = vchFromString(strSignature);

    std::string strPublicKey = aux::to_hex(std::string(pk.bytes.data(), ED25519_PUBLIC_KEY_BYTE_LENGTH));
    CharString vchPublicKey = vchFromString(strPublicKey);
    if (!CheckPubKey(vchPublicKey)) {
        LogPrintf("%s -- Invalid pubkey used (%s).  DHT put storage request failed.\n", __func__, strPublicKey);
        return;
    }
    std::unique_ptr<char[]> saltValue;
    ExtractValueFromSpan(saltValue, salt);
    std::string strSalt = std::string(saltValue.get(), salt.size());
    //bool CheckSalt(const std::string& strSalt, const unsigned int nHeight, std::string& strErrorMessage)
    std::string strErrorMessage;
    unsigned int nHeight = (unsigned int)chainActive.Height();
    if (!CheckSalt(strSalt, nHeight, strErrorMessage)) {
        LogPrintf("%s -- Invalid salt used (%s) at height %d.  DHT put storage request failed. %s\n", __func__, strSalt, nHeight, strErrorMessage);
        return;
    }
    CharString vchSalt = vchFromString(strSalt);

    CMutableData putMutableData(vchInfoHash, vchPublicKey, vchSignature, seq.value, vchSalt, vchPutValue);
    LogPrint("dht", "CDHTStorage::%s -- put_mutable_item info_hash = %s, buf_value = %s, salt = %s, seq = %d, put_size = %d, sig_size = %d, pubkey_size = %d, salt_size = %d\n", 
                    __func__, strInfoHash, strPutValue, strSalt, putMutableData.SequenceNumber, 
                    vchPutValue.size(), vchSignature.size(), vchPublicKey.size(), vchSalt.size());

    CMutableData previousData;
    if (!GetLocalMutableData(vchInfoHash, previousData)) {
        if (PutLocalMutableData(vchInfoHash, putMutableData)) {
            LogPrintf("CDHTStorage::%s added successfully\n", __func__);
        }
    }
    else {
        if (putMutableData.SequenceNumber > previousData.SequenceNumber) {
            if (UpdateLocalMutableData(vchInfoHash, putMutableData)) {
                LogPrintf("CDHTStorage::%s updated successfully\n", __func__);
            }
        }
        else {
            LogPrintf("CDHTStorage::%s value unchanged. No database operation needed.\n", __func__);
        }
    }
    // TODO: Log from address (addr). See touch_item in the default storage implementation.
    return;
}

int CDHTStorage::get_infohashes_sample(entry& item)
{
    LogPrint("dht", "CDHTStorage -- get_infohashes_sample\n");
    return pDefaultStorage->get_infohashes_sample(item);
}

void CDHTStorage::tick()
{
    LogPrint("dht", "CDHTStorage -- tick\n");
    pDefaultStorage->tick();
}

dht_storage_counters CDHTStorage::counters() const
{
    LogPrint("dht", "CDHTStorage -- counters\n");
    return pDefaultStorage->counters();
}

std::unique_ptr<dht_storage_interface> CDHTStorageConstructor(dht_settings const& settings)
{
    return std::unique_ptr<CDHTStorage>(new CDHTStorage(settings));
}