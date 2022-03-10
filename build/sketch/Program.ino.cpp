#include <Arduino.h>
#line 1 "d:\\Projekty lokalne\\bodyfatiguedevice\\Program.ino"
#include <SoftwareSerial.h>
#include <Wire.h>
#include "MAX30105.h"
#include "heartRate.h"

SoftwareSerial BTserial(2, 3);

MAX30105 particleSensor;

const byte RATE_SIZE = 4; //Increase this for more averaging. 4 is good.
byte rates[RATE_SIZE]; //Array of heart rates
byte rateSpot = 0;
long lastBeat = 0; //Time at which the last beat occurred

float beatsPerMinute;
int beatAvg;

int lastAvgValue = 0;

char cstr[16];

int count = 0;
int myBest = 0;

#line 25 "d:\\Projekty lokalne\\bodyfatiguedevice\\Program.ino"
void setup();
#line 45 "d:\\Projekty lokalne\\bodyfatiguedevice\\Program.ino"
void loop();
#line 25 "d:\\Projekty lokalne\\bodyfatiguedevice\\Program.ino"
void setup()
{
	Serial.begin(9600);
    BTserial.begin(9600);

    Serial.println("Initializing...");

  // Initialize sensor
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) //Use default I2C port, 400kHz speed
  {
    Serial.println("MAX30105 was not found. Please check wiring/power. ");
    while (1);
  }
  Serial.println("Place your index finger on the sensor with steady pressure.");

  particleSensor.setup(); //Configure sensor with default settings
  particleSensor.setPulseAmplitudeRed(0x0A); //Turn Red LED to low to indicate sensor is running
  particleSensor.setPulseAmplitudeGreen(0); //Turn off Green LED
}

void loop()
{
    long irValue = particleSensor.getIR();
    
    if (checkForBeat(irValue) == true)
    {
        //We sensed a beat!
        long delta = millis() - lastBeat;
        lastBeat = millis();

        beatsPerMinute = 60 / (delta / 1000.0);

        if (beatsPerMinute < 255 && beatsPerMinute > 20)
        {
            rates[rateSpot++] = (byte)beatsPerMinute; //Store this reading in the array
            rateSpot %= RATE_SIZE; //Wrap variable

            //Take average of readings
            beatAvg = 0;
            for (byte x = 0 ; x < RATE_SIZE ; x++)
            {
                beatAvg += rates[x];
            }

            beatAvg /= RATE_SIZE;
        }
    }

    
    myBest += beatAvg;

    if(count >= 20)
    {
        myBest /= 20;

        Serial.print("myBest=");
        Serial.print(myBest);

        Serial.print(", Avg BPM=");
        Serial.print(beatAvg);
        Serial.println();

        BTserial.write(itoa(beatAvg, cstr, 10));
        BTserial.write('\n');
        count = 0;
        myBest = 0;
    }
    else
    {
        count++;
    }
}

