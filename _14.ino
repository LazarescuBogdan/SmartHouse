/*
 SMS sender

 This sketch, for the Arduino GSM shield,sends an SMS message
 you enter in the serial monitor. Connect your Arduino with the
 GSM shield and SIM card, open the serial monitor, and wait for
 the "READY" message to appear in the monitor. Next, type a
 message to send and press "return". Make sure the serial
 monitor is set to send a newline when you press return.

 Circuit:
 * GSM shield
 * SIM card that can send SMS

 created 25 Feb 2012
 by Tom Igoe

 This example is in the public domain.

 http://arduino.cc/en/Tutorial/GSMExamplesSendSMS

 */

// Include the GSM library
#include <GSM.h>
#include "DHT.h"
#include <string.h>
#define PINNUMBER ""
#define DHTTYPE DHT11
#define DHTPIN 4
#include <IRremote.h>;
GSM gsmAccess;
GSM_SMS sms;
DHT dht(DHTPIN, DHTTYPE);
//const int buttonPin = 5;
int speakerPin=5;
const int ldr =0;
int buttonState = 0,k=0,p=0,ldrval; 
int irRemotePin=6;
IRrecv irrecv(irRemotePin);
decode_results results;
char senderNumber[20];

void setup()
{
  pinMode(speakerPin,OUTPUT);
  //pinMode(buttonPin, INPUT);
  pinMode(irRemotePin,INPUT);
  pinMode(13, OUTPUT);
  // initialize serial communications and wait for port to open:
  Serial.begin(9600);
  Serial.println("SMS Messages Sender");

  // connection state
  boolean notConnected = true;

  // Start GSM shield
  // If your SIM has PIN, pass it as a parameter of begin() in quotes
  while (notConnected)
  {
    if (gsmAccess.begin(PINNUMBER) == GSM_READY)
      notConnected = false;
    else
    {
      Serial.println("Not connected");
      delay(1000);
    }
  }

  Serial.println("GSM initialized");
  dht.begin();
  irrecv.enableIRIn();
}

void loop()
{
  char c[30],i=0;
  // buttonState = digitalRead(buttonPin);
  //if(buttonState==HIGH)k=1;
    float h = dht.readHumidity();
    float t = dht.readTemperature();
       if (isnan(t) || isnan(h)) Serial.println("Failed to read from DHT");
  if (sms.available())
  {
    i=0;
    Serial.println("Message received from:");

    // Get remote number
    sms.remoteNumber(senderNumber, 20);
    Serial.println(senderNumber);

    // Any messages starting with # should be discarded
    if (sms.peek() == '#')
    {
      Serial.println("Discarded SMS");
      sms.flush();
    }
    // Read message bytes and print them
    while (c[i] = sms.read())
      {Serial.print(c[i]);
      i++;
      }
      Serial.print(c[0]);
   if(((c[0]=='t')||(c[0]=='T'))&&(c[1]=='e'))k=1;
   if(((c[0]=='u')||(c[0]=='U'))&&(c[1]=='m'))k=2;
   if(((c[0]=='L')||(c[0]=='l'))&&(c[1]=='p'))digitalWrite(13, HIGH);
   if(((c[0]=='L')||(c[0]=='l'))&&(c[1]=='o'))digitalWrite(13, LOW);
 
   // Delete message from modem memory
    sms.flush();
  }
  
     if(irrecv.decode(&results))
        {
            Serial.println(results.value);
            irrecv.resume();
            if(results.value==16724175) digitalWrite(13, HIGH);
            if(results.value==16718055) digitalWrite(13, LOW);
         }
  
       if((t>39)&&(p==0))
        {
        sms.beginSMS(senderNumber);
        sms.print("Alerta temperatura ridicata: ");
        sms.print(t);
        sms.print("*C");
        sms.endSMS();
        Serial.println("\nCOMPLETE!\n");
        p=1;
        digitalWrite(speakerPin,HIGH);
        //delay(5000);
        //digitalWrite(speakerPin,LOW);
        }
  if(k==1)
  {
    sms.beginSMS(senderNumber);
    sms.print("temperatura: ");
    sms.print(t);
    sms.endSMS();
    Serial.print(" %\t");
    Serial.print("Temperatura: "); 
    Serial.print(t);
    Serial.println(" *C");
    k=0;
    p=0;
    delay(2000);
  }
  if(k==2)
  {
    sms.beginSMS(senderNumber);
    sms.print("  umiditatea: ");
    sms.print(h);
    sms.endSMS();
    Serial.println(senderNumber);
    Serial.println("\nCOMPLETE!\n");
    Serial.print("Umiditate: "); 
    Serial.print(h);
    k=0;
    p=0;
    delay(2000);
  }
  ldrval=analogRead(ldr);
  
}

/*
  Read input serial
 */
int readSerial(char result[])
{
  int i = 0;
  while (1)
  {
    while (Serial.available() > 0)
    {
      char inChar = Serial.read();
      if (inChar == '\n')
      {
        result[i] = '\0';
        Serial.flush();
        return 0;
      }
      if (inChar != '\r')
      {
        result[i] = inChar;
        i++;
      }
    }
  }
}
