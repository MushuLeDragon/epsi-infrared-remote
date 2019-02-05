#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include <IRremote.h>

IRsend irsend;

byte mac[]    = {  0xDE, 0xED, 0xDA, 0xFE, 0xFE, 0xED };
IPAddress ip(192, 168, 30, 10);
IPAddress server(192, 168, 30, 132);

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
    if((char)payload[i] == '1'){
        commandPower(true);
    }else if((char)payload[i] == '0'){
        commandPower(false);
    }else{
        commandMenu((char)payload[i]);
    }
  }
  Serial.println();
}
void commandPower(bool status){
    if(status){
	    irsend.sendNEC(0xC40BF, 32);
    }else{
	    irsend.sendNEC(0xC40BF, 32);
        delay(1500);
	    irsend.sendNEC(0xC40BF, 32);
    }
}

void commandMenu(char command){
    switch (command)
    {
        case 'h':
	        irsend.sendNEC(0xCD02F, 32);
            break;
        case 'b':
	        irsend.sendNEC(0xC30CF, 32);
            break;
        case 'g':
	        irsend.sendNEC(0xCB04F, 32);
            break;
        case 'd':
	        irsend.sendNEC(0xC708F, 32);
            break;
        case 'k':
	        irsend.sendNEC(0xC08F7, 32);
            break;
        case 'm':
        // menu and exit
	        irsend.sendNEC(0xCF00F, 32);
            break;
        case 'e':
        // menu and exit
	        irsend.sendNEC(0xCF00F, 32);
            break;
        case '+':
	        irsend.sendNEC(0xC41BE, 32);
            break;
        case '-':
	        irsend.sendNEC(0xCC13E, 32);
            break;
        case 's':
	        irsend.sendNEC(0xC20DF, 32);
            break;
    }
}
EthernetClient ethClient;
PubSubClient client(ethClient);

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("arduinoClient")) {
      Serial.println("connected");
      client.publish("outTopic","hello world");
      client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup()
{
  Serial.begin(57600);
  
  client.setServer(server, 1883);
  client.setCallback(callback);

  Ethernet.begin(mac, ip);
  delay(1500);
}

void loop()
{
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
