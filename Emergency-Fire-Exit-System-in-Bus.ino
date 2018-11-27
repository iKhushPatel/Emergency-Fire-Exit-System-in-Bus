/**
 * servomotor
 *  yellow - 3
 *  red - MQ2 5v
 *  brown - GND
 *  
 * MQ2
 *  A0 - A5 
 *  D0 - blank no pin
 *  GND - GND
 *  VCC - 5v
 * 
 * DHT11 - Blue version
 * SN number this start karvanu
 * 1 st - GND
 * 2 nd - blank - no pin
 * 3 rd - A0
 * 4 th - 3.3V
 * 
 * GSM
 *  TX - 4
 *  RX - 5
 * 
 * LED 
 *  +ve  - kvm13
 *  -ve  - GND
 * 
 * */



#include <Servo.h> 
Servo myservo;
const int tmp = A0  ;
const int p = 8;
const int buz=13;
int smokeA5 = A5;
int servoPin = 3; 

#include <SoftwareSerial.h>
#include <String.h>
 
SoftwareSerial mySerial(4, 5);

void setup() {
  mySerial.begin(9600);               // the GPRS baud rate   
  myservo.attach(servoPin);
  Serial.begin (9600);
  pinMode(p, OUTPUT);
  pinMode(buz,OUTPUT);
  pinMode(smokeA5, INPUT);
  pinMode(tmp, INPUT);
}

void loop() {
  
  digitalWrite(p,LOW);
  int Temp = analogRead(tmp);
  float volts = (Temp / 965.0) * 5;
  float c = (volts - .5) * 100;
  float f = (c * 9 / 5 + 32);
  c = c-273;
  //Serial.println(f);
  Serial.print("Temp is :: ");
  Serial.println(c);

  int analogSensor = analogRead(smokeA5);
  Serial.print("Smoke Sensor :: ");
  Serial.println(analogSensor);
  
  
  if(c>50)
  {
   buzz();
   servo();
  }
  
  
  if(analogSensor>300)
  {
   buzz(); 
   servo();
  }
  Send2Cloud(c);
  
  
  if (mySerial.available())
    Serial.write(mySerial.read());
  // put your main code here, to run repeatedly:
  
  
  //reset();
}

void buzz()
{
  digitalWrite(buz, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);  // wait for a second

  digitalWrite(buz, LOW);   // turn the LED on (HIGH is the voltage level)
  delay(1000);  // wait for a second

}
void servo()
{

   myservo.write(0);
   delay(1000); 
   myservo.write(90);
   delay(1000);
}
void reset()
{
    exit(1);
}


void Send2Cloud(float dist)
{
  mySerial.println("AT");
  delay(1000);

  mySerial.println("AT+CPIN?");
  delay(1000);

  mySerial.println("AT+CREG?");
  delay(1000);

  mySerial.println("AT+CGATT?");
  delay(1000);

  mySerial.println("AT+CIPSHUT");
  delay(1000);

  mySerial.println("AT+CIPSTATUS");
  delay(2000);

  mySerial.println("AT+CIPMUX=0");
  delay(2000);
 
  ShowSerialData();
 
  mySerial.println("AT+CSTT=\"internet\"");//start task and setting the APN,
  delay(1000);
 
  ShowSerialData();
 
  mySerial.println("AT+CIICR");//bring up wireless connection
  delay(3000);
 
  ShowSerialData();
 
  mySerial.println("AT+CIFSR");//get local IP adress
  delay(2000);
 
  ShowSerialData();
 
  mySerial.println("AT+CIPSPRT=0");
  delay(3000);
 
  ShowSerialData();
  
  mySerial.println("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",\"80\"");//start up the connection
  delay(6000);
 
  ShowSerialData();
 
  mySerial.println("AT+CIPSEND");//begin send data to remote server
  delay(4000);
  ShowSerialData();

  String str="GET https://api.thingspeak.com/update?api_key=FPGQCTAUMXN3NA1P&field1=" + String(dist);
    //String str="GET https://api.thingspeak.com/update?api_key=OU84C4DMG5837MQU&field1=" + String(dist);
    //String str="GET https://api.thingspeak.com/update?api_key=G5059NUM24W190SB&field2=" + String(dist);
  mySerial.println(str);//begin send data to remote server
  delay(4000);
  ShowSerialData();

  mySerial.println((char)26);//sending
  delay(5000);//waitting for reply, important! the time is base on the condition of internet 
  mySerial.println();
 
  ShowSerialData();
 
  mySerial.println("AT+CIPSHUT");//close the connection
  delay(100);
  ShowSerialData();
} 

void ShowSerialData()
{
  while(mySerial.available()!=0)
    Serial.write(mySerial.read());
}
