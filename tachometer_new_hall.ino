//https://www.homemade-circuits.com/tachometer-using-arduino/

int sensor = 2; // Hall sensor at pin 2
volatile byte counts;
unsigned int rpm; //unsigned gives only positive values
unsigned long previoustime;
void count_function()
{ /*The ISR function
Called on Interrupt
Update counts*/
counts++;
}
void setup() {
Serial.begin(9600);
//Intiates Serial communications
attachInterrupt(0, count_function, RISING); //Interrupts are called on Rise of Input
pinMode(sensor, INPUT); //Sets sensor as input
counts= 0;
rpm = 0;
previoustime = 0; //Initialise the values
}
void loop()
{
delay(1000);//Update RPM every second
detachInterrupt(0); //Interrupts are disabled
rpm = 60*1000/(millis() - previoustime)*counts;
previoustime = millis(); //Resets the clock
counts= 0; //Resets the counter
Serial.print("RPM=");
Serial.println(rpm); //Calculated values are displayed
attachInterrupt(0, count_function, RISING); //Counter restarted
}
