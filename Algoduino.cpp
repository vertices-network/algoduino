/*
 * Algoduino.cpp - Algorand IoT library for Arduino 
 * Copyright (c) 2020 Algoduino.  All right reserved.
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
*/

#include "Algoduino.h"
#include <Arduino.h>

#define ARDUINOJSON_USE_LONG_LONG 1
#include <ArduinoJson.h>

#ifdef ESP8266
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#else
#include <WiFi.h>
#include <HTTPClient.h>
#endif

#include <Crypto.h>
#include <Ed25519.h>
#include <SHA512.h>
#include <RNG.h>
#include <utility/ProgMemUtil.h>
#include <string.h>
#include <Base32.h>
#include <base64.hpp>

#include "algo_tx.h"

#define FINGERPRINT_LENGTH 59

Algoduino::Algoduino(AlgoduinoInit algoduinoInit)
{
  _ssid = algoduinoInit.ssid;
  _password = algoduinoInit.password;
  _apiKey = algoduinoInit.apiKey;
  //_fingerprint = fingerprint.c_str();
  algoduinoInit.fingerprint.toCharArray(_fingerprint, FINGERPRINT_LENGTH);

  switch (algoduinoInit.network)
  {
    /*
  case BETANET:
    _endpoint = "https://betanet-algorand.api.purestake.io/ps1";
    break;
    */
  case TESTNET:
    _endpoint = "https://testnet-algorand.api.purestake.io/ps2";
    break;
    /*
  case MAINNET:
    _endpoint = "https://mainnet-algorand.api.purestake.io/ps1";
    break;
    */
  default:
    _endpoint = "https://testnet-algorand.api.purestake.io/ps2";
  }
}

void Algoduino::begin(void)
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(_ssid.c_str(), _password.c_str());

  Serial.begin(115200);
}

String Algoduino::getHealth(void)
{
  return _fetch("/health");
}

String Algoduino::getSwaggerJson(void)
{
  return _fetch("/swagger.json");
}

String Algoduino::getAccountInformation(String address)
{
  return _fetch("/v2/accounts/" + address);
}

String Algoduino::getPendingTransactionsList(String address)
{
  return _fetch("/v2/accounts/" + address + "/transactions/pending");
}

String Algoduino::getApplication(String applicationId)
{
  return _fetch("/v2/applications/" + applicationId);
}

String Algoduino::getBlock(String round)
{
  return _fetch("/v2/blocks/" + round);
}

String Algoduino::getLedgerSupply(void)
{
  return _fetch("/v2/ledger/supply");
}

String Algoduino::getStatus(void)
{
  return _fetch("/v2/status");
}

String Algoduino::getStatus(String round)
{
  return _fetch("/v2/status/wait-for-block-after/" + round);
}

/*
String Algoduino::getTransactionParams(void)
{
  return _fetch("/v2/transactions/params");
}
*/

TransactionParams Algoduino::getTransactionParams(void)
{
  TransactionParams transactionParams;

  const size_t capacity = JSON_OBJECT_SIZE(6) + 240;
  DynamicJsonDocument doc(capacity);

  String json = _fetch("/v2/transactions/params");

  deserializeJson(doc, json);

  transactionParams.fee = doc["fee"];
  transactionParams.genesisID = doc["genesis-id"].as<String>();
  transactionParams.genesisHash = doc["genesis-hash"].as<String>();
  transactionParams.lastRound = doc["last-round"];
  transactionParams.consensusVersion = doc["consensus-version"].as<String>();
  transactionParams.minFee = doc["min-fee"];

  return transactionParams;
}

String Algoduino::getPendingTransactionsList(void)
{
  return _fetch("/v2/transactions/pending");
}

String Algoduino::getPendingTransactionInformation(String txid)
{
  return _fetch("/v2/transactions/pending/" + txid);
}

String Algoduino::_fetch(String route)
{
  String payload = "[void]";

  if ((WiFi.status() == WL_CONNECTED))
  {
    HTTPClient https;

    if (https.begin(_endpoint + route, _fingerprint))
    {
      https.addHeader("accept", "application/json");
      https.addHeader("x-api-key", _apiKey);

      int httpCode = https.GET();

      if (httpCode > 0)
      {
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)
        {
          payload = https.getString();
        }
      }

      https.end();
    }
  }

  return payload;
}

void Algoduino::_post(String route, String contentType, uint8_t *payload, size_t size)
{
  if ((WiFi.status() == WL_CONNECTED))
  {
    HTTPClient https;

    if (https.begin(_endpoint + route, _fingerprint))
    {
      https.addHeader("Content-Type", contentType);
      https.addHeader("x-api-key", _apiKey);

      // Send HTTP POST request
      int httpResponseCode = https.POST(payload, size);

      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);

      //Get the response to the request
      String response = https.getString();
      Serial.println(response);

      https.end();
    }
  }
  else
  {
    Serial.println("ERROR: WiFi Disconnected");
    Serial.println("");
  }
}

void Algoduino::sha_512_256(const void *data, size_t dataLen, void *hash, size_t hashLen)
{
  SHA512 sha512;
  sha512.reset256();
  sha512.update(data, dataLen);
  sha512.finalize(hash, hashLen);
}

void printNumber(const char *name, const uint8_t *x, uint8_t len)
{
  static const char hexchars[] = "0123456789ABCDEF";
  Serial.print(name);
  Serial.print(" = ");
  for (uint8_t posn = 0; posn < len; ++posn)
  {
    Serial.print(hexchars[(x[posn] >> 4) & 0x0F]);
    Serial.print(hexchars[x[posn] & 0x0F]);
  }
  Serial.println();
}

unsigned int Algoduino::decodeFromBase32(byte *input, long inputLength, byte *&output)
{
  // !!! --> output must be initialized as: byte *output = NULL;
  Base32 base32;
  return base32.fromBase32(input, inputLength, output);
  /*
  Serial.println("");
  Serial.print("decode - outputLength = ");
  Serial.print(outputLength);
  Serial.println("");
  */
}

unsigned int Algoduino::encodeToBase32(byte *input, long inputLength, byte *&output, boolean usePadding)
{
  // !!! --> output must be initialized as: byte *output = NULL;
  Base32 base32;
  return base32.toBase32(input, inputLength, output, usePadding);
  /*
  Serial.println("");
  Serial.print("encode - outputLength = ");
  Serial.print(outputLength);
  Serial.println("");
  */
}

unsigned int Algoduino::encodeToBase64(byte *input, long inputLength, byte *output)
{
  encode_base64(input, 64, output);
}

void Algoduino::createWallet(AlgorandWallet *algorandWallet)
{
  RNG.begin("Create Wallet");

  byte publicKeyHash[32];
  byte last4bytes[4];
  byte concatenateKeys[36];

  Ed25519::generatePrivateKey(algorandWallet->privateKey);
  Ed25519::derivePublicKey(algorandWallet->publicKey, algorandWallet->privateKey);

  sha_512_256(algorandWallet->publicKey, 32, publicKeyHash, 32);

  for (int i = 0; i < 4; i++)
  {
    last4bytes[i] = publicKeyHash[i + 28];
  }

  for (int i = 0; i < 32; i++)
  {
    concatenateKeys[i] = algorandWallet->publicKey[i];
  }

  for (int i = 0; i < 4; i++)
  {
    concatenateKeys[i + 32] = last4bytes[i];
  }

  byte *tempEncoded = NULL;
  encodeToBase32(concatenateKeys, 36, (byte *&)tempEncoded, false);

  for (int i = 0; i < 58; i++)
  {
    algorandWallet->algorandAddress[i] = tempEncoded[i];
  }

  byte privateKeyConcatenated[64];
  byte output[128];

  for (int i = 0; i < 32; i++)
  {
    privateKeyConcatenated[i] = algorandWallet->privateKey[i];
  }

  for (int i = 0; i < 32; i++)
  {
    privateKeyConcatenated[i + 32] = algorandWallet->publicKey[i];
  }

  encode_base64(privateKeyConcatenated, 64, output);

  for (int i = 0; i < 88; i++)
  {
    algorandWallet->base64PrivateKey[i] = output[i];
  }
}

void Algoduino::setWallet(AlgorandWallet algorandWallet)
{
  memcpy(_algorandWallet.privateKey, algorandWallet.privateKey, sizeof(algorandWallet.privateKey));
  memcpy(_algorandWallet.publicKey, algorandWallet.publicKey, sizeof(algorandWallet.publicKey));
  memcpy(_algorandWallet.algorandAddress, algorandWallet.algorandAddress, sizeof(algorandWallet.algorandAddress));
  memcpy(_algorandWallet.base64PrivateKey, algorandWallet.base64PrivateKey, sizeof(algorandWallet.base64PrivateKey));
}

void Algoduino::printWallet(AlgorandWallet algorandWallet)
{
  Serial.println();
  Serial.println("*** SET OF 32-bytes KEYS");

  printNumber("privateKey ", algorandWallet.privateKey, 32);
  printNumber("publicKey  ", algorandWallet.publicKey, 32);
  Serial.println();

  Serial.println("*** ALGORAND WALLET GENERATED");
  Serial.print("Algorand Address = ");
  Serial.printf("%.*s", 58, algorandWallet.algorandAddress);
  Serial.println();

  Serial.print("Base64 Private Key = ");
  Serial.printf("%.*s", 88, algorandWallet.base64PrivateKey);
  Serial.println();
  Serial.println();
}

void Algoduino::printTransactionObject(TransactionObject transactionObject)
{
  Serial.println();
  Serial.println("transactionObject.from = " + transactionObject.from);
  Serial.println("transactionObject.to = " + transactionObject.to);
  Serial.println("transactionObject.fee = " + String(transactionObject.fee));
  Serial.println("transactionObject.amount = " + String(transactionObject.amount));
  Serial.println("transactionObject.firstRound = " + String(transactionObject.firstRound));
  Serial.println("transactionObject.lastRound = " + String(transactionObject.lastRound));
  Serial.println("transactionObject.genesisID = " + transactionObject.genesisID);
  Serial.println("transactionObject.genesisHash = " + transactionObject.genesisHash);
  Serial.println("transactionObject.note = " + transactionObject.note);
  Serial.println("transactionObject.type = " + transactionObject.type);
  Serial.println();
}

unsigned int Algoduino::packMessage(txn_t *current_txn, byte *msgpacked)
{
  byte msgpack_buf[900];
  unsigned int msg_len;

  /*
  msgpack_buf[0] = 'T';
  msgpack_buf[1] = 'X';

  msg_len = 2 + tx_encode(current_txn, msgpack_buf + 2, sizeof(msgpack_buf) - 2);
*/

  msg_len = tx_encode(current_txn, msgpack_buf, sizeof(msgpack_buf));

/*
  Serial.print("msg_len = ");
  Serial.println(msg_len);
  Serial.println("");
*/

  for (int i = 0; i < msg_len; i++)
  {
    msgpacked[i] = msgpack_buf[i];
  }

  return msg_len;
}

void Algoduino::signMessage(byte *signature, byte *msgpacked, unsigned int msg_len)
{
  byte toSign[msg_len + 2];
  toSign[0] = 'T';
  toSign[1] = 'X';

  for (int i = 0; i < msg_len; i++)
  {
    toSign[i + 2] = msgpacked[i];
  }

  Ed25519::sign(signature, _algorandWallet.privateKey, _algorandWallet.publicKey, toSign, sizeof(toSign));

  if (!Ed25519::verify(signature, _algorandWallet.publicKey, toSign, sizeof(toSign)))
  {
    // The signature is invalid.
    Serial.println("SIGNATURE NOT VALID!");
  }
}

void Algoduino::createTransactionBlob(byte* note, unsigned int noteLength, byte *blob, unsigned int *blobLength)
{
  TransactionParams transactionParams = getTransactionParams();

  txn_t current_txn;

  /// "fee"
  current_txn.fee = 1000;

  /// "fv"
  current_txn.firstValid = transactionParams.lastRound;

  /// "gen"
  String genesisID = transactionParams.genesisID;
  char genesisIDChar[32];
  genesisID.toCharArray(genesisIDChar, 32);
  memcpy(current_txn.genesisID, genesisIDChar, 32);

  /// "gh"
  byte genesisHashByte[32] = {0x48, 0x63, 0xb5, 0x18, 0xa4, 0xb3, 0xc8, 0x4e,
                              0xc8, 0x10, 0xf2, 0x2d, 0x4f, 0x10, 0x81, 0xcb,
                              0x0f, 0x71, 0xf0, 0x59, 0xa7, 0xac, 0x20, 0xde,
                              0xc6, 0x2f, 0x7f, 0x70, 0xe5, 0x09, 0x3a, 0x22};
                              
  memcpy(current_txn.genesisHash, genesisHashByte, 32);

  /// "lv"
  current_txn.lastValid = current_txn.firstValid + 1000;

  /// "note"
  memcpy(current_txn.note, note, noteLength);

  /// "rcv"
  memcpy(current_txn.payment.receiver, _algorandWallet.publicKey, sizeof(_algorandWallet.publicKey));

  /// "snd"
  memcpy(current_txn.sender, _algorandWallet.publicKey, sizeof(_algorandWallet.publicKey));

  /// "type"
  current_txn.type = PAYMENT;

  byte msgpacked[900];
  unsigned int msg_len = packMessage(&current_txn, msgpacked);

  printWallet(_algorandWallet);

  /*
  Serial.println("fee: " + String(transactionParams.fee));
  Serial.println("genesisID: " + transactionParams.genesisID);
  Serial.println("genesisHash: " + transactionParams.genesisHash);
  Serial.println("lastRound: " + String(transactionParams.lastRound));

  printNumber("msgpack_buf ", msgpacked, msg_len);
*/

  byte sig[64];
  byte txn[msg_len];

  for (int i = 0; i < msg_len; i++)
  {
    txn[i] = msgpacked[i];
  }

  printNumber("txn ", txn, sizeof(txn));

  signMessage(sig, txn, sizeof(txn));
  printNumber("sig ", sig, 64);

  byte output[128];
  encodeToBase64(sig, 64, output);
  Serial.print("Base64 Signature Key = ");
  Serial.printf("%.*s", 88, output);

  // 82 A3 73 69 67 C4 40
  unsigned char sigHeader[7] = {0x82, 0xA3, 0x73, 0x69, 0x67, 0xC4, 0x40};

  // A3 74 78 6E
  unsigned char txnHeader[4] = {0xA3, 0x74, 0x78, 0x6E};

  unsigned int blobSize = sizeof(sigHeader) + sizeof(sig) + sizeof(txnHeader) + sizeof(txn);

  *blobLength = blobSize;

  //unsigned char blob[blobSize];

  for (int i = 0; i < sizeof(sigHeader); i++)
  {
    blob[i] = sigHeader[i];
  }

  for (int i = 0; i < sizeof(sig); i++)
  {
    blob[i + sizeof(sigHeader)] = sig[i];
  }

  for (int i = 0; i < sizeof(txnHeader); i++)
  {
    blob[i + sizeof(sigHeader) + sizeof(sig)] = txnHeader[i];
  }

  for (int i = 0; i < sizeof(txn); i++)
  {
    blob[i + sizeof(sigHeader) + sizeof(sig) + sizeof(txnHeader)] = txn[i];
  }

  Serial.println("");
  printNumber("blob ", blob, blobSize);
  Serial.println("");
}

void Algoduino::sendRawTransaction(byte *blob, int blobLength)
{
  _post("/v2/transactions", "application/x-binary", blob, blobLength);
}

//curl -X POST "https://testnet-algorand.api.purestake.io/ps2/v2/transactions" -H "accept: application/x-binary" -H "Content-Type: application/x-binary" -H "x-api-key: af4Dyq6Pxb8c7I0ddWtJDH8naWswPcM6P1IYuiXb" -d "11000101111111011111011101001000011101100011001101010110101000011000111101111110011110110110001101101001101010100110101101000101011010011010100001101001100101011110010110100100011000101110011011111010000011001011010001011011001110000000101100100001110101101011101000000101000100010010111000011010100111101001001000110111110000010110000001011111010110111001101100001011100001000101001011010010110000100000000101100110111111110011001001000010001001110010110001111001101100000010111101101101000000010011101100000001"