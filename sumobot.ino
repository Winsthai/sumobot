const int trigPin = 14;
const int echoPin = 27;
const int lightpin_front = 35;
const int lightpin_rear = 34;
const int in1 = 25; //for motor 
const int in2 = 33;
const int in3 = 18; //used to be 0
const int in4 = 5; // used to be 4
const int testled = 26;

//define sound speed in cm/uS
#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701

// Photoresistor values
#define COLOR_DIFFERENCE 200 //used to be 500
int photofront = 4000;
int photorear = 4000;

#define LEFT 0
#define RIGHT 1
int currdirection = LEFT;

long duration;
float distanceCm;
float distanceInch;
#define LED_BUILTIN 2
int lightval;
int x;
int * ptr = &x;

//function prototypes
void forwards();
void backwards();
int photoresistor();
float calc_dist();
void run_motor2();
void rotate_right();
void rotate_left();
void stop();

float calc_dist() {
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);

  // Calculate the distance
  distanceCm = duration * SOUND_SPEED/2;

  // Convert to inches
  distanceInch = distanceCm * CM_TO_INCH;
  //Serial.print("ultrasonic ");
  //Serial.println(distanceCm);
  return distanceCm;

}
int photoresistor_front() {
  lightval = analogRead(lightpin_front);
  //Serial.print("photoresistor FRONT");
  Serial.println(lightval);
  return lightval;
}

int photoresistor_rear() {
  lightval = analogRead(lightpin_rear);
  //Serial.print("photoresistor REAR");
  Serial.println(lightval);
  return lightval;
}

// movement funcs
void forwards() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  digitalWrite(testled, HIGH);
  *ptr = 1;
  
}
void backwards() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  
  digitalWrite(testled, LOW);
  *ptr = 0;
}

void rotate_right() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void rotate_left() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void stop(){
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

void led() {
  digitalWrite(testled, LOW);
  delay(200);
  digitalWrite(testled, HIGH);
  delay(200);
  digitalWrite(testled, LOW);
  delay(200);
  digitalWrite(testled, HIGH);
  delay(200);
  digitalWrite(testled, LOW);
  delay(200);
  if (photoresistor() < 2400) {
    digitalWrite(testled, LOW);
    delay(1500);
  }
  else if (photoresistor() >= 2400){
    digitalWrite(testled, HIGH);
    delay(1500);
  }
}

void backOff(int direction) {
  backwards();
  delay(3000);
  if (direction == LEFT) {
    rotate_right();
    currdirection = RIGHT;
  } 
  else {
    rotate_left();
    currdirection = LEFT;
  }
  Serial.print("direction ");
  Serial.println(currdirection);
}

void startFunction() {
  rotate_right();
  delay(200);
  forwards();
  delay(500);
  rotate_left();
}

void setup() {
  Serial.begin(9600); // Starts the serial communication
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(lightpin_front, INPUT);
  pinMode(lightpin_rear, INPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(testled, OUTPUT);

  digitalWrite(testled, LOW);
  delay(200);
  digitalWrite(testled, HIGH);
  delay(200);
  digitalWrite(testled, LOW);
  delay(200);
  digitalWrite(testled, HIGH);
  delay(200);
  digitalWrite(testled, LOW);
  delay(200);

  startFunction();
}

void loop() {

  if (((photoresistor_front() - photofront) >= COLOR_DIFFERENCE) || ((photoresistor_rear() - photorear) >= COLOR_DIFFERENCE)){
    Serial.print(" edge detected");
    if (x == 0) {
      forwards();
      Serial.print("Going Forwards\n");
    }
    else if (x == 1)
      backwards();
      Serial.print("Going Backwards\n");
  }

  else if (calc_dist() > 25) {
    //Serial.print("rotate\n");
    digitalWrite(testled, LOW);
    rotate_right();
  }

  else if (calc_dist() <= 25) {
    //Serial.print("forwards\n");
    digitalWrite(testled, HIGH);
    forwards();
  }  
  
  photofront = photoresistor_front();
  photorear = photoresistor_rear();
  
  delay(50);
}




