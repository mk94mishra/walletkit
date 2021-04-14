/*
 * Created by Michael Carrara <michael.carrara@breadwallet.com> on 5/31/18.
 * Copyright (c) 2018 Breadwinner AG.  All right reserved.
 *
 * See the LICENSE file at the project root for license information.
 * See the CONTRIBUTORS file at the project root for a list of contributors.
 */
package com.breadwallet.corenative.crypto;

public enum BRCryptoWalletManagerDisconnectReasonType {

    CRYPTO_WALLET_MANAGER_DISCONNECT_REASON_REQUESTED {
        @Override
        public int toCore() {
            return CRYPTO_WALLET_MANAGER_DISCONNECT_REASON_REQUESTED_VALUE;
        }
    },

    CRYPTO_WALLET_MANAGER_DISCONNECT_REASON_UNKNOWN {
        @Override
        public int toCore() {
            return CRYPTO_WALLET_MANAGER_DISCONNECT_REASON_UNKNOWN_VALUE;
        }
    },

    CRYPTO_WALLET_MANAGER_DISCONNECT_REASON_POSIX {
        @Override
        public int toCore() {
            return CRYPTO_WALLET_MANAGER_DISCONNECT_REASON_POSIX_VALUE;
        }
    };

    private static final int CRYPTO_WALLET_MANAGER_DISCONNECT_REASON_REQUESTED_VALUE = 0;
    private static final int CRYPTO_WALLET_MANAGER_DISCONNECT_REASON_UNKNOWN_VALUE = 1;
    private static final int CRYPTO_WALLET_MANAGER_DISCONNECT_REASON_POSIX_VALUE = 2;

    public static BRCryptoWalletManagerDisconnectReasonType fromCore(int nativeValue) {
        switch (nativeValue) {
            case CRYPTO_WALLET_MANAGER_DISCONNECT_REASON_REQUESTED_VALUE: return CRYPTO_WALLET_MANAGER_DISCONNECT_REASON_REQUESTED;
            case CRYPTO_WALLET_MANAGER_DISCONNECT_REASON_UNKNOWN_VALUE:   return CRYPTO_WALLET_MANAGER_DISCONNECT_REASON_UNKNOWN;
            case CRYPTO_WALLET_MANAGER_DISCONNECT_REASON_POSIX_VALUE:     return CRYPTO_WALLET_MANAGER_DISCONNECT_REASON_POSIX;
            default: throw new IllegalArgumentException("Invalid core value");
        }
    }

    public abstract int toCore();
}
