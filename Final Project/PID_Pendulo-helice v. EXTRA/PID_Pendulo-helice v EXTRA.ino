// Controle PID
#include <TimerOne.h>
volatile float E[3] = {0.0000,0.0000,0.0000};
volatile float u[3] = {0.0000,0.0000,0.0000};
volatile float pot_angulo = A0;
volatile float pot_referencia = A1;
volatile float motor = 9;
volatile float d = 0.0000;
//volatile int controle = 0;
volatile float referencia = 70.0; // Valor de referência inicial

//Variaveis para medicao de tempo
volatile unsigned long tempo = 0;

volatile float angulo;

void setup() {
  pinMode(motor, OUTPUT);
  Serial.begin(115200);
  Timer1.initialize(10000);
  Timer1.attachInterrupt(Controle_Q); // blinkLED to run every 0.15 seconds

}


void Controle_Q(){
  angulo = map(analogRead(pot_angulo), 436.0, 800.0, 0.0, 90.0)+3.000;

 d = (float)controle(angulo,referencia);
  analogWrite(9,(d*255)/100.0);
  //Timer1.pwm(9, (d / 100.0) * 1023);
  // Convercao dos valores medidos pelos potenciometros em graus
   // Leitura do angulo
  //angulo = analogRead(pot_angulo);
  // Ler dados do monitor serial se disponível
  if (Serial.available() > 0) {
    // Ler o valor de referência enviado pelo serial
    referencia = Serial.parseInt();
    // Aguardar que o buffer serial seja esvaziado
    while (Serial.available() > 0) {
      Serial.read();
    }
  }

//  Serial.print("Tempo: ");
//  Serial.print(millis()); // Imprime o tempo atual
//  Serial.print(" Ag: ");
  Serial.print(angulo);
  Serial.print(" ");
//  Serial.print(" Ref: ");
  Serial.print(referencia);
  Serial.print(" ");
//  Serial.print(" Ctr: ");
  Serial.println(u[0]);
 // Serial.print(" ");
//  Serial.print(" Erro: ");
//  Serial.println(E[0]);

}

void loop(){}


float controle(float angulo,float referencia){
  float r0 = 157.1454873176381;
  float r1 =  -296.2238937922621;
  float r2 = 140.4816151178916;
  float s1 = -1.749155281829828;
  float s2 = 0.749155281829828;

  E[0] = referencia - angulo;


  u[0] = (-s1*u[1]-s2*u[2]+r0*E[0]+r1*E[1]+r2*E[2]) ;

  if(u[0] >= 100.0){
    u[0] = 100.0;
  }
  if(u[0] <= 0.0){
    u[0] = 0.0;
  }
  

  u[2] = u[1];
  u[1] = u[0];

  E[2] = E[1];
  E[1] = E[0];


 // if (E[0] < 0){
 //   u[0] - 0.1;
 //   if(u[0] >= 1.0){
 //     u[0] = 1.0;
 //   }
 //   if(u[0] <= 0.0){
 //     u[0] = 0.0;
 //   }
 // }

//  if(E[0] > 0){
//    u[0] + 0.11;
//    if(u[0] >= 1.0){
//      u[0] = 1.0;
//    }
//    if(u[0] <= 0.0){
//      u[0] = 0.0;
//    }
//  }

  
  

  

  //u[0] = constrain(u[0], 0, 255);

  return u[0];

}