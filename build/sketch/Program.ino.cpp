#include <Arduino.h>
#line 1 "d:\\Projekty lokalne\\bodyfatiguedevice\\Program.ino"
#include <SoftwareSerial.h>
SoftwareSerial BTserial(2, 3);

char cstr[16];

#line 6 "d:\\Projekty lokalne\\bodyfatiguedevice\\Program.ino"
void setup();
#line 12 "d:\\Projekty lokalne\\bodyfatiguedevice\\Program.ino"
void loop();
#line 6 "d:\\Projekty lokalne\\bodyfatiguedevice\\Program.ino"
void setup()
{
	Serial.begin(9600);
    BTserial.begin(9600);
}

void loop()
{
    int sensorValue = analogRead(A1);
    
    if(sensorValue) 
    {
        Serial.println(itoa(sensorValue, cstr, 10));
        BTserial.write(itoa(sensorValue, cstr, 10));
        BTserial.write('\n');
    }
    
    delay(10);   
}

