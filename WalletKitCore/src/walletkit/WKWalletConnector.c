//
//  WKWalletConnector.c
//  WalletKitCore
//
//  Created by Bryan Goring on 8/23/21
//  Copyright © 2021 Breadwinner AG. All rights reserved.
//
//  See the LICENSE file at the project root for license information.
//  See the CONTRIBUTORS file at the project root for a list of contributors.

#include "WKWalletConnectorP.h"
#include "WKHandlersP.h"

private_extern WKWalletConnector
wkWalletConnectorAllocAndInit (size_t sizeInBytes,
                               WKNetworkType type,
                               WKWalletManager manager) {
    WKWalletConnector connector = calloc (1, sizeof (struct WKWalletConnectorRecord));

    connector->type = type;
    connector->handlers = wkHandlersLookup(type)->connector;
    connector->sizeInBytes = sizeInBytes;

    connector->manager = wkWalletManagerTake (manager);
    
    return connector;
}

extern WKWalletConnector
wkWalletConnectorCreate (WKWalletManager manager) {
    const WKHandlers *handlers = wkHandlersLookup (manager->type);

    return (NULL != handlers &&
            NULL != handlers->connector &&
            NULL != handlers->connector->create
            ? handlers->connector->create (manager)
            : NULL);
}

extern void
wkWalletConnectorRelease (WKWalletConnector connector) {
    if (NULL != connector->handlers->release)
        connector->handlers->release (connector);

    wkWalletManagerGive (connector->manager);
    memset (connector, 0, connector->sizeInBytes);
    free (connector);
}

extern uint8_t*
wkWalletConnectorGetDigest (
        WKWalletConnector       connector,
        const uint8_t           *msg,
        size_t                  msgLen,
        WKBoolean               addPrefix,
        size_t                  *digestLength,
        WKWalletConnectorError  *err            ) {

    *err = WK_WALLET_CONNECTOR_ERROR_IS_UNDEFINED;

    return NULL;
}

extern uint8_t*
wkWalletConnectorSignData   (
        WKWalletConnector         connector,
        const uint8_t*            data,
        size_t                    dataLen,
        WKKey                     key,
        size_t                    *signatureLength,
        WKWalletConnectorError    *err            ) {

    *err = WK_WALLET_CONNECTOR_ERROR_IS_UNDEFINED;

    return NULL;
}

extern uint8_t*
wkWalletConnectorCreateTransactionFromArguments  (
        WKWalletConnector         connector,
        const char*               keys,
        const char*               values,
        size_t*                   serializationLength,
        WKWalletConnectorError    *err            ) {

    *err = WK_WALLET_CONNECTOR_ERROR_IS_UNDEFINED;

    return NULL;
}

extern uint8_t*
wkWalletConnectorCreateTransactionFromSerialization  (
        WKWalletConnector       connector,
        uint8_t                 *data,
        size_t                  dataLength,
        size_t                  *serializationLength,
        WKBoolean               *isSigned,
        WKWalletConnectorError  *err           ) {

    *err = WK_WALLET_CONNECTOR_ERROR_IS_UNDEFINED;

    return NULL;
}
