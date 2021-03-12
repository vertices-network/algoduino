/** 
 * AlgoduinoTypes.h - Algorand IoT library for Arduino 
 * Copyright (c) 2020 Algoduino.  All right reserved.
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#ifndef AlgoduinoTypes_h
#define AlgoduinoTypes_h

#include "Arduino.h"

/** 
 * Algorand network enum.
 */
enum Network
{
  BETANET = 0,
  TESTNET,
  MAINNET
};

/** 
 * @brief Algoduino initialization structure.
 * @param ssid Wifi network SSID.
 * @param password Wifi network password.
 * @param apiKey API key to connect to a node.
 * @param network Algorand network: BETANET, TESTNET or MAINNET. 
 * @param fingerprint Fingerprint of the API provider..
 */
typedef struct
{
  String ssid;
  String password;
  String apiKey;
  Network network;
  String fingerprint;
} AlgoduinoInit;

/** 
 * @brief Algorand Wallet structure.
 * @param privateKey Wifi network SSID.
 * @param publicKey Wifi network password.
 * @param algorandAddress API key to connect to a node.
 * @param base64PrivateKey Algorand network: BETANET, TESTNET or MAINNET. 
 */
typedef struct
{
  byte privateKey[32];
  byte publicKey[32];
  byte algorandAddress[58];
  byte base64PrivateKey[88];
} AlgorandWallet;

/** 
 * @brief TransactionParams structure.
 * @param fee Wifi network SSID.
 * @param genesisID Wifi network password.
 * @param genesisHash API key to connect to a node.
 * @param lastRound Algorand network: BETANET, TESTNET or MAINNET. 
 * @param consensusVersion Fingerprint of the API provider..
 * @param minFee Fingerprint of the API provider..
 */
typedef struct
{
  int fee;
  String genesisID;
  String genesisHash;
  long lastRound;
  String consensusVersion;
  int minFee;
} TransactionParams;

/** 
 * @brief TransactionObject structure.
 * @param from Wifi network SSID.
 * @param to Wifi network password.
 * @param fee API key to connect to a node.
 * @param amount Algorand network: BETANET, TESTNET or MAINNET. 
 * @param firstRound Fingerprint of the API provider..
 * @param lastRound Wifi network SSID.
 * @param genesisID Wifi network password.
 * @param genesisHash API key to connect to a node.
 * @param note Algorand network: BETANET, TESTNET or MAINNET. 
 * @param type Fingerprint of the API provider..
 */
typedef struct
{
  String from;
  String to;
  int fee;
  int amount;
  int firstRound;
  int lastRound;
  String genesisID;
  String genesisHash;
  String note;
  String type;
} TransactionObject;

////////////////////////////////////////////////////

enum TXTYPE
{
    UNKNOWN,
    PAYMENT,
    KEYREG,
    ASSET_XFER,
    ASSET_FREEZE,
    ASSET_CONFIG,
    ALL_TYPES,
};

struct asset_params
{
    uint64_t total;
    uint64_t decimals;
    unsigned char default_frozen;
    char unitname[8];
    char assetname[32];
    char url[32];
    unsigned char metadata_hash[32];
    unsigned char manager[32];
    unsigned char reserve[32];
    unsigned char freeze[32];
    unsigned char clawback[32];
};

struct txn_payment
{
    unsigned char receiver[32];
    uint64_t amount;
    unsigned char close[32];
};

struct txn_keyreg
{
    unsigned char votepk[32];
    unsigned char vrfpk[32];
    uint64_t voteFirst;
    uint64_t voteLast;
    uint64_t keyDilution;
    unsigned char nonpartFlag;
};

struct txn_asset_xfer
{
    uint64_t id;
    uint64_t amount;
    unsigned char sender[32];
    unsigned char receiver[32];
    unsigned char close[32];
};

struct txn_asset_freeze
{
    uint64_t id;
    unsigned char account[32];
    unsigned char flag;
};

struct txn_asset_config
{
    uint64_t id;
    struct asset_params params;
};

typedef struct
{
    enum TXTYPE type;
    // Account Id asscociated with this transaction.
    uint32_t accountId;

    // Common header fields
    unsigned char sender[32];
    unsigned char rekey[32];
    uint64_t fee;
    uint64_t firstValid;
    uint64_t lastValid;
    char genesisID[32];
    unsigned char genesisHash[32];

    unsigned char note[80];

    // Fields for specific tx types
    union
    {
        struct txn_payment payment;
        struct txn_keyreg keyreg;
        struct txn_asset_xfer asset_xfer;
        struct txn_asset_freeze asset_freeze;
        struct txn_asset_config asset_config;
    };
} txn_t;

struct SignedTxn {
  byte signature[64];
  txn_t txn; 
};

#endif

/*




SIG 82A3736967C440

{SIG} 7F00A8433E7C17CD330DB13A3D7F2FC64D14863C9EBC8BB15284443F37E96DC0580CD76E6E30030F8E6524E06CD68CE8E4292F1BDEEBE61D22DA8BF2790D3205

TXN A374786E

{TXN} 89A3666565CD03E8A26676CE0095D30AA367656EAC746573746E65742D76312E30A26768C4204863B518A4B3C84EC810F22D4F1081CB0F71F059A7AC20DEC62F7F70E5093A22A26C76CE0095D6F2A46E6F7465C4046232733DA3726376C420619858BDBE3203D18A6BCD0706A3951C8DD6ED5EB080AADC53EC975587B5CC61A3736E64C420619858BDBE3203D18A6BCD0706A3951C8DD6ED5EB080AADC53EC975587B5CC61A474797065A3706179
*/


