#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

WiFiClientSecure client;


const char* ssid = "AutomaticSpray";          // Nama SSID WiFi
const char* password = "mabesjunior";  // Kata sandi WiFi

#define BOTtoken "6926413039:AAGqlcZfjGc---rww7ahwlXwvUHkz7z77YI"
#define CHAT_ID "-4183803774"


UniversalTelegramBot bot(BOTtoken, client);

unsigned long previousMillis = 0;
const long botRequestDelay = 1000;
unsigned long lastTimeBotRan;

String pesan1 = "haloooo";
String pesan2 = "nama saya bot";
String pesan3 = "cuaca hari ini bagus";
void setup() {
  Serial.begin(115200);
  connectToWiFi();
  configTime(0, 0, "pool.ntp.org");
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT);
}

void loop() {
  if (millis() > lastTimeBotRan + botRequestDelay) {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    while (numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }
}

void connectToWiFi() {
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
  }
  Serial.println("Connected to WiFi");
}

void sendMessage() {
  // Tidak ada implementasi yang diperlukan di sini, karena mengirim pesan tidak perlu dipanggil di loop utama
}

void handleNewMessages(int numNewMessages) {
  for (int i = 0; i < numNewMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);
    if (chat_id != CHAT_ID) {
      bot.sendMessage(chat_id, "Unauthorized user", "");
      continue;
    }

    String text = bot.messages[i].text;
    Serial.println(text);

    String from_name = bot.messages[i].from_name;

    if (text == "/start") {
      String welcome = "Welcome, " + from_name + ",\n";
      welcome += "Gunakan perintah berikut untuk mendapatkan pesan\n";
      welcome += "/getESP32Value\n";
      welcome += "/quit";
      bot.sendMessage(chat_id, welcome, "");
    }

    if (text == "/getESP32Value") {
      while (1) {
        unsigned long currentMillis = millis();
        if (currentMillis - previousMillis > 1000) {
          bot.sendMessage(chat_id, pesan1, "");
          bot.sendMessage(chat_id, pesan2, "");
          bot.sendMessage(chat_id, pesan3, "");
          previousMillis = currentMillis;
        }
        bot.getUpdates(bot.last_message_received + 1);
        if (bot.messages[i].text == "/quit") {
          Serial.println("Quit dari LOOP");
          bot.sendMessage(chat_id, "Keluar dari notifikasi", "");
          break;
        }
      }
    }
  }
}
