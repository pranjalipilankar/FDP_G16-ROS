//Including Required To use ROS
#include <WiFi.h>
#include <ros.h>
#include <std_msgs/String.h>

// Pin assignments for motor control
const int motor1Pin1 = 26;
const int motor1Pin2 = 27;
const int motor1EnablePin = 14; // PWM capable pin
const int motor2Pin1 = 25;
const int motor2Pin2 = 33;
const int motor2EnablePin = 32; // PWM capable pin
const int trig_front=4;
const int echo_front=36;
const int trig_back=17;
const int echo_back=18;
int speed1 = 0;
long duration;
long x_front;
long x_back;

//
const char *ssid = "POCO M2 Pro";
const char *pass = "bhulgaye";
IPAddress server(192,168,69,98);
//Defining sensors
//Ultrasonic ultrasonic1(trig_front, echo_front);
//Ultrasonic ultrasonic2(trig_back, echo_back);

// client used to connect to wifi
WiFiClient client;

// this is hardware class
class WiFiHardware
{
  public:

    WiFiHardware() {};

    void init()
    {
      // do your initialization here. this probably includes TCP server/client setup
      client.connect(server, 11511);
    }

    // read a byte from the serial port. -1 = failure
    int read()
    {
      // implement this method so that it reads a byte from the TCP connection and returns it
      //  you may return -1 is there is an error; for example if the TCP connection is not open
      return client.read();  //will return -1 when it will works
    }

    // write data to the connection to ROS
    void write(uint8_t *data, int length)
    {
      // implement this so that it takes the arguments and writes or prints them to the TCP connection
      for (int i = 0; i < length; i++)
        client.write(data[i]);
    }

    // returns milliseconds since start of program
    unsigned long time()
    {
      return millis();  // easy; did this one for you
    }
};

ros::NodeHandle_<WiFiHardware> nh;

void ConnectToWifi()
{
  WiFi.mode(WIFI_STA);
  delay(10);
  Serial.println("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
}


std_msgs::String keycode;
std_msgs::String s;
//s.data
String key;

void brake(){
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, LOW);
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, LOW);
    analogWrite(motor1EnablePin, 0);
    analogWrite(motor2EnablePin, 0);

}
void backward(int s2){
  //if(ultrasonic1.distanceRead()>5){
    digitalWrite(motor1Pin1, HIGH);
    digitalWrite(motor1Pin2, LOW);
    digitalWrite(motor2Pin1, HIGH);
    digitalWrite(motor2Pin2, LOW);
    analogWrite(motor1EnablePin, s2);
    analogWrite(motor2EnablePin, s2);
  //}
}
void forward(int s2){
  //if(ultrasonic2.read()>5){
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, HIGH);
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, HIGH);
    analogWrite(motor1EnablePin, s2);
    analogWrite(motor2EnablePin, s2);
  //}
}
void right(int s2){
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, HIGH);
    digitalWrite(motor2Pin1, HIGH);
    digitalWrite(motor2Pin2, LOW);
    analogWrite(motor1EnablePin, s2);
    analogWrite(motor2EnablePin, s2);
}
void left(int s2){
    digitalWrite(motor1Pin1,HIGH);
    digitalWrite(motor1Pin2,LOW);
    digitalWrite(motor2Pin1,LOW);
    digitalWrite(motor2Pin2,HIGH);
    analogWrite(motor1EnablePin,s2);
    analogWrite(motor2EnablePin,s2);
}
// Function to process keyboard input
void processInput1(String s1, int x1,int x2){
  // Extract the keyboard command
  String command = s1;

  // Motor control logic based on keyboard command
  if ((command == "W" || command =="w") && x1>7){forward(speed1); }
  else if ((command == "S" || command =="s") && x2>7){backward(speed1);}
  else if (command == "A" ||command =="a" ){left(speed1);}
  else if (command == "D" || command =="d"){right(speed1);}
  else if (command == "X" || command =="x"){brake();}
  else if (command == "+"){
    if((speed1+5)>255){
      speed1=255;
    }
    else{
      speed1+=5;
    }
  }
  else if (command == "-"){
    if (speed1-5<0){
      speed1=0;
    }
    else{
      speed1-=5;
    }
  }
  else if(command=="0"){speed1=0;}
  else if(command=="1"){speed1=85;}
  else if(command=="2"){speed1=170;}
  else if(command=="3"){speed1=255;}
  return;
}

void messageCb( const std_msgs::String& msg)
{
  key = msg.data;
  Serial.println(key);
  Serial.println(1);
}

ros::Subscriber<std_msgs::String> sub("keyboard_input", &messageCb );

void setup()
{
  Serial.begin(115200);
  ConnectToWifi();
  nh.initNode();
  nh.subscribe(sub); 
  // Configure motor control pins as outputs
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(motor1EnablePin, OUTPUT);
  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);
  pinMode(motor2EnablePin, OUTPUT);
  pinMode(trig_front,OUTPUT);
  pinMode(echo_front,INPUT);
  pinMode(trig_back,OUTPUT);
  pinMode(echo_back,INPUT);
}

void loop()
{
  //Serial.println(key);
  digitalWrite(trig_front, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trig_front, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_front, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echo_front, HIGH);
  
  // Calculate the distance
  x_front = duration * 0.017;
  
  digitalWrite(trig_back, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trig_back, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_back, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echo_back, HIGH);
  
  // Calculate the distance
  x_back = duration * 0.017;
  processInput1(key,x_front,x_back);
  nh.spinOnce();
}
