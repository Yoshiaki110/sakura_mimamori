# sakura_mimamori

TODO
・音量は小さいほうが大きい
・３つから中間を求める式の誤り



ログの説明
motion:{人感} sound:{音量} {送信時刻} + {送信間隔} < {現在時刻} {受信データ}


LEDの説明
短く５回点滅：受信した
短く３回点滅：送信した
ゆっくり点滅（消えている時間が長い）：正常に動いている
ゆっくり点滅（点灯している時間が長い）：初期化中


InCommingチャンネル
０：送信時刻の変更
１：送信間隔の変更
２：リセット
他：現状の状態をリクエスト
受信失敗もリセット

送信

https://api.sakura.io/incoming/v1/docs/
「token」と「module」を設定

curl -X POST --header 'Content-Type: application/json' --header 'Accept: application/json' -d '{"type":"channels","module":"＊＊","payload":{"channels":[{"channel":0,"type":"L","value":123456}]}}' 'https://api.sakura.io/incoming/v1/＊＊＊'




OutGoingチャンネル
０：人感
１：音量
２：送信時刻
３：送信間隔
４：送信時刻
５：
　１バイト目：発生個所
　２バイト目：ConnectionStatus
　３バイト目：SignalQuality


USB抜き差しorターミナルのとの接続でか？リセットがかかる


LCDバージョン(ミリ秒でなく秒にする)2x16 7桁で115日
１行目 _time + int < time
２行目 S:time   R:time

  lcd.begin(16, 2);
  lcd.setRGB(0, 0, 0);
  lcd.noCursor();
  lcd.noBlink();
  lcd.
  lcd.
  lcd.
  lcd.setCursor(0, 0);
  lcd.print("initialising..");
  lcd.setCursor(1, 0);
  lcd.print(i);

void lcd_time_int() {
  // １行目クリア
  lcd.setCursor(0, 0);
  lcd.print(_time / 1000);
  lcd.setCursor(0, 8);   // カーソルが移動するなら不要
  lcd.print("+");
  lcd.setCursor(0, 10);
  lcd.print(_interval / 1000);
}
void lcd_time_int(unsigned long time) {
  // １行目クリア
  lcd.setCursor(0, 0);
  lcd.print((_time + _interval) / 1000);
  lcd.setCursor(0, 8);   // カーソルが移動するなら不要
  lcd.print("<");
  lcd.setCursor(0, 10);
  lcd.print(time / 1000);
}
void lcd_sendtime(unsigned long time) {
  lcd.setCursor(1, 0);
  lcd.print("S");
  lcd.setCursor(1, 1);   // カーソルが移動するなら不要
  lcd.print(time / 1000);
}
void lcd_recvtime(unsigned long time) {
  lcd.setCursor(1, 8);
  lcd.print("R");
  lcd.setCursor(1, 9);   // カーソルが移動するなら不要
  lcd.print(time / 1000);
}
