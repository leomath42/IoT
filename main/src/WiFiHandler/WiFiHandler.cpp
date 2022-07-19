#include "WiFiHandler.h"
#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>

//defines de id mqtt e tópicos para publicação e subscribe
#define TOPICO_SUBSCRIBE_P1 "led/switch" 
                                                   
#define ID_MQTT  "m5020"

// MQTT
const char* BROKER_MQTT = "test.mosquitto.org";
int BROKER_PORT = 1883;

//Variáveis e objetos globais
WiFiClient espClient;
PubSubClient MQTT(espClient);

WiFiManager wifiManager;
ESP8266WiFiClass wifi;

void reconnectWiFi()
{
    if (WiFi.status() == WL_CONNECTED)
        return;
	
    WiFi.begin(); 
    
    while (WiFi.status() != WL_CONNECTED) 
    {
        delay(100);
        Serial.print(".");
    }
  
    Serial.println();
    Serial.print("Conectado com sucesso!");
    Serial.println();
    Serial.print("IP obtido: ");
    Serial.print(WiFi.localIP());  // mostra o endereço IP obtido via DHCP
    Serial.println();
    Serial.print("Endereço MAC: ");
    Serial.print(WiFi.macAddress()); // mostra o endereço MAC do esp8266
}

void setup_wifi()
{
    // wifiManager.resetSettings();
    wifiManager.setConnectTimeout(60);

    logger_print("Starting WiFi...");
    if(wifi.status() == WL_DISCONNECTED)
    {
        logger_print("Failed to connect.");
        delay(1000);
        logger_print("Going to init AP mode.");
        delay(1000);
        logger_print("AP: ESP");
        wifiManager.autoConnect("ESP", "12345678");
    }

}

void handle_ota()
{
    ArduinoOTA.handle();
}

void setup_ota()
{
  Serial.println("Iniciando OTA....");
  ArduinoOTA.setHostname("ota-port");

  ArduinoOTA.setPassword((const char *)"123OTA654");
  ArduinoOTA.onStart([]() {
    Serial.println("Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
}


void setup_MQTT()
{
    MQTT.setServer(BROKER_MQTT, BROKER_PORT);
    MQTT.setCallback(mqtt_callback);
}

void mqtt_callback(char* topic, byte* payload, unsigned int length)
{
  String msg;
  
  for(int i = 0; i < length; i++) 
  {
     char c = (char)payload[i];
     msg += c;
  }
  
  Serial.println("msg = " +  msg);
  
  if (msg.equals("ON"))
  {
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("LED Ligado");
  }
  
  if (msg.equals("OFF"))
  {
    digitalWrite(LED_BUILTIN, LOW);
    Serial.println("LED Desligado");
  }   
}


void reconnectMQTT()
{
    while (!MQTT.connected()) 
    {
        Serial.print("* Tentando se conectar ao Broker MQTT: ");
        Serial.println(BROKER_MQTT);
 
        if (MQTT.connect(ID_MQTT))
        {
            Serial.println("Conectado com sucesso ao broker MQTT!");
            MQTT.subscribe(TOPICO_SUBSCRIBE_P1);
        } 
        else 
        {
            Serial.println("Falha ao reconectar no broker.");
            Serial.println("Havera nova tentativa de conexao em 2s");
            delay(2000);
        }
    }
}


void checkConnectionWiFiMQTT()
{
    if (!MQTT.connected()) 
        reconnectMQTT();
    
     reconnectWiFi();
}