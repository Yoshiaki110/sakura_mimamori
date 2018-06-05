# sakura_mimamori

ログの説明
motion:{人感} sound:{音量} {送信時刻} + {送信間隔} < {現在時刻} {受信データ}


チャンネル
０：送信時刻の変更
１：送信間隔の変更
２：リセット
受信失敗もリセット

送信

https://api.sakura.io/incoming/v1/docs/
「token」と「module」を設定

curl -X POST --header 'Content-Type: application/json' --header 'Accept: application/json' -d '{"type":"channels","module":"＊＊","payload":{"channels":[{"channel":0,"type":"L","value":123456}]}}' 'https://api.sakura.io/incoming/v1/＊＊＊'
