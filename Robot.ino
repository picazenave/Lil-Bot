

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
    ready;
    start;
    stop;
};
//define de tous les pins
//pin de distance
byte pintrig[4]={1,2,3,4},pinecho[4]={1,2,3,4},pinligne[4]={1,2,3,4};


struct led
{
    byte red;
    byte green;
    byte blue;
};
led ledBatt;
led ledEtat;
//////////////////////////////////////////////////////////////////////////////////////////
void setup()
{
}

void loop()
{
    state=getIR();
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


void getDistance()
{
    for(byte i=0;i<4;i++)
    {
    if(digitalRead(pinligne[i]))
    line[i]=1;
    else 
    ligne[i]=0;
    }
}

void getIR()
{
    if(digitalRead(PIN_IR)==HIGH)
    return start;
}
/*
processing
*/

/*
action
*/