#include <Arduino.h>
#include <M5Stack.h>
#include <Wire.h>
#include <VL53L0X.h>
#include <Ambient.h>

WiFiClient client;
Ambient ambient;
VL53L0X sensor;

//Ambientの設定
unsigned int channelId = 12345;//チャネルID
const char *writeKey = "write key";//ライトキー
const char *URL = "https://ambidata.io/bd/board.html?id=12345";//チャネルのURL

//wifiの設定
const char *ssid = "ssid";
const char *password = "password"; 

byte i = 0;
int b_meter =1000 ; //フローの最大可動域量を入力(mm)
int ave = 0;

void setup()
{
  M5.begin();
  Serial.begin(9600);
  Wire.begin();
  WiFi.begin(ssid, password);
  delay(10);
  ambient.begin(channelId, writeKey, &client);
  M5.lcd.setCursor(0,0);
  M5.Lcd.setTextSize(2);
  
  while (WiFi.status() != WL_CONNECTED) //  Wi-Fi AP接続待ち
  {
    while (i >= 120) //20秒立ってもwifiに接続されない場合10秒間隔でエラー表示
    {
      Serial.println("ERRER: wifi don't conect");
      M5.Lcd.println("ERRER: wifi don't conect");
      delay(1000);
      WiFi.begin(ssid, password);
    }
    delay(500);
    Serial.printf(".");
    M5.Lcd.printf(".");
    i++;
  }
  i = 0;
  Serial.print("WiFi connected\r\nIP address: ");
  Serial.println(WiFi.localIP());
  M5.Lcd.print("WiFi connected\r\nIP address: ");
  M5.Lcd.println(WiFi.localIP());

  sensor.setTimeout(500);
  if (!sensor.init())
  {
    Serial.println("Failed to detect and initialize sensor!");
    M5.Lcd.println("Failed to detect and initialize sensor!");
    while (1)
    {
    }
  }

  // Start continuous back-to-back mode (take readings as
  // fast as possible).  To use continuous timed mode
  // instead, provide a desired inter-measurement period in
  // ms (e.g. sensor.startContinuous(100)).
  sensor.startContinuous();
  delay(1000);
  M5.Lcd.clear();
  M5.Lcd.qrcode(URL,70,60,180,6);
}

void loop()
{
  M5.update();
  int f_meter =0; // フローメーター:水槽の実際の水かさを格納
  
  
  M5.lcd.setCursor(0,0);
  M5.Lcd.setTextSize(2);
  
  f_meter = b_meter - sensor.readRangeContinuousMillimeters();//水位計算

  while (f_meter < 0)
  {
    Serial.print("Error");
    M5.Lcd.print("Error");
    delay(1000);
    f_meter = b_meter - sensor.readRangeContinuousMillimeters();
    M5.Lcd.clear();
  }
  

  //センサーの実測値を表示
  Serial.print("Distance meter: ");
  Serial.println(sensor.readRangeContinuousMillimeters());
  M5.Lcd.print("Distance meter: ");
  M5.Lcd.println(sensor.readRangeContinuousMillimeters());

  //水位を表示
  M5.Lcd.print("Flow meter: ");
  M5.Lcd.println(f_meter);
  Serial.print("Flow meter: ");
  Serial.println(f_meter);

  

  ave= ave + f_meter;
  i++;
  M5.Lcd.println("To Send:");
  M5.Lcd.fillRect(95,33,220,20,0);
  M5.Lcd.progressBar(95,33,220,20, i*5);

  if (i >= 20)
  {
    ave = ave / 21;
    ambient.set(1, ave);
    ambient.send();//20秒に1回Ambientに送信
    Serial.println("send data");
    
    i=0;
    delay(2000);
  }

  
  
  delay(1000);
  
  if (sensor.timeoutOccurred())
  {
    Serial.print(" TIMEOUT");
  }

}