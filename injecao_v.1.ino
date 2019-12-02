/* By Truog2003 - e_carburator project
   https://www.arduino.cc/en/Tutorial/SecretsOfArduinoPWM
   https://create.arduino.cc/example/library/servo_1_1_5/servo_1_1_5%5Cexamples%5CKnob/Knob/preview
*/

#include <Servo.h>

Servo myservo;  // create servo object to control a servo

int pedala;
int pedalb;
int pedaltotal;
int tpsa;
int tpsb;
int partida;
int dutycycle;
int safemode;
const int frequencie = 200;
const int minfuel = 5; //duty cicle minimo em marcha lenta
const int mintbi = 0; //em graus
const int maxtbi = 90; //em graus

//..............................................

void setup()
{
  pinMode(A0, INPUT); //Pedala
  pinMode(A1, INPUT); //Pedalb
  pinMode(A2, INPUT); //TPSa
  pinMode(A3, INPUT); //TPSb
  pinMode(A4, INPUT); //partida
  pinMode(A5, INPUT); //safemode
  pinMode(6, OUTPUT); //Arranque
  pinMode(9, OUTPUT); //Bico injetor mosfet driver
  pinMode(10, OUTPUT); //Servo do acelerador
  pinMode(11, OUTPUT); //Rele da bomba do combustivel

  myservo.attach(10);  // attaches the servo on pin 10 to the servo object
}

//..............................................

void loop()
{
  /*Parametros gerais*/

  pedala = analogRead(A0);
  pedalb = analogRead(A1);
  tpsa = analogRead(A2);
  tpsb = analogRead(A3);
  partida = analogRead(A4);
  safemode = analogRead(A5);

  int pedala = map(A0, 0, 1023, mintbi, maxtbi); //Ajustar abertura max. da TBI
  int pedalb = map(A1, 0, 512, mintbi, maxtbi); //O 5 eh o valor AR da marcha lenta, aumentar se estiver morrendo
  int pedaltotal = ((pedala + pedalb) / 2); //Abertura do servo

  int tpsa = map(A2, 0, 1023, minfuel, frequencie); //Ajustar o max. pra frequencia desejada
  int tpsb = map(A3, 0, 512, minfuel, frequencie); // O minfuel eh o COMBUSTIVEL da injeção mínima, aumentar lá na const se estiver morrendo
  int dutycycle = ((tpsa + tpsb) / 2);

  /*Ligar a bomba*/

  digitalWrite(11, HIGH); //Liga a bomba

  /*Partida*/

  if (safemode > 100 && partida > 100) {

    myservo.write(maxtbi); // Posicao abertura borboleta partida
    digitalWrite(6, HIGH); //Liga o arranque
    { //Fechar este ciclo
      digitalWrite(9, HIGH);
      delayMicroseconds(150); // % duty cycle @ 200mHz
      digitalWrite(9, LOW);
      delayMicroseconds(50);
    } //Fechar este ciclo
  }

  /*Funcionando normal*/

  if (safemode > 100 && partida < 100) {
    /*Controle servo carburador*/
    myservo.write(pedaltotal); //tem de usar autuador de marcha lenta mecânico
    /*Pulsar o bico, 200mHz de frequencia*/
    { //Fechar este ciclo
      digitalWrite(9, HIGH);
      delayMicroseconds(dutycycle); // % duty cycle @ 200mHz
      digitalWrite(9, LOW);
      delayMicroseconds(frequencie - dutycycle);
    } //Fechar este ciclo
  }
}
