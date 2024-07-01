// Change These Credentials with your Blynk Template credentials
#define BLYNK_TEMPLATE_ID "TMPL6bvcL2kj5"
#define BLYNK_TEMPLATE_NAME "fireproject"
#define BLYNK_AUTH_TOKEN "ixsxdFGgw5hDVYDASgH16FX48C773_B7"

#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "air ho";
char pass[] = "22222221";

BlynkTimer timer;

#define FIRE_SENSOR D2
#define GREEN_LED D5
#define RED_LED D6
#define BUZZER D7
#define RELAY_LIGHT D3
#define RELAY_FAN D4

int fireVal = 0;
bool isFireDetected = false;
bool manualLightState = false;
bool manualFanState = false;

WidgetLED led(V1);
WidgetLED lightRelay(V2);
WidgetLED fanRelay(V3);

void setup() {
  Serial.begin(9600);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(FIRE_SENSOR, INPUT_PULLUP);
  pinMode(RED_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(RELAY_LIGHT, OUTPUT);
  pinMode(RELAY_FAN, OUTPUT);

  Blynk.begin(auth, ssid, pass);
  delay(2000);

  timer.setInterval(500L, mySensor);
}

void loop() {
  Blynk.run();
  timer.run();
}

void mySensor() {
  fireVal = digitalRead(FIRE_SENSOR);

  if (fireVal == LOW) {
    if (!isFireDetected) {
      Blynk.logEvent("fire_alert");
      isFireDetected = true;
    }

    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, HIGH);
    digitalWrite(BUZZER, HIGH);
    Blynk.virtualWrite(V0, 1);
    Serial.print("FIRE Level: ");
    Serial.println(fireVal);
    led.on();

    // Turn off the light if it's not manually controlled
    if (!manualLightState) {
      digitalWrite(RELAY_LIGHT, HIGH);
      Blynk.virtualWrite(V2, HIGH);
    }

    // Turn off the fan if it's not manually controlled
    if (!manualFanState) {
      digitalWrite(RELAY_FAN, LOW);
      Blynk.virtualWrite(V3, LOW);
    }
  } else {
    isFireDetected = false;
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(RED_LED, LOW);
    digitalWrite(BUZZER, LOW);
    Blynk.virtualWrite(V0, 0);
    Serial.print("FIRE Level: ");
    Serial.println(fireVal);
    led.off();

    // Turn on the light if it's not manually controlled
    if (!manualLightState) {
      digitalWrite(RELAY_LIGHT, LOW);
      Blynk.virtualWrite(V2, LOW);
    }

    // Turn on the fan if it's not manually controlled
    if (!manualFanState) {
      digitalWrite(RELAY_FAN, HIGH);
      Blynk.virtualWrite(V3, HIGH);
    }
  }
}

BLYNK_WRITE(V2) {
  int lightState = param.asInt();
  manualLightState = true; // Indicate manual control
  digitalWrite(RELAY_LIGHT, lightState);
}

BLYNK_WRITE(V3) {
  int fanState = param.asInt();
  manualFanState = true; // Indicate manual control
  digitalWrite(RELAY_FAN, fanState);
}