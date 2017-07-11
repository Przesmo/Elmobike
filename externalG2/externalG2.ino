#include <CurieBLE.h>

int oldLedState = LOW;
int pin = 13;
BLEPeripheral blePeripheral;
BLEService externalService("19b10003-e8f2-537e-4f6c-d104768a1215");
BLEUnsignedCharCharacteristic led("19b10004-e8f2-537e-4f6c-d104768a1215", BLENotify | BLEWrite);
void setup()
{
  Serial.begin(115200);
  blePeripheral.setLocalName("ledService");
  blePeripheral.setAdvertisedServiceUuid(externalService.uuid());
  // add service and characteristic:
  blePeripheral.addAttribute(externalService);
  blePeripheral.addAttribute(led);
  led.setValue(0);
  blePeripheral.begin();
  pinMode(pin, oldLedState);
}
int ledState;
void loop()
{
  BLECentral central = blePeripheral.central();
  if (central)
  {
    Serial.println("Connected to central: "); //just for test
    // while the central is still connected to peripheral:
    while (central.connected())
    {
      Serial.println("CONNECTED");
      if (Serial.available() > 0)
      {
        ledState = Serial.read() - 48;

        if (ledState == 1)
        {
          digitalWrite(pin, HIGH);
          led.setValue(1);
//          Serial.print("Value: ");
//          Serial.println(led.value());
        }
        else if ( ledState == 0)
        {
          digitalWrite(pin, LOW);
          led.setValue(0);
//          Serial.print("Value: ");
//          Serial.println(led.value());
        }

      }
    }
  }
}


