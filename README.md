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


https://sakura.io/developer/pdf/sakura_module_datasheet_latest.pdf

接続状態
最上位ビットが１なら、接続状態
それを除いた、値がエラーコード
00h エラーなし
01h 圏外
02h 接続エラー
03h 意図しない切断

電界強度
00h 圏外
01h 非常に弱い
02h 弱い
03h 中くらい
04h 強い
05h 非常に強い





