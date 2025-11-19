# 🌡️ Monitoramento de Temperatura com ESP32 + DHT22 + MQTT

Este projeto utiliza um **ESP32**, um sensor **DHT22**, um **OLED 128x64**, LEDs de indicação e um **buzzer** para criar um sistema completo de monitoramento de temperatura ambiente.  
As leituras são exibidas no display, enviadas via **MQTT** e acompanhadas por alertas visuais e sonoros.

---

## 📦 Estrutura do Projeto

```text
projeto-temperatura-esp32/
│── sketch.ino
│── diagram.json
│── libraries.txt
│── README.md

---

## 🚀 Como Rodar no Wokwi

1. Acesse **[https://wokwi.com](https://wokwi.com)**
2. Clique em **New Project**
3. Envie estes arquivos:
   - `sketch.ino`
   - `diagram.json`
   - `libraries.txt`
4. Clique em **Start Simulation**
5. Abra o **Serial Monitor** para acompanhar os dados MQTT

---

## 🛠️ Hardware Utilizado

- ESP32 DevKit C V4  
- Sensor de temperatura DHT22  
- Display OLED SSD1306 (I2C)  
- LED Amarelo (temperatura baixa)  
- LED Verde (temperatura normal)  
- LED Vermelho (temperatura alta)  
- Buzzer  
- Jumpers e protoboard  

---

## 🌐 Comunicação via MQTT

O ESP32 publica as informações em dois tópicos:

- `temperatura/ambiente` → Valor lido do DHT22  
- `temperatura/alerta` → Estado atual da temperatura  

Ferramentas compatíveis:  

- MQTT Explorer  
- HiveMQ Web Client  
- Mosquitto CLI  
- Node-RED  

---

## ⚙️ Bibliotecas Utilizadas

- **Adafruit_SSD1306** — Controle do display  
- **Adafruit_GFX** — Renderização dos textos  
- **WiFi** — Conexão com rede  
- **PubSubClient** — Publicação MQTT  
- **DHT Sensor Library** — Leitura do sensor DHT22  
- **Wire** — Comunicação I2C  

---

## 📏 Regras de Funcionamento

| Faixa de Temperatura | LED Ativo | Publicação MQTT |
|----------------------|-----------|----------------|
| Abaixo de 18°C       | Amarelo   | LOW_TEMP       |
| Entre 18°C e 27°C    | Verde     | OK_TEMP        |
| Acima de 27°C        | Vermelho  | HIGH_TEMP      |

> O buzzer toca sempre que a temperatura estiver na faixa **baixa** ou **alta**.

---

## 📺 Interface no Display

O OLED apresenta:

   - `Temperatura: XX.X°C`
   - `Estado: FRIO / NORMAL / QUENTE`

Atualizado a cada ciclo de leitura.

---

## 🔌 Ligações do Hardware

**DHT22**  
- VCC → 3.3V  
- GND → GND  
- SDA → GPIO 15  
- NC → não conectado  

**OLED SSD1306**  
- VCC → 5V  
- GND → GND  
- SCL → GPIO 22  
- SDA → GPIO 21  

**LEDs**  
- Amarelo → GPIO 2  
- Verde → GPIO 4  
- Vermelho → GPIO 5  

**Buzzer**  
- Sinal → GPIO 25  
- GND → GND  

---

## 🧪 Possíveis Aprimoramentos

- Envio de alertas para Telegram  
- Dashboard em tempo real no Node-RED  
- Integração com Home Assistant  
- Controle automático de ventilação/refrigeração  
- Salvamento das leituras em banco de dados  

---

## 📝 Licença

Este projeto é aberto para uso acadêmico, testes e estudos.
