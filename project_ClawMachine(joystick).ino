// 4방향 스위치 핀 설정
int dirDown = 10;
int dirUp = 9;
int dirRight = 3;
int dirLeft = 11;

// 스텝 모터 핀 설정
int M1dirpin = 4;
int M1steppin = 5;
int M2dirpin = 7;
int M2steppin = 6;

// 속도 설정 (중간값으로 조정)
int minSpeed = 300; // 최소 딜레이 (마이크로초)
int maxSpeed = 800; // 최대 딜레이 (마이크로초)

void setup() {
  // 4방향 스위치 핀 설정
  pinMode(dirDown, INPUT);
  pinMode(dirUp, INPUT);
  pinMode(dirRight, INPUT);
  pinMode(dirLeft, INPUT);

  // 스텝 모터 핀 설정
  pinMode(M1dirpin, OUTPUT);
  pinMode(M1steppin, OUTPUT);
  pinMode(M2dirpin, OUTPUT);
  pinMode(M2steppin, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  // 가변저항 값을 읽어 속도 계산
  int potValue = analogRead(A0); // 가변저항 값 읽기
  int speed = map(potValue, 0, 1023, minSpeed, maxSpeed); // 속도 계산

  // 4방향 스위치 입력 처리
  if (digitalRead(dirDown) == LOW) {
    Serial.print("Down, Speed: ");
    Serial.println(speed);
    moveMotor(M2dirpin, M2steppin, LOW, speed); // 아래쪽: Y축 모터 역방향
  } else if (digitalRead(dirUp) == LOW) {
    Serial.print("Up, Speed: ");
    Serial.println(speed);
    moveMotor(M2dirpin, M2steppin, HIGH, speed); // 위쪽: Y축 모터 정방향
  } else if (digitalRead(dirRight) == LOW) {
    Serial.print("Right, Speed: ");
    Serial.println(speed);
    moveMotor(M1dirpin, M1steppin, HIGH, speed); // 오른쪽: X축 모터 정방향
  } else if (digitalRead(dirLeft) == LOW) {
    Serial.print("Left, Speed: ");
    Serial.println(speed);
    moveMotor(M1dirpin, M1steppin, LOW, speed); // 왼쪽: X축 모터 역방향
  } else {
    Serial.println("Stop");
    stopMotors(); // 모든 모터 정지
  }

  delay(10); // 상태 확인 딜레이
}

// 스텝 모터 제어 함수
void moveMotor(int dirPin, int stepPin, int direction, int speed) {
  digitalWrite(dirPin, direction); // 방향 설정

  for (int i = 0; i < 50; i++) { // 반복 횟수를 줄여 속도를 부드럽게
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(speed); // 계산된 속도로 회전
    digitalWrite(stepPin, LOW);
    delayMicroseconds(speed);
  }
}

// 모터 정지 함수
void stopMotors() {
  digitalWrite(M1steppin, LOW);
  digitalWrite(M2steppin, LOW);
}
