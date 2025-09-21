int motorPinL1 = 7; // Pin direccion motor izquierdo 1
int motorPinL2 = 8; // Pin direccion motor izquierdo 2
int motorPinLENA = 6; // Pin encendido motor izquierdo

int motorPinR1 = 9; // Pin direccion motor derecho 1
int motorPinR2 = 10; // Pin direccion motor derecho 2
int motorPinRENA = 11; // Pin direccion motor derecho


void setup() {
  // Seteamos todos los pines como OUTPUT
  pinMode(motorPinL1, OUTPUT); 
  pinMode(motorPinL2, OUTPUT);
  pinMode(motorPinLENA, OUTPUT);
  pinMode(motorPinR1, OUTPUT); 
  pinMode(motorPinR2, OUTPUT);
  pinMode(motorPinRENA, OUTPUT);
}

void loop() {
   // put your main code here, to run repeatedly:

  //Velocidad (0 = no se mueve & 255 = velocidad maxima):
  analogWrite(motorPinLENA, 100); // Le ponemos velocidad 100 al motor izquierdo
  analogWrite(motorPinRENA, 100); // Le ponemos velocidad 100 al motor derecho
  
  // Primer modo - Se mueve hacia adelante

  digitalWrite(motorPinL1, HIGH);
  digitalWrite(motorPinL2, LOW);

  digitalWrite(motorPinR1, LOW);
  digitalWrite(motorPinR2, HIGH);

  delay(3000);

  // Segundo modo - Se mueve hacia atras

  digitalWrite(motorPinL1, LOW);
  digitalWrite(motorPinL2, HIGH);

  digitalWrite(motorPinR1, HIGH);
  digitalWrite(motorPinR2, LOW);

  delay(3000);

  // Tercer modo - Gira hacia la derecha

  analogWrite(motorPinRENA, 0); // Apagamos el motor derecho

  digitalWrite(motorPinL1, HIGH);
  digitalWrite(motorPinL2, LOW);

  delay(3000);

  // Cuarto modo - Gira hacia la izquierda
  analogWrite(motorPinRENA, 100) // Volvemos a prender el motor derecho
  analogWrite(motorPinLENA, 0); // Apagamos el motor izquierdo

  digitalWrite(motorPinR1, LOW);
  digitalWrite(motorPinR2, HIGH);

  delay(3000);
}