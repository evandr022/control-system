#include <PID_v1.h>

#define PIN_INPUT A1
#define PIN_OUTPUT 9

// Define Variables we'll be connecting to
double Setpoint, Input, Output, pont;

// Specify the links and initial tuning parameters
double Kp=1.5, Ki=1.5, Kd=0.0;
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

unsigned long timet;
unsigned long previousTime;
unsigned long outputTime; // Variável para armazenar o tempo da última saída
boolean enterFunction = true;

void setup() {
  Serial.begin(9600);
  // initialize the variables we're linked to
  Input = analogRead(PIN_INPUT);
  // Setpoint = analogRead(A0);
  Setpoint = Serial.read();

  // turn the PID on
  myPID.SetMode(AUTOMATIC);
}

void loop() {
  Input = map(analogRead(PIN_INPUT), 0, 1023, 0, 100);
  // Setpoint = 40;
  Setpoint = map(analogRead(A0), 0, 1023, 0, 100);
  myPID.Compute();
  analogWrite(PIN_OUTPUT, Output);

  // Imprimir valores e tempo
  Serial.print(millis() - outputTime); // Tempo desde a última saída
  Serial.print(" "); 
  Serial.print(analogRead(A0));
  Serial.print(" ");
  Serial.print(analogRead(A1));
  Serial.print(" ");
  Serial.print(map(analogRead(PIN_INPUT), 0, 1023, 0, 100));
  Serial.println();

  outputTime = millis(); // Atualiza o tempo da última saída
}
