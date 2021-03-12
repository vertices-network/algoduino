#include <Algoduino.h>

AlgoduinoInit algoduinoInit = {
    "ALGODUINO",
    "ALGODUINO",
    "af4Dyq6Pxb8c7I0ddWtJDH8naWswPcM6P1IYuiXb",
    TESTNET,
    "09 BD 76 B2 48 AA 8D 72 E1 63 D6 29 76 7A EC 7C EA 49 93 DB"};

Algoduino myAlgoduino = Algoduino(algoduinoInit);

void setup()
{
    myAlgoduino.begin();

    myAlgoduino.createWallet(&algorandWallet);

    myAlgoduino.setWallet(algorandWallet);

    myAlgoduino.printWallet(algorandWallet);
}

void loop()
{

    TransactionObject transactionObject;

    TransactionParams transactionParams = myAlgoduino.getTransactionParams();

    /*
    transactionObject.from = "UUOB7ZC2IEE4A7JO4WY4TXKXWDFNATM43TL73IZRAFIFFOE6ORPKC7Q62E";
    transactionObject.to = transactionObject.from;
    transactionObject.fee = transactionParams.fee;
    transactionObject.amount = 1000;
    transactionObject.firstRound = transactionParams.lastRound - 1000;
    transactionObject.lastRound = transactionParams.lastRound;
    transactionObject.genesisID = transactionParams.genesisID;
    transactionObject.genesisHash = transactionParams.genesisHash;
    transactionObject.note = "";
    transactionObject.type = "pay";

    myAlgoduino.printTransactionObject(transactionObject);
*/

    delay(500);
}