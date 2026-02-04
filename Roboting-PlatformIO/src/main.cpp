#include <Arduino.h>
#include <objects.h>
#include <BluetoothSerial.h>

//Globals
MotorShield motors;
BluetoothSerial server;
PixelRing front_led, back_led;
VoltageSensor vSensor;

//VARIABLES
#define NOMBRE_BLUETOOTH "RobotingUC"
int LEFT_SPEED = 250;
int RIGHT_SPEED = 250;

#define RED 255, 0, 0
#define BLUE 0, 0, 255

//Bluetooth stop
#define CONECTION_DELAY 2000
unsigned long time_now=0, last_check=0;
bool running = false;


//==========================================
//Functions
void handleBluetoothStop();
void handleCommand(String command);
void spin();


//==========================================
//MAIN PROGRAM

void setup(){
  Serial.begin(115200);
  server.begin(NOMBRE_BLUETOOTH);
  delay(500);

  motors.init(ENABLE_L_PIN, A1_PIN, A2_PIN, ENABLE_R_PIN, B1_PIN, B2_PIN);
  // front_led.init(NUM_PIXELS, FRONT_LED, RED);
  // back_led.init(NUM_PIXELS, BACK_LED, BLUE);
  vSensor.init(VOLTAGE_PIN);

  Serial.println("Systems ready!\n");
  motors.stopMotors();
}


void loop(){
  time_now = get_time();
  
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
  char letter = command[0];
  //STOP
  if(letter == 'S'){
      motors.stopMotors();
      running = false;
      return;
  }

  running = true;
  switch (letter)
  {
  case 'F':
      motors.controlMotors(LEFT_SPEED, RIGHT_SPEED);
      break;
    case 'B':
      motors.controlMotors(-LEFT_SPEED, -RIGHT_SPEED);
      break;
    case 'L':
      motors.controlMotors(-LEFT_SPEED, RIGHT_SPEED);
      break;
    case 'R':
      motors.controlMotors(LEFT_SPEED, -RIGHT_SPEED);
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