#include <SoftwareSerial.h>
#include <Wire.h>
#include "MAX30105.h"
#include "heartRate.h"

SoftwareSerial BTserial(2, 3);

MAX30105 particleSensor;

const byte RATE_SIZE = 4;
byte rateSpot = 0;
long lastBeat = 0; //Time at which the last beat occurred
float beatsPerMinute;
int beatAvg;
int ibiAvg;
int count = 0;
char cstr[16];

void setup()
{
	Serial.begin(9600);
    BTserial.begin(9600);

    Serial.println("Initializing...");

    if (!particleSensor.begin(Wire, I2C_SPEED_FAST))
    {
        Serial.println("Sensor was not found. Please check wiring/power. ");
        while (1);
    }
  
    particleSensor.setup();
    particleSensor.setPulseAmplitudeRed(0x0A);
    particleSensor.setPulseAmplitudeGreen(0);
}

void loop()
{
    long irValue = particleSensor.getIR();
    
    if (checkForBeat(irValue) == true)
    {
        long delta = millis() - lastBeat;
        
        lastBeat = millis();

        beatsPerMinute = 60 / (delta / 1000.0);

        if (beatsPerMinute < 255 && beatsPerMinute > 20)
        {
            beatAvg += beatsPerMinute;
            ibiAvg += delta;
            count++;
        }
    }

    if(count >= RATE_SIZE)
    {
        beatAvg /= RATE_SIZE;
        ibiAvg /= RATE_SIZE;

        BTserial.write(itoa(beatAvg, cstr, 10));
        BTserial.write("; ");
        BTserial.write(itoa(ibiAvg, cstr, 10));
        BTserial.write('\n');

        Serial.print(beatAvg);
        Serial.print("; ");
        Serial.print(ibiAvg);
        Serial.println();

        count = 0;
        beatAvg = 0;
        ibiAvg = 0;
    }
}
