#include "MQTT.h"
#include "oled-wing-adafruit.h"

SYSTEM_THREAD(ENABLED);

void callback(char *topic, byte *payload, unsigned int length);

MQTT client("lab.thewcl.com", 1883, callback);
OledWingAdafruit display;
bool aPushSent = false;
bool bPushSent = false;
bool cPushSent = false;

void setup()
{
  Serial.begin(9600);
  while (!Serial.isConnected())
  {
  }
  display.setup();
  display.clearDisplay();
  pinMode(D7, OUTPUT);
  digitalWrite(D7, LOW);
  client.subscribe("inTopic/message");
  Serial.println("works?");
}

void loop()
{
  display.loop();
  if (client.isConnected())
  {
    client.loop();
  }
  else
  {
    client.connect(System.deviceID());
    client.subscribe("inTopic/message");
  }
  if (display.pressedA())
  {
    aPushSent = !aPushSent;
  }
  if (aPushSent)
  {
    client.publish("inTopic/message", "buttonA");
    aPushSent = false;
  }
  if (display.pressedB())
  {
    bPushSent = !bPushSent;
  }
  if (bPushSent)
  {
    client.publish("inTopic/message", "buttonB");
    bPushSent = false;
  }
  if (display.pressedC())
  {
    cPushSent = !cPushSent;
  }
  if (cPushSent)
  {
    client.publish("inTopic/message", "buttonC");
    cPushSent = false;
  }
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.clearDisplay();
  display.display();
}

void callback(char *topic, byte *payload, unsigned int length)
{
  char p[length + 1];
  memcpy(p, payload, length);
  p[length] = NULL;
  Serial.printf("%s", p);

  if (String(p).equals("1"))
  {
    digitalWrite(D7, HIGH);
    Serial.println("???????");
  }
  else if (String(p).equals("0"))
  {
    digitalWrite(D7, LOW);
  }
  else if (String(p).equals("buttonA"))
  {
    display.println("Button A pressed!");
    delay(500);
  }
  else if (String(p).equals("buttonB"))
  {
    display.println("Button B pressed!");
    delay(500);
  }
  else if (String(p).equals("buttonC"))
  {
    display.println("Button C pressed!");
    delay(500);
  }
  else
  {
    Serial.println(p);
  }
  display.display();
}