

byte distance[4];
byte ligne[4];
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
enum statee 
{
	READY;
    START;
    STOP;
};
statee state;
//define de tous les pins
//pin de distance
static byte pintrig[4]={1,2,3,4},pinecho[4]={1,2,3,4},pinligne[4]={1,2,3,4};
#define PIN_IR
//========================================
#define PIN_BATT
#define PIN_A1 //gauche
#define PIN_A2
#define PIN_B1 //droite
#define PIN_B2

static float ratioBit=1023/5;
static float ratio=10/25;
//========================================
unsigned long t0=millis(),t1batt,t1line;
#define limiteLine 500 //treshold for a line to be detected
//========================================
struct led
{
    byte green;
    byte blue;
};
led ledEtat;
//////////////////////////////////////////////////////////////////////////////////////////
void setup()
{
}

void loop()
{
	bool problem=false;
	t1=millis();
	//depart ou non
    state=getIR();
	//verification
	if (state==START)
	{
	state=testBatt();
	if (verifyInfo==-1)// verify if info are good 
		problem=true;
	}

	if (!problem && state==START)
	{
	//little brain
	/*gewstion line*/
	if (gestionLine()==-1)
	{
		while(t1line-t0<200)//wait to escape a line
		{}
	lineCrossed=false;
	}
	/*search ennemy*/
	adv_is=gestionPosAdv();
	// use adv_is to know where to go
	attack();

	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
acquire
*/
//puts distance from sensor in distance[]
void getDistance()
{
    for(byte i=0;i<4;i++)
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
distance[i]= duration*0.0343/2;
    }
}


void getLine()
{
    for(byte i=0;i<4;i++)
    {
	static int dummy=analogRead(pinligne[i]);
    line[i]=analogRead(pinligne[i]);
    }
}

byte getIR()
{
    if(digitalRead(PIN_IR)==HIGH)
    return START;
	else return STOP;
}

int getBatteryVoltage()
{
	return analogRead(PIN_BATT);
}
/*
processing
*/
bool low=false;
byte testBatt()
{
	float val=getBatteryVoltage();
	val=val/ratioBit;
	val=val/ratio;
	if (val<6.7)
	{
		t1batt=millis();
		low=true
	ledBatt_Action(255);
	if (!low)
	{
		t0=millis();
		ledBatt_Action(0);
	}
	if (low==true && t1batt-t0>100)
		return STOP;
	}
	if (val<7)
	ledBatt_Action(100);
return START;
}

int verifyInfo()
{
		if(distance[0]<10 && distance[3]<10)
			return -1;
}
//===========
byte gestionLine()
{
	bool lineCrossed=false;
	if (line[0]>limiteLine)//back left
	{
		lineCrossed=true
		/////////moteur vers avant droite
		motor_left(1,0);
		motor_right(0,0);
	}
	if (line[1]>limiteLine)//front left
	{
		lineCrossed=true
		/////////moteur vers arriere droite
		motor_left(0,1);
		motor_right(0,0);
	}
	if (line[2]>limiteLine)//front right
	{
		lineCrossed=true
		/////////moteur vers arriere gauche
		motor_left(0,0);
		motor_right(0,1);
	}
	if (line[3]>limiteLine)//back right
	{
		lineCrossed=true
		/////////moteur vers avant gauche
		motor_left(0,0);
		motor_right(1,0);
	}
	if (lineCrossed)
		return -1
	else 
		return 0;
}


byte gestionPosAdv()
{
	byte i=0;
	for(i=0;i<5;i++)
	{
		if (i<4)
		{
		if (distance[i]<80)
		return
		}

	}
	if (i==0)
		return left;
	if (i==3)
		return right;
	if (i==1)
	{
		if (distance[2]<80)
		return front;
		else return front_left;
	}

	if (i==2)
	{
		if(distance[3]<80)
		return front;
		else return front_right;
	}
	if (i==5)
	return back
}

void attack()
{
	static byte last
	switch (adv_is)
	{
	case left:
		motor_left(1,1);
		motor_right(1,0);
		break;

	case front_left:
		motor_left(0,0);
		motor_right(1,0);
		break;
	case front:
		motor_left(1,0);
		motor_right(1,0);
		break;
	case front_right:
		motor_left(1,0);
		motor_right(0,0);
		break;
	case right:
		motor_left(1,0);
		motor_right(1,1);
		break;

	case back:
	if (last==right)
	{
		motor_left(1,0);
		motor_right(0,1);
	}
	if (last==left)
	{
		motor_left(0,1);
		motor_right(1,0);
	}
		break;
	if (last!=back)
	last=adv_is;
	default:
		break;
	}
}
/*
action
*/
void ledBatt_Action(byte ratio)
{
	analogWrite(PIN_LEDBATT,ratio);
}

void ledEtat_Action()
{
	analogWrite(PIN_LEDG,ledEtat.green);
	analogWrite(PIN_LEDB,ledEtat.blue);
}

void motor_left(byte in1,byte in2)
{
	if (in1==1)
	digitalWrite(PIN_A1,HIGH);
	else digitalWrite(PIN_A1,LOW); 
	if (in2==1)
	digitalWrite(PIN_A2,HIGH);
	else digitalWrite(PIN_A2,LOW); 

}

void motor_right(byte in1,byte in2)
{
	if (in1==1)
	digitalWrite(PIN_B1,HIGH);
	else digitalWrite(PIN_B1,LOW); 
	if (in2==1)
	digitalWrite(PIN_B2,HIGH);
	else digitalWrite(PIN_B2,LOW); 

}