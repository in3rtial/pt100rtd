
#include "pt100rtd.h"

static String inString;
static float resistance;
static pt100rtd RTD;

void setup() {
  Serial.begin(9600);

  if(true) // false to shut down debug
  {
      for(int i = 100; i < 150; ++i)
      {
        resistance = i;
        Serial.println("Resistance = " + String(resistance));
        Serial.println(RTD.celsius(resistance));
        Serial.println(RTD.celsius_cvd(resistance));
        Serial.println(RTD.celsius_polynomial (resistance));
        Serial.println(RTD.celsius_rationalpolynomial(resistance));
      }
  }
}

void loop() {

  while (Serial.available() > 0)
  {
    inString = Serial.readString();// read the incoming data as string
    Serial.print("Input string: ");
    Serial.println(inString);
    Serial.print("\tAfter conversion to float:");
    resistance = inString.toFloat();
    Serial.println(resistance);
    Serial.println(RTD.celsius(resistance));
    Serial.println(RTD.celsius_cvd(resistance));
    Serial.println(RTD.celsius_polynomial (resistance));
    Serial.println(RTD.celsius_rationalpolynomial(resistance));
  }

}
