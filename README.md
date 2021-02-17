# M5Stackで塗装設備のフロー監視システム
M5Stackで水位を計測しAmbientへ送る
## ~ インストール ~
 
    1,  Download ZIP もしくは、Git Cloneする。
      
    2,  Ambientの設定、WiFiの設定を行う。
        //Ambientの設定
        unsigned int channelId = 12345;//チャネルID
        const char *writeKey = "write key";//ライトキー
        const char *URL = "https://ambidata.io/bd/board.html?id=12345";//チャネルのURL
        ※チャネルのURLも入力してください。
        
        //wifiの設定
        const char *ssid = "ssid";
        const char *password = "password";  
        それぞれwifiのIDとPASを入力してください。 
        
    4,  M5 Stackにコンパイル
    以上
  