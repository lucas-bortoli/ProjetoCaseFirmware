# Projeto de Monitoramento Remoto: Integração de ESP32 e Node.js com TypeScript

O objetivo deste projeto é coletar e monitorar dados de temperatura, umidade, corrente e tensão de um sistema, utilizando um sensor SHT40 e um sensor PZEM, conectados a um microcontrolador ESP32. O ESP32 transmite esses dados para um servidor MQTT, desenvolvido em Node.js com TypeScript, que armazena essas informações em arquivos CSV.

## Componentes e Equipamentos Utilizados

- Microcontrolador ESP32
- Sensor de temperatura e umidade SHT40
- Sensor de corrente e tensão PZEM004Tv30

## Requisitos do Ambiente de Desenvolvimento

- Ambiente de desenvolvimento para o microcontrolador ESP32
- Biblioteca MQTT para o ESP32
- Ambiente de desenvolvimento Node.js com TypeScript

## Etapas para Execução do Projeto

1. Instalar as bibliotecas necessárias para o ESP32 e Node.js com TypeScript.
2. Configurar as conexões no código do ESP32 e do Node.js com TypeScript.
3. Inicializar os sensores no código do ESP32.
4. Inicializar os arquivos CSV e os escritores no código do Node.js com TypeScript.
5. Conectar o ESP32 ao Wi-Fi e ao broker MQTT.
6. Iniciar o servidor do Node.js com TypeScript.

## Funcionamento do Projeto

- O microcontrolador ESP32 lê os dados dos sensores SHT40 e PZEM.
- O ESP32 envia esses dados para o servidor MQTT.
- O cliente, desenvolvido em Node.js com TypeScript, recebe as mensagens publicadas no broker e as grava em arquivos CSV.
