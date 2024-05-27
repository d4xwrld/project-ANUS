#include <ESP8266WiFi.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

// Replace with your network credentials
const char* ssid = "Esp8266";
const char* password = "dowhatuwill";

const long botRequestDelay = 1000; // delay between bot requests in milliseconds
unsigned long lastTimeBotRan; // last time the bot ran

// Initialize Telegram BOT
#define BOTtoken "6636575835:AAHh2X5plj7oBfRNLwzyTsnmuLUuyxAgSSM"

// Initialize Telegram Chat ID
#define CHAT_ID "5499264854"

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

// Set up function
void setup() {
  Serial.begin(9600);
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

  for (int i=0; i<numNewMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);

    if (chat_id != CHAT_ID) {
      bot.sendMessage(chat_id, "Unauthorized user", "");
      continue;
    }

    String text = bot.messages[i].text;

    if (text == "/start" && !welcomeSent) {
      String welcome = "Welcome, " + bot.messages[i].from_name + ".\n";
      welcome += "Belum ada CMD!";
      bot.sendMessage(CHAT_ID, welcome, "");
      welcomeSent = true;
    }

    // if (text == "/led_on") {
    //   bot.sendMessage(CHAT_ID, "LED is ON", "");
    // }

    // if (text == "/led_off") {
    //   bot.sendMessage(CHAT_ID, "LED is OFF", "");
    // }
  }
}