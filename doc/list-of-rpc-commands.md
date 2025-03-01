![0DYNC logo](https://github.com/Zero-Dynamics/Logos/blob/master/0-DYN%20Logos/Cash/PNG/128x128.png)

List of Cash RPC Commands
============================
￼
Address Index
-------------------
* getaddressbalance
* getaddressdeltas
* getaddressmempool
* getaddresstxids
* getaddressutxos


Blockchain
----------
* getbestblockhash
* getblock "hash" ( verbose )
* getblockchaininfo
* getblockcount
* getblockhash index
* getblockhashes timestamp
* getblockheader "hash" ( verbose )
* getblockheaders "hash" ( count verbose )
* getchaintips ( count branchlen )
* getdifficulty
* getmempoolinfo
* getrawmempool ( verbose )
* getspentinfo
* gettxout "txid" n ( includemempool )
* gettxoutproof ["txid",...] ( blockhash )
* gettxoutsetinfo
* verifychain ( checklevel numblocks )
* verifytxoutproof "proof"


Control
-------
* debug ( 0|1|addrman|alert|bench|coindb|db|lock|rand|rpc|selectcoins|mempool|mempoolrej|net|proxy|prune|http|libevent|tor|zmq|cash|privatesend|instantsend|masternode|spork|keepass|mnpayments|gobject )
* getinfo
* getmemoryinfo
* help ( "command" )
* stop


DDNS
----
* name_delete [name]
* name_filter [regexp] [maxage=0] [from=0] [nb=0] [stat] [valuetype]
* name_history [name] [fullhistory] [valuetype]
* name_list [name] [valuetype]
* name_mempool [valuetype]
* name_new [name] [value] [name] [toaddress] [valuetype]
* name_scan [start-name] [max-returned] [max-value-length=-1] [valuetype]
* name_show [name] [valuetype] [filepath]
* name_update [name] [value] [days] [toaddress] [valuetype]


Cash
-------
* mnsync [status|next|reset]
* masternode "command"...
* masternodebroadcast "command"...
* masternodelist ( "mode" "filter" )
* getmasternoderewardstart [nHeight]
* getgovernanceinfo
* getpoolinfo
* getsuperblockbudget index
* gobject "command"...
* privatesend "command"
* spork [name] ([value])
* voteraw [masternode-tx-hash] [masternode-tx-index] [governance-hash] [vote-signal] (yes|no|abstain) [time] [vote-sig]


Generating
----------
* generate numblocks
* getgenerate
* gethashespersec
* setgenerate generate ( genproclimit )


Hidden
------
* invalidateblock "hash"
* name_debug
* reconsiderblock "hash"
* resendwallettransactions
* setmocktime timestamp


Mining
------
* getblocktemplate ( "jsonrequestobject" )
* getmininginfo
* getnetworkhashps ( blocks height )
* getpowrewardstart [nHeight]
* getwork ( "data" )
* prioritisetransaction [txid] [priority delta] [fee delta]
* submitblock "hexdata" ("jsonparametersobject")


Network
-------
* addnode "node" "add|remove|onetry"
* clearbanned
* disconnectnode "node"
* getaddednodeinfo dns ( "node" )
* getconnectioncount
* getnettotals
* getnetworkinfo
* getpeerinfo
* listbanned
* ping
* setban "ip(/netmask)" "add|remove" (bantime) (absolute)


Raw Transactions
----------------
* createrawtransaction [{"txid":"id","vout":n},...] {"address":amount,"data":"hex",...} ( locktime )
* decoderawtransaction "hexstring"
* decodescript "hex"
* fundrawtransaction "hexstring" includeWatching
* getrawtransaction "txid" ( verbose )
* sendrawtransaction "hexstring" ( allowhighfees instantsend )
* signrawtransaction "hexstring" ( [{"txid":"id","vout":n,"scriptPubKey":"hex","redeemScript":"hex"},...] ["privatekey1",...] sighashtype )


Utility Function
----------------
* createmultisig nrequired ["key",...]
* estimatefee nblocks
* estimatepriority nblocks
* estimatesmartfee nblocks
* estimatesmartpriority nblocks
* validateaddress "debitaddress"
* verifymessage "debitaddress" "signature" "message"


Wallet
------
* abandontransaction "txid"
* addmultisigaddress nrequired ["key",...] ( "account" )
* backupwallet "destination"
* dumphdinfo
* dumpprivkey "debitaddress"
* dumpwallet "filename"
* getaccount "debitaddress"
* getaccountaddress "account"
* getaddressesbyaccount "account"
* getbalance ( "account" minconf includeWatchonly )
* getnewaddress ( "account" )
* getrawchangeaddress
* getreceivedbyaccount "account" ( minconf )
* getreceivedbyaddress "debitaddress" ( minconf )
* gettransaction "txid" ( includeWatchonly )
* getunconfirmedbalance
* getwalletinfo
* importaddress "address" ( "label" rescan p2sh )
* importprivkey "cashprivkey" ( "label" rescan )
* importpubkey "pubkey" ( "label" rescan )
* importwallet "filename"
* instantsendtoaddress "debitaddress" amount ( "comment" "comment-to" subtractfeefromamount )
* keepass <genkey|init|setpassphrase>
* keypoolrefill ( newsize )
* listaccounts ( minconf includeWatchonly)
* listaddressgroupings
* listlockunspent
* listreceivedbyaccount ( minconf includeempty includeWatchonly)
* listreceivedbyaddress ( minconf includeempty includeWatchonly)
* listsinceblock ( "blockhash" target-confirmations includeWatchonly)
* listtransactions ( "account" count from includeWatchonly)
* listunspent ( minconf maxconf  ["address",...] )
* lockunspent unlock [{"txid":"txid","vout":n},...]
* makekeypair [prefix]
* move "fromaccount" "toaccount" amount ( minconf "comment" )
* sendfrom "fromaccount" "todebitaddress" amount ( minconf "comment" "comment-to" )
* sendmany "fromaccount" {"address":amount,...} ( minconf "comment" ["address",...] subtractfeefromamount use_is use_ps )
* sendtoaddress "debitaddress" amount ( "comment" "comment-to" subtractfeefromamount use_is use_ps )
* setaccount "debitaddress" "account"
* settxfee amount
* signmessage "debitaddress" "message"
* walletlock
* walletpassphrase "passphrase" timeout ( mixingonly )
* walletpassphrasechange "oldpassphrase" "newpassphrase"
