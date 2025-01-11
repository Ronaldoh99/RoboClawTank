#define CUSTOM_SETTINGS
#define INCLUDE_GAMEPAD_MODULE
#include <DabbleESP32.h>
#include <ESP32Servo.h> // Librería para servos compatible con ESP32

// Pines de los motores (L298N)
int enableRightMotor = 22;
int rightMotorPin1 = 16;
int rightMotorPin2 = 17;
int enableLeftMotor = 23;
int leftMotorPin1 = 18;
int leftMotorPin2 = 19;

#define MAX_MOTOR_SPEED 255
const int PWMFreq = 1000; // Frecuencia de 1 KHz
const int PWMResolution = 8;
const int rightMotorPWMSpeedChannel = 4;
const int leftMotorPWMSpeedChannel = 5;

// Pines de los servos
#define SERVO1_PIN 15
#define SERVO2_PIN 2

// Objetos para controlar los servos
Servo servo1;
Servo servo2;

// Ángulos iniciales de los servos
int servo1Angle = 90; // Posición inicial
int servo2Angle = 90;

// Función para configurar los pines
void setUpPinModes() {
  pinMode(enableRightMotor, OUTPUT);
  pinMode(rightMotorPin1, OUTPUT);
  pinMode(rightMotorPin2, OUTPUT);

  pinMode(enableLeftMotor, OUTPUT);
  pinMode(leftMotorPin1, OUTPUT);
  pinMode(leftMotorPin2, OUTPUT);

  // Configuración de PWM para velocidad
  ledcSetup(rightMotorPWMSpeedChannel, PWMFreq, PWMResolution);
  ledcSetup(leftMotorPWMSpeedChannel, PWMFreq, PWMResolution);
  ledcAttachPin(enableRightMotor, rightMotorPWMSpeedChannel);
  ledcAttachPin(enableLeftMotor, leftMotorPWMSpeedChannel);

  // Motores inicialmente apagados
  rotateMotor(0, 0);
}

// Función para controlar los motores
void rotateMotor(int rightMotorSpeed, int leftMotorSpeed) {
  if (rightMotorSpeed < 0) {
    digitalWrite(rightMotorPin1, LOW);
    digitalWrite(rightMotorPin2, HIGH);
  } else if (rightMotorSpeed > 0) {
    digitalWrite(rightMotorPin1, HIGH);
    digitalWrite(rightMotorPin2, LOW);
  } else {
    digitalWrite(rightMotorPin1, LOW);
    digitalWrite(rightMotorPin2, LOW);
  }

  if (leftMotorSpeed < 0) {
    digitalWrite(leftMotorPin1, LOW);
    digitalWrite(leftMotorPin2, HIGH);
  } else if (leftMotorSpeed > 0) {
    digitalWrite(leftMotorPin1, HIGH);
    digitalWrite(leftMotorPin2, LOW);
  } else {
    digitalWrite(leftMotorPin1, LOW);
    digitalWrite(leftMotorPin2, LOW);
  }

  ledcWrite(rightMotorPWMSpeedChannel, abs(rightMotorSpeed));
  ledcWrite(leftMotorPWMSpeedChannel, abs(leftMotorSpeed));
}

void setup() {
  Serial.begin(115200);      // Monitor Serial
  Dabble.begin("MyEsp32"); // Nombre Bluetooth del ESP32

  // Configuración de pines de motores
  setUpPinModes();

  // Configuración de servos
  servo1.attach(SERVO1_PIN);
  servo2.attach(SERVO2_PIN);
  servo1.write(servo1Angle);
  servo2.write(servo2Angle);
}

void loop() {
  Dabble.processInput(); // Procesa datos del GamePad

  // Control de los motores del carro
  int rightMotorSpeed = 0;
  int leftMotorSpeed = 0;

  if (GamePad.isUpPressed()) {
    rightMotorSpeed = MAX_MOTOR_SPEED;
    leftMotorSpeed = MAX_MOTOR_SPEED;
  }

  if (GamePad.isDownPressed()) {
    rightMotorSpeed = -MAX_MOTOR_SPEED;
    leftMotorSpeed = -MAX_MOTOR_SPEED;
  }

  if (GamePad.isLeftPressed()) {
    rightMotorSpeed = MAX_MOTOR_SPEED;
    leftMotorSpeed = -MAX_MOTOR_SPEED;
  }

  if (GamePad.isRightPressed()) {
    rightMotorSpeed = -MAX_MOTOR_SPEED;
    leftMotorSpeed = MAX_MOTOR_SPEED;
  }

  rotateMotor(rightMotorSpeed, leftMotorSpeed);

  // Control de la garra
  if (GamePad.isCrossPressed()) { 
    // Gira 45 grados desde la posición actual
    servo1Angle = min(servo1Angle + 45, 180); // Limita a 180 grados
    servo2Angle = min(servo2Angle + 45, 180);
    servo1.write(servo1Angle);
    servo2.write(servo2Angle);
    Serial.println("Cross pressed: Rotating servos +45 degrees");
  }

  if (GamePad.isCirclePressed()) {
    // Devuelve los servos a la posición inicial (90 grados)
    servo1Angle = 90;
    servo2Angle = 90;
    servo1.write(servo1Angle);
    servo2.write(servo2Angle);
    Serial.println("Circle pressed: Returning servos to initial position");
  }

  // Depuración
  Serial.print("Servo1 Angle: ");
  Serial.print(servo1Angle);
  Serial.print("\tServo2 Angle: ");
  Serial.println(servo2Angle);
}
