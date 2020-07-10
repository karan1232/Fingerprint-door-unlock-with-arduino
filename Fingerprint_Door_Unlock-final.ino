/*
 * Echo Pin =Pin 13
 * Trigger Pin= Pin 12
 * LED=Pin 10
 * SOlenoid's Relay IN1/IN2=9
 * FP Sen1=Pin 3
 * FP Sen2=Pin 2
 */

#include <Adafruit_Fingerprint.h>

SoftwareSerial mySerial(2, 3);

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
const int trigPin=12;
const int echoPin=13;
// defines variables
long duration;
int distance;
void setup()  
{
  pinMode(9,OUTPUT);  //Solenoid Control using Relay
  pinMode(10,OUTPUT); //led pin
  pinMode(trigPin,OUTPUT);
  pinMode(echoPin,INPUT); 
  Serial.begin(9600);
  while (!Serial);  // For Yun/Leo/Micro/Zero/...
  delay(100);
  Serial.println("\n\nAdafruit finger detect test");

  // set the data rate for the sensor serial port
  finger.begin(57600);
  
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
    digitalWrite(9,LOW);  //Low=on
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    digitalWrite(9,HIGH);
    while (1) { delay(1); }
  }

  finger.getTemplateCount();
  Serial.print("Sensor contains "); Serial.print(finger.templateCount); Serial.println(" templates");
  Serial.println("Waiting for valid finger...");

}

void loop()                     // run over and over again
{
  
  getFingerprintIDez();
  delay(1);            
    // Clears the trigPin
digitalWrite(trigPin, LOW);
delayMicroseconds(2);

// Sets the trigPin on HIGH state for 10 micro seconds
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);

// Reads the echoPin, returns the sound wave travel time in microseconds
duration = pulseIn(echoPin, HIGH);

// Calculating the distance
distance= duration*0.034/2;
  // Prints the distance on the Serial Monitor
Serial.print("Distance: ");
Serial.println(distance);
if(distance<=20)
  {
    digitalWrite(9,LOW);
    delay(5000);
    digitalWrite(9,HIGH);
  }
delay(1000);//don't ned to run this at full speed.
}

uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("No finger detected");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  
  // OK converted!
  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Found a print match!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Did not find a match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }   
  
  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  Serial.print(" with confidence of "); Serial.println(finger.confidence); 
  digitalWrite(9,LOW);
  digitalWrite(10,HIGH);
  digitalWrite(13,HIGH);
  digitalWrite(12,LOW);
  delay(5000);
  digitalWrite(9,HIGH);
  digitalWrite(10,LOW);
  digitalWrite(13,LOW);
  digitalWrite(12,HIGH);
  return finger.fingerID;
}

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();                                                                          
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;
  
  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
  digitalWrite(9,LOW);
  digitalWrite(10,HIGH);
  digitalWrite(13,HIGH);
  digitalWrite(12,LOW);
  delay(5000);
  digitalWrite(9,HIGH);
  digitalWrite(10,LOW);
  digitalWrite(13,LOW);
  digitalWrite(12,HIGH);
  return finger.fingerID; 
}
