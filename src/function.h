/*
	acquire
*/
//puts distance from ultrasonic sensor in distance[]
void getDistance();
//puts contrast values from adc in ligne[]
void getLine();
//get info from IR to update state
uint8_t getIR();
//read battery voltage from adc
float getBatteryVoltage();
/*
	processing
	*/
//convert adc value in Volt and activate ledBatt
//can stop the robot if battery is too low
uint8_t testBatt();
//check that value from right and left sensor are not wrong
int verifyInfo();
//compare threshold and adc value from contrast sensor
//can put the robot in escape
void gestionLine();
//use value from distance[] to know where the enemy robot is
uint8_t gestionPosAdv();
//use gestionPosAdv to move in the enemy direction
void attack();

/*
	action
	*/
//wite pwm ratio on the battery led
void ledBatt_Action(uint8_t ratio);
//wite pwm ratio on the statut rgb led
void ledEtat_Action();
//write values to H bridge for left motor
void motor_left(uint8_t in1, uint8_t in2);
//write values to H bridge for right motor
void motor_right(uint8_t in1, uint8_t in2);