// 4방향 스위치 핀 설정
int dirDown = 3;
int dirUp = 11;
int dirRight = 9;
int dirLeft = 10;

// 스텝 모터 핀 설정
int M1dirpin = 7;
int M1steppin = 6;
int M2dirpin = 4;
int M2steppin = 5;

// 이동 범위 설정
const int xMinPosition = 0;
const int xMaxPosition = 104;
const int yMinPosition = -132;
const int yMaxPosition = 0;

// 현재 위치 추적
int currentX = 0;
int currentY = 0;
int savedX = 0;
int savedY = 0;

// 속도 설정
int minSpeed = 300;
int maxSpeed = 800;

// 동작 플래그
bool isActive = false;
bool isAutoMoving = false; // 자동 이동 여부
bool waitingForFive = false; // '5' 입력 대기 상태

// 함수 프로토타입 선언
void moveToPositionWithMotor(int targetX, int targetY, int speed);
void stopMotors();
void handleJoystickControl();
void moveMotor(int dirPin, int stepPin, int direction, int speed);

void setup() {
  Serial.begin(9600);
  Serial.print("Initial Position - X: ");
  Serial.print(currentX);
  Serial.print(", Y: ");
  Serial.println(currentY);
  
  pinMode(dirDown, INPUT);
  pinMode(dirUp, INPUT);
  pinMode(dirRight, INPUT);
  pinMode(dirLeft, INPUT);

  pinMode(M1dirpin, OUTPUT);
  pinMode(M1steppin, OUTPUT);
  pinMode(M2dirpin, OUTPUT);
  pinMode(M2steppin, OUTPUT);

  Serial.println("Enter 1 to start, 0 to stop");
}

void loop() {
  if (Serial.available() > 0) {
    char command = Serial.read();
    if (command == '1') {
      isActive = true;
    } else if (command == '2') {
      isActive = false;
      stopMotors();
      delay(1000);
      Serial.println("3");
      savedX = currentX;
      savedY = currentY;
    } else if (command == '4') {
      isAutoMoving = true;
      moveToPositionWithMotor(20, -132, 600);
      Serial.println("5");
      waitingForFive = true;
    } else if (command == '6' && waitingForFive) {
      moveToPositionWithMotor(0, 0, 600);
      waitingForFive = false;
      isAutoMoving = false;
      Serial.println("7");
    }
  }
  
  if (!isActive || isAutoMoving) return;
  
  handleJoystickControl();
}

void stopMotors() {
  digitalWrite(M1steppin, LOW);
  digitalWrite(M2steppin, LOW);
}

void moveToPositionWithMotor(int targetX, int targetY, int speed) {
  targetX = constrain(targetX, xMinPosition, xMaxPosition);
  targetY = constrain(targetY, yMinPosition, yMaxPosition);

  while (currentX != targetX) {
    if (currentX < targetX) {
      moveMotor(M1dirpin, M1steppin, LOW, speed);
      currentX++;
    } else if (currentX > targetX) {
      moveMotor(M1dirpin, M1steppin, HIGH, speed);
      currentX--;
    }
    Serial.print("Moving X: ");
    Serial.println(currentX);
  }

  while (currentY != targetY) {
    if (currentY < targetY) {
      moveMotor(M2dirpin, M2steppin, LOW, speed);
      currentY++;
    } else if (currentY > targetY) {
      moveMotor(M2dirpin, M2steppin, HIGH, speed);
      currentY--;
    }
    Serial.print("Moving Y: ");
    Serial.println(currentY);
  }
  
  stopMotors();
  Serial.println("Target position reached");
}

void handleJoystickControl() {
  int potValue = analogRead(A0);
  int speed = map(potValue, 0, 1023, minSpeed, maxSpeed);

  bool up = digitalRead(dirUp) == LOW;
  bool down = digitalRead(dirDown) == LOW;
  bool left = digitalRead(dirLeft) == LOW;
  bool right = digitalRead(dirRight) == LOW;

  if (up && currentY < yMaxPosition) {
    moveMotor(M2dirpin, M2steppin, LOW, speed);
    currentY++;
  } else if (down && currentY > yMinPosition) {
    moveMotor(M2dirpin, M2steppin, HIGH, speed);
    currentY--;
  } else if (right && currentX < xMaxPosition) {
    moveMotor(M1dirpin, M1steppin, LOW, speed);
    currentX++;
  } else if (left && currentX > xMinPosition) {
    moveMotor(M1dirpin, M1steppin, HIGH, speed);
    currentX--;
  } else {
    stopMotors();
  }
  Serial.print("Current X: ");
  Serial.print(currentX);
  Serial.print(", Current Y: ");
  Serial.println(currentY);
}

void moveMotor(int dirPin, int stepPin, int direction, int speed) {
  digitalWrite(dirPin, direction);
  for (int i = 0; i < 50; i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(speed);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(speed);
  }
}
