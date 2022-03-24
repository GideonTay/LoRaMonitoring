#include <MKRWAN.h>
#include "DHT.h"
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht (DHTPIN,DHTTYPE);

LoRaModem modem;

//#include "arduino_secrets.h"
// Please enter your sensitive data in the Secret tab or arduino_secrets.h
String appEui = "0000000000000000";
String appKey = "EDD5A095615BA359A5231C13535A881D";

void setup()
{
  Serial.begin(9600);
  while(!Serial);
  dht.begin();

  pinMode(LED_BUILTIN, OUTPUT);        //check if board is receiving
  for (int i=1; i<=5; i++) {           
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(1000);                       // wait for a second
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    delay(1000);
    // wait for a second
    Serial.println(5-i);
  }
  
  // change this to your regional band (eg. US915, AS923, ...)
  if (!modem.begin(AU915)) {
    Serial.println("Failed to start module");
    while (1) {}
  };
  delay(5000);
  Serial.print("Your module version is: ");
  Serial.println(modem.version());
  Serial.print("Your device EUI is: ");
  Serial.println(modem.deviceEUI());
  modem.sendMask("ff000000f000ffff00020000");
  Serial.println(modem.getChannelMask());
  modem.setADR(true);
  join();

  modem.minPollInterval(30);
}

void join(){
  Serial.println("Trying to join TTN ...");
  int connected = modem.joinOTAA(appEui, appKey);
  if (!connected) {
    Serial.println("Something went wrong; are you indoor? Move near a window and retry");
    join();
  }
  else
    Serial.println("Successfully joined");  // Set poll interval to 60 secs.
    modem.minPollInterval(60);
  // NOTE: independently by this setting the modem will
  // not allow to send more than one message every 2 minutes,
  // this is enforced by firmware and can not be changed.
}

void loop()
{
float h = dht.readHumidity();
float t = dht.readTemperature();
float f = dht.readTemperature(true);
float fc = (t*9/5)+32;
if (isnan(h)||isnan(t)||isnan(f)){
  Serial.println(F("Fail to read sensor"));
  return;
}
int err;
modem.beginPacket();
modem.write(t);
modem.write(h);
    err = modem.endPacket(true);
    Serial.println("Packet Sent. Sending Sensor data...");
    if (err > 0) {
      Serial.println("Message sent correctly!");
      Serial.print(F(" Humidity(%): "));
      Serial.println(h);
      Serial.print(F(" Temperature(°C): "));
      Serial.println(t);
      Serial.print(F(" Temperature(°F): "));
      Serial.println(fc);
    } else {
      Serial.println("Error sending message :(");
      Serial.println("(you may send a limited amount of messages per minute, depending on the signal strength");
      Serial.println("it may vary from 1 message every couple of seconds to 1 message every minute)");
    }
    delay(1000);
    Serial.println("waiting 120 seconds");
    delay(60000*2); 
}
