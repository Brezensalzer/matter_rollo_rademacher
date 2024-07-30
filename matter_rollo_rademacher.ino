/*
  matter_rollo_rademacher.ino
  Rollladensteuerung

  based on:

   Matter lightbulb example

   The example shows how to create multiple on/off lightbulb instances with the Arduino Matter API.

   The example lets users control the onboard LEDs through Matter.
   The device has to be commissioned to a Matter hub first.

   Compatible boards:
   - SparkFun Thing Plus MGM240P
   - xG24 Explorer Kit

   Author: Tamas Jozsi (Silicon Labs)
 */
#include <Matter.h>
#include <MatterLightbulb.h>

// If the board doesn't have a second onboard LED - define a pin on which a second LED can be connected
#define UP PC3    // state ON
#define DOWN PC2  // state OFF

#define DEBUG false

MatterLightbulb matter_bulb_0;

void handle_rollo();

void setup()
{
  if (DEBUG) { Serial.begin(115200); }
  Matter.begin();
  matter_bulb_0.begin();

  pinMode(UP, OUTPUT);
  digitalWrite(UP, LOW);
  pinMode(DOWN, OUTPUT);
  digitalWrite(DOWN, LOW);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  if (DEBUG) { Serial.println("Matter Rollo Rademacher"); }

  if (!Matter.isDeviceCommissioned()) {
    if (DEBUG) { 
      Serial.println("Matter device is not commissioned");
      Serial.println("Commission it to your Matter hub with the manual pairing code or QR code");
      Serial.printf("Manual pairing code: %s\n", Matter.getManualPairingCode().c_str());
      Serial.printf("QR code URL: %s\n", Matter.getOnboardingQRCodeUrl().c_str());
    }
  }
  while (!Matter.isDeviceCommissioned()) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(200);
    digitalWrite(LED_BUILTIN, LOW);
  }

  if (!Matter.isDeviceThreadConnected()) {
    if (DEBUG) { Serial.println("Waiting for network connection..."); }
  }
  while (!Matter.isDeviceThreadConnected()) {
    delay(200);
  }
  if (DEBUG) { Serial.println("Device connected"); }
}

void loop()
{
  handle_rollo();
}

void handle_rollo()
{
  static bool matter_lightbulb_last_state = false;
  bool matter_lightbulb_current_state = matter_bulb_0.get_onoff();

  // If the current state is ON and the previous was OFF - send UP command
  if (matter_lightbulb_current_state && !matter_lightbulb_last_state) {
    matter_lightbulb_last_state = matter_lightbulb_current_state;
    digitalWrite(UP, HIGH);
    digitalWrite(LED_BUILTIN, HIGH);
    if (DEBUG) { Serial.println("Rollo UP pulse"); }
    delay(1000);
    digitalWrite(UP, LOW);
    digitalWrite(LED_BUILTIN, LOW);
  }

  // If the current state is OFF and the previous was ON - send DOWN command
  if (!matter_lightbulb_current_state && matter_lightbulb_last_state) {
    matter_lightbulb_last_state = matter_lightbulb_current_state;
    digitalWrite(DOWN, HIGH);
    digitalWrite(LED_BUILTIN, HIGH);
    if (DEBUG) { Serial.println("Rollo DOWN pulse"); }
    delay(1000);
    digitalWrite(DOWN, LOW);
    digitalWrite(LED_BUILTIN, LOW);
  }
}
