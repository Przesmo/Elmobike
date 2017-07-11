#include <CurieBLE.h>

BLEPeripheral blePeripheral;
BLEService externalService("19b10000-e8f2-537e-4f6c-d104768a1215"); // BLE LED Service

BLEUnsignedCharCharacteristic data("19b10001-e8f2-537e-4f6c-d104768a1215", BLERead | BLEWrite);
BLEUnsignedCharCharacteristic led("19b10002-e8f2-537e-4f6c-d104768a1215", BLERead | BLEWrite);
void setup()
{
  Serial.begin(115200);

  // set advertised local name and service UUID:
  blePeripheral.setLocalName("data");
  blePeripheral.setAdvertisedServiceUuid(externalService.uuid());

  // add service and characteristic:
  blePeripheral.addAttribute(externalService);
  blePeripheral.addAttribute(data);
  blePeripheral.addAttribute(led);
  // begin advertising BLE service:
  blePeripheral.begin();
}

void loop()
{
  // listen for BLE peripherals to connect:
  BLECentral central = blePeripheral.central();

  // if a central is connected to peripheral:
  if (central)
  {
    Serial.println("Connected to central: "); //just for test
    // while the central is still connected to peripheral:
    while (central.connected())
    {
      Serial.print("CONNECTED: "); //just for test (JFT)
      Serial.println(data.value());
      Serial.println(led.value());
      delay(500);
    }
  }
  // when the central disconnects, print it out:
  Serial.println(F("Disconnected from central: ")); //JFT
}

