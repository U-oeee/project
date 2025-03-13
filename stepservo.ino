#include <Servo.h>

const int M1dirpin = 4;  // 스텝 모터 방향 핀
const int M1steppin = 5; // 스텝 모터 스텝 핀
const int servoPin = 3;  // 서보 모터 핀

Servo myServo;  // 서보 모터 객체 생성

int stepDelay = 1000;  // 스텝 모터 속도 조절 (마이크로초)
int rotationTime1 = 6800;  // 스텝 모터 동작 시간 (4초)
int rotationTime2 = 6800;

// 🛠 Setup: 핀 모드 설정 및 시리얼 초기화
void setup() {
    pinMode(M1dirpin, OUTPUT);
    pinMode(M1steppin, OUTPUT);
    myServo.attach(servoPin);  // 서보 모터 핀 연결

    Serial.begin(9600);
    Serial.println("Enter '3' or '5' to start the motor sequence.");
}

// 🎯 Main Loop: 시리얼 입력을 받아 동작 수행
void loop() {
    if (Serial.available() > 0) {
        char command = Serial.read();
        
        if (command == '3') {
            Serial.println("Starting motor sequence...");

            myServo.write(180);
            delay(4500);
            myServo.write(90);

            myServo.write(0);
            delay(3000);
            myServo.write(90);


            // 🔵 3.5초 동안 스텝 모터 역회전
            Serial.println("Step Motor: Rotating forward...");
            digitalWrite(M1dirpin, LOW);
            rotateStepper(rotationTime1);
            
            
            // 🔵 서보 모터 3.5초 동안 시계 방향 회전 후 정지
            Serial.println("Servo Motor: Rotating clockwise...");
           myServo.write(180);
            delay(4500);
            myServo.write(90);   // 정지
               // 정지
            
            // 🔵 3.5초 동안 스텝 모터 정회전
            Serial.println("Step Motor: Rotating backward...");
            digitalWrite(M1dirpin, HIGH);
            rotateStepper(rotationTime2);

            // 🔵 완료 메시지 출력
            Serial.println("4");
        }

        if (command == '5') {
            Serial.println("Starting 3.5s motor sequence...");

            // 🔵 3.5초 동안 스텝 모터 역회전
            Serial.println("Step Motor: Rotating forward...");
            digitalWrite(M1dirpin, LOW);
            rotateStepper(4000);
            
            myServo.write(0);  // 시계 방향 회전
            delay(3500);
            myServo.write(90);
            myServo.write(180);
            delay(4000);
            myServo.write(90);   // 정지
            
            // 🔵 3.5초 동안 스텝 모터 정회전
            Serial.println("Step Motor: Rotating backward...");
            digitalWrite(M1dirpin, HIGH);
            rotateStepper(4000);

            myServo.write(180);
            delay(2000);
            myServo.write(90);


            // 🔵 완료 메시지 출력
            Serial.println("6");
        }
    }
}

// 📌 스텝 모터를 일정 시간 동안 회전하는 함수
void rotateStepper(int duration) {
    unsigned long startTime = millis();
    while (millis() - startTime < duration) {
        digitalWrite(M1steppin, HIGH);
        delayMicroseconds(stepDelay);
        digitalWrite(M1steppin, LOW);
        delayMicroseconds(stepDelay);
    }
}
