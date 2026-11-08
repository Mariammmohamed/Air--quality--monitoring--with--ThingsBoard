

//knolleary/PubSubClient@^2.8


#include <WiFi.h>
#include <DHT.h>
#include <Arduino.h>
#include <ThingsBoard.h>
#include <Arduino_MQTT_Client.h>
 
// Wi-Fi credentials
const char *SSID = "mm34";
const char *PASS = "cfthikm1475963";




//things board server
#define TOKEN  "MotQKdtNqaN1oJY11ulF"
#define THINGSBOARD_SERVER "thingsboard.cloud"


//WiFiClientSecure espClient;  
WiFiClient espClient;
Arduino_MQTT_Client mqttClient(espClient);
ThingsBoard tb(mqttClient);




// ---------------- Pin definitions -------------------


#define DHTPIN   23        // DHT22 data pin
#define DHTTYPE  DHT11


const int MQ = 35;
const int LED = 13;
int button = 12;
int  LED_STATE;
unsigned long bootMillis = 0;


DHT dht(DHTPIN, DHTTYPE);


 bool setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to WiFi: ");
  Serial.println(SSID);


  WiFi.begin(SSID, PASS);


  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }


  Serial.println();
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());


  return true;
}


void connectThingsBoard() {
  while (!tb.connected()) {
    Serial.print("Connecting to ThingsBoard... ");
    if (tb.connect(THINGSBOARD_SERVER, TOKEN)) {
      Serial.println("connected");
    } else {
      Serial.print("failed. Retry in 5s...");
      delay(5000);
    }
  }
}
int getAverageReading() {
  long total = 0;

  for (int i = 0; i < 10; i++) {
    total += analogRead(MQ);
    delay(100);
  }

  return total / 10;
}
 void setup() {
  Serial.begin(115200);


 //pinMode(LED_PIN, OUTPUT);
 pinMode(MQ, INPUT);
pinMode(LED, OUTPUT);
pinMode(button, INPUT);
 setup_wifi();
 dht.begin();

 }


 void loop()
 {


 
 if (WiFi.status() != WL_CONNECTED) {
   setup_wifi();
  }


  if (!tb.connected()) {
    connectThingsBoard();
  }

if (button == HIGH )
{
  digitalWrite(LED,HIGH);
    LED_STATE = 1; 
}
else 
{
digitalWrite(LED,LOW);
    LED_STATE = 0;
}

  tb.loop();


  float temp = dht.readTemperature();
  float hum = dht.readHumidity();
  int mqValue = getAverageReading();


  int avg = 250; 

  float Pol_I = ((mqValue-avg)/avg)*100.0;

  if(0<=Pol_I <10)
  {
    
  }


 //if (isnan(temp) || isnan(hum)) {
    //Serial.println("Failed to read from DHT sensor!");
   //return;
  //}

//Serial.print("Sending Temp: ");
//Serial.print(temp);
//Serial.print(" °C, Hum: ");
//Serial.println(hum);
Serial.print("MQ135 Raw Value: ");
Serial.println(mqValue);   
Serial.print("Pollution index percent % : ");
Serial.println(Pol_I);   
delay(1000);

   //Send data to ThingsBoard
  tb.sendTelemetryData("temperature", temp);
  tb.sendTelemetryData("humidity", hum);
  tb.sendTelemetryData("mq135_raw", mqValue);
  tb.sendTelemetryData("pollution ",Pol_I);


  delay(5000);  // Delay between readings


 }







