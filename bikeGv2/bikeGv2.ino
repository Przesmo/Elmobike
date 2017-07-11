
/*
  PROGRAM FOR GENUINO ON THE BIKE
  Genuino collects data from whole sensors and next send to external Genuino
*/
#include <CurieBLE.h> // special library for bluetooth

void setup()
{
  Serial.begin(115200);
  delay(6000);
  // initialize the BLE hardware
  BLE.begin();
  BLE.scanForUuid("19b10003-e8f2-537e-4f6c-d104768a1215");
}
int potencjometr;
unsigned char b;
int con1, con2 = 0;
void loop()
{
  delay(500);

  // check if a peripheral has been discovered
  BLEDevice peripheral = BLE.available();
  Serial.println("Start");
  if (peripheral)
  {
    // discovered a peripheral, print out address, local name, and advertised service
    Serial.print("Found: ");
    Serial.print(peripheral.address());
    Serial.print(" '");
    Serial.print(peripheral.localName());
    Serial.print(" '");
    Serial.print(peripheral.advertisedServiceUuid());
    Serial.println();
    peripheral.connect();
    con1 = 1;
    // stop scanning
    BLE.stopScan();
  }
  delay(500);
  /////////////////////////
  BLE.scanForUuid("19b10000-e8f2-537e-4f6c-d104768a1215");
  delay(500);
  BLEDevice peripheral1 = BLE.available();
  if (peripheral1) {
    Serial.print("Found: ");
    Serial.print(peripheral1.address());
    Serial.print(" '");
    Serial.print(peripheral1.localName());
    Serial.print(" '");
    Serial.print(peripheral1.advertisedServiceUuid());
    Serial.println();
    peripheral1.connect();
    con2 = 1;
  }
  BLE.stopScan();
  delay (500);
  Serial.print(peripheral);
  Serial.print("  ");
  Serial.println(peripheral1);
  if (peripheral && peripheral1)
    checkCharacteristic(peripheral, peripheral1);
  else{
    peripheral.disconnect();
    peripheral1.disconnect();
  }
  BLE.scanForUuid("19b10003-e8f2-537e-4f6c-d104768a1215");
}

void checkCharacteristic(BLEDevice peripheral, BLEDevice peripheral1)
{

  // instructions check properly of connections
  ///////////////////////////// prepiheral
  if (!peripheral.connected())
  {
    Serial.println("DISCONNECTED!"); //JFT
    return;
  }
  if (!peripheral.discoverAttributes())
  {
    Serial.println("Attribute discovered!"); //JFT
    peripheral.disconnect();
    return;
  }
  BLECharacteristic externalCharacteristic = peripheral.characteristic("19b10004-e8f2-537e-4f6c-d104768a1215");

  if (!externalCharacteristic)
  {
    Serial.println("Peripheral does have characteristic!"); //JFT
    peripheral.disconnect();
    return;
  }
  ////////////////////////////////////// perpiheral1
  if (!peripheral1.connected())
  {
    Serial.println("1DISCONNECTED!"); //JFT
    return;
  }
  if (!peripheral1.discoverAttributes())
  {
    Serial.println("1Attribute discovered!"); //JFT
    peripheral1.disconnect();
    return;
  }
  /////////////////////////////////////// charakterystyka potencjometr
  BLECharacteristic externalCharacteristic1 = peripheral1.characteristic("19b10001-e8f2-537e-4f6c-d104768a1215");
  if (!externalCharacteristic1)
  {
    Serial.println("11Peripheral does have characteristic!"); //JFT
    peripheral1.disconnect();
    return;
  }
  if (!externalCharacteristic1.canWrite())
  {
    Serial.println("11Peripheral does have a writable characteristic!"); //JFT
    peripheral1.disconnect();
    return;
  }
  ///////////////////////////////////// charakterystyka dioda
  BLECharacteristic externalCharacteristic2 = peripheral1.characteristic("19b10002-e8f2-537e-4f6c-d104768a1215");

  if (!externalCharacteristic)
  {
    Serial.println("22Peripheral does have characteristic!"); //JFT
    peripheral1.disconnect();
    return;
  }
  if (!externalCharacteristic.subscribe())
  {
    Serial.println("22Peripheral does not have a notify characteristic!"); //JFT
    peripheral1.disconnect();
    return;
  }
  /////////////////////////////////////// przesylanie danych

  //check if characteristic exist
  while (peripheral.connected() && peripheral1.connected()) {
    potencjometr = analogRead(A0);
    externalCharacteristic1.writeInt(potencjometr);
    if (externalCharacteristic.valueUpdated()) {
      int ledValue = externalCharacteristic.intValue();
      externalCharacteristic2.writeInt(ledValue);
      Serial.print("Led: ");
      Serial.println(ledValue);
    }
  }
  delay(500);
  peripheral.disconnect();
  delay(500);
  peripheral1.disconnect();
  con1 = 0;
  con2 = 0;
}














