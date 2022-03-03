# 1 "d:\\Projekty lokalne\\bodyfatiguedevice\\Program.ino"
# 2 "d:\\Projekty lokalne\\bodyfatiguedevice\\Program.ino" 2
SoftwareSerial BTserial(2, 3);

char cstr[16];

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
