/** 
 * Algoduino.h - Algorand IoT library for Arduino 
 * Copyright (c) 2020 Algoduino.  All right reserved.
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#ifndef Algoduino_h
#define Algoduino_h

#include "Arduino.h"
#include "AlgoduinoTypes.h"

/**
 *  Algoduino class. 
 */
class Algoduino
{
public:
  /**
   * @brief Initialize the connection.
   * @param ssid Wifi network SSID.
   * @param password Wifi network password.
   * @param apiKey API key to connect to a node.
   * @param network Algorand network: BETANET, TESTNET or MAINNET.   
   */
  Algoduino(AlgoduinoInit algoduinoInit);

  /**
   * @brief Start the Wifi connection.
   */
  void begin(void);

  /**
   * @brief Get the network health.
   * @return String containing the response.
   */
  String getHealth(void);

  /**
   * @brief Get the swagger.json definition file.
   * @return String containing the response.
   */
  String getSwaggerJson(void);

  /**
   * @brief Get account information.
   * @param address An account address.
   * @return String containing the response.
   */
  String getAccountInformation(String address);

  /**
   * @brief Get a list of unconfirmed transactions currently in the transaction pool by address.
   * @param address An account address.
   * @return String containing a list of unconfirmed transactions by address.
   */
  String getPendingTransactionsList(String address);

  /**
   * @brief Get a specific application.
   * @param applicationId An application id.
   * @return String containing the response.
   */
  String getApplication(String applicationId);

  /**
   * @brief Get the block for the given round.
   * @param round The round from which to fetch block information.
   * @return String containing the block information.
   */
  String getBlock(String round);

  /**
   * @brief Get the current supply reported by the ledger.
   * @return String containing the current supply information.
   */
  String getLedgerSupply(void);

  /**
   * @brief Gets the current node status.
   * @return String containing the current node status information.
   */
  String getStatus(void);

  /**
   * @brief Gets the node status after waiting for the given round.
   * @param round The round from which to fetch block information.
   * @return String containing the node status information.
   */
  String getStatus(String round);

  /**
   * @brief Get parameters for constructing a new transaction.
   * @return String containing the transaction parameters information.
   */
  //String getTransactionParams(void);

  /**
   * @brief Get parameters for constructing a new transaction.
   * @return TransactionParams structure containing the transaction parameters information.
   */
  TransactionParams getTransactionParams(void);

  /**
   * @brief Get a list of unconfirmed transactions currently in the transaction pool.
   * @return String containing a list of unconfirmed transactions.
   */
  String getPendingTransactionsList(void);

  /**
   * @brief Get a specific pending transaction.
   * @param txid A transaction id.
   * @return String containing the pending transaction information.
   */
  String getPendingTransactionInformation(String txid);

  void sha_512_256(const void *data, size_t dataLen, void *hash, size_t hashLen);

  unsigned int decodeFromBase32(byte *input, long inputLength, byte *&output);

  unsigned int encodeToBase32(byte *input, long inputLength, byte *&output, boolean usePadding);

  unsigned int encodeToBase64(byte *input, long inputLength, byte *output);

  void createWallet(AlgorandWallet *algorandWallet);

  void setWallet(AlgorandWallet algorandWallet);

  void printWallet(AlgorandWallet algorandWallet);

  void printTransactionObject(TransactionObject transactionObject);

  unsigned int packMessage(txn_t *current_txn, byte *msgpack_buf);

  void signMessage(byte *signature, byte *msgpacked, unsigned int msg_len);

  void createTransactionBlob(byte* note, unsigned int noteLength, byte *blob, unsigned int *blobLength);

  void sendRawTransaction(byte *blob, int blobLength);

private:
  /// Wifi network SSID.
  String _ssid;

  /// Wifi network password.
  String _password;

  /// API key to connect to a node.
  String _apiKey;

  /// Requested endpoint URL.
  String _endpoint;

  /// Fingerprint of the API provider.
  char *_fingerprint;

  /// Algorand Wallet.
  AlgorandWallet _algorandWallet;

  /// Transaction Object.
  //TransactionObject _transactionObject;

  /**
   * @brief Fetch the requested information from the API.
   * @param route API route.
   * @return String containing the requested information.
   */
  String _fetch(String route);

  void _post(String route, String contentType, uint8_t *payload, size_t size);
};

#endif
