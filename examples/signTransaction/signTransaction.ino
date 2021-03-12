#include <Algoduino.h>

AlgoduinoInit algoduinoInit = {
    "ALGODUINO",
    "ALGODUINO",
    "af4Dyq6Pxb8c7I0ddWtJDH8naWswPcM6P1IYuiXb",
    TESTNET,
    "09 BD 76 B2 48 AA 8D 72 E1 63 D6 29 76 7A EC 7C EA 49 93 DB"};

Algoduino myAlgoduino = Algoduino(algoduinoInit);

AlgorandWallet algorandWallet;

void setup()
{
    myAlgoduino.begin();

    myAlgoduino.createWallet(&algorandWallet);

    myAlgoduino.setWallet(algorandWallet);

    myAlgoduino.printWallet(algorandWallet);
}

void loop()
{
    TransactionParams transactionParams = myAlgoduino.getTransactionParams();

    txn_t current_txn;

    current_txn.payment.amount = 0;

    current_txn.fee = transactionParams.fee;

    current_txn.firstValid = transactionParams.lastRound - 1000;

    String genesisID = transactionParams.genesisID;
    char genesisIDChar[32];
    genesisID.toCharArray(genesisIDChar, 32);

    for (int i = 0; i < 32; i++)
    {
        current_txn.genesisID[i] = genesisIDChar[i];
    }

    String genesisHash = transactionParams.genesisHash;
    byte genesisHashChar[32];
    genesisHash.getBytes(genesisHashChar, 32);

    for (int i = 0; i < 32; i++)
    {
        current_txn.genesisHash[i] = genesisHashChar[i];
    }

    current_txn.lastValid = transactionParams.lastRound;

    current_txn.note[0] = {7};

    for (int i = 0; i < 32; i++)
    {
        current_txn.payment.receiver[i] = algorandWallet.publicKey[i];
        current_txn.sender[i] = algorandWallet.publicKey[i];
    }

    current_txn.type = PAYMENT;

    byte msgpacked[900];
    unsigned int msg_len = myAlgoduino.packMessage(&current_txn, msgpacked);

    printNumber2("msgpack_buf ", msgpacked, msg_len);
    //Serial.println();
    //Serial.print("MessageLength = ");
    //Serial.println(msg_len);
    //Serial.println();

    byte signature[64];
    myAlgoduino.signMessage(signature, msgpacked, msg_len);
    printNumber2("signature ", signature, 64);
    Serial.println();
    Serial.println("//////////////////////////////////////////");
    Serial.println();
}