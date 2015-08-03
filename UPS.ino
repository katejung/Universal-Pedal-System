char command;
String string;

//Indicator Leds
#define led1 10
#define led2 11

//Encoder Parameters
int encoder0PinA = 2;
int encoder0PinB = 3;
volatile int encoder0Pos = 0;
volatile int encoder0PinALast = LOW;
volatile int n = LOW;
int valNew = 0;
int valOld = 0;
volatile int m = LOW;

//Torque Motor Parameter
#define torquePWM 9
int PWM;
int dutyCycle;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  // LED Signal
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  // Encoder Part
  pinMode (encoder0PinA, INPUT);
  pinMode (encoder0PinB, INPUT);
  attachInterrupt(1, CountA, CHANGE);
  attachInterrupt(0, StateB, FALLING);
  //Torque Motor
  pinMode(torquePWM, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available() > 0)
  {
    string = "";
  }

  while (Serial.available() > 0) {
    command = ((byte) Serial.read());
    if (command == ':')
    { break;
    }
    else {
      string += command;
    }

    delay(20);
  }

  if ((string == "TOH") || (string == "TOL")) {
    TurnOn();
    if (string == "TOH") {
      MotorHigh();
    } else if (string == "TOL") {
      MotorLow();
    }
    encoder0PinALast = n;
    valNew = encoder0Pos;
    if (valNew != valOld) {
      Serial.println(encoder0Pos, DEC);
      valOld = valNew;
    }
  } else if (string == "TF") {
    TurnOff();
  }

}

void CountA() {
  n = digitalRead(encoder0PinA);
  if ((encoder0PinALast == LOW) && (n == HIGH)) {
    if (m == LOW) {
      encoder0Pos--;
    }
    else {
      encoder0Pos++;
    }
  }
}


void StateB() {
  m = digitalRead(encoder0PinB);
}

void TurnOn() {
  digitalWrite(led1, HIGH);
  delay(100);
}

void TurnOff() {
  digitalWrite(led1, LOW);
  delay(100);
}
void MotorHigh() {
  digitalWrite(led2, HIGH);
  dutyCycle = 60;
  PWM = round(dutyCycle*255/100);
  analogWrite(torquePWM, PWM);
  delay(100);
}

void MotorLow() {
  digitalWrite(led2, LOW);
  dutyCycle = 56;
  PWM = round(dutyCycle*255/100);
  analogWrite(torquePWM, 128);
  delay(100);
}
