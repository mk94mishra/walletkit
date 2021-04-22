//
//  BRCryptoListener.h
//  BRCrypto
//
//  Created by Ed Gamblle on 8/11/20.
//  Copyright © 2019 Breadwinner AG. All rights reserved.
//
//  See the LICENSE file at the project root for license information.
//  See the CONTRIBUTORS file at the project root for a list of contributors.

#ifndef BRCryptoListener_h
#define BRCryptoListener_h

#include "BRCryptoBase.h"
#include "event/BRCryptoTransfer.h"
#include "event/BRCryptoWallet.h"
#include "event/BRCryptoWalletManager.h"
#include "event/BRCryptoNetwork.h"
#include "event/BRCryptoSystem.h"


#ifdef __cplusplus
extern "C" {
#endif

// MARK: - Listener

typedef void (*BRCryptoListenerSystemCallback) (BRCryptoListenerContext context,
                                                BRCryptoSystem system,
                                                BRCryptoSystemEvent event);

typedef void (*BRCryptoListenerNetworkCallback) (BRCryptoListenerContext context,
                                                 BRCryptoNetwork network,
                                                 BRCryptoNetworkEvent event);

typedef void (*BRCryptoListenerWalletManagerCallback) (BRCryptoListenerContext context,
                                                       BRCryptoWalletManager manager,
                                                       BRCryptoWalletManagerEvent event);


typedef void (*BRCryptoListenerWalletCallback) (BRCryptoListenerContext context,
                                                BRCryptoWalletManager manager,
                                                BRCryptoWallet wallet,
                                                BRCryptoWalletEvent event);

typedef void (*BRCryptoListenerTransferCallback) (BRCryptoListenerContext context,
                                                  BRCryptoWalletManager manager,
                                                  BRCryptoWallet wallet,
                                                  BRCryptoTransfer transfer,
                                                  BRCryptoTransferEvent event);

extern BRCryptoListener
cryptoListenerCreate (BRCryptoListenerContext context,
                      BRCryptoListenerSystemCallback systemCallback,
                      BRCryptoListenerNetworkCallback networkCallback,
                      BRCryptoListenerWalletManagerCallback managerCallback,
                      BRCryptoListenerWalletCallback walletCallback,
                      BRCryptoListenerTransferCallback transferCallback);


DECLARE_CRYPTO_GIVE_TAKE (BRCryptoListener, cryptoListener);

#ifdef __cplusplus
}
#endif

#endif /* BRCryptoListener_h */
