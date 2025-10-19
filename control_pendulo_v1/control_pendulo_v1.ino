int motorPinL1 = 7; // Pin direccion motor izquierdo 1
int motorPinL2 = 8; // Pin direccion motor izquierdo 2
int motorPinLENA = 6; // Pin encendido motor izquierdo

int motorPinR1 = 11; // Pin direccion motor derecho 1
int motorPinR2 = 10; // Pin direccion motor derecho 2
int motorPinRENA = 9; // Pin direccion motor derecho

int motorSpeed = 100; // Velocidad del motor, 
                      // (0 = no se mueve & 255 = velocidad maxima):

void setup() {
  // Seteamos todos los pines como OUTPUT
  pinMode(motorPinL1, OUTPUT);
  pinMode(motorPinL2, OUTPUT);
  pinMode(motorPinLENA, OUTPUT);

  pinMode(motorPinR1, OUTPUT); 
  pinMode(motorPinR2, OUTPUT);
  pinMode(motorPinRENA, OUTPUT);
}

void motorForward() {
  // Funcion que mueve el robot hacia adelante con una velocidad motorSpeed en ambos motores
  analogWrite(motorPinLENA, motorSpeed); // Le ponemos velocidad motorSpeed al motor izquierdo
  analogWrite(motorPinRENA, motorSpeed); // Le ponemos velocidad motorSpeed al motor derecho

  // Le damos direccion al motor izquierdo
  digitalWrite(motorPinL1, HIGH);
  digitalWrite(motorPinL2, LOW);

  // Le damos direccion contraria al motor derecho
  digitalWrite(motorPinR1, LOW);
  digitalWrite(motorPinR2, HIGH);

}

void motorStop() {
  // Funcion que apaga los motores del robor seteando la velocidad de ambos en 0

  analogWrite(motorPinLENA, 0); // Le ponemos velocidad 0 al motor izquierdo
  analogWrite(motorPinRENA, 0); // Le ponemos velocidad 0 al motor derecho
}

void motorBackwards() {
  // Funcion que hace que el robot vaya hacia atras con velocidad motorSpeed

  analogWrite(motorPinLENA, motorSpeed); // Le ponemos velocidad motorSpeed al motor izquierdo
  analogWrite(motorPinRENA, motorSpeed); // Le ponemos velocidad motorSpeed al motor derecho

  // Le damos direccion contraria al motor izquierdo
  digitalWrite(motorPinL1, LOW);
  digitalWrite(motorPinL2, HIGH);

  // Le damos direccion al motor derecho
  digitalWrite(motorPinR1, HIGH);
  digitalWrite(motorPinR2, LOW);
}

void motorLeft() {
  // Funcion que hace que el robot se mueva hacia la izquierda seteando la velocidad
  // del motor izquierdo en 0 y la del derecho en motorSpeed

  analogWrite(motorPinLENA, 0); // Seteamos la velocidad del motor izquierdo en 0
  analogWrite(motorPinRENA, motorSpeed); // Seteamos la velocidad del motor derecho en motorSpeed

  // Le damos direccion al motor derecho
  digitalWrite(motorPinR1, LOW);
  digitalWrite(motorPinR2, HIGH);
}

void motorRight() {
  // Funcion que hace que el robot se mueva hacia la derecha seteando la velocidad
  // del motor izquierdo en motorSpeed y la del derecho en 0
  analogWrite(motorPinLENA, motorSpeed); // Le ponemos velocidad motorSpeed al motor izquierdo
  analogWrite(motorPinRENA, 0); // Le ponemos velocidad 0 al motor derecho

  // Le damos direccion al motor izquierdo
  digitalWrite(motorPinL1, HIGH);
  digitalWrite(motorPinL2, LOW);
}


void loop() {
   // put your main code here, to run repeatedly:
}