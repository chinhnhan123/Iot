/*************************************************************

  This is a simple demo of sending and receiving some data.
  Be sure to check out other examples!
 *************************************************************/

// Template ID, Device Name and Auth Token are provided by the Blynk.Cloud
// See the Device Info tab, or Template settings

#define BLYNK_TEMPLATE_ID "TMPLSKzhrj_S"
#define BLYNK_DEVICE_NAME "e"
#define BLYNK_AUTH_TOKEN "hqiDgXHamu3EXGkS9-Nsj7x4GVIyDTbj"
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <WiFiManager.h>

char auth[] = BLYNK_AUTH_TOKEN;
int warnPin = 16; 
int switch1  ;
int switch2  ;
int value;
BlynkTimer timer;
WiFiServer server(80);
// This function is called every time the Virtual Pin 0 state changes
void setup()
{
  // Debug console
  Serial.begin(115200);
  WiFiManager wifimanager;
  wifimanager.autoConnect("autoConnectAP");
  String ssid =  WiFi.SSID();
  String pass =  WiFi.psk();
  int str_ssid = ssid.length() + 1; 
  int str_pass = pass.length() + 1; 
  char char_arrays[str_ssid];
  char char_arrayp[str_pass];
  ssid.toCharArray(char_arrays, str_ssid);
  pass.toCharArray(char_arrayp, str_pass);
  
  server.begin();
  pinMode(warnPin, OUTPUT);
  timer.setInterval(3000, setupvalue);
  Blynk.begin(auth,char_arrays,char_arrayp);
}

void setupvalue(){
  Blynk.virtualWrite(V5,value );  
}

BLYNK_WRITE(V0)
{
  int pinValue = param.asInt();
  if(pinValue == 1)
{
    switch1 = 1;
    Serial.println("Button switched to 1");
  }
  else
{ 
    switch1 = 0;
    //digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("Button switched to 0");
  }
}

BLYNK_WRITE(V1)
{
  int pinValue1 = param.asInt();
  if(pinValue1 == 1)
{
    switch2 = 1;
  }
  else
{ 
    switch2 = 0;
  }
}

void loop()
{
  Blynk.run();
  timer.run();
  WiFiManager wifimanager;
  value = analogRead(A0);   //đọc giá trị điện áp ở chân A0 - chân cảm biến

  if( switch2 == 1){
    wifimanager.resetSettings();
    ESP.eraseConfig(); 
    delay(2000);
    ESP.reset(); 
  }
  if(WiFi.status() == WL_CONNECTED){
      Blynk.virtualWrite(V6, "Connected WiFi");
    }
  
  if( switch1 == 1){
        Blynk.virtualWrite(V3, "TURN ON");
      if(value > 450 & value < 550){
    Blynk.virtualWrite(V4, " NONG DO KHI HYDROCARBON VƯỢT MỨC BINH THUONG. KHONG TOT CHO SUC KHOE NEN MO CUC CHO THOANG MAT PHONG");
    Serial.println(value);
    digitalWrite(warnPin,LOW);
    delay(1000);
  }

  if(value >= 550 & value <= 800){
    Blynk.virtualWrite(V4, " NONG DO KHI GAS CAO. CO THE GAY CHAY NO");
    Serial.println(value);
    digitalWrite(warnPin,HIGH);
    delay(100);
  }
  if(value >= 800 ){
    Blynk.virtualWrite(V4, "KHA NANG CHAY NO CAO. LAP TUC SƠ TÁN");
    Serial.println(value);
    digitalWrite(warnPin,HIGH);

  }
 if(value <= 450){
    Blynk.virtualWrite(V4, "KHONG KHI TRONG LANH. TOT CHO SUC KHOE" );
    Serial.println(value);
    digitalWrite(warnPin,LOW);
  }
  }
  else if(switch1 == 0){
    Blynk.virtualWrite(V3, "TURN OFF");
    Blynk.virtualWrite(V4, " tắt rồi sao chạy " );
    digitalWrite(warnPin,LOW);
    value = 0;
    }
}
