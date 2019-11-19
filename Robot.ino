//========led struc to get cleaner code
struct led
{
	byte green;
	byte blue;
};
led ledEtat; //create a led object
//========= enum all adv_is values
enum adv_ise
{
	front,
	front_left,
	front_right,
	right,
	left,
	back
};
adv_ise adv_is;
//======== enum all state values
enum statee
{
	READY;
	START;
	STOP;
};
statee state;
//define for all pins
//pin de distance
//========================================
static byte pintrig[4] = {1, 2, 3, 4}, pinecho[4] = {1, 2, 3, 4}, pinligne[4] = {1, 2, 3, 4};
#define PIN_IR   //pin for ir sensor
#define PIN_BATT //pin for battery readings
//======= motors pin
#define PIN_A1 //left
#define PIN_A2
#define PIN_B1 //right
#define PIN_B2
//======== led pin
//led for status
#define PIN_LEDB
#define PIN_LEDG
//led for battery
#define PIN_LEDBATT
//========================================
//adc ratios to get a Volt value
static float ratioBit = 1023 / 5;
static float ratio = 10 / 25;
//======================================== global variables
unsigned long t0 = millis(), t1batt, t1line; //time variable to do timed stuff
byte distance[4];							 // tab to store distance values from ultrasonic sensors
byte line[4];								 //tab to store adc values from contrast sensors
bool lineCrossed = false;					 //warning used by line sensor, puts robot in escape mode
bool low = false;							 //warning for low battery voltage used to calculate time between low spike
//===========
#define limiteLine 500 //treshold for a line to be detected
//////////////////////////////////////////////////////////////////////////////////////////
void setup() //nothing to do for now in setup
{
}

void loop()
{
	bool problem = false;
	t1 = millis();
	//wait for a command from IR
	state = getIR();

	if (state == START)
	{
		state = testBatt();
		if (verifyInfo == -1) //verify info from ultrasonic sensors
			problem = true;
	}
	ledEtat.blue = 0;
	ledEtat.green = 0;
	if (!problem && state == START) //if battery and info are good then go
	{
		//little brain
		/*gewstion line*/
		if (gestionLine() == -1)
		{
			while (t1line - t0 < 200) //wait to escape a line
			{
				ledEtat.blue = 255; //signal that we are escaping
			}
			ledEtat.blue = 0;
			lineCrossed = false; //reset the warning from contrast sensor
		}
		/*search ennemy*/
		adv_is = gestionPosAdv();
		// use adv_is to know where to go
		attack(); // activate motors in the right direction
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
	for (byte i = 0; i < 4; i++)
	{
		digitalWrite(trigpin[i], LOW);
		delayMicroseconds(2);
		// Sets the trigPin on HIGH state for 10 micro seconds
		digitalWrite(trigpin[i], HIGH);
		delayMicroseconds(10);
		digitalWrite(trigpin[i], LOW);
		// Reads the echoPin, returns the sound wave travel time in microseconds
		duration = pulseIn(echopin[i], HIGH);
		// Calculating the distance
		distance[i] = duration * 0.0343 / 2;
	}
}

//puts contrast values from adc in ligne[]
void getLine()
{
	for (byte i = 0; i < 4; i++)
	{
		static int dummy = analogRead(pinligne[i]);
		line[i] = analogRead(pinligne[i]);
	}
}
//get info from IR to update state
byte getIR()
{
	if (digitalRead(PIN_IR) == HIGH)
		return START;
	else
		return STOP;
}
//read battery voltage from adc
int getBatteryVoltage()
{
	return analogRead(PIN_BATT); //return a raw value
}

/*
processing
*/

//convert adc value in Volt and activate ledBatt
//can stop the robot if battery is too low
byte testBatt()
{
	float val = getBatteryVoltage(); //get raw value
	val = val / ratioBit;			 //do calc to get Volt
	val = val / ratio;
	if (val < 6.7) //if low start timing
	{
		t1batt = millis();
		low = true ledBatt_Action(255);
		if (!low)
		{
			t0 = millis();
			ledBatt_Action(0);
		}
		if (low == true && t1batt - t0 > 100) //if low for too long STOP robot
			return STOP;					  //if low for too long then stop the robot
	}
	if (val < 7) //led is mid brightness to say im low
		ledBatt_Action(100);
	return START; //everything is good
}

//check that value from right and left sensor are not wrong
int verifyInfo()
{
	if (distance[0] < 10 && distance[3] < 10) //verify that both captor are not activated at the same time
		return -1;
}

//compare threshold and adc value from contrast sensor
//can put the robot in escape
byte gestionLine()
{
	if (line[0] > limiteLine) //back left
	{
		lineCrossed = true
			/////////moteur vers avant droite
			motor_left(1, 0);
		motor_right(0, 0);
	}
	if (line[1] > limiteLine) //front left
	{
		lineCrossed = true
			/////////moteur vers arriere droite
			motor_left(0, 1);
		motor_right(0, 0);
	}
	if (line[2] > limiteLine) //front right
	{
		lineCrossed = true
			/////////moteur vers arriere gauche
			motor_left(0, 0);
		motor_right(0, 1);
	}
	if (line[3] > limiteLine) //back right
	{
		lineCrossed = true
			/////////moteur vers avant gauche
			motor_left(0, 0);
		motor_right(1, 0);
	}
	if (lineCrossed)
		return -1 else return 0;
}

//use value from distance[] to know where the ennemy robot is
byte gestionPosAdv()
{
	byte i = 0;
	for (i = 0; i < 5; i++)
	{
		if (i < 4)
		{
			if (distance[i] < 80)
				return
		}
	}
	if (i == 0)
		return left;
	if (i == 3)
		return right;
	if (i == 1)
	{
		if (distance[2] < 80)
			return front;
		else
			return front_left;
	}

	if (i == 2)
	{
		if (distance[3] < 80)
			return front;
		else
			return front_right;
	}
	if (i == 5)
		return back
}

//use gestionPosAdv to move in the ennemy direction
void attack()
{
	ledEtat.green = 255;
	static byte last switch (adv_is)
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
void ledBatt_Action(byte ratio)
{
	analogWrite(PIN_LEDBATT, ratio);
}

//wite pwm ratio on the statut rgb led
void ledEtat_Action()
{
	analogWrite(PIN_LEDG, ledEtat.green);
	analogWrite(PIN_LEDB, ledEtat.blue);
}

//write values to H bridge for left motor
void motor_left(byte in1, byte in2)
{
	if (in1 == 1)
		digitalWrite(PIN_A1, HIGH);
	else
		digitalWrite(PIN_A1, LOW);
	if (in2 == 1)
		digitalWrite(PIN_A2, HIGH);
	else
		digitalWrite(PIN_A2, LOW);
}

//write values to H bridge for right motor
void motor_right(byte in1, byte in2)
{
	if (in1 == 1)
		digitalWrite(PIN_B1, HIGH);
	else
		digitalWrite(PIN_B1, LOW);
	if (in2 == 1)
		digitalWrite(PIN_B2, HIGH);
	else
		digitalWrite(PIN_B2, LOW);
}