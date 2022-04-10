#include <SoftwareSerial.h>
#include <Wire.h>
#include "MAX30105.h"
#include "heartRate.h"

SoftwareSerial BTserial(2, 3);

MAX30105 particleSensor;

const byte RATE_SIZE = 6;
unsigned long starttime = 0;
long lastBeat = 0;
float beats;
int beatAvg;
int ibiAvg;
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
    starttime = millis();
    
    while (millis() < starttime + 10000)
    {
        long irValue = particleSensor.getIR();
        
        if (irValue < 50000)
        {
            Serial.println("Correct the sensor!");
            delay(1000);
            break;
        }
        else
        {
            if (checkForBeat(irValue) == true)
            {
                long delta = millis() - lastBeat;
            
                lastBeat = millis();
            
                ibiAvg += delta;
            
                beats++;
            }
        }
    }

    ibiAvg /=  beats;
    beatAvg = beats * RATE_SIZE;

    if (beatAvg < 255 && beatAvg > 20)
    {
        printData();
        sendData();
    }

    clear();
}

void sendData()
{
    BTserial.write(itoa(beatAvg, cstr, 10));
    BTserial.write(";");
    BTserial.write(itoa(ibiAvg, cstr, 10));
    BTserial.write('\n');
}

void printData()
{
    Serial.print(beatAvg);
    Serial.print(";");
    Serial.print(ibiAvg);
    Serial.println();
}

void clear()
{
    beats = 0;
    beatAvg = 0;
    ibiAvg = 0;     
}
