// Arduino pin assignment
#define PIN_LED  9    // LED 핀 설정
#define PIN_TRIG 12   // 초음파 발사 출력
#define PIN_ECHO 13   // 초음파 반사 입력

// configurable parameters
#define SND_VEL 346.0     // 섭씨 24도 기준 음속 (unit: m/sec)
#define INTERVAL 25      // 샘플링 주기 (단위 msec)
#define PULSE_DURATION 10 // 해당 시간동안 대기 (단위 usec)
#define _DIST_MIN 100.0   // 최소 거리 (단위 mm)
#define _DIST_MAX 300.0   // 최대 거리 (단위 mm)

#define TIMEOUT ((INTERVAL / 2) * 1000.0) // 최대 에코 시간, 샘플링 주기에 절반 곱하기 1000, 10ms -> 5usec (단위 usec)
#define SCALE (0.001 * 0.5 * SND_VEL) // 거리 절반 곱하기 음속

unsigned long last_sampling_time;   // 단위 msec

void setup() {
  // 핀 초기화 설정
  pinMode(PIN_LED, OUTPUT);   // LED 출력으로 설정
  pinMode(PIN_TRIG, OUTPUT);  // TRIGGER 출력으로 설정
  pinMode(PIN_ECHO, INPUT);   // ECHO 입력으로 설정
  digitalWrite(PIN_TRIG, LOW);  // TRIGGER LOW로 꺼두기
  
  // 시리얼 포트 초기화
  Serial.begin(57600);
}

void loop() { 
  float distance;

  // millis는 아두이노가 시작된 이후 경과된 시간을 msec로 반환
  // 마지막 샘플링 시간에서 샘플링 주기만큼 지나지 않았을 때 종료 ( 일정한 간격 유지용 )
  if (millis() < (last_sampling_time + INTERVAL))
    return;

  distance = USS_measure(PIN_TRIG, PIN_ECHO); // 거리 감지

  // 거리가 0이거나, 거리가 최대 이상일 때
  if ((distance == 0.0) || (distance > _DIST_MAX)) {
      // 거리를 최대 + 10
      distance = _DIST_MAX + 10.0;    // 최댓값으로 거리값 설정
  // 거리가 최소 이하일 때
  } else if (distance < _DIST_MIN) {
      // 거리를 최소 - 10
      distance = _DIST_MIN - 10.0;    // 최솟값으로 거리값 설정
  } else {    // In desired Range
      digitalWrite(PIN_LED, 0);       // LED 킴      
  }

  if (distance >= 100 && distance <= 200 ){
    analogWrite(PIN_LED, (200-distance)/100*255);       // LED 킴
    Serial.print("LightValue:"); Serial.print((200-distance)/100*255);
  } else if ( distance > 200 && distance <= 300){
    analogWrite(PIN_LED, (distance-200)/100*255);       // LED 킴
    Serial.print("LightValue:"); Serial.print((distance-200)/100*255);
  }else {
    analogWrite(PIN_LED, 255);       // LED 끔
    Serial.print("LightValue:"); Serial.print(255);
  }

  // output the distance to the serial port
  Serial.print(", Min:");        Serial.print(_DIST_MIN);
  Serial.print(", distance:");  Serial.print(distance);
  Serial.print(", Max:");       Serial.print(_DIST_MAX);
  Serial.println("");
  
  // 마지막 샘플링 시간에 샘플링 주기 추가
  last_sampling_time += INTERVAL;
}

// 거리를 msec로 반환
float USS_measure(int TRIG, int ECHO)
{
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(PULSE_DURATION);
  digitalWrite(TRIG, LOW);

  // Echo가 High였던 시간 측정
  return pulseIn(ECHO, HIGH, TIMEOUT) * SCALE; // 단위 mm, 최대시간 초과시 0
  // 만약 반환값이 100000(usec)라면 즉 0.1초일 때 100000 * 0.001 * 0.5 * 346 = 17.3m, 왕복 34.6m

  // Pulse duration to distance conversion example (target distance = 17.3m)
  // - pulseIn(ECHO, HIGH, timeout) returns microseconds (음파의 왕복 시간)
  // - 편도 거리 = (pulseIn() / 1,000,000) * SND_VEL / 2 (미터 단위)
  //   mm 단위로 하려면 * 1,000이 필요 ==>  SCALE = 0.001 * 0.5 * SND_VEL
  //
  // - 예, pusseIn()이 100,000 이면 (= 0.1초, 왕복 거리 34.6m)
  //        = 100,000 micro*sec * 0.001 milli/micro * 0.5 * 346 meter/sec
  //        = 100,000 * 0.001 * 0.5 * 346
  //        = 17,300 mm  ==> 17.3m
}
