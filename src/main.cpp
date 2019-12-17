#include <Arduino.h>
#include "function.h"
//========led struc to get cleaner code
struct led
{
	uint8_t green;
	uint8_t blue;
};
led ledEtat; //create a led object
//========= enum all adv_is values(changd to define bc broken on avr compiler)
#define front 0
#define front_left 1
#define front_right 2
#define right 3
#define left 4
#define back 5
uint8_t adv_is;
//======== enum all state values
#define READY 0
#define START 1
#define STOP 2
uint8_t state;
//define for all pins
//pin de distance
//========================================
static uint8_t pintrig[4] = {0, A5, 12, 8}, pinecho[4] = {1, 2, 13, 5}, pinligne[4] = {A0, A2, A7, A1};
//                           0                             1
#define PIN_IR A3   //pin for ir sensor
#define PIN_BATT A6 //pin for battery readings
//======= motors pin
#define PIN_BM1 4 //left
#define PIN_BM2 3
#define PIN_AM1 6 //right
#define PIN_AM2 7
//======== led pin
//led for status
#define PIN_LEDB 9
#define PIN_LEDG 10
//led for battery
#define PIN_LEDBATT 11
//========================================
//adc ratios to get a Volt value
static float ratio = 25 / 10;
//======================================== global variables
unsigned long t0 = millis(), t1batt, t1line, t1; //time variable to do timed stuff
uint8_t distance[4];							 // tab to store distance values from ultrasonic sensors
uint8_t line[4];								 //tab to store adc values from contrast sensors
bool lineCrossed = false;						 //warning used by line sensor, puts robot in escape mode
bool low = false;								 //warning for low battery voltage used to calculate time between low spike
//===========
#define limiteLine 80 //treshold for a line to be detected
#define distance_trig 40
//////////////////////////////////////////////////////////////////////////////////////////
void setup() //nothing to do for now in setup
{
	//Serial.begin(9600);
	pinMode(PIN_IR,INPUT);
	for (int i = 0; i < 4; i++)
	{
		pinMode(pintrig[i], OUTPUT);
		pinMode(pinecho[i], INPUT);
		pinMode(pinligne[i], INPUT);
	}
	while (getIR()==STOP)
	{}
	delay(5000);
}

void loop()
{
	
	ledEtat.blue = 0;
	ledEtat.green = 0;
	if (getIR() == START) //if battery and info are good then go
	{
	
	if (testBatt() == START)
	{
		//little brain
		/*gewstion line*/
		gestionLine();
		/*search ennemy*/
		adv_is = gestionPosAdv();
		// use adv_is to know where to go
		attack(); // activate motors in the right direction
	}
	}
	else
	{
		motor_left(1,1);
	motor_right(1,1);
	}
	
	ledEtat_Action(); //write pwm values on leds
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
	acquire
*/
//puts distance from ultrasonic sensor in distance[]
void getDistance()
{
	for (uint8_t i = 0; i < 4; i++)
	{
		digitalWrite(pintrig[i], LOW);
		delayMicroseconds(2);
		// Sets the trigPin on HIGH state for 10 micro seconds
		digitalWrite(pintrig[i], HIGH);
		delayMicroseconds(10);
		digitalWrite(pintrig[i], LOW);
		// Reads the echoPin, returns the sound wave travel time in microseconds
		float duration = pulseIn(pinecho[i], HIGH);
		// Calculating the distance
		distance[i] = duration * 0.0343 / 2;
		//Serial.println((String)"distance"+i+":"+distance[i]);
	}
	//Serial.println("------------------");
}

//puts contrast values from adc in ligne[]
void getLine()
{
	for (uint8_t i = 0; i < 4; i++)
	{
		static int dummy = analogRead(pinligne[i]);
		line[i] = analogRead(pinligne[i]);
		//Serial.println((String)"line"+i+":"+line[i]);
	}
}
//get info from IR to update state
uint8_t getIR()
{
	if (digitalRead(PIN_IR) == HIGH)
		return START;
	else
		return STOP;
}
//read battery voltage from adc
int getBatteryVoltage()
{
	int dummy=analogRead(PIN_BATT);
	int val=analogRead(PIN_BATT)*10;
	return  val;//return a raw value
}

/*
	processing
	*/

//convert adc value in Volt and activate ledBatt
//can stop the robot if battery is too low
uint8_t testBatt()
{
	float val=map(getBatteryVoltage(),0,10230.0,0,50);
	val = val / 4;
	//Serial.println(val);
	
	if (val < 6.7) //if low start timing
	{
		ledBatt_Action(255);
		motor_left(0,0);
		motor_right(0,0);
		return STOP; //if low for too long then stop the robot
	}
	if (val < 7) //led is mid brightness to say im low
		ledBatt_Action(100);
	else
	{
		ledBatt_Action(0);
	}
	
	return START; //everything is good
}

//compare threshold and adc value from contrast sensor
//can put the robot in escape
void gestionLine()
{
	getLine();
	//for (int i = 0; i < 4; i++)
	//Serial.println((String)"line"+i+":"+line[i]);
	lineCrossed=false;
	if (line[0] < limiteLine) //back left
	{
		lineCrossed = true;
		/////////moteur vers avant droite
		
		motor_left(1, 0);
		motor_right(1, 1);
	} else if (line[1] < limiteLine) //front left
	{
		lineCrossed = true;
		/////////moteur vers arriere droite
		motor_left(1, 1);
		motor_right(0,1);
	} else if (line[2] < limiteLine) //front right
	{
		lineCrossed = true;
		/////////moteur vers arriere gauche
		motor_left(0, 1);
		motor_right(1, 1);
	}else if (line[3] < limiteLine) //back right
	{
		lineCrossed = true;
		/////////moteur vers avant gauche
		motor_left(1, 1);
		motor_right(1, 0);
	} 
	
	if (lineCrossed)
	{
		ledEtat.blue=255;
		ledEtat_Action();
		delay(400);
		motor_left(1, 0);
		motor_right(1, 0);
			delay(100);
		ledEtat.blue=0;
	}
}

//use value from distance[] to know where the enemy robot is
uint8_t gestionPosAdv()
{
	getDistance();
	uint8_t i, j = 0;

	for (i = 0; i < 4; i++)
	{
		if (distance[i] < distance[j])
			j = i;
	}
	if (distance[j] > distance_trig)
	{
		j = 4;
	}

	//Serial.println(j);
	if (j == 0)
		return left;
	if (j == 3)
		return right;
	if (j == 1)
	{
		if (distance[2] < distance_trig)
			return front;
		else
			return front_left;
	}

	if (j == 2)
	{
		if (distance[3] < distance_trig)
			return front;
		else
			return front_right;
	}
	if (j == 4)
		return back;
	return back;
}

//use gestionPosAdv to move in the enemy direction
void attack()
{
	ledEtat.green = 255;
	static uint8_t last;
	switch (adv_is)
	{
	case left:
		motor_left(1, 1);
		motor_right(1, 0);
		break;

	case front_left:
		motor_left(0, 0);
		motor_right(1, 0);
		break;
	case front:
		motor_left(1, 0);
		motor_right(1, 0);
		break;
	case front_right:
		motor_left(1, 0);
		motor_right(0, 0);
		break;
	case right:
		motor_left(1, 0);
		motor_right(1, 1);
		break;

	case back: //little trick to know wich path is faster to get adv in line of sight
		if (last == right)
		{
			motor_left(1, 0);
			motor_right(0, 1);
		}
		if (last == left)
		{
			motor_left(0, 1);
			motor_right(1, 0);
		}
		break;

	default:
		break;
	}
	if (last != back)
		last = adv_is;
}
/*
	action
	*/

//wite pwm ratio on the battery led
void ledBatt_Action(uint8_t ratio)
{
	analogWrite(PIN_LEDBATT, 255 - ratio);
}

//wite pwm ratio on the statut rgb led
void ledEtat_Action()
{
	analogWrite(PIN_LEDG, 255 - ledEtat.green);
	analogWrite(PIN_LEDB, 255 - ledEtat.blue);
}

//write values to H bridge for left motor
void motor_left(uint8_t in1, uint8_t in2)
{
	if (in1 == 1)
		digitalWrite(PIN_AM1, HIGH);
	else
		digitalWrite(PIN_AM1, LOW);
	if (in2 == 1)
		digitalWrite(PIN_AM2, HIGH);
	else
		digitalWrite(PIN_AM2, LOW);
}

//write values to H bridge for right motor
void motor_right(uint8_t in1, uint8_t in2)
{
	if (in1 == 1)
		digitalWrite(PIN_BM1, HIGH);
	else
		digitalWrite(PIN_BM1, LOW);
	if (in2 == 1)
		digitalWrite(PIN_BM2, HIGH);
	else
		digitalWrite(PIN_BM2, LOW);
}