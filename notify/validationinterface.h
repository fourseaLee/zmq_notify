// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2017 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_VALIDATIONINTERFACE_H
#define BITCOIN_VALIDATIONINTERFACE_H

#include <functional>
#include <memory>
#include "dataindex.h"

class CValidationInterface {
protected:
    /**
     * Notifies listeners when the block chain tip advances.
     *
     * When multiple blocks are connected at once, UpdatedBlockTip will be called on the final tip
     * but may not be called on every intermediate tip. If the latter behavior is desired,
     * subscribe to BlockConnected() instead.
     *
     * Called on a background thread.
     */
    virtual void UpdatedDataTip(const CDataIndex *pindexNew, const CDataIndex *pindexFork, bool fInitialDownload) {}
    /**
     * Notifies listeners of a transaction having been added to mempool.
     *
     * Called on a background thread.
     */
    //virtual void TransactionAddedToMempool(const CTransactionRef &ptxn) {}
    /**
     * Notifies listeners of a transaction leaving mempool.
     *
     * This only fires for transactions which leave mempool because of expiry,
     * size limiting, reorg (changes in lock times/coinbase maturity), or
     * replacement. This does not include any transactions which are included
     * in BlockConnectedDisconnected either in block->vtx or in txnConflicted.
     *
     * Called on a background thread.
     */
    //virtual void TransactionRemovedFromMempool(const CTransactionRef &ptx) {}
    /**
     * Notifies listeners of a block being connected.
     * Provides a vector of transactions evicted from the mempool as a result.
     *
     * Called on a background thread.
     */
    //virtual void BlockConnected(const std::shared_ptr<const CBlock> &block, const CDataIndex *pindex, const std::vector<CTransactionRef> &txnConflicted) {}
    /**
     * Notifies listeners of a block being disconnected
     *
     * Called on a background thread.
     */
    //virtual void BlockDisconnected(const std::shared_ptr<const CBlock> &block) {}
    /**
     * Notifies listeners of the new active block chain on-disk.
     *
     * Called on a background thread.
     */
    //virtual void SetBestChain(const CBlockLocator &locator) {}
    /** Tells listeners to broadcast their data. */
    //virtual void ResendWalletTransactions(int64_t nBestBlockTime, CConnman* connman) {}
    /**
     * Notifies listeners of a block validation result.
     * If the provided CValidationState IsValid, the provided block
     * is guaranteed to be the current best block at the time the
     * callback was generated (not necessarily now)
     */
    //virtual void BlockChecked(const CBlock&, const CValidationState&) {}
    /**
     * Notifies listeners that a block which builds directly on our current tip
     * has been received and connected to the headers tree, though not validated yet */
    //rtual void NewPoWValidBlock(const CDataIndex *pindex, const std::shared_ptr<const CBlock>& block) {};
    //iend void ::RegisterValidationInterface(CValidationInterface*);
    //iend void ::UnregisterValidationInterface(CValidationInterface*);
    //iend void ::UnregisterAllValidationInterfaces();
};

#endif // BITCOIN_VALIDATIONINTERFACE_H
