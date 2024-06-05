#include <Servo.h>
#include <ESP8266WiFi.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>


// Replace with your network credentials
const char* ssid = "SSID-NAME";
const char* password = "HOTSPOT-PASSWORD";

const long botRequestDelay = 1000; // delay between bot requests in milliseconds
unsigned long lastTimeBotRan; // last time the bot ran

// Initialize Telegram BOT
#define BOTtoken "TOKEN-BOT"

// Initialize Telegram Chat ID
#define CHAT_ID "CHAT-ID"
Servo s1;

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

// Set up function
void setup() {
  Serial.begin(9600);
  s1.attach(0);
  WiFi.begin(ssid, password);

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");

  client.setInsecure();
}

// Loop function
void loop() {
  // Check for new messages every 1 second
  if (millis() - lastTimeBotRan > botRequestDelay) {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while (numNewMessages) {
      Serial.println("handleNewMessages");
      Serial.println(String(numNewMessages));

      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }

    lastTimeBotRan = millis();
  }
}

bool welcomeSent = false;

// Handle new messages
void handleNewMessages(int numNewMessages) {
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));

  for (int i = 0; i < numNewMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);
    String text = bot.messages[i].text;

    if (chat_id != CHAT_ID) {
      bot.sendMessage(chat_id, "Unauthorized user", "");
      continue;
    }

    if (text == "/start" && !welcomeSent) {
      String welcome = "Welcome, " + bot.messages[i].from_name + " to Project:ANUS.\nAuto Nutrition System\n";
      welcome += "Gunakan /feed untuk memberi makan!";
      bot.sendMessage(CHAT_ID, welcome, "");
      welcomeSent = true;
    }

    if (text == "/feed") {
      s1.write(0);  
      delay(1000);  
      s1.write(180);  
      delay(1000);  
      s1.write(0);  
      delay(1000);  
      Serial.println("Servo moved");
      bot.sendMessage(CHAT_ID, "Berhasil!", "");
    }
  }
}
