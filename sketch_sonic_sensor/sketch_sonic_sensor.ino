// ---------------------------------------------------------------- //
// Arduino Ultrasoninc Sensor HC-SR04
// Re-writed by Arbi Abdul Jabbaar
// Using Arduino IDE 1.8.7
// Using HC-SR04 Module
// Tested on 17 September 2019
// ---------------------------------------------------------------- //

#define echoPin 2 // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin 3 //attach pin D3 Arduino to pin Trig of HC-SR04

#define lalarmPin 7 // attach pin D8 Arduino to pin Echo of HC-SR04
#define lresetPin 8 //attach pin D9 Arduino to pin Trig of HC-SR04
#define resetPin 9 //attach pin D9 Arduino to pin Trig of HC-SR04

// defines variables
long duration; // variable for the duration of sound wave travel
int distance; // variable for the distance measurement
int frequency;
float lfreq;
float hfreq;

void setup() {
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
  pinMode(lalarmPin, OUTPUT); // Sets the alarmPin as an OUTPUT
  pinMode(lresetPin, OUTPUT); // Sets the resetPin as an INPUT
  pinMode(resetPin, INPUT_PULLUP); // Sets the resetPin as an INPUT
  Serial.begin(9600); // // Serial Communication is starting with 9600 of baudrate speed
  Serial.println("Ultrasonic Sensor HC-SR04 Test"); // print some text in Serial Monitor
  Serial.println("with Arduino UNO R3");
}

void reset()
{
  digitalWrite(alarmPin, LOW);
  Serial.print("High Frequency: ");
  Serial.print(hfreq);
  Serial.print(" hz.....");
  Serial.print("Low Frequency: ");
  Serial.print(lfreq);
  Serial.println(" hz");
  Serial.print("High Distance: ");
  Serial.print(hdist);
  Serial.print(" cm.....");
  Serial.print("Low Distance: ");
  Serial.print(ldist);
  Serial.println(" cm");
  
  lfreq=0;
  hfreq=0;
  ldur=0;
  hdur=0;
}

void loop() {
  // Clears the trigPin condition
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  
  if (digitalRead(resetPin))
  {
    reset();
    while (digitalRead(resetPin));
  }
  
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  
  // Calculating the distance
  distance = duration * 0.034 / 2; //  Speed of sound wave divided by 2 (go and back)

  if (distance > hdist)
  {
    hdist=distance;
  }
  else if (distance < ldist)
  {
    ldist=distance;
  }

  frequency = 1/duration;
  if (frequency > hfreq)
  {
    hfreq=frequency;
  }
  else if (frequency < lfreq)
  {
    lfreq = frequency;
  }
  
    
  Serial.print("Frequency: ");
  Serial.print(frequency);
  Serial.println(" hz");
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
}
