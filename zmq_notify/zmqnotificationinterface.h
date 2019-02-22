// Copyright (c) 2015-2017 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_ZMQ_ZMQNOTIFICATIONINTERFACE_H
#define BITCOIN_ZMQ_ZMQNOTIFICATIONINTERFACE_H

#include <validationinterface.h>
#include <string>
#include <map>
#include <list>

class CDataIndex;
class CZMQAbstractNotifier;

class CZMQNotificationInterface final : public CValidationInterface
{
public:
    virtual ~CZMQNotificationInterface();

    static CZMQNotificationInterface* Create();

protected:
    bool Initialize();
    void Shutdown();

    // CValidationInterface

   void UpdatedDataTip(const CDataIndex *pindexNew, const CDataIndex *pindexFork, bool fInitialDownload) override;
private:
    CZMQNotificationInterface();

    void *pcontext;
    std::list<CZMQAbstractNotifier*> notifiers;
};

#endif // BITCOIN_ZMQ_ZMQNOTIFICATIONINTERFACE_H
