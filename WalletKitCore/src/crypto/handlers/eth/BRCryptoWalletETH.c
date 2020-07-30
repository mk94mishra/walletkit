//
//  BRCryptoWalletETH.c
//  Core
//
//  Created by Ed Gamble on 05/07/2020.
//  Copyright © 2019 Breadwallet AG. All rights reserved.
//
//  See the LICENSE file at the project root for license information.
//  See the CONTRIBUTORS file at the project root for a list of contributors.
//
#include "BRCryptoETH.h"
#include "crypto/BRCryptoAmountP.h"

#define DEFAULT_ETHER_GAS_LIMIT    21000ull

extern BRCryptoWalletETH
cryptoWalletCoerce (BRCryptoWallet wallet) {
    assert (CRYPTO_NETWORK_TYPE_ETH == wallet->type);
    return (BRCryptoWalletETH) wallet;
}

private_extern BRCryptoWallet
cryptoWalletCreateAsETH (BRCryptoWalletListener listener,
                         BRCryptoUnit unit,
                         BRCryptoUnit unitForFee,
                         BREthereumToken   ethToken,
                         BREthereumAccount ethAccount) {
    BRCryptoWallet walletBase = cryptoWalletAllocAndInit (sizeof (struct BRCryptoWalletETHRecord),
                                                          CRYPTO_NETWORK_TYPE_ETH,
                                                          listener,
                                                          unit,
                                                          unitForFee,
                                                          NULL,
                                                          NULL);
    BRCryptoWalletETH wallet = cryptoWalletCoerce (walletBase);

    wallet->ethAccount  = ethAccount;
    wallet->ethToken    = ethToken;
    wallet->ethGasLimit = (NULL == ethToken
                           ? ethGasCreate (DEFAULT_ETHER_GAS_LIMIT)
                           : ethTokenGetGasLimit (ethToken));
    
    return walletBase;
}

static void
cryptoWalletReleaseETH (BRCryptoWallet wallet) {
}


static BRCryptoAddress
cryptoWalletGetAddressETH (BRCryptoWallet walletBase,
                           BRCryptoAddressScheme addressScheme) {
    BRCryptoWalletETH wallet = cryptoWalletCoerce(walletBase);
    BREthereumAddress ethAddress = ethAccountGetPrimaryAddress (wallet->ethAccount);

    return cryptoAddressCreateAsETH (ethAddress);
}

static bool
cryptoWalletHasAddressETH (BRCryptoWallet walletBase,
                           BRCryptoAddress address) {
    BRCryptoWalletETH wallet = cryptoWalletCoerce (walletBase);
    return (ETHEREUM_BOOLEAN_TRUE == ethAddressEqual (cryptoAddressAsETH (address),
                                                      ethAccountGetPrimaryAddress (wallet->ethAccount)));
}

extern size_t
cryptoWalletGetTransferAttributeCountETH (BRCryptoWallet wallet,
                                          BRCryptoAddress target) {
    return 0;
}

extern BRCryptoTransferAttribute
cryptoWalletGetTransferAttributeAtETH (BRCryptoWallet wallet,
                                       BRCryptoAddress target,
                                       size_t index) {
    return NULL;
}

extern BRCryptoTransferAttributeValidationError
cryptoWalletValidateTransferAttributeETH (BRCryptoWallet wallet,
                                          OwnershipKept BRCryptoTransferAttribute attribute,
                                          BRCryptoBoolean *validates) {
    *validates = CRYPTO_TRUE;
    return (BRCryptoTransferAttributeValidationError) 0;
}

static char *
cryptoTransferProvideOriginatingData (BREthereumTransferBasisType type,
                                      BREthereumAddress targetAddress,
                                      UInt256 value) {
    switch (type) {
        case TRANSFER_BASIS_TRANSACTION:
            return strdup ("");

        case TRANSFER_BASIS_LOG: {
            char address[ADDRESS_ENCODED_CHARS];
            ethAddressFillEncodedString (targetAddress, 0, address);

            // Data is a HEX ENCODED string
            return (char *) ethContractEncode (ethContractERC20, ethFunctionERC20Transfer,
                                               // Address
                                               (uint8_t *) &address[2], strlen(address) - 2,
                                               // Amount
                                               (uint8_t *) &value, sizeof (UInt256),
                                               NULL);
        }

        case TRANSFER_BASIS_EXCHANGE:
            return NULL;
    }
}

static BREthereumAddress
cryptoTransferProvideOriginatingTargetAddress (BREthereumTransferBasisType type,
                                               BREthereumAddress targetAddress,
                                               BREthereumToken   token) {
    switch (type) {
        case TRANSFER_BASIS_TRANSACTION:
            return targetAddress;
        case TRANSFER_BASIS_LOG:
            return ethTokenGetAddressRaw (token);
        case TRANSFER_BASIS_EXCHANGE:
            assert (0);
            return EMPTY_ADDRESS_INIT;
    }
}

static BREthereumEther
cryptoTransferProvideOriginatingAmount (BREthereumTransferBasisType type,
                                        UInt256 value) {
    switch (type) {
        case TRANSFER_BASIS_TRANSACTION:
            return ethEtherCreate(value);
        case TRANSFER_BASIS_LOG:
            return ethEtherCreateZero ();
        case TRANSFER_BASIS_EXCHANGE:
            assert (0);
            return ethEtherCreateZero();
    }
}

#if 0
static void
transferProvideOriginatingTransaction (BREthereumTransfer transfer) {
    if (NULL != transfer->originatingTransaction)
        transactionRelease (transfer->originatingTransaction);

    char *data = transferProvideOriginatingTransactionData(transfer);

    transfer->originatingTransaction =
    transactionCreate (transfer->sourceAddress,
                       transferProvideOriginatingTransactionTargetAddress (transfer),
                       transferProvideOriginatingTransactionAmount (transfer),
                       ethFeeBasisGetGasPrice(transfer->feeBasis),
                       ethFeeBasisGetGasLimit(transfer->feeBasis),
                       data,
                       TRANSACTION_NONCE_IS_NOT_ASSIGNED);
    free (data);
}
#endif
extern BRCryptoTransfer
cryptoWalletCreateTransferETH (BRCryptoWallet  wallet,
                               BRCryptoAddress target,
                               BRCryptoAmount  amount,
                               BRCryptoFeeBasis estimatedFeeBasis,
                               size_t attributesCount,
                               OwnershipKept BRCryptoTransferAttribute *attributes,
                               BRCryptoCurrency currency,
                               BRCryptoUnit unit,
                               BRCryptoUnit unitForFee) {
    BRCryptoWalletETH walletETH = cryptoWalletCoerce (wallet);
    assert (cryptoWalletGetType(wallet) == cryptoAddressGetType(target));
    assert (cryptoAmountHasCurrency (amount, currency));

    BREthereumToken    ethToken         = walletETH->ethToken;
    BREthereumFeeBasis ethFeeBasis      = cryptoFeeBasisAsETH (estimatedFeeBasis);

    BREthereumAddress  ethSourceAddress = ethAccountGetPrimaryAddress (walletETH->ethAccount);
    BREthereumAddress  ethTargetAddress = cryptoAddressAsETH (target);

    BREthereumTransferBasisType type = (NULL == ethToken ? TRANSFER_BASIS_TRANSACTION : TRANSFER_BASIS_LOG);

    UInt256 value = cryptoAmountGetValue (amount);
    char   *data  = cryptoTransferProvideOriginatingData (type, ethTargetAddress, value);

    BREthereumTransaction ethTransaction =
    transactionCreate (ethSourceAddress,
                       cryptoTransferProvideOriginatingTargetAddress (type, ethTargetAddress, ethToken),
                       cryptoTransferProvideOriginatingAmount (type, value),
                       ethFeeBasisGetGasPrice(ethFeeBasis),
                       ethFeeBasisGetGasLimit(ethFeeBasis),
                       data,
                       TRANSACTION_NONCE_IS_NOT_ASSIGNED);

    free (data);

    BRCryptoTransferDirection direction = (ETHEREUM_BOOLEAN_TRUE == ethAccountHasAddress (walletETH->ethAccount, ethTargetAddress)
                                           ? CRYPTO_TRANSFER_RECOVERED
                                           : CRYPTO_TRANSFER_SENT);

    BRCryptoAddress  source   = cryptoAddressCreateAsETH  (ethSourceAddress);

    BRCryptoTransfer transfer = cryptoTransferCreateAsETH (wallet->listenerTransfer,
                                                           unit,
                                                           unitForFee,
                                                           estimatedFeeBasis,
                                                           amount,
                                                           direction,
                                                           source,
                                                           target,
                                                           walletETH->ethAccount,
                                                           type,
                                                           ethTransaction);

    transfer->sourceAddress = cryptoAddressCreateAsETH (ethSourceAddress);
    transfer->targetAddress = cryptoAddressCreateAsETH (ethTargetAddress);
    transfer->feeBasisEstimated = cryptoFeeBasisCreateAsETH (unitForFee, transactionGetFeeBasisLimit(ethTransaction));

    if (NULL != transfer && attributesCount > 0) {
        BRArrayOf (BRCryptoTransferAttribute) transferAttributes;
        array_new (transferAttributes, attributesCount);
        array_add_array (transferAttributes, attributes, attributesCount);
        cryptoTransferSetAttributes (transfer, transferAttributes);
        array_free (transferAttributes);
    }

    cryptoAddressGive(source);

    return transfer;
}

static BRCryptoTransfer
cryptoWalletCreateTransferMultipleETH (BRCryptoWallet walletBase,
                                       size_t outputsCount,
                                       BRCryptoTransferOutput *outputs,
                                       BRCryptoFeeBasis estimatedFeeBasis,
                                       BRCryptoCurrency currency,
                                       BRCryptoUnit unit,
                                       BRCryptoUnit unitForFee) {
    BRCryptoWalletETH wallet = cryptoWalletCoerce (walletBase);
    (void) wallet;

    if (0 == outputsCount) return NULL;

    return NULL;
}

static OwnershipGiven BRSetOf(BRCryptoAddress)
cryptoWalletGetAddressesForRecoveryETH (BRCryptoWallet walletBase) {
    BRSetOf(BRCryptoAddress) addresses = cryptoAddressSetCreate (1);
    BRSetAdd (addresses, cryptoWalletGetAddressETH (walletBase, CRYPTO_ADDRESS_SCHEME_ETH_DEFAULT));
    return addresses;
}

extern BRCryptoTransferETH
cryptoWalletLookupTransferByIdentifier (BRCryptoWalletETH wallet,
                                        BREthereumHash hash) {
    if (ETHEREUM_BOOLEAN_IS_TRUE (ethHashEqual (hash, EMPTY_HASH_INIT))) return NULL;

    for (int i = 0; i < array_count(wallet->base.transfers); i++) {
        BRCryptoTransferETH transfer = cryptoTransferCoerce (wallet->base.transfers[i]);
        BREthereumHash identifier = cryptoTransferGetIdentifierETH(transfer);
        if (ETHEREUM_BOOLEAN_IS_TRUE (ethHashEqual (hash, identifier)))
            return transfer;
    }
    return NULL;
}

extern BRCryptoTransferETH
cryptoWalletLookupTransferByOriginatingHash (BRCryptoWalletETH wallet,
                                             BREthereumHash hash) {
    if (ETHEREUM_BOOLEAN_IS_TRUE (ethHashEqual (hash, EMPTY_HASH_INIT))) return NULL;

    for (int i = 0; i < array_count(wallet->base.transfers); i++) {
        BRCryptoTransferETH transfer = cryptoTransferCoerce (wallet->base.transfers[i]);
        BREthereumTransaction transaction = transfer->originatingTransaction;
        if (NULL != transaction && ETHEREUM_BOOLEAN_IS_TRUE (ethHashEqual (hash, transactionGetHash (transaction))))
            return transfer;
    }
    return NULL;
}

static bool
cryptoWalletIsEqualETH (BRCryptoWallet wb1, BRCryptoWallet wb2) {
    return wb1 == wb2;
}


#ifdef REFACTOR
private_extern void
walletUpdateBalance (BREthereumWallet wallet) {
    int overflow = 0, negative = 0, fee_overflow = 0;

    UInt256 recv = UINT256_ZERO;
    UInt256 sent = UINT256_ZERO;
    UInt256 fees = UINT256_ZERO;

    for (size_t index = 0; index < array_count (wallet->transfers); index++) {
        BREthereumTransfer transfer = wallet->transfers[index];
        BREthereumAmount   amount = transferGetAmount(transfer);
        assert (ethAmountGetType(wallet->balance) == ethAmountGetType(amount));
        UInt256 value = (AMOUNT_ETHER == ethAmountGetType(amount)
                         ? ethAmountGetEther(amount).valueInWEI
                         : ethAmountGetTokenQuantity(amount).valueAsInteger);

        // Will be ZERO if transfer is not for ETH
        BREthereumEther fee = transferGetFee(transfer, &fee_overflow);

        BREthereumBoolean isSend = ethAddressEqual(wallet->address, transferGetSourceAddress(transfer));
        BREthereumBoolean isRecv = ethAddressEqual(wallet->address, transferGetTargetAddress(transfer));

        if (ETHEREUM_BOOLEAN_IS_TRUE (isSend)) {
            sent = uint256Add_Overflow(sent, value, &overflow);
            assert (!overflow);

            fees = uint256Add_Overflow(fees, fee.valueInWEI, &fee_overflow);
            assert (!fee_overflow);
        }

        if (ETHEREUM_BOOLEAN_IS_TRUE (isRecv)) {
            recv = uint256Add_Overflow(recv, value, &overflow);
            assert (!overflow);
        }
    }

    // A wallet balance can never be negative; however, as transfers arrive in a sporadic manner,
    // the balance could be negative until all transfers arrive, eventually.  If negative, we'll
    // set the balance to zero.
    UInt256 balance = uint256Sub_Negative(recv, sent, &negative);

    // If we are going to be changing the balance here then 1) shouldn't we call walletSetBalance()
    // and shouldn't we also ensure that an event is generated (like all calls to
    // walletSetBalance() ensure)?

    if (AMOUNT_ETHER == ethAmountGetType(wallet->balance)) {
        balance = uint256Sub_Negative(balance, fees, &negative);
        if (negative) balance = UINT256_ZERO;
        wallet->balance = ethAmountCreateEther (ethEtherCreate(balance));
    }
    else {
        if (negative) balance = UINT256_ZERO;
        wallet->balance = ethAmountCreateToken (ethTokenQuantityCreate(ethAmountGetToken (wallet->balance), balance));
    }
}

extern uint64_t
walletGetTransferCountAsSource (BREthereumWallet wallet) {
    unsigned int count = 0;

    for (int i = 0; i < array_count(wallet->transfers); i++)
         if (ETHEREUM_BOOLEAN_IS_TRUE(ethAddressEqual(wallet->address, transferGetSourceAddress(wallet->transfers[i]))))
             count += 1;

    return count;
}

extern uint64_t
walletGetTransferNonceMaximumAsSource (BREthereumWallet wallet) {
    uint64_t nonce = TRANSACTION_NONCE_IS_NOT_ASSIGNED;

#define MAX(x,y)    ((x) >= (y) ? (x) : (y))
    for (int i = 0; i < array_count(wallet->transfers); i++)
        if (ETHEREUM_BOOLEAN_IS_TRUE(ethAddressEqual(wallet->address, transferGetSourceAddress(wallet->transfers[i])))) {
            uint64_t newNonce = (unsigned int) transferGetNonce(wallet->transfers[i]);
            // wallet->transfers can have a newly created transfer that does not yet have
            // an assigned nonce - avoid such a transfer.
            if ( TRANSACTION_NONCE_IS_NOT_ASSIGNED != newNonce  &&
                (TRANSACTION_NONCE_IS_NOT_ASSIGNED == nonce     || newNonce > nonce))
                nonce = (unsigned int) newNonce;
        }
#undef MAX
    return nonce;
}

#endif

BRCryptoWalletHandlers cryptoWalletHandlersETH = {
    cryptoWalletReleaseETH,
    cryptoWalletGetAddressETH,
    cryptoWalletHasAddressETH,
    cryptoWalletGetTransferAttributeCountETH,
    cryptoWalletGetTransferAttributeAtETH,
    cryptoWalletValidateTransferAttributeETH,
    cryptoWalletCreateTransferETH,
    cryptoWalletCreateTransferMultipleETH,
    cryptoWalletGetAddressesForRecoveryETH,
    cryptoWalletIsEqualETH
};