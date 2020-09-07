/*http://mundoprojetado.com.br/executar-duas-coisas-ao-mesmo-tempo/
  https://www.youtube.com/watch?v=AqfEu2cJrw0&t=166s
  https://www.youtube.com/watch?v=xpLw8xXlJKg&t=76s
  https://www.youtube.com/watch?v=L530rN5W9ow //transistor npn como funciona
  NPN,TIP35C, BASE Pino 1 recebe sinal pwm 5V que vem do arduino, COLETOR pino 2 sai o PWM NEGATIVO para acionar o NEGATIVO da BOBINA/MOTOR, EMISSOR Pino 3 ligado no negativo da bateria ...
  o GND do arduino tem de conectar direto no negativo da bateria... os POSITIVOS fica ligado direto nos 12v+
  TIP35C, o negativo eh pwm que vai pro bico, 12V+ liga direto
  Usar resistor 6R no positivo do bico de baixa pra não esquentar o bico...
  Pino com pwm no UNO 3, 5, 6, 9, 10, 11
  Pinagem pedal, da frente pra trás: fio 1 e 2 GND, fio 3 pedal -->A1, fio 4 e 5 positivo 5V+, fio 6 pedal -->A0
  /********************************************/

# define TBI  5           //Saida TBI
# define Bico 3           //Saida Bico
# define analogInPin1 A0  // Entrada analogica do potenciometro1 do pedal, comanda TBI
# define analogInPin2 A1  // Entrada analogica do potenciometro2 do pedal, comanda Bico

void pwmTBI();// Funçao que controla abertura da TBI
void pwmBico();// Funçao que controla o pulsar do Bico

int TempoTBI = 0;
int Tempo1 = 0;

/********************************************/
/********************************************/

void setup() {
  Serial.begin(9600);             // configura a porta serial
  pinMode (TBI, OUTPUT);          //Gera o PWM pro TBI abrir, pino 3 eh saida
  pinMode (Bico, OUTPUT);         //Pulsador Bico, PWM, pino 5 eh saida
  pinMode (analogInPin1, INPUT);  //A0
  pinMode (analogInPin1, INPUT);  //A1
}

/********************************************/
/********************************************/

void loop() {
  pwmTBI();
  pwmBico();

  //Serial.print("Bico ligado mS = ");
  //Serial.print(Tempo1);
  //Serial.print(" PWM TBI = ");
  //Serial.print(" Pedal 1 pot = " );
  //Serial.print(sensorValue1);
  //Serial.print("  Pedal 2 pot = " );
  //Serial.print(sensorValue2);
  //Serial.print("  Tempo1 " );
  //Serial.print(Tempo1);
  //Serial.print(" TempoTBI ");
  //Serial.println(TempoTBI);

}


/***************** TBI ********************/
void pwmTBI() {
  int TempoTBI = map(analogRead (analogInPin1), 922, 630, 45, 165);   //TBI, double=decimal 45-165
  analogWrite(TBI, TempoTBI); // analogRead values go from 0 to 1023, analogWrite values from 0 to 255
}


/************* Pulsador de bico ***********/
void pwmBico() {
  int Tempo1 = map(analogRead (analogInPin2), 822, 235, 10, 200);   //BICO, Tempo bico aberto, tem de ajustar Tempo0 e Tempo1 inversos proporcionais
  digitalWrite (Bico, 1); //Pulsador Bico, ON
  delay (Tempo1);         //Pulsador Bico, aqui que varia tempo ligado
  digitalWrite (Bico, 0); //Pulsador Bico, OFF
  delay (100);
}
