DIST_SUBDIRS = secp256k1 univalue

AM_LDFLAGS = $(PTHREAD_CFLAGS) $(LIBTOOL_LDFLAGS) $(HARDENED_LDFLAGS)
AM_CXXFLAGS = $(HARDENED_CXXFLAGS)
AM_CPPFLAGS = $(HARDENED_CPPFLAGS)
EXTRA_LIBRARIES =

CASH_CONFIG_INCLUDES=-I$(builddir)/config
CASH_INCLUDES=-I$(builddir) -I$(builddir)/obj $(BOOST_CPPFLAGS) $(LEVELDB_CPPFLAGS) $(LIBTORRENT_CPPFLAGS) $(LIBVGP_CPPFLAGS) $(CRYPTO_CFLAGS) $(SSL_CFLAGS)

CASH_INCLUDES += -I$(srcdir)/secp256k1/include
CASH_INCLUDES += -I$(srcdir)/univalue/include

LIBCASH_SERVER=libcash_server.a
LIBCASH_WALLET=libcash_wallet.a
LIBCASH_CONSENSUS=libcash_consensus.a
LIBCASH_COMMON=libcash_common.a
LIBCASH_CLI=libcash_cli.a
LIBCASH_UTIL=libcash_util.a
LIBCASH_CRYPTO=crypto/libcash_crypto.a
LIBCASHQT=qt/libcashqt.a
LIBSECP256K1=secp256k1/libsecp256k1.la
LIBUNIVALUE=univalue/libunivalue.la

$(LIBSECP256K1): $(wildcard secp256k1/src/*) $(wildcard secp256k1/include/*)
	$(AM_V_at)$(MAKE) $(AM_MAKEFLAGS) -C $(@D) $(@F)

$(LIBUNIVALUE): $(wildcard univalue/lib/*) $(wildcard univalue/include/*)
	$(AM_V_at)$(MAKE) $(AM_MAKEFLAGS) -C $(@D) $(@F)

# Make is not made aware of per-object dependencies to avoid limiting building parallelization
# But to build the less dependent modules first, we manually select their order here:
EXTRA_LIBRARIES += \
  $(LIBCASH_CRYPTO) \
  $(LIBCASH_UTIL) \
  $(LIBCASH_COMMON) \
  $(LIBCASH_CONSENSUS) \
  $(LIBCASH_SERVER) \
  $(LIBCASH_CLI) \
  $(LIBCASH_WALLET)

if ENABLE_WALLET
CASH_INCLUDES += $(BDB_CPPFLAGS)
EXTRA_LIBRARIES += $(LIBCASH_WALLET)
endif
if ENABLE_ZMQ
EXTRA_LIBRARIES += libcash_zmq.a
endif

if BUILD_CASH_LIBS
lib_LTLIBRARIES = libcashconsensus.la
LIBCASHCONSENSUS=libcashconsensus.la
else
LIBCASHCONSENSUS=
endif

bin_PROGRAMS =
TESTS =
BENCHMARKS =
DHT_TESTS =

if BUILD_CASHD
  bin_PROGRAMS += cashd
endif

if BUILD_CASH_UTILS
  bin_PROGRAMS += cash-cli cash-tx
endif

.PHONY: FORCE check-symbols check-security
# cash #
CASH_CORE_H = \
  activemasternode.h \
  addrdb.h \
  addressindex.h \
  addrman.h \
  alert.h \
  base58.h \
  bdap/audit.h \
  bdap/auditdb.h \
  bdap/bdap.h \
  bdap/certificatedb.h \
  bdap/domainentry.h \
  bdap/domainentrydb.h \
  bdap/fees.h \
  bdap/linking.h \
  bdap/linkingdb.h \
  bdap/linkmanager.h \
  bdap/linkstorage.h \
  bdap/stealth.h \
  bdap/utils.h \
  bdap/vgpmessage.h \
  bdap/x509certificate.h \
  bip39.h \
  blockencodings.h \
  bloom.h \
  cachemap.h \
  cachemultimap.h \
  chain.h \
  chainparams.h \
  chainparamsbase.h \
  chainparamsseeds.h \
  checkpoints.h \
  checkqueue.h \
  clientversion.h \
  coins.h \
  compat.h \
  compat/byteswap.h \
  compat/endian.h \
  compat/sanity.h \
  compressor.h \
  consensus/consensus.h \
  core_io.h \
  core_memusage.h \
  cuckoocache.h \
  dbwrapper.h \
  dht/datachunk.h \
  dht/dataheader.h \
  dht/datarecord.h \
  dht/dataset.h \
  dht/ed25519.h \
  dht/limits.h \
  dht/mutable.h \
  dht/mutabledb.h \
  dht/session.h \
  dht/sessionevents.h \
  dht/settings.h \
  dht/storage.h \
  masternode.h \
  masternode-payments.h \
  masternode-sync.h \
  masternodeman.h \
  masternodeconfig.h \
  flat-database.h \
  fluid/banaccount.h \
  fluid/fluid.h \
  fluid/fluiddb.h \
  fluid/fluidmasternode.h \
  fluid/fluidmining.h \
  fluid/fluidmint.h \
  fluid/fluidsovereign.h \
  fluid/operations.h \
  governance.h \
  governance-classes.h \
  governance-exceptions.h \
  governance-object.h \
  governance-validators.h \
  governance-vote.h \
  governance-votedb.h \
  hdchain.h \
  httprpc.h \
  httpserver.h \
  indirectmap.h \
  init.h \
  instantsend.h \
  key.h \
  keepass.h \
  keystore.h \
  limitedmap.h \
  memusage.h \
  merkleblock.h \
  messagesigner.h \
  miner/impl/miner-cpu.h \
  miner/impl/miner-gpu.h \
  miner/internal/hash-rate-counter.h \
  miner/internal/miner-base.h \
  miner/internal/miner-context.h \
  miner/internal/miners-controller.h \
  miner/internal/miners-group.h \
  miner/internal/thread-group.h \
  miner/miner-util.h \
  miner/miner.h \
  net.h \
  net_processing.h \
  netaddress.h \
  netbase.h \
  netfulfilledman.h \
  netmessagemaker.h \
  noui.h \
  ntp.h \
  policy/fees.h \
  policy/policy.h \
  policy/rbf.h \
  pow.h \
  privatesend-client.h \
  privatesend.h \
  privatesend-server.h \
  privatesend-util.h \
  psnotificationinterface.h \
  protocol.h \
  random.h \
  reverselock.h \
  reverse_iterator.h \
  rpc/client.h \
  rpc/protocol.h \
  rpc/register.h \
  rpc/server.h \
  rpc/wallet.h \
  scheduler.h \
  script/sigcache.h \
  script/sign.h \
  script/standard.h \
  spentindex.h \
  spork.h \
  streams.h \
  support/allocators/secure.h \
  support/allocators/zeroafterfree.h \
  support/cleanse.h \
  support/lockedpool.h \
  support/events.h \
  sync.h \
  threadinterrupt.h \
  threadsafety.h \
  timedata.h \
  torcontrol.h \
  txdb.h \
  txmempool.h \
  ui_interface.h \
  undo.h \
  util.h \
  utilmoneystr.h \
  utilstrencodings.h \
  utiltime.h \
  validation.h \
  validationinterface.h \
  versionbits.h \
  wallet/coincontrol.h \
  wallet/crypter.h \
  wallet/db.h \
  wallet/wallet.h \
  wallet/wallet_ismine.h \
  wallet/walletdb.h \
  wordlists/chinese_simplified.h \
  wordlists/chinese_traditional.h \
  wordlists/english.h \
  wordlists/french.h \
  wordlists/german.h \
  wordlists/italian.h \
  wordlists/japanese.h \
  wordlists/korean.h \
  wordlists/russian.h \
  wordlists/spanish.h \
  wordlists/ukrainian.h \
  warnings.h \
  zmq/zmqabstractnotifier.h \
  zmq/zmqconfig.h\
  zmq/zmqnotificationinterface.h \
  zmq/zmqpublishnotifier.h

obj/build.h: FORCE
	@$(MKDIR_P) $(builddir)/obj
	@$(top_srcdir)/share/genbuild.sh $(abs_top_builddir)/src/obj/build.h \
	  $(abs_top_srcdir)
libcash_util_a-clientversion.$(OBJEXT): obj/build.h

# server: shared between cashd and cash-qt
libcash_server_a_CPPFLAGS = $(AM_CPPFLAGS) $(CASH_INCLUDES) $(MINIUPNPC_CPPFLAGS) $(EVENT_CFLAGS) $(EVENT_PTHREADS_CFLAGS)
libcash_server_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libcash_server_a_SOURCES = \
  activemasternode.cpp \
  addrdb.cpp \
  addrman.cpp \
  alert.cpp \
  blockencodings.cpp \
  bloom.cpp \
  chain.cpp \
  checkpoints.cpp \
  bdap/audit.cpp \
  bdap/auditdb.cpp \
  bdap/certificatedb.cpp \
  bdap/domainentry.cpp \
  bdap/domainentrydb.cpp \
  bdap/fees.cpp \
  bdap/linking.cpp \
  bdap/linkingdb.cpp \
  bdap/linkmanager.cpp \
  bdap/linkstorage.cpp \
  bdap/utils.cpp \
  bdap/vgpmessage.cpp \
  bdap/x509certificate.cpp \
  dbwrapper.cpp \
  dht/datachunk.cpp \
  dht/dataheader.cpp \
  dht/datarecord.cpp \
  dht/dataset.cpp \
  dht/limits.cpp \
  dht/mutable.cpp \
  dht/mutabledb.cpp \
  dht/session.cpp \
  dht/sessionevents.cpp \
  dht/settings.cpp \
  dht/storage.cpp \
  masternode.cpp \
  masternode-payments.cpp\
  masternode-sync.cpp \
  masternodeconfig.cpp \
  masternodeman.cpp \
  fluid/banaccount.cpp \
  fluid/fluid.cpp \
  fluid/fluiddb.cpp \
  fluid/fluidmasternode.cpp \
  fluid/fluidmining.cpp \
  fluid/fluidmint.cpp \
  fluid/fluidsovereign.cpp \
  fluid/operations.cpp \
  governance.cpp \
  governance-classes.cpp \
  governance-object.cpp \
  governance-validators.cpp \
  governance-vote.cpp \
  governance-votedb.cpp \
  httprpc.cpp \
  httpserver.cpp \
  init.cpp \
  instantsend.cpp \
  merkleblock.cpp \
  messagesigner.cpp \
  miner/impl/miner-cpu.cpp \
  miner/impl/miner-gpu.cpp \
  miner/internal/hash-rate-counter.cpp \
  miner/internal/miner-base.cpp \
  miner/internal/miner-context.cpp \
  miner/internal/miners-controller.cpp \
  miner/miner-util.cpp \
  miner/miner.cpp \
  net.cpp \
  netfulfilledman.cpp \
  net_processing.cpp \
  noui.cpp \
  ntp.cpp \
  policy/fees.cpp \
  policy/policy.cpp \
  pow.cpp \
  privatesend.cpp \
  privatesend-server.cpp \
  psnotificationinterface.cpp \
  rest.cpp \
  rpc/audits.cpp \
  rpc/blockchain.cpp \
  rpc/certificates.cpp \
  rpc/dht.cpp \
  rpc/domainentry.cpp \
  rpc/masternode.cpp \
  rpc/fluid.cpp \
  rpc/governance.cpp \
  rpc/linking.cpp \
  rpc/mining.cpp \
  rpc/misc.cpp \
  rpc/net.cpp \
  rpc/rawbdap.cpp \
  rpc/rawtransaction.cpp \
  rpc/server.cpp \
  script/sigcache.cpp \
  sendalert.cpp \
  spork.cpp \
  timedata.cpp \
  torcontrol.cpp \
  txdb.cpp \
  txmempool.cpp \
  ui_interface.cpp \
  validation.cpp \
  validationinterface.cpp \
  versionbits.cpp \
  warnings.cpp \
  $(CASH_CORE_H)

if ENABLE_ZMQ
LIBCASH_ZMQ=libcash_zmq.a

libcash_zmq_a_CPPFLAGS = $(CASH_INCLUDES) $(ZMQ_CFLAGS)
libcash_zmq_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libcash_zmq_a_SOURCES = \
  zmq/zmqabstractnotifier.cpp \
  zmq/zmqnotificationinterface.cpp \
  zmq/zmqpublishnotifier.cpp
endif

# wallet: shared between cashd and cash-qt, but only linked
# when wallet enabled
libcash_wallet_a_CPPFLAGS = $(AM_CPPFLAGS) $(CASH_INCLUDES)
libcash_wallet_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libcash_wallet_a_SOURCES = \
  bdap/stealth.cpp \
  keepass.cpp \
  policy/rbf.cpp \
  privatesend-client.cpp \
  privatesend-util.cpp \
  rpc/wallet.cpp \
  rpc/walletdump.cpp \
  uint256.cpp \
  uint256.h \
  wallet/crypter.cpp \
  wallet/db.cpp \
  wallet/wallet.cpp \
  wallet/wallet_ismine.cpp \
  wallet/walletdb.cpp \
  $(CASH_CORE_H)

# crypto primitives library
crypto_libcash_crypto_a_CPPFLAGS = $(AM_CPPFLAGS) $(CASH_CONFIG_INCLUDES) $(PIC_FLAGS)
crypto_libcash_crypto_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS) $(PIC_FLAGS)
crypto_libcash_crypto_a_SOURCES = \
  crypto/argon2d/argon2.c \
  crypto/argon2d/argon2.h \
  crypto/argon2d/core.c \
  crypto/argon2d/core.h \
  crypto/argon2d/encoding.c \
  crypto/argon2d/encoding.h \
  crypto/argon2d/opt.c \
  crypto/argon2d/thread.c \
  crypto/argon2d/thread.h \
  crypto/blake2/blake2-impl.h \
  crypto/blake2/blake2.h \
  crypto/blake2/blake2b.c \
  crypto/blake2/blamka-round-opt.h \
  crypto/blake2/blamka-round-ref.h \
  crypto/chacha20.cpp \
  crypto/chacha20.h \
  crypto/common.h \
  crypto/hmac_sha256.cpp \
  crypto/hmac_sha256.h \
  crypto/hmac_sha512.cpp \
  crypto/hmac_sha512.h \
  crypto/ripemd160.cpp \
  crypto/ripemd160.h \
  crypto/sha1.cpp \
  crypto/sha1.h \
  crypto/sha256.cpp \
  crypto/sha256.h \
  crypto/sha256_avx2.cpp \
  crypto/sha256_shani.cpp \
  crypto/sha256_sse4.cpp \
  crypto/sha256_sse41.cpp \
  crypto/sha512.cpp \
  crypto/sha512.h

# consensus: shared between all executables that validate any consensus rules.
libcash_consensus_a_CPPFLAGS = $(AM_CPPFLAGS) $(CASH_INCLUDES)
libcash_consensus_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libcash_consensus_a_SOURCES = \
  amount.cpp \
  amount.h \
  arith_uint256.cpp \
  arith_uint256.h \
  consensus/merkle.cpp \
  consensus/merkle.h \
  consensus/params.h \
  consensus/validation.h \
  hash.cpp \
  hash.h \
  prevector.h \
  primitives/block.cpp \
  primitives/block.h \
  primitives/transaction.cpp \
  primitives/transaction.h \
  pubkey.cpp \
  pubkey.h \
  script/cashconsensus.cpp \
  script/interpreter.cpp \
  script/interpreter.h \
  script/script.cpp \
  script/script.h \
  script/script_error.cpp \
  script/script_error.h \
  serialize.h \
  tinyformat.h \
  uint256.cpp \
  uint256.h \
  version.h

# common: shared between cashd, and cash-qt and non-server tools
libcash_common_a_CPPFLAGS = $(AM_CPPFLAGS) $(CASH_INCLUDES)
libcash_common_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libcash_common_a_SOURCES = \
  base58.cpp \
  bdap/stealth.cpp \
  bip39.cpp \
  chainparams.cpp \
  coins.cpp \
  compressor.cpp \
  core_read.cpp \
  core_write.cpp \
  dht/ed25519.cpp \
  hdchain.cpp \
  key.cpp \
  keystore.cpp \
  netaddress.cpp \
  netbase.cpp \
  protocol.cpp \
  scheduler.cpp \
  script/sign.cpp \
  script/standard.cpp \
  wallet/wallet_ismine.cpp \
  $(CASH_CORE_H)

# util: shared between all executables.
# This library *must* be included to make sure that the glibc
# backward-compatibility objects and their sanity checks are linked.
libcash_util_a_CPPFLAGS = $(AM_CPPFLAGS) $(CASH_INCLUDES)
libcash_util_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libcash_util_a_SOURCES = \
  chainparamsbase.cpp \
  clientversion.cpp \
  compat/glibc_sanity.cpp \
  compat/glibcxx_sanity.cpp \
  compat/strnlen.cpp \
  random.cpp \
  rpc/protocol.cpp \
  support/cleanse.cpp \
  support/lockedpool.cpp \
  sync.cpp \
  threadinterrupt.cpp \
  util.cpp \
  utilmoneystr.cpp \
  utilstrencodings.cpp \
  utiltime.cpp \
  $(CASH_CORE_H)

if GLIBC_BACK_COMPAT
libcash_util_a_SOURCES += compat/glibc_compat.cpp
AM_LDFLAGS += $(COMPAT_LDFLAGS)
endif

# cli: shared between cash-cli and cash-qt
libcash_cli_a_CPPFLAGS = $(AM_CPPFLAGS) $(CASH_INCLUDES)
libcash_cli_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libcash_cli_a_SOURCES = \
  rpc/client.cpp \
  $(CASH_CORE_H)

nodist_libcash_util_a_SOURCES = $(srcdir)/obj/build.h
#

# cashd binary #
cashd_SOURCES = cashd.cpp
cashd_CPPFLAGS = $(AM_CPPFLAGS) $(CASH_INCLUDES)
cashd_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
cashd_LDFLAGS = $(RELDFLAGS) $(AM_LDFLAGS) $(LIBTOOL_APP_LDFLAGS)

if TARGET_WINDOWS
cashd_SOURCES += cashd-res.rc
endif

cashd_LDADD = \
  $(LIBCASH_SERVER) \
  $(LIBCASH_COMMON) \
  $(LIBUNIVALUE) \
  $(LIBCASH_UTIL) \
  $(LIBCASH_CONSENSUS) \
  $(LIBCASH_CRYPTO) \
  $(LIBVGP) \
  $(LIBLEVELDB) \
  $(LIBLEVELDB_SSE42) \
  $(LIBMEMENV) \
  $(LIBSECP256K1) \
  $(LIBTORRENT)

if ENABLE_ZMQ
cashd_LDADD += $(LIBCASH_ZMQ) $(ZMQ_LIBS)
endif

if ENABLE_WALLET
cashd_LDADD += $(LIBCASH_WALLET)
endif

cashd_LDADD += $(BOOST_LIBS) $(BDB_LIBS) $(SSL_LIBS) $(CRYPTO_LIBS) $(MINIUPNPC_LIBS) $(EVENT_PTHREADS_LIBS) $(EVENT_LIBS)

# cash-cli binary #
cash_cli_SOURCES = cash-cli.cpp
cash_cli_CPPFLAGS = $(AM_CPPFLAGS) $(CASH_INCLUDES) $(EVENT_CFLAGS)
cash_cli_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
cash_cli_LDFLAGS = $(RELDFLAGS) $(AM_LDFLAGS) $(LIBTOOL_APP_LDFLAGS)

if TARGET_WINDOWS
cash_cli_SOURCES += cash-cli-res.rc
endif

cash_cli_LDADD = \
  $(LIBCASH_CLI) \
  $(LIBUNIVALUE) \
  $(LIBCASH_UTIL) \
  $(LIBCASH_CRYPTO)

cash_cli_LDADD += $(BOOST_LIBS) $(SSL_LIBS) $(CRYPTO_LIBS) $(EVENT_LIBS)
#

# cash-tx binary #
cash_tx_SOURCES = cash-tx.cpp
cash_tx_CPPFLAGS = $(AM_CPPFLAGS) $(CASH_INCLUDES)
cash_tx_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
cash_tx_LDFLAGS = $(RELDFLAGS) $(AM_LDFLAGS) $(LIBTOOL_APP_LDFLAGS)

if TARGET_WINDOWS
cash_tx_SOURCES += cash-tx-res.rc
endif

cash_tx_LDADD = \
  $(LIBUNIVALUE) \
  $(LIBCASH_COMMON) \
  $(LIBCASH_UTIL) \
  $(LIBCASH_CONSENSUS) \
  $(LIBCASH_CRYPTO) \
  $(LIBVGP) \
  $(LIBSECP256K1)  \
  $(LIBTORRENT)

cash_tx_LDADD += $(BOOST_LIBS) $(CRYPTO_LIBS)
#

# cashconsensus library #
if BUILD_CASH_LIBS
include_HEADERS = script/cashconsensus.h
libcashconsensus_la_SOURCES = $(crypto_libcash_crypto_a_SOURCES) $(libcash_consensus_a_SOURCES)

if GLIBC_BACK_COMPAT
  libcashconsensus_la_SOURCES += compat/glibc_compat.cpp
endif

libcashconsensus_la_LDFLAGS = $(AM_LDFLAGS) -no-undefined $(RELDFLAGS)
libcashconsensus_la_LIBADD = $(LIBSECP256K1) $(BOOST_LIBS)
libcashconsensus_la_CPPFLAGS = $(AM_CPPFLAGS) -I$(builddir)/obj -I$(srcdir)/secp256k1/include -DBUILD_CASH_INTERNAL
libcashconsensus_la_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)

endif
#

CLEANFILES = leveldb/libleveldb.a leveldb/libmemenv.a
CLEANFILES += *.gcda *.gcno
CLEANFILES += bdap/*.gcda bdap/*.gcno
CLEANFILES += bdap/vgp/*.gcda bdap/vgp/*.gcno
CLEANFILES += compat/*.gcda compat/*.gcno
CLEANFILES += consensus/*.gcda consensus/*.gcno
CLEANFILES += crypto/*.gcda crypto/*.gcno
CLEANFILES += dht/*.gcda dht/*.gcno
CLEANFILES += fluid/*.gcda fluid/*.gcno
CLEANFILES += miner/*.gcda miner/*.gcno
CLEANFILES += policy/*.gcda policy/*.gcno
CLEANFILES += primitives/*.gcda primitives/*.gcno
CLEANFILES += script/*.gcda script/*.gcno
CLEANFILES += support/*.gcda support/*.gcno
CLEANFILES += univalue/*.gcda univalue/*.gcno
CLEANFILES += wallet/*.gcda wallet/*.gcno
CLEANFILES += wallet/test/*.gcda wallet/test/*.gcno
CLEANFILES += zmq/*.gcda zmq/*.gcno

DISTCLEANFILES = obj/build.h

EXTRA_DIST = leveldb

clean-local:
	-$(MAKE) -C leveldb clean
	-$(MAKE) -C secp256k1 clean
	-$(MAKE) -C univalue clean
	-rm -f leveldb/*/*.gcda leveldb/*/*.gcno leveldb/helpers/memenv/*.gcda leveldb/helpers/memenv/*.gcno
	-rm -f config/cash-config.* config/stamp-h1

.rc.o:
	@test -f $(WINDRES)
	$(AM_V_GEN) $(WINDRES) -DWINDRES_PREPROC -i $< -o $@

.mm.o:
	$(AM_V_CXX) $(OBJCXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) \
	  $(CPPFLAGS) $(AM_CXXFLAGS) $(QT_INCLUDES) $(AM_CXXFLAGS) $(PIE_FLAGS) $(CXXFLAGS) -c -o $@ $<

check-symbols: $(bin_PROGRAMS)
if GLIBC_BACK_COMPAT
	@echo "Checking glibc back compat..."
	$(AM_V_at) READELF=$(READELF) CPPFILT=$(CPPFILT) $(top_srcdir)/contrib/devtools/symbol-check.py < $(bin_PROGRAMS)
endif

check-security: $(bin_PROGRAMS)
if HARDEN
	@echo "Checking binary security..."
	$(AM_V_at) READELF=$(READELF) OBJDUMP=$(OBJDUMP) $(top_srcdir)/contrib/devtools/security-check.py < $(bin_PROGRAMS)
endif

if EMBEDDED_LEVELDB
include Makefile.leveldb.include
endif

if EMBEDDED_LIBTORRENT
include Makefile.libtorrent.include
#include Makefile.dht.test.include
endif

include Makefile.vgp.include

if ENABLE_GPU
include Makefile.gpu.include
endif

if ENABLE_TESTS
include Makefile.test.include
endif

if ENABLE_BENCH
include Makefile.bench.include
endif

if ENABLE_QT
include Makefile.qt.include
endif

if ENABLE_QT_TESTS
include Makefile.qttest.include
endif
