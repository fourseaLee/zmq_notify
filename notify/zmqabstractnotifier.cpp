// Copyright (c) 2015-2017 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "zmqabstractnotifier.h"
//#include <util.h>
#include "dataindex.h"
#include <assert.h>


CZMQAbstractNotifier::~CZMQAbstractNotifier()
{
    assert(!psocket);
}

bool CZMQAbstractNotifier::NotifyData(const CDataIndex * /*CBlockIndex*/)
{
    return true;
}

//bool CZMQAbstractNotifier::NotifyTransaction(const CTransaction &/*transaction*/)
//{
//    return true;
//}
