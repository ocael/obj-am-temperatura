#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

// --------- OLED ---------
#define OLED_W 128
#define OLED_H 64
#define OLED_RST -1
Adafruit_SSD1306 tela(OLED_W, OLED_H, &Wire, OLED_RST);

// --------- SENSOR DHT22 ---------
#define PINO_DHT 15
#define TIPO_DHT DHT22
DHT dht(PINO_DHT, TIPO_DHT);

// --------- LEDS & BUZZER ---------
#define LED_FRIO 2      // amarelo
#define LED_QUENTE 5    // vermelho
#define BUZZER 25

// --------- LIMITES DE TEMPERATURA ---------
float LIMITE_FRIO = 16.0;
float LIMITE_QUENTE = 30.0;

// --------- WIFI / MQTT ---------
const char* WIFI_SSID = "Wokwi-GUEST";
const char* WIFI_SENHA = "";
const char* SERVIDOR_MQTT = "test.mosquitto.org";

WiFiClient net;
PubSubClient mqtt(net);

// --------- FUNÇÕES ---------
void desligarLeds() {
  digitalWrite(LED_FRIO, LOW);
  digitalWrite(LED_QUENTE, LOW);
}

void buzzerAviso() {
  for(int i = 0; i < 3; i++){
    tone(BUZZER, 900 + i * 100);
    delay(120);
    noTone(BUZZER);
    delay(120);
  }
}

void conectarWifi(){
  Serial.print("WiFi: ");
  WiFi.begin(WIFI_SSID, WIFI_SENHA);
  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }
  Serial.println("\nConectado");
}

void conectarMQTT(){
  while(!mqtt.connected()){
    Serial.print("MQTT...");
    if(mqtt.connect("ESP32_TempNode")){
      Serial.println("OK");
    } else {
      Serial.print("Erro ");
      Serial.println(mqtt.state());
      delay(1500);
    }
  }
}

String classificar(float t){
  if(t < LIMITE_FRIO)  return "fria";
  if(t > LIMITE_QUENTE) return "quente";
  return "estavel";
}

void exibir(float temp, String cond){
  tela.clearDisplay();
  tela.setTextSize(1);
  tela.setTextColor(SSD1306_WHITE);

  tela.setCursor(0,0);
  tela.println("Monitor Climatico");

  tela.setCursor(0, 22);
  tela.print("Temp: ");
  tela.print(temp);
  tela.println(" C");

  tela.setCursor(0, 40);
  tela.print("Status: ");
  tela.println(cond);

  tela.display();
}

void publicar(float temp, String cond){
  // tópicos totalmente novos
  String topT = "ambiente/clima/temp";
  String topC = "ambiente/clima/estado";

  String msgT = "valor=" + String(temp, 1);
  String msgC = "cond=" + cond;

  mqtt.publish(topT.c_str(), msgT.c_str());
  mqtt.publish(topC.c_str(), msgC.c_str());
}

// --------- SETUP ---------
void setup(){
  Serial.begin(115200);

  pinMode(LED_FRIO, OUTPUT);
  pinMode(LED_QUENTE, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  desligarLeds();

  dht.begin();

  if(!tela.begin(SSD1306_SWITCHCAPVCC, 0x3C)){
    Serial.println("Falha OLED");
    while(true);
  }

  tela.clearDisplay();
  tela.setCursor(10, 25);
  tela.setTextSize(1);
  tela.println("Carregando...");
  tela.display();
  delay(1000);

  conectarWifi();
  mqtt.setServer(SERVIDOR_MQTT, 1883);
}

// --------- LOOP ---------
void loop(){
  if(!mqtt.connected()) conectarMQTT();
  mqtt.loop();

  float temperatura = dht.readTemperature();

  if(isnan(temperatura)){
    Serial.println("Erro DHT");
    return;
  }

  String cond = classificar(temperatura);

  desligarLeds();

  if(cond == "fria")   digitalWrite(LED_FRIO, HIGH);
  if(cond == "quente") digitalWrite(LED_QUENTE, HIGH);

  if(cond != "estavel") buzzerAviso();

  exibir(temperatura, cond);
  publicar(temperatura, cond);

  Serial.print("Temp: ");
  Serial.print(temperatura);
  Serial.print(" | Estado: ");
  Serial.println(cond);

  delay(1500);
}
