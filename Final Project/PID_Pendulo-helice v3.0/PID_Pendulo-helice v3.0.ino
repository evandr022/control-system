
// Controle PID
#include <TimerOne.h>
 
int pot_angulo = A0;
int pot_referencia = A1;
int motor = 9;
int erro = 0;
int controle = 0;
int referencia = 0; // Valor de referência inicial

//Variaveis para medicao de tempo
unsigned long tempo_antigo = 0; // Alteração para 'unsigned long' para evitar estouro
unsigned long tempo = 0;
float delta_t = 0;
// Variavel para medicao da variacao do angulo
int erro_antigo = 0;
int delta_erro = 0;
//Derivada
float derivada = 0;
// Integral
float integral = 0;
float integral_antiga = 0;



void setup() {
  pinMode(motor, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // Constantes usadas no controle. Determinam como o sistema se comportará
  //float kp = 0.008;
  //float kd = 0.1;
  //float ki = 0.1;

  float kp =  3.2710;
  float kd = 0.2433;
  float ki = 2.8123;
  // Convercao dos valores medidos pelos potenciometros em graus
  int angulo = map(analogRead(pot_angulo), 436, 800, 0, 90); // Leitura do angulo

  // Ler dados do monitor serial se disponível
  if (Serial.available() > 0) {
    // Ler o valor de referência enviado pelo serial
    referencia = Serial.parseInt();
    // Aguardar que o buffer serial seja esvaziado
    while (Serial.available() > 0) {
      Serial.read();
    }
    if (referencia < angulo) {
      analogWrite(motor, LOW);
      erro = 0;
    }
  }
  // referencia =45;
  // Declaracao do valor do controle
  erro = referencia - angulo;
  controle = floor(abs(kp * (erro + kd * derivada + ki * integral)));

  if (controle > 255) {
    controle = 255;
  }
  if (controle <= 0) {
    controle = 0;
  }


  Serial.print("Tempo: ");
  Serial.print(millis()); // Imprime o tempo atual
  Serial.print(" Ag: ");
  Serial.print(angulo);
  Serial.print(" Ref: ");
  Serial.print(referencia);
  Serial.print(" Ctr: ");
  Serial.println(controle);

  // Controle
  if (angulo <= referencia) {
    analogWrite(motor, controle);
  }
  if (angulo > referencia) {
    if (controle - erro < 0) {
      analogWrite(motor, 0);
    } else if (controle - erro > 255) {
      analogWrite(motor, 255);
    } else {
      analogWrite(motor, controle - erro);
    }

  }

  // Medição do tempo
  tempo = millis();
  delta_t = tempo - tempo_antigo;
  // Calculo da integral
  integral = integral_antiga + erro * delta_t;
  // Calculo da derivada
  delta_erro = erro - erro_antigo;
  derivada = delta_erro / delta_t;

  // Atualizacao de variaveis
  erro_antigo = erro;
  tempo_antigo = tempo;
  integral_antiga = integral;
}
