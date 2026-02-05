#include <Arduino.h>
#include <objects.h>
#include <BluetoothSerial.h>

//Globals
MotorShield motors;
BluetoothSerial server;
PixelRing front_led, back_led;
VoltageSensor vSensor;

//VARIABLES
int LEFT_SPEED = 250;
int RIGHT_SPEED = 250;

//Bluetooth
#define NOMBRE_BLUETOOTH "RobotingUC"
#define CONECTION_DELAY 750
#define BT_BLINK_DELAY 1000

unsigned long time_now=0, last_bt_check=0, last_bt_blink=0;
bool active = false;
bool last_bt_state = true;

//Battery
bool low_battery = false;
unsigned long last_battery_blink=0;
#define BATTERY_BLINK_DELAY 2000

//==========================================
//Functions
void BTsearch();
void handleBluetoothStop();
void handleCommand(char command);

void lowBatteryMode();

void spin();
void stop();


//==========================================
//MAIN PROGRAM

void setup(){
  Serial.begin(115200);
  server.begin(NOMBRE_BLUETOOTH);

  motors.init(ENABLE_L_PIN, A1_PIN, A2_PIN, ENABLE_R_PIN, B1_PIN, B2_PIN);
  front_led.init(NUM_PIXELS, FRONT_LED, RED);
  back_led.init(NUM_PIXELS, BACK_LED, BLUE);
  vSensor.init(VOLTAGE_PIN);

  Serial.println("Systems ready!\n");
  motors.stopMotors();
  vTaskDelay(500);
}


void loop(){
  time_now = get_time();

  //Handle battery
  if(vSensor.get_voltage() <= LOW_VOLTAGE_THRESHOLD && !active){
    if(!low_battery){
      low_battery = true;
      front_led.turnOff();
      back_led.setColor(PURPLE);
    }

    lowBatteryMode();
    vTaskDelay(80);
    return;
  }

  if(low_battery){
    low_battery = false;
    front_led.setColor(RED);
    back_led.setColor(BLUE);
  }


  //Check bluetooth connection
  if(!server.hasClient()){

    if(last_bt_state){
      last_bt_state = false;
      front_led.setColor(BLUE);
      back_led.setColor(BLUE);
    }

    BTsearch();
    vTaskDelay(80);
    return;
  }

  if(!last_bt_state){
    last_bt_state = true;
    front_led.setColor(RED);
    back_led.setColor(BLUE);
  }

  //Handle commands
  while(server.available()){
    last_bt_check = time_now;
    handleCommand(server.read());
  }

  //Check if controller communication broke
  handleBluetoothStop();
}

//==========================================
//Implementations

void handleCommand(char letter){
  //STOP
  if(letter == 'S'){
      stop();
      return;
  }

  active = true;
  switch (letter)
  {
  case 'F':
      motors.controlMotors(LEFT_SPEED, RIGHT_SPEED);
      front_led.setColor(RED);
      back_led.turnOff();
      break;
    case 'B':
      motors.controlMotors(-LEFT_SPEED, -RIGHT_SPEED);
      front_led.turnOff();
      back_led.setColor(BLUE);
      break;
    case 'L':
      motors.controlMotors(-LEFT_SPEED, RIGHT_SPEED);
      front_led.setColor(RED);
      back_led.setColor(RED);
      break;
    case 'R':
      motors.controlMotors(LEFT_SPEED, -RIGHT_SPEED);
      front_led.setColor(BLUE);
      back_led.setColor(BLUE);
      break;
    case 'Z':
      front_led.setColor(ORANGE);
      back_led.setColor(PINK);
      spin();
      break;
  //------------------------
  default:
      break;
  }
}


void lowBatteryMode(){
  if(time_now - last_battery_blink > BATTERY_BLINK_DELAY){
    back_led.toggle();
    last_battery_blink = time_now;
  }
}


void BTsearch(){
  if(time_now - last_bt_blink > BT_BLINK_DELAY){
    front_led.toggle();
    back_led.toggle();
    last_bt_blink = time_now;
  }
}


void handleBluetoothStop(){
  if(active && (time_now-last_bt_check) > CONECTION_DELAY){
    stop();
  }
}


void stop(){
    motors.stopMotors();
    active = false;
    front_led.setColor(RED);
    back_led.setColor(BLUE);
}


void spin(){
  motors.controlMotors(LEFT_SPEED, -RIGHT_SPEED);
  vTaskDelay(1800);
  motors.stopMotors();
}