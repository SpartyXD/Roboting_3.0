#include <Arduino.h>
#include <objects.h>
#include <BluetoothSerial.h>
#include <Preferences.h>

//Globals
MotorShield motors;
BluetoothSerial server;
Preferences data;
PixelRing front_led, back_led;

//VARIABLES
#define NOMBRE_BLUETOOTH "Limoning UC"
int LEFT_SPEED = 250;
int RIGHT_SPEED = 250;
int FRONT_COLOR[3] = {255, 0, 0};
int BACK_COLOR[3] = {0, 0, 255};

//Bluetooth stop
#define CONECTION_DELAY 1000
unsigned long time_now=0, last_check=0;
bool running = false;


//==========================================
//Functions
void handleBluetoothStop();
void handleCommand(String command);
void handleConfig(String command);
void spin();


//==========================================
//MAIN PROGRAM

void setup(){
  Serial.begin(115200);
  data.begin("RobotingUC");
  server.begin(NOMBRE_BLUETOOTH);
  delay(500);

  motors.init(PWM_A, A1_PIN, A2_PIN, PWM_B, B1_PIN, B2_PIN);
  front_led.init(NUM_PIXELS, FRONT_LED, 255, 0, 0);
  back_led.init(NUM_PIXELS, BACK_LED, 0, 0, 255);

  LEFT_SPEED = data.getInt("L", 250);
  RIGHT_SPEED = data.getInt("R", 250);

  Serial.println("Systems ready!\n");
}


void loop(){
  time_now = get_time();
  front_led.setColor(255, 0, 0);

  while(server.available()){
    last_check = time_now;
    handleCommand(server.readStringUntil('\n'));
  }

  //Check if bluetooth lost conection
  handleBluetoothStop();
}

//==========================================
//Implementations

void handleCommand(String command){
  //Config
  if(command.length() > 2 && (command[1] >= '0' && command[1] <= '9')){
    handleConfig(command);
    return;
  }

  char letter = command[0];
  //STOP
  if(letter == 'S'){
      motors.stopMotors();
      running = false;
      Serial.println("STOP!\n");
      return;
  }

  running = true;
  switch (letter)
  {
  case 'F':
      motors.controlMotors(LEFT_SPEED, RIGHT_SPEED);
      Serial.println("Forward!\n");
      break;
    case 'B':
      motors.controlMotors(-LEFT_SPEED, -RIGHT_SPEED);
      Serial.println("Backwards!\n");
      break;
    case 'L':
      motors.controlMotors(-LEFT_SPEED, RIGHT_SPEED);
      Serial.println("Turning Left!\n");
      break;
    case 'R':
      motors.controlMotors(LEFT_SPEED, -RIGHT_SPEED);
      Serial.println("Turning right!\n");
      break;
    case 'Z':
      spin();
      break;
  //------------------------
  default:
      break;
  }
}


void handleBluetoothStop(){
  if(running && (time_now-last_check) > CONECTION_DELAY){
    motors.stopMotors();
    running = false;
  }
}


void spin(){
  motors.controlMotors(LEFT_SPEED, -RIGHT_SPEED);
  delay(1250);
  motors.stopMotors();
}


void handleConfig(String command){
  command.trim();
  char letter = command[0];
  int split = command.indexOf(" ");

  if(letter == 'M'){
    //Motor config
    LEFT_SPEED = constrain(command.substring(1, split).toInt(), 0, 255);
    RIGHT_SPEED = constrain(command.substring(split+1).toInt(), 0, 255);

    data.putInt("L", LEFT_SPEED);
    data.putInt("R", RIGHT_SPEED);

    Serial.println("Set motor speeds: " + String(LEFT_SPEED) + " " + String(RIGHT_SPEED));
  }

  if(letter == 'F' || letter == 'B'){
    // Light config
    command = command.substring(1);
    
    int vals[3] = {0, 0, 0};
    rep(i, 3){
      split = command.indexOf(" ");
      String valStr;

      if(split == -1){
        valStr = command; // último valor
      } else {
        valStr = command.substring(0, split);
        command = command.substring(split + 1);
      }

      vals[i] = constrain(valStr.toInt(), 0, 255);
    }

    if(letter == 'F')
      front_led.setColor(vals[0], vals[1], vals[2]);
    else
      back_led.setColor(vals[0], vals[1], vals[2]);
    
    Serial.println("Set LED {" + String(letter) + "} -> " + String(vals[0]) + ", " + String(vals[1]) + ", " + String(vals[2]));
  }

  
}