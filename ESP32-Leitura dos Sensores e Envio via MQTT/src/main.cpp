#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>
#include <cstdio>
#include <Wire.h>

// exigido pela SensirionI2cSht4x, para usar a definição correta de NO_ERROR
#ifdef NO_ERROR
#undef NO_ERROR
#endif
#define NO_ERROR 0

// biblioteca para comunicação com o SHT40 via I2C
#include <SensirionI2cSht4x.h>
// biblioteca para comunicação com o PZEM
#include <PZEM004Tv30.h>

// criar instância para comunicação com o SHT40
SensirionI2cSht4x sht40;
// criar instância da classe no hardware serial rx=16 tx=17
PZEM004Tv30 pzem(Serial2, 16, 17);

const char *TOPICO_TEMPERATURA = "/org_geminataprojeto_case/sensor_temperatura";
const char *TOPICO_UMIDADE = "/org_geminataprojeto_case/sensor_umidade";
const char *TOPICO_CORRENTE = "/org_geminataprojeto_case/sensor_corrente";
const char *TOPICO_TENSAO = "/org_geminataprojeto_case/sensor_tensao";

const char *WIFI_SSID = "Nome do Wi-Fi";
const char *WIFI_SENHA = "12345678";

const char *mqtt_client_id = "firmware-testes-0001";
const char *mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;

WiFiClient wifi_client;
PubSubClient mqtt_client(wifi_client);

float valor_temperatura;
float valor_umidade;
float valor_corrente;
float valor_tensao;

void setup()
{
  Serial.begin(115200);

  // Inicializar I2C
  Wire.begin();
  sht40.begin(Wire, SHT40_I2C_ADDR_44);
  sht40.softReset();

  Serial.print("Conectando ao Wi-Fi...");
  WiFi.begin(WIFI_SSID, WIFI_SENHA);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
  }
  Serial.println();
  Serial.println("Conectado!");

  // Inicializar pzem
  pzem.resetEnergy();

  // inicializar MQTT
  mqtt_client.setServer(mqtt_server, mqtt_port);
}

void send(float valor, const char *topico)
{
  char buffer[48];
  snprintf(buffer, sizeof(buffer), "%.2f", valor);
  mqtt_client.publish(topico, buffer);
}

void loop()
{
  // se temos wi-fi mas não temos mqtt...
  while (wifi_client.connected() && !mqtt_client.connected())
  {
    // conectar ao mqtt
    while (!mqtt_client.connect(mqtt_client_id))
    {
      Serial.println("Fazendo conexão ao MQTT...");
      delay(100);
    }

    Serial.println("Conectado!");
  }

  valor_tensao = pzem.voltage();
  valor_corrente = pzem.current();
  sht40.measureLowestPrecision(valor_temperatura, valor_umidade);

  send(valor_corrente, TOPICO_CORRENTE);
  send(valor_tensao, TOPICO_TENSAO);
  send(valor_temperatura, TOPICO_TEMPERATURA);
  send(valor_umidade, TOPICO_UMIDADE);

  delay(100);
}
