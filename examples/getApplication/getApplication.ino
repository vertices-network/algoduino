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
  String application = myAlgoduino.getApplication("163650");

  Serial.println("application: ");
  Serial.println("");
  Serial.println(application);
  Serial.println("");

  delay(500);
}
