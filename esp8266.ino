#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>

#define STASSID "TP-LINK_XXXX"  // 你的wifi名字
#define STAPSK  "********"      // 你的wifi密码

#define STASSID2 "TP-LINK_XXXX"  // 你的备用wifi名字
#define STAPSK2  "********"      // 你的备用wifi密码

char* ssid = STASSID;
char* password = STAPSK;

// https://arduino-esp8266.readthedocs.io/en/latest/

// 板子的web服务器，可以通过板子的ip地址访问相应的url快速控制和测试功能，这里去除了http示例中的mDNS部分，因为不知道什么原因导致加上就无法正常运行，所以只能以ip的形式访问
ESP8266WebServer server(80);

const uint16_t kIrLed = 4;  // 红外发射引脚 ESP8266 GPIO pin to use. Recommended: 4 (D2).
IRsend irsend(kIrLed);


void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void drawGraph() {
  String out = "";
  char temp[100];
  out += "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" width=\"400\" height=\"150\">\n";
  out += "<rect width=\"400\" height=\"150\" fill=\"rgb(250, 230, 210)\" stroke-width=\"1\" stroke=\"rgb(0, 0, 0)\" />\n";
  out += "<g stroke=\"black\">\n";
  int y = rand() % 130;
  for (int x = 10; x < 390; x += 10) {
    int y2 = rand() % 130;
    sprintf(temp, "<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" stroke-width=\"1\" />\n", x, 140 - y, x + 10, 140 - y2);
    out += temp;
    y = y2;
  }
  out += "</g>\n</svg>\n";
  server.send(200, "image/svg+xml", out);
}

// 遥控器dump下来的代码
void irOn(void) {
  uint16_t rawDataOn2[227] = {3800, 1488,  516, 1260,  516, 1260,  516, 526,  516, 528,  514, 526,  516, 1260,  516, 526,  516, 526,  516, 1260,  516, 1260,  514, 526,  516, 1260,  516, 528,  514, 526,  516, 1260,  516, 1260,  516, 526,  516, 1260,  516, 1260,  516, 526,  516, 526,  516, 1260,  516, 526,  516, 526,  516, 1260,  516, 526,  516, 526,  516, 526,  516, 526,  516, 526,  516, 526,  514, 526,  516, 526,  516, 526,  516, 526,  516, 526,  516, 524,  516, 526,  516, 526,  516, 526,  516, 526,  516, 526,  516, 1260,  516, 524,  516, 526,  514, 1260,  516, 524,  516, 526,  514, 1260,  516, 1260,  514, 528,  514, 526,  514, 526,  516, 524,  516, 524,  516, 526,  516, 1260,  516, 526,  516, 1258,  516, 526,  516, 526,  516, 526,  516, 526,  516, 526,  516, 526,  516, 1258,  516, 526,  516, 1258,  516, 526,  516, 526,  516, 526,  516, 526,  514, 524,  516, 526,  514, 526,  514, 526,  516, 526,  516, 526,  516, 526,  514, 526,  516, 526,  514, 526,  516, 526,  516, 524,  516, 526,  514, 526,  516, 526,  516, 526,  514, 1258,  516, 526,  514, 526,  514, 526,  516, 526,  516, 524,  516, 526,  516, 526,  516, 524,  516, 524,  516, 1258,  516, 526,  516, 526,  516, 526,  516, 526,  514, 526,  516, 526,  516, 526,  516, 524,  516, 526,  514, 1260,  514, 526,  516, 1258,  516, 526,  516};  // UNKNOWN 5795BA90
  irsend.sendRaw(rawDataOn2, 227, 38); // Send a raw data capture at 38kHz.
}

void irOff(void) {
  uint16_t rawDataOff2[227] = {3804, 1464,  518, 1258,  516, 1258,  518, 524,  518, 522,  518, 524,  516, 1258,  516, 526,  516, 524,  518, 1256,  516, 1260,  516, 526,  516, 1258,  516, 524,  518, 524,  518, 1258,  516, 1258,  516, 526,  516, 1258,  516, 1258,  516, 524,  516, 524,  518, 1258,  516, 524,  516, 524,  516, 1258,  516, 526,  516, 524,  518, 524,  516, 524,  516, 524,  516, 526,  516, 524,  516, 524,  518, 522,  518, 524,  516, 524,  518, 524,  518, 524,  518, 524,  518, 524,  518, 524,  516, 526,  516, 524,  518, 522,  518, 524,  516, 1258,  516, 524,  518, 524,  518, 1258,  516, 1258,  518, 524,  516, 524,  518, 522,  516, 524,  516, 526,  516, 524,  516, 1258,  516, 524,  502, 1272,  518, 526,  516, 524,  516, 524,  518, 524,  518, 524,  516, 524,  518, 1256,  516, 524,  496, 1280,  516, 524,  518, 524,  494, 546,  516, 526,  518, 524,  518, 524,  518, 524,  518, 524,  516, 524,  518, 524,  518, 524,  516, 524,  516, 524,  516, 524,  516, 526,  516, 524,  516, 524,  516, 524,  516, 524,  518, 524,  516, 1258,  516, 524,  518, 524,  518, 524,  516, 524,  518, 524,  518, 524,  518, 524,  516, 526,  516, 524,  520, 1256,  518, 524,  518, 524,  516, 524,  518, 522,  518, 524,  518, 524,  516, 526,  516, 1260,  518, 1258,  518, 524,  516, 524,  518, 1258,  518, 524,  516};  // UNKNOWN EF62489E
  irsend.sendRaw(rawDataOff2, 227, 38); // Send a raw data capture at 38kHz.
}

void clearCmd(const char* id) {
  WiFiClient client;
  HTTPClient http;

  http.setTimeout(1000 * 10);
  String url = "http://hehe.com/esp8266/clearCmd?password=***&id=" + String(id);
  Serial.println("[HTTP] begin clearCmd: " + url);
  if (http.begin(client, url)) {
    Serial.println("[HTTP] GET " + url);
    int httpCode = http.GET();

    // httpCode will be negative on error
    if (httpCode > 0) {
      Serial.printf("[HTTP] GET... code: %d\n", httpCode);

      if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        Serial.println(payload);
      }
    } else {
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }
    http.end();
  } else {
    Serial.printf("[HTTP} Unable to connect\n");
  }
}

void ledOn(void) {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW); // led on
}

void ledOff(void) {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH); // led on
}

void getData(void) {
  WiFiClient client;
  HTTPClient http;

  http.setTimeout(1000 * 10);
  Serial.print("[HTTP] begin...\n");
  if (http.begin(client, "http://hehe.com/esp8266/getCmd")) {
    Serial.print("[HTTP] GET...\n");
    // start connection and send HTTP header
    int httpCode = http.GET();

    // httpCode will be negative on error
    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTP] GET... code: %d\n", httpCode);

      // file found at server
      if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        Serial.println(payload);

        Serial.printf("ARDUINOJSON_VERSION: %s\n", ARDUINOJSON_VERSION); // 6.10.1
        const size_t capacity = JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(5) + 130;
        DynamicJsonDocument doc(capacity);
        deserializeJson(doc, payload);
        Serial.printf("doc.size():%d\n", doc.size());
        if (doc.size() > 0) {
          JsonObject root_0 = doc[0];
          const char*  id = root_0["_id"];
          const char* cmd = root_0["cmd"];
          Serial.println("id:" + String(id) + ",  cmd:" + String(cmd));

          ledOn();
          for (int i = 1; i <= 5; i++) {
            if (strcmp(cmd, "iron") == 0) {
              Serial.println("iron");
              irOn();
              delay(3000);
            } else if (strcmp(cmd, "iroff") == 0) {
              Serial.println("iroff");
              irOff();
              delay(3000);
            }
          } // for
          ledOff();
          clearCmd(id);

        } // if doc.size > 0
      } // http ok
    } else {
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
  } else {
    Serial.printf("[HTTP} Unable to connect\n");
  }
}

void waitWifiConnect(void) {
  for (int i = 0; WiFi.status() != WL_CONNECTED; i++) {
    if (i == 60) {
      Serial.printf("\ntry another ssid\n");
      if (ssid == STASSID) {
        ssid = STASSID2;
        password = STAPSK2;
      } else {
        ssid = STASSID;
        password = STAPSK;
      }
      WiFi.begin(ssid, password);
    }
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void setup(void) {
  irsend.begin();
  Serial.begin(74880);
  Serial.println("");

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  waitWifiConnect();

  ledOn();
  delay(1000);
  ledOff();
  delay(1000);
  ledOn();
  delay(1000);
  ledOff();

  server.on("/", []() {
    unsigned long currentMillis = millis();
    server.send(200, "text/plain", "currentMillis:" + String(currentMillis));
  });

  server.on("/ledon", []() {
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW); // led on
  });

  server.on("/ledoff", []() {
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH); // led off
  });

  server.on("/on", []() {
    irOn();
    server.send(200, "text/plain", "on");
  });

  server.on("/off", []() {
    irOff();
    server.send(200, "text/plain", "off");
  });
  server.on("/test.svg", drawGraph);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
}

// 延时（毫秒），每隔多长时间获取一次命令
#define GET_DATA_TIME (1000*60)
unsigned long preMillis = 0;

void loop(void) {
  server.handleClient();

#if 1
  unsigned long currentMillis = millis();
  if (currentMillis - preMillis > GET_DATA_TIME) {
    preMillis = currentMillis;
    Serial.println(preMillis);
    waitWifiConnect();
    getData();
  }
#endif
}
