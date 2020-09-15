

/*http://mundoprojetado.com.br/executar-duas-coisas-ao-mesmo-tempo/
  https://www.youtube.com/watch?v=AqfEu2cJrw0&t=166s
  https://www.youtube.com/watch?v=xpLw8xXlJKg&t=76s
  https://www.youtube.com/watch?v=L530rN5W9ow //transistor npn como funciona
  https://www.arduino.cc/en/pmwiki.php?n=Reference/If
  https://create.arduino.cc/projecthub/PracticeMakesBetter/easy-peasy-tachometer-20e73a
  https://hacksterio.s3.amazonaws.com/uploads/attachments/1057433/hall_sensor_vRoxwAejEW.JPG
  https://www.electronics-tutorials.ws/transistor/tran_4.html
  https://www.youtube.com/watch?v=pBgVsvhCzFM
  https://www.makerguides.com/character-i2c-lcd-arduino-tutorial/
  NPN,TIP35C, BASE Pino 1 recebe sinal pwm 5V que vem do arduino, COLETOR pino 2 sai o PWM NEGATIVO para acionar o NEGATIVO da BOBINA/MOTOR, EMISSOR Pino 3 ligado no negativo da bateria ...
  o GND do arduino tem de conectar direto no negativo da bateria... os POSITIVOS fica ligado direto nos 12v+
  TIP35C, o negativo eh pwm que vai pro bico, 12V+ liga direto
  Usar resistor 6R no positivo do bico de baixa pra não esquentar o bico...
  Pino com pwm no UNO 3, 5, 6, 9, 10, 11
  Pinagem pedal, da frente pra trás: fio 1 e 2 GND, fio 3 pedal -->A1, fio 4 e 5 positivo 5V+, fio 6 pedal -->A0
  || or arduino
  && and arduino
  /********************************************/

#include <Adafruit_SSD1306.h>
#include <splash.h>
#include <Wire.h>

# define TBI  5           //  Saida TBI
# define Bico 3           //  Saida Bico
# define TachInPin 2      //  Entrada RPM do modulo, usando TIP35 
# define analogInPin1 A0  //  Entrada analogica do potenciometro1 do pedal, comanda TBI
# define analogInPin2 A1  //  Entrada analogica do potenciometro2 do pedal, comanda Bico
# define LentaOutPin 9    //  Saida solenoide lenta, comanda marcha lenta  (ar), sugestão IRF840 como driver, não precisa de resistor na saida do pino

void pwmTBI();    // Funçao que controla abertura da TBI
void pwmBico();   // Funçao que controla o pulsar do Bico
void RPM();       // Função do contagiro, RPM
void LENTA();     // Função de controle ar marcha lenta solenoide

int TempoTBI = 0;
int Tempo1 = 0;
int TempoLenta = 0;

/***************** RPM **********************/

#define OLED_WIDTH 128
#define OLED_HEIGHT 64
#define OLED_ADDR   0x3C // A very common address for these displays.
Adafruit_SSD1306 display(OLED_WIDTH, OLED_HEIGHT);
float value = 0;
float rev = 0;
int rpm;
int oldtime = 0;
int time;

void isr()          //interrupt service routine
{
  rev++;
}

/***************** CONFIG *******************/

const int a = 100; //Constante tempo desligado bico OFF, delay
const int b = 90;
const int c = 80;
const int d = 70;
const int e = 60;
const int f = 50;
const int g = 40;
const int h = 30;
const int i = 20;
const int j = 10;

const int TBImin = 45;    //TBI valor minimo map-pwm
const int TBImax = 165;   //TBI valor maximo map-pwm

const int Bicomin = 10;   //Bico valor minimo map-pwm
const int Bicomax = 95;  //Bico valor maximo map-pwm

const int RPMpartidamin = 35;  //RPM minima de partida com arranque

/********************************************/
/********************************************/

void setup() {
  Serial.begin(9600);             // configura a porta serial
  pinMode (TBI, OUTPUT);          //Gera o PWM pro TBI abrir, pino 3 eh saida, colocar um resistor de 1K na saida do pino, colocar diodo entre os fios POS e GND do motor
  pinMode (Bico, OUTPUT);         //Pulsador Bico, PWM, pino 5 eh saida
  pinMode (analogInPin1, INPUT);  //A0
  pinMode (analogInPin2, INPUT);  //A1
  pinMode (LentaOutPin, OUTPUT);  //Soenoide lenta pino 9

  /***************** RPM **********************/

  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();
  pinMode (TachInPin, INPUT);
  digitalWrite(TachInPin , HIGH); // Ao invés de usar pull up resistor ... talvez usar LOW e FALLING ou retirar(???)
  attachInterrupt(0, isr, RISING); //attaching the interrupt, RISING = de low pra High, inverso eh FALLING

}

/********************************************/
/********************************************/

void loop() {

  RPM();
  LENTA();
  pwmTBI();
  pwmBico();

  Serial.print(" RPM = ");
  Serial.print(rpm);
  Serial.print(" TempoTBI = ");
  Serial.print(TempoTBI);
  Serial.print(" PWM Bico = " );
  Serial.println(Tempo1);
  //Serial.print(" " );
  //Serial.print(Tempo1);
  //Serial.print(" ");
  //Serial.println(TempoTBI);
}

/***************** RPM ********************/

void RPM() {
  delay(2000);// 2 second delay
  detachInterrupt(0);           //detaches the interrupt while calculating
  time = millis() - oldtime;    //finds the time
  rpm = (rev / time) * 60000;   //calculates rpm
  oldtime = millis();           //saves the current time
  rev = 0;

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);// Vertical, Horizontal.
  display.println("RPM:");
  display.setTextSize(5);
  display.setTextColor(WHITE);
  display.setCursor(0, 25);
  display.println(rpm);
  display.display();

  attachInterrupt(0, isr, RISING);
}

/************ SOLENOIDE LENTA *************/

void LENTA() {
  if (rpm < 800) {
    digitalWrite (LentaOutPin, 1);
    delay (a);
  }

  if ((rpm > 800) && (rpm < 900)) {
    digitalWrite (LentaOutPin, 1);
    delay (c);
    digitalWrite (LentaOutPin, 0);
    delay (i);
  }

  if ((rpm > 900) && (rpm < 1000)) {
    digitalWrite (LentaOutPin, 1);
    delay (d);
    digitalWrite (LentaOutPin, 0);
    delay (h);
  }

  if (rpm > 1100) {
    digitalWrite (LentaOutPin, 0);
    delay (a);
  }
}

/***************** TBI ********************/

void pwmTBI() {
  int TempoTBI = map(analogRead (analogInPin1), 922, 630, TBImin, TBImax);   //TBI, double=decimal 45-165
  analogWrite(TBI, TempoTBI); // analogRead values go from 0 to 1023, analogWrite values from 0 to 255
}

/************* Pulsador de bico ***********/

void pwmBico() {
  int Tempo1 = map(analogRead (analogInPin2), 822, 235, Bicomin, Bicomax);   //BICO, Tempo bico aberto

  if ((Tempo1 < 20) && (rpm > RPMpartidamin)) {
    digitalWrite (Bico, 1); //Pulsador Bico, ON
    delay (Tempo1);         //Pulsador Bico, aqui que varia tempo ligado
    digitalWrite (Bico, 0); //Pulsador Bico, OFF
    delay (a);
  }

  if ((Tempo1 > 20) && (Tempo1 < 30) && (rpm > RPMpartidamin)) {
    digitalWrite (Bico, 1); //Pulsador Bico, ON
    delay (Tempo1);         //Pulsador Bico, aqui que varia tempo ligado
    digitalWrite (Bico, 0); //Pulsador Bico, OFF
    delay (b);
  }

  if ((Tempo1 > 30) && (Tempo1 < 40) && (rpm > RPMpartidamin)) {
    digitalWrite (Bico, 1); //Pulsador Bico, ON
    delay (Tempo1);         //Pulsador Bico, aqui que varia tempo ligado
    digitalWrite (Bico, 0); //Pulsador Bico, OFF
    delay (c);
  }

  if ((Tempo1 > 40) && (Tempo1 < 50) && (rpm > RPMpartidamin)) {
    digitalWrite (Bico, 1); //Pulsador Bico, ON
    delay (Tempo1);         //Pulsador Bico, aqui que varia tempo ligado
    digitalWrite (Bico, 0); //Pulsador Bico, OFF
    delay (d);
  }

  if ((Tempo1 > 50) && (Tempo1 < 60) && (rpm > RPMpartidamin)) {
    digitalWrite (Bico, 1); //Pulsador Bico, ON
    delay (Tempo1);         //Pulsador Bico, aqui que varia tempo ligado
    digitalWrite (Bico, 0); //Pulsador Bico, OFF
    delay (e);
  }

  if ((Tempo1 > 60) && (Tempo1 < 70) && (rpm > RPMpartidamin)) {
    digitalWrite (Bico, 1); //Pulsador Bico, ON
    delay (Tempo1);         //Pulsador Bico, aqui que varia tempo ligado
    digitalWrite (Bico, 0); //Pulsador Bico, OFF
    delay (f);
  }

  if ((Tempo1 > 70) && (Tempo1 < 80) && (rpm > RPMpartidamin)) {
    digitalWrite (Bico, 1); //Pulsador Bico, ON
    delay (Tempo1);         //Pulsador Bico, aqui que varia tempo ligado
    digitalWrite (Bico, 0); //Pulsador Bico, OFF
    delay (g);
  }

  if ((Tempo1 > 80) && (Tempo1 < 90) && (rpm > RPMpartidamin)) {
    digitalWrite (Bico, 1); //Pulsador Bico, ON
    delay (Tempo1);         //Pulsador Bico, aqui que varia tempo ligado
    digitalWrite (Bico, 0); //Pulsador Bico, OFF
    delay (h);
  }

  if ((Tempo1 > 90) && (rpm > RPMpartidamin)) {
    digitalWrite (Bico, 1); //Pulsador Bico, ON
    delay (Tempo1);         //Pulsador Bico, aqui que varia tempo ligado
    digitalWrite (Bico, 0); //Pulsador Bico, OFF
    delay (i);
  }
}
