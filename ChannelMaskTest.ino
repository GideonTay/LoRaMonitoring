#include <MKRWAN.h>


LoRaModem modem;

void setup() {
  int status;
  // put your setup code here, to run once:
  Serial.begin(9600);
  while (!Serial);

  loraSetup();
  
  Serial.println("Initial Mask");
  Serial.println(modem.getChannelMask());
  
  for (unsigned int j = 0; j < 72; j++) {
    modem.disableChannel(j);
  }
  
  Serial.println("All Channels off");
  Serial.println(modem.getChannelMask());
  
  for (unsigned int j = 8; j < 16; j++) {
    modem.enableChannel(j);
  }
  modem.enableChannel(65);
  Serial.println("Channels 8 to 15 plus 65");
  Serial.println(modem.getChannelMask());
}

void loraSetup() {
  if (!modem.begin(AU915)) {
    Serial.println("Failed to start module");
    while (1) {}
  };
  delay(5000);
  
  // Set poll interval to 30 secs.
  modem.minPollInterval(30);
  // NOTE: independently by this setting the modem will
  // not allow to send more than one message every 2 minutes,
  // this is enforced by firmware and can not be changed.
}

void loop (){
}
