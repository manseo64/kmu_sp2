#define PIN_LED 7

void setup() {
  pinMode(PIN_LED, OUTPUT);   // 7번 핀을 출력 모드로 설정
}

void loop() {
  // 1. 처음 1초 동안 LED를 켬
  digitalWrite(PIN_LED, LOW);
  delay(1000);

  // 2. 그 다음 1초 동안 5회 깜빡임 (0.2초 간격)
  for (int i = 0; i < 5; i++) {
    digitalWrite(PIN_LED, LOW);
    delay(100);
    digitalWrite(PIN_LED, HIGH);
    delay(100);
  }

  // 3. LED 끄고 무한 루프 
  digitalWrite(PIN_LED, HIGH);
  while (1) {
    
  }
}
