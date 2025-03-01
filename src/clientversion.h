// Copyright (c) 2016-2021 Duality Blockchain Solutions Developers
// Copyright (c) 2014-2021 The Dash Core Developers
// Copyright (c) 2009-2021 The Bitcoin Developers
// Copyright (c) 2009-2021 Satoshi Nakamoto
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef CASH_CLIENTVERSION_H
#define CASH_CLIENTVERSION_H

#if defined(HAVE_CONFIG_H)
#include "config/cash-config.h"
#else

/**
 * client versioning and copyright year
 */

//! These need to be macros, as clientversion.cpp's and cash*-res.rc's voodoo requires it
#define CLIENT_VERSION_MAJOR 1
#define CLIENT_VERSION_MINOR 2
#define CLIENT_VERSION_REVISION 0
#define CLIENT_VERSION_BUILD 0

//! Set to true for release, false for prerelease or test build
#define CLIENT_VERSION_IS_RELEASE true

#define COPYRIGHT_YEAR 2021

#endif //HAVE_CONFIG_H

/**
 * Converts the parameter X to a string after macro replacement on X has been performed.
 * Don't merge these into one macro!
 */
#define STRINGIZE(X) DO_STRINGIZE(X)
#define DO_STRINGIZE(X) #X

//! Copyright string used in Windows .rc files
#define COPYRIGHT_STR "2009-" STRINGIZE(COPYRIGHT_YEAR) " Zero Dynamics, 2024-" STRINGIZE(COPYRIGHT_YEAR) " The Bitcoin Core Developers, 2015-"

/**
 * cashd-res.rc includes this file, but it cannot cope with real c++ code.
 * WINDRES_PREPROC is defined to indicate that its pre-processor is running.
 * Anything other than a define should be guarded below.
 */

#if !defined(WINDRES_PREPROC)

#include <string>
#include <vector>

static const int CLIENT_VERSION =
    1000000 * CLIENT_VERSION_MAJOR + 10000 * CLIENT_VERSION_MINOR + 100 * CLIENT_VERSION_REVISION + 1 * CLIENT_VERSION_BUILD;

extern const std::string CLIENT_NAME;
extern const std::string CLIENT_BUILD;


std::string FormatVersion(int nVersion);
std::string FormatFullVersion();
std::string FormatSubVersion(const std::string& name, int nClientVersion, const std::vector<std::string>& comments);

#endif // WINDRES_PREPROC

#endif // CASH_CLIENTVERSION_H
