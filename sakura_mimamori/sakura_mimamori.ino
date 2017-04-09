#include <avr/wdt.h>
#include <SakuraIO.h>

#define MOTION  9
#define SOUND  A0
void reset(long time, int addr);

SakuraIO_I2C _sakuraio;

unsigned long _interval = 0;
int _motion = 0;
int _sound = 0;
unsigned long _time = 0;

void setup() {
  //delay(5000);
  Serial.begin(9600);
  pinMode(MOTION, INPUT);
  pinMode(SOUND, INPUT);
  wdt_enable(WDTO_8S);      // ※ 8秒のウォッチドッグタイマーをセット

  _interval = 60 * 60 * 1000L;
  Serial.print("Waiting to come online");
  for (int i = 0;; i++) {
    if ( (_sakuraio.getConnectionStatus() & 0x80) == 0x80 ) {
      break;
    }
    Serial.print(".");
    delay(1000);
    if (i < 600) {			// ※ 10分以降はタイマーをクリアーしないのでリセット
        wdt_reset();
    }
  }
  Serial.println("end");
  delay(2000);
  _time = millis();
}

void send(long time, int addr, bool errer) {
  if (_sakuraio.enqueueTx(0, (uint32_t)_motion) != CMD_ERROR_NONE) {
    Serial.println("[ERR] enqueue error");
    reset(time, 11);		// リセット
  }
  if (_sakuraio.enqueueTx(1, (uint32_t)_sound) != CMD_ERROR_NONE) {
    Serial.println("[ERR] enqueue error");
    reset(time, 12);    // リセット
  }
  if (_sakuraio.enqueueTx(2, (uint64_t)_time) != CMD_ERROR_NONE) {
    Serial.println("[ERR] enqueue error");
    reset(time, 13);    // リセット
  }
  if (_sakuraio.enqueueTx(3, (uint64_t)_interval) != CMD_ERROR_NONE) {
    Serial.println("[ERR] enqueue error");
    reset(time, 14);    // リセット
  }
  if (_sakuraio.enqueueTx(4, (uint64_t)time) != CMD_ERROR_NONE) {
    Serial.println("[ERR] enqueue error");
    reset(time, 15);    // リセット
  }
  if (_sakuraio.enqueueTx(5, (uint32_t)addr) != CMD_ERROR_NONE) {
    Serial.println("[ERR] enqueue error");
    reset(time, 16);    // リセット
  }
  _sakuraio.send();
  wdt_reset();
  delay(1000);
  wdt_reset();

  uint8_t available;
  uint8_t queued;
  if (_sakuraio.getTxQueueLength(&available, &queued) != CMD_ERROR_NONE) {
    Serial.println("[ERR] get tx queue length error");
    reset(time, 17);    // リセット
  }
  Serial.print("Available :");
  Serial.print(available);
  Serial.print(" Queued :");
  Serial.println(queued);
}

void reset(long time, int addr) {
  send(time, addr, true); 
  delay(10000);    // ※ クリアーできないのでリセットがかかる
}

int recv() {
  uint8_t avail;
  uint8_t queued;
  _sakuraio.getRxQueueLength(&avail, &queued);
//  Serial.print("Rx Available=");
//  Serial.print(avail);
//  Serial.print(" Queued=");
//  Serial.println(queued);

  for (uint8_t i = 0; i < queued; i++) {
    uint8_t channel;
    uint8_t type;
    uint8_t values[8];
    int64_t offset;
    uint8_t ret = _sakuraio.dequeueRx(&channel, &type, values, &offset);
    Serial.print("Dequeue ");
    Serial.print(ret);
    if (ret == 0x01) {
      Serial.print(" ch="); Serial.print(channel);
      Serial.print(" type="); Serial.print((char)type);
      Serial.print(" values=[");
      for (uint8_t b=0; b<7; b++) {
        Serial.print(values[b]);
        Serial.print(" ");
      }
      Serial.print(values[7]);
      Serial.print("] offset="); Serial.println((int32_t)offset);
      if (channel == 0 && type == 'L') {
        _time = values[0] +
                    values[1] * 256L +
                    values[2] * 256 * 256L +
                    values[3] * 256 * 256 * 256L;
      }
      if (channel == 1 && type == 'L') {
        _interval = values[0] +
                    values[1] * 256L +
                    values[2] * 256 * 256L +
                    values[3] * 256 * 256 * 256L;
      }
      if (channel == 2 && type == 'L') {
        int t = values[0] +
                values[1] * 256L +
                values[2] * 256 * 256L +
                values[3] * 256 * 256 * 256L;
        delay(t);
      }
    } else {
      Serial.println(" ERROR");
      reset(millis(), 12);    // リセット
    }
  }
  return queued;
}

int getMotion(){
  int motion = digitalRead(MOTION);
  delay(25);
  motion += digitalRead(MOTION);
  delay(25);
  motion += digitalRead(MOTION);
  if (motion >= 2) {
    return 1;
  }
  return 0;
}
int getSound(){
  int sound1 = analogRead(SOUND);
  delay(25);
  int sound2 = analogRead(SOUND);
  delay(25);
  int sound3 = analogRead(SOUND);
  int ret = sound1 > sound1 ? sound1 : sound2; // 大きいのとって
  ret = ret < sound3 ? ret : sound3;           // 小さいのをとる
  return ret;
}

void loop() {
  long time = millis();
  int motion = getMotion();
  int sound = getSound();
/*  Serial.print(time);
  Serial.print(" motion:");
  Serial.print(motion);
  Serial.print(" sound:");
  Serial.println(sound);*/
  _motion += motion;  // 人感センサはONの累計
  _sound = sound > _sound ? sound : _sound; // 音量は最大値
  int r = recv();
  Serial.print(_time);
  Serial.print(" + ");
  Serial.print(_interval);
  Serial.print(" < ");
  Serial.print(time);
  Serial.print(" ");
  Serial.println(r);
  if (_time + _interval < time || _time == 0 || _time > time || r != 0) {
    Serial.print("**send motion:");
    Serial.print(_motion);
    Serial.print(" sound:");
    Serial.println(_sound);
    send(time, 0, false);                     // 送信
    _time = time;
    _motion = 0;
    _sound = 0;
  }
  wdt_reset();
  delay(900);
  wdt_reset();
}
