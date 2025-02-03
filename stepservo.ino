#include <Servo.h>

const int M1dirpin = 7;  // 스텝 모터 방향 핀
const int M1steppin = 6; // 스텝 모터 스텝 핀
const int servoPin = 3;  // 서보 모터 핀

Servo myServo;  // 서보 모터 객체 생성

int stepDelay = 1000;  // 스텝 모터 속도 조절 (마이크로초)
int rotationTime = 5000;  // 스텝 모터 동작 시간 (5초)

// 🛠 Setup: 핀 모드 설정 및 시리얼 초기화
void setup() {
    pinMode(M1dirpin, OUTPUT);
    pinMode(M1steppin, OUTPUT);
    myServo.attach(servoPin);  // 서보 모터 핀 연결

    Serial.begin(9600);
    Serial.println("Enter '3' to start the motor sequence.");
}

// 🎯 Main Loop: 시리얼 입력을 받아 동작 수행
void loop() {
    if (Serial.available() > 0) {
        char command = Serial.read();
        
        if (command == '3') {
            Serial.println("Starting motor sequence...");

            // 🔵 5초 동안 스텝 모터 정회전
            Serial.println("Step Motor: Rotating forward...");
            digitalWrite(M1dirpin, HIGH);
            rotateStepper(rotationTime);
            
            // 🔵 서보 모터 180도 회전
            Serial.println("Servo Motor: Moving to 180 degrees...");
            myServo.write(180);
            delay(1000);  // 서보 모터가 움직일 시간
            
            // 🔵 5초 동안 스텝 모터 역회전
            Serial.println("Step Motor: Rotating backward...");
            digitalWrite(M1dirpin, LOW);
            rotateStepper(rotationTime);

            // 🔵 완료 메시지 출력
            Serial.println("Motor sequence complete.");
            Serial.println(4);
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
