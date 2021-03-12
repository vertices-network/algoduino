#include <Algoduino.h>

AlgoduinoInit algoduinoInit = {
  "ALGODUINO", 
  "ALGODUINO", 
  "af4Dyq6Pxb8c7I0ddWtJDH8naWswPcM6P1IYuiXb", 
  TESTNET, 
  "09 BD 76 B2 48 AA 8D 72 E1 63 D6 29 76 7A EC 7C EA 49 93 DB"
};

Algoduino myAlgoduino = Algoduino(algoduinoInit);

void setup()
{
  myAlgoduino.begin();
  Serial.begin(115200);
}

void loop()
{
  TransactionParams transactionParams = myAlgoduino.getTransactionParams();

  int fee = transactionParams.fee;
  String genesisID = transactionParams.genesisID;
  String genesisHash = transactionParams.genesisHash;
  long lastRound = transactionParams.lastRound;
  String consensusVersion = transactionParams.consensusVersion;
  int minFee = transactionParams.minFee;

  Serial.println("fee: " + String(fee));
  Serial.println("genesisID: " + genesisID);
  Serial.println("genesisHash: " + genesisHash);
  Serial.println("lastRound: " + String(lastRound));
  Serial.println("consensusVersion: " + consensusVersion);
  Serial.println("minFee: " + String(minFee));
  Serial.println("");

  delay(500);
}
