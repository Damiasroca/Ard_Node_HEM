//#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#if defined(ESP32)
#include <WiFiMulti.h>
WiFiMulti wifiMulti;
#define DEVICE "ESP32"
#elif defined(ESP8266)
#include <ESP8266WiFiMulti.h>
ESP8266WiFiMulti wifiMulti;
#define DEVICE "NodeMCU"
#endif

#include <InfluxDbClient.h>
#include <InfluxDbCloud.h>


#define WIFI_SSID "NOM_WiFi"

#define WIFI_PASSWORD "CONTRASENYA_WiFi"

#define INFLUXDB_URL "http://servidor.com:8086"

#define INFLUXDB_TOKEN "TOKEN_PERSONAL"

#define INFLUXDB_ORG "Main_ORG"

#define INFLUXDB_BUCKET "BUCKET_PERSONAL"

// Set timezone string according to https://www.gnu.org/software/libc/manual/html_node/TZ-Variable.html
// Examples:
//  Pacific Time: "PST8PDT"
//  Eastern: "EST5EDT"
//  Japanesse: "JST-9"
//  Central Europe: "CET-1CEST,M3.5.0,M10.5.0/3"
#define TZ_INFO "CET-1CEST,M3.5.0,M10.5.0/3"


InfluxDBClient client(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN, InfluxDbCloud2CACert);

// Data point
Point sensor("Ca_meva");

float data1;
float data2;
float data3;
float data4;
float data5;
float data6;
float data7;
float data8;
int data9;
int data10;

void wifi() {


  // Setup wifi
  WiFi.mode(WIFI_STA);
  wifiMulti.addAP(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting to wifi");
  while (wifiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }
  Serial.println();

  // Add tags
  sensor.addTag("device", DEVICE);


  // Accurate time is necessary for certificate validation and writing in batches
  // For the fastest time sync find NTP servers in your area: https://www.pool.ntp.org/zone/
  // Syncing progress and the time will be printed to Serial.
  timeSync(TZ_INFO, "pool.ntp.org", "time.nis.gov");

  // Check server connection
  if (client.validateConnection()) {
    Serial.print("Connected to InfluxDB: ");
    Serial.println(client.getServerUrl());
  } else {
    Serial.print("InfluxDB connection failed: ");
    Serial.println(client.getLastErrorMessage());
  }
}

void setup() {
  // Initialize Serial port
  Serial.begin(115200);
  wifi();
  //s.begin(115200);
  while (!Serial) continue;
}

void loop() {
  StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(Serial);

  if (root == JsonObject::invalid())
  {
    return;
  }
  influxdb();
  //Print the data in the serial monitor
  //Serial.println("JSON received and parsed");
  root.prettyPrintTo(Serial);
 // Serial.println("");
  //Serial.print("Potència ");
  data1 = root["pow"];
  //Serial.println(data1);
  //Serial.print("Potència aparent    ");
  data2 = root["apow"];
  //Serial.println(data2);
  //Serial.print("Factor potència  ");
  data3 = root["pf"];
  //Serial.println(data3);
  //Serial.print("Voltatge  ");
  data4 = root["volt"];
  //Serial.println(data4);
  //Serial.print("Intensitat  ");
  data5 = root["inte"];
  //Serial.println(data5);
  //Serial.print("Temperatura  ");
  data6 = root["temp"];
  //Serial.println(data6);
  //Serial.print("Pressiò  ");
  data7 = root["pres"];
  //Serial.println(data7);
  //Serial.print("Humitat  ");
  data8 = root["hum"];
  //Serial.println(data8);
  //Serial.print("CO2  ");
  data9 = root["gas"];
 // Serial.println(data9);
  //Serial.print("TVOC  ");
  data10 = root["voc"];
  //Serial.println(data10);
  //Serial.println("");
  //Serial.println("---------------------xxxxx--------------------");
  //Serial.println("");
  delay(10000);
}
