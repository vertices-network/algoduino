#include <Algoduino.h>

AlgoduinoInit algoduinoInit = {
  "ALGODUINO",
  "ALGODUINO",
  "af4Dyq6Pxb8c7I0ddWtJDH8naWswPcM6P1IYuiXb",
  TESTNET,
  "09 BD 76 B2 48 AA 8D 72 E1 63 D6 29 76 7A EC 7C EA 49 93 DB"
};

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
}