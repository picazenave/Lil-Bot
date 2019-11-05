

byte distance[4];
byte ligne[4];
enum adv_is 
{
    front,
    front_left,
    front_right,
    right,
    left,
    back
};
enum state 
{
	READY;
    START;
    STOP;
};
//define de tous les pins
//pin de distance
static byte pintrig[4]={1,2,3,4},pinecho[4]={1,2,3,4},pinligne[4]={1,2,3,4};
#define PIN_IR
//========================================
#define PIN_BATT
static float ratioBit=1023/5;
static float ratio=15/25;
//========================================
unsigned long t0=millis(),t1batt,t1line;
bool lineCrossed=false;
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
	t1=millis();
	//depart ou non
    state=getIR();
	//verification
	if (state==START)
	{
	state=testBatt();
	if (verifyInfo==-1)
		return;
	}
	//little brain
	/*gewstion line*/
	if (gestionLine()==-1)
	{
		while(t1line-t0<200)
		{}
	lineCrossed=false;
	}
	/*search ennemy*/
	adv_is=gestionPosAdv();
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
    if(digitalRead(pinligne[i]))
    line[i]=1;
    else 
    ligne[i]=0;
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
	ledBatt(/////////) //a completer pour allumé
	if (!low)
		t0=millis();
	if (low==true && t1batt-t0>100)
		return STOP;
	}
	if (val<7)
	ledBatt(/////////) //a completer pour clignoter
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
	if (line[0]==HIGH)//back left
	{
		lineCrossed=true
		/////////moteur vers avant droite
	}
	if (line[1]==HIGH)//front left
	{
		lineCrossed=true
		/////////moteur vers arriere droite
	}
	if (line[2]==HIGH)//front right
	{
		lineCrossed=true
		/////////moteur vers arriere gauche
	}
	if (line[3]==HIGH)//back right
	{
		lineCrossed=true
		/////////moteur vers avant gauche
	}
	if (lineCrossed)
		return -1
	else 
		return 0;
}


byte gestionPosAdv()
{
	byte i=0;
	for(i=0;i<4;i++)
	{
		if (distance[i]<80)// en cours
		return
	}
	if (i==0)
		return left;
	if (i==3)
		return right;
	if (i==
}
/*
action
*/
