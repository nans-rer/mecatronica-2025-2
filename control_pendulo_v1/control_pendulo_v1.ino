#include "I2Cdev.h"
#include "ADXL345.h"
#include "Wire.h"


int motorPinL1 = 7; // Pin direccion motor izquierdo 1
int motorPinL2 = 8; // Pin direccion motor izquierdo 2
int motorPinLENA = 6; // Pin encendido motor izquierdo

int motorPinR1 = 11; // Pin direccion motor derecho 1
int motorPinR2 = 10; // Pin direccion motor derecho 2
int motorPinRENA = 9; // Pin direccion motor derecho

int ax, ay, az; // Valores sin procesar del acelerometro en x, y, z

float roll = 0, pitch = 0;

// Parametro PID

float Kp = 2.0;
float Ki = 0.6;
float Kd = 0.1;
float setpoint = 0;   // ° deseados

// Variables de control

float angulo = 0.0;
float error = 0.0;
float error_prev = 0.0;
float integral = 0.0;
float derivada = 0.0;
float salida = 0.0;
int pwm = 0;

// Control de tiempo
unsigned long lastTime = 0;
const unsigned long sampleTime = 100; // ms (10 Hz)

ADXL345 sensor;

void setup() {
  Wire.begin();
  sensor.initialize();
  Serial.begin(57600);

  if (sensor.testConnection()) Serial.println("Sensor iniciado correctamente");
  else Serial.println("Error al inicial el sensor");

  // Seteamos todos los pines como OUTPUT
  pinMode(motorPinL1, OUTPUT);
  pinMode(motorPinL2, OUTPUT);
  pinMode(motorPinLENA, OUTPUT);

  pinMode(motorPinR1, OUTPUT); 
  pinMode(motorPinR2, OUTPUT);
  pinMode(motorPinRENA, OUTPUT);
}

void motorForward(int motorSpeed, float error) {
  // Limitamos motorSpeed al rango válido
  motorSpeed = constrain(motorSpeed, 0, 255);
  
  if (error > 0) {
    // Motor izquierdo hacia adelante
    digitalWrite(motorPinL1, HIGH);
    digitalWrite(motorPinL2, LOW);

    // Motor derecho hacia atrás
    digitalWrite(motorPinR1, HIGH);
    digitalWrite(motorPinR2, LOW);
  }
  else if (error < 0) {
    // Motor izquierdo hacia atrás
    digitalWrite(motorPinL1, LOW);
    digitalWrite(motorPinL2, HIGH);

    // Motor derecho hacia adelante
    digitalWrite(motorPinR1, LOW);
    digitalWrite(motorPinR2, HIGH);
  }
  else {
    // En equilibrio, detenerse
    digitalWrite(motorPinL1, LOW);
    digitalWrite(motorPinL2, LOW);
    digitalWrite(motorPinR1, LOW);
    digitalWrite(motorPinR2, LOW);
  }

  // Aplicamos la velocidad PWM a ambos motores
  analogWrite(motorPinLENA, motorSpeed);
  analogWrite(motorPinRENA, motorSpeed);
}

void motorStop() {
  // Funcion que apaga los motores del robor seteando la velocidad de ambos en 0
  digitalWrite(motorPinL1, HIGH);
  digitalWrite(motorPinL2, HIGH);

  // Le damos direccion contraria al motor derecho
  digitalWrite(motorPinR1, HIGH);
  digitalWrite(motorPinR2, HIGH);

  analogWrite(motorPinLENA, 0); // Le ponemos velocidad 0 al motor izquierdo
  analogWrite(motorPinRENA, 0); // Le ponemos velocidad 0 al motor derecho
}

void motorBackwards(int motorSpeed) {
  // Funcion que hace que el robot vaya hacia atras con velocidad motorSpeed

  analogWrite(motorPinLENA, motorSpeed); // Le ponemos velocidad motorSpeed al motor izquierdo
  analogWrite(motorPinRENA, motorSpeed); // Le ponemos velocidad motorSpeed al motor derecho

  // Le damos direccion contraria al motor izquierdo
  digitalWrite(motorPinL1, HIGH);
  digitalWrite(motorPinL2, LOW);

  // Le damos direccion al motor derecho
  digitalWrite(motorPinR1, HIGH);
  digitalWrite(motorPinR2, LOW);
}

void motorLeft(int motorSpeed) {
  // Funcion que hace que el robot se mueva hacia la izquierda seteando la velocidad
  // del motor izquierdo en 0 y la del derecho en motorSpeed

  analogWrite(motorPinLENA, 0); // Seteamos la velocidad del motor izquierdo en 0
  analogWrite(motorPinRENA, motorSpeed); // Seteamos la velocidad del motor derecho en motorSpeed

  // Le damos direccion al motor derecho
  digitalWrite(motorPinR1, LOW);
  digitalWrite(motorPinR2, HIGH);
}

void motorRight(int motorSpeed) {
  // Funcion que hace que el robot se mueva hacia la derecha seteando la velocidad
  // del motor izquierdo en motorSpeed y la del derecho en 0
  analogWrite(motorPinLENA, motorSpeed); // Le ponemos velocidad motorSpeed al motor izquierdo
  analogWrite(motorPinRENA, 0); // Le ponemos velocidad 0 al motor derecho

  // Le damos direccion al motor izquierdo
  digitalWrite(motorPinL1, LOW);
  digitalWrite(motorPinL2, HIGH);
}


void loop() {

  unsigned long now = millis();
  if (now - lastTime >= sampleTime) {
    sensor.getAcceleration(&ax, &ay, &az);

    float ax_f = (float)ax;
    float ay_f = (float)ay;
    float az_f = (float)az;

    roll = atan2(ay_f, -az_f) * 180.0 / PI;
    pitch = atan2(-ax_f, sqrt(ay_f * ay_f + az_f * az_f)) * 180.0 / PI;

    float dt = (now - lastTime) / 1000.0;
    lastTime = now;


    // 2) Calcular error
    error = setpoint - roll;

    // 3) PID (manual)
    float P = Kp * error;
    integral += error * dt;
    float I = Ki * integral;
    derivada = (error - error_prev) / dt;
    float D = Kd * derivada;

    salida = P + I + D;

    // 4) Saturación y anti–windup
    if (salida > 255) {
      salida = 255;
      integral -= error * dt; // Evita acumulación excesiva
    } else if (salida < 150) {
      salida = 150;
      integral -= error * dt;
    }
    // Otra opción
    //
    pwm = (int)salida;

    // 5)
    motorForward(pwm, error);

    // 6) Mostrar datos (para el Serial Plotter)
    Serial.print(angulo);
    Serial.print(",");
    Serial.print(setpoint);
    Serial.print(",");
    Serial.println(pwm);
    Serial.print("a[x y z]:\t");
    Serial.print(ax); Serial.print("\t");
    Serial.print(ay); Serial.print("\t");
    Serial.print(az); Serial.print("\t");
    Serial.print("Roll:\t");
    Serial.print(roll); Serial.print("\t");
    Serial.print("Pitch:\t");
    Serial.println(pitch);

    // 7) Guardar error anterior
    error_prev = error;
  }
}