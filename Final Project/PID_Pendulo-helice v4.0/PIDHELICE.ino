  // Controle PID

int pot_angulo = A0;
int pot_referencia = A1;
int motor = 9;
int erro = 0;
int controle = 0;
int referencia = 10;

//Variaveis para medicao de tempo
float tempo_antigo = 0;
float tempo = 0;
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
 float kp = 0.008;
 float kd = 0.1;
 float ki = 0.1;
 
  // Convercao dos valores medidos pelos potenciometros em graus
  //int referencia = map(analogRead(pot_referencia), 0, 1022, 0, 90); //Leitura do setpoint
  
  int angulo = map(analogRead(pot_angulo), 444, 830, 0, 90); //Leitura do angulo
  // int referencia = 45;
  // Declaracao do valor do controle
  erro = referencia - angulo;
  controle = floor(abs(kp * ( erro + kd * derivada + ki * integral)));

  Serial.print("Ag: ");
  Serial.print(angulo);
  Serial.print(" Ref: ");
  Serial.print(referencia);
  Serial.print(" Ctr: ");
  Serial.println(controle);
  // Controle
  if(angulo <= referencia){
    analogWrite(motor, controle);
  }
  
  tempo = millis();
  delta_t = tempo - tempo_antigo;
  // Calculo da integral. Aqui o programa soma o erro vezes a diferenca de tempo ao valor da soma do loop anterior
  integral = integral_antiga + erro * delta_t;
  // Calculo da derivada. A variacao de angulo e dividida pela variacao de tempo
  delta_erro = erro - erro_antigo;
  derivada = delta_erro/delta_t;
  // Atualizacao de variaveis
  erro_antigo = erro;
  tempo_antigo = tempo;
  integral_antiga = integral;
}
