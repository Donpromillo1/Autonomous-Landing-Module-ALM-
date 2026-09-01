#include <ESP32Servo.h> // Servo Bibliothek für PWM Signal zur Motorsteuerung
#include <Adafruit_NeoPixel.h> // Bibliothek für addressierbaren LED ansteuerung

Servo myservo;  // legt objekt Servo an um das PWM signal in grad zu steuern
uint pos = 0;    // pwm anfangs auf 0 setzten
/*
const int trigPin = 19; // Pinbelegung für Uschall
const int echoPin = 18;
*/
int servoPin = 27;
const int button1 = 13; // Pinbelegung für Fernbedienung
const int button2 = 5;
const int button3 = 23;
bool lastState1 = LOW; // stati für knöpfe auf 0 setzten
bool lastState2 = HIGH;
bool lastState3 = LOW;
 
void setup() 
{
	// Allow allocation of all timers
  ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);
	myservo.setPeriodHertz(50);    // standard 50 hz servo
	myservo.attach(servoPin, 1000, 2000); // attaches the servo on pin 27 to the servo object
  //myservo.attach(27); // pwm pin auf Servo object
  myservo.write(pos); // write initial 0 value to Servo
  //Ersatzt inital code für nicht gefundene ESP32 HCSR04 library
  Serial.begin(9600);
  // Für Fernbedienung internen Pulldown Widerstände setzten   
  pinMode(button1, INPUT_PULLDOWN);
  pinMode(button2, INPUT_PULLDOWN);
  pinMode(button3, INPUT_PULLDOWN);
  Serial.println("ALM System gestartet");
  Serial.println("start position:");
  Serial.print(pos); // schreibe 
}

void loop() 
{
  bool upButton = digitalRead(button1);
  bool stopButton = digitalRead(button2);
  bool downButton = digitalRead(button3);
  Serial.println("void loop position:");
  Serial.println(pos);
  // Aufwärts Knopf Code:
    if (upButton != lastState1)
    {
      if (upButton == HIGH)
      {
        Serial.println("Knopf 1 gedrückt -- ALM Start-kommando gesendet!");
        for (pos = 100; pos <= 179; pos += 1) { // goes from 150 degrees to 180 degrees
        // in steps of 1 degree
        myservo.write(pos);              // tell servo to go to position in variable 'pos'
        Serial.println(pos);
        delay(150);                       // waits 15 ms for the servo to reach the position
        }
        Serial.println("ALM Start-kommando durchgeführt");
      }
    }
  // StoppKnopf Code:
  if (stopButton != lastState2 || pos >= 1)
  {
    if (stopButton == HIGH)
    {
      Serial.println("Knopf 3 gedrückt -- ALM Lande-kommando gesendet!");
      //for (pos != 0; pos <= 100; pos -= 1) { // goes from 180 degrees to 0 degrees
      // darf nicht 0 sein wegen ESC und unsigned integer
      for (int i = 1; i < 120; i += 1){
        pos -= 1;
        myservo.write(pos);              // tell servo to go to position in variable 'pos'
        Serial.println(pos);
          delay(150);                       // waits 15 ms for the servo to reach the position
       
        if (pos <= 100) // Motoren abstellen
        {
          pos = 1;
          myservo.write(pos);
        }
       
      }
      Serial.println("ALM Lande-kommando durchgeführt");
    }
  }
  // AbwärtsKnopf Code:
  // while (pos > 140)
  if (downButton != lastState3)
  {
    if (downButton == HIGH && pos >= 150)
    {
      Serial.println("Knopf 3 gedrückt -- ALM Lande-kommando gesendet!");
      for (pos = 180; pos >= 150; pos -= 1) { // goes from 180 degrees to 0 degrees
        myservo.write(pos);              // tell servo to go to position in variable 'pos'
        Serial.println(pos);
        delay(150);                       // waits 15 ms for the servo to reach the position
      }
      Serial.println("ALM Lande-kommando durchgeführt");
    }
  }
}