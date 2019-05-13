#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>

#ifndef STASSID
#define STASSID "TP-LINK_xxxx"  // 你的wifi名字
#define STAPSK  "12345678"      // 你的wifi密码
#endif

// https://arduino-esp8266.readthedocs.io/en/latest/
const char* ssid = STASSID;
const char* password = STAPSK;

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
  String url = "http://haha.com/esp8266/clearCmd?password=xxx&id=" + String(id);
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

void getData(void) {
  WiFiClient client;
  HTTPClient http;

  http.setTimeout(1000 * 10);
  Serial.print("[HTTP] begin...\n");
  if (http.begin(client, "http://haha.com/esp8266/getCmd")) {
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

          pinMode(LED_BUILTIN, OUTPUT);
          digitalWrite(LED_BUILTIN, LOW); // led on
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
          digitalWrite(LED_BUILTIN, HIGH); // led off
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

void setup(void) {

  irsend.begin();
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", []() {
    server.send(200, "text/plain", "ok");
  });

  server.on("/get", []() {
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

  // 小米手机万能遥控dump下来的代码
  server.on("/on", []() {
    uint16_t rawDataOn[227] = {3706, 1596, 634, 1152, 632, 1144, 634, 422, 632, 422, 632, 396, 654, 1112, 658, 380, 654, 380, 658, 1112, 656, 1146, 634, 420, 632, 1146, 626, 436, 634, 418, 634, 1148, 634, 1154, 634, 422, 634, 1148, 632, 1150, 634, 420, 634, 420, 632, 1148, 634, 418, 634, 422, 634, 1150, 632, 418, 634, 418, 634, 420, 632, 418, 634, 418, 664, 386, 634, 418, 634, 418, 634, 400, 658, 414, 634, 404, 656, 418, 634, 426, 662, 392, 634, 418, 664, 390, 634, 418, 662, 1120, 662, 398, 634, 420, 662, 1084, 630, 404, 628, 402, 628, 1162, 634, 1148, 634, 420, 662, 390, 634, 402, 654, 416, 634, 414, 634, 416, 634, 416, 664, 1114, 636, 1144, 634, 414, 664, 386, 634, 422, 662, 392, 664, 390, 634, 420, 634, 418, 634, 424, 662, 392, 664, 1118, 634, 414, 634, 418, 634, 420, 664, 396, 664, 388, 628, 434, 664, 372, 684, 390, 664, 388, 664, 392, 664, 392, 636, 422, 634, 418, 664, 392, 634, 416, 634, 424, 664, 386, 664, 390, 634, 414, 664, 1086, 688, 1078, 684, 1082, 660, 1138, 662, 392, 664, 388, 664, 390, 634, 424, 662, 394, 664, 364, 658, 1142, 664, 392, 634, 424, 662, 368, 686, 382, 664, 386, 662, 1122, 636, 418, 662, 1120, 662, 392, 664, 394, 634, 1152, 636, 1152, 634, 414, 636}; // UNKNOWN B5A34799
    irsend.sendRaw(rawDataOn, 227, 38); // Send a raw data capture at 38kHz.
    server.send(200, "text/plain", "on");
  });

  server.on("/off", []() {
    uint16_t rawDataOff[227] = {3744, 1502, 686, 1080, 712, 1056, 712, 330, 710, 330, 714, 326, 712, 1058, 712, 328, 714, 324, 688, 1080, 714, 1056, 714, 324, 712, 1060, 710, 332, 712, 326, 688, 1080, 686, 1084, 686, 356, 712, 1058, 686, 1080, 688, 352, 688, 350, 714, 1056, 712, 326, 688, 354, 688, 1082, 686, 350, 688, 350, 688, 352, 684, 352, 686, 352, 714, 326, 688, 352, 712, 328, 686, 348, 684, 350, 630, 406, 684, 352, 686, 350, 686, 356, 688, 356, 686, 352, 686, 354, 688, 352, 688, 350, 686, 352, 688, 1080, 686, 352, 686, 352, 688, 1066, 702, 1082, 712, 328, 686, 354, 688, 352, 688, 354, 686, 352, 684, 354, 686, 354, 686, 1084, 686, 1084, 710, 332, 686, 352, 686, 350, 686, 352, 686, 354, 686, 350, 688, 352, 688, 354, 686, 352, 686, 1086, 688, 352, 688, 352, 686, 354, 688, 350, 686, 352, 686, 354, 688, 350, 684, 354, 686, 356, 682, 352, 682, 352, 682, 352, 682, 354, 686, 352, 688, 352, 686, 352, 686, 350, 686, 352, 686, 352, 688, 1080, 686, 1080, 684, 1082, 686, 1082, 686, 354, 686, 352, 688, 352, 688, 350, 688, 354, 686, 354, 684, 1082, 686, 352, 684, 352, 686, 350, 686, 350, 686, 350, 686, 1082, 682, 354, 684, 350, 686, 350, 686, 352, 688, 1082, 684, 1082, 688, 352, 688};  // UNKNOWN 6081BAF1
    irsend.sendRaw(rawDataOff, 227, 38); // Send a raw data capture at 38kHz.
    server.send(200, "text/plain", "off");
  });


  server.on("/on2", []() {
    irOn();
    server.send(200, "text/plain", "on");
  });

  server.on("/off2", []() {
    irOff();
    server.send(200, "text/plain", "off");
  });
  server.on("/test.svg", drawGraph);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");

}

// 延时（毫秒），每隔多长时间获取一次命令
#define GET_DATA_TIME (1000*60*2)
unsigned long preMillis = 0;

void loop(void) {
  server.handleClient();
#if 1
  unsigned long currentMillis = millis();
  if (currentMillis - preMillis > GET_DATA_TIME) {
    preMillis = currentMillis;
    Serial.println(preMillis);
    getData();
  }
#else
  getData();
  delay(GET_DATA_TIME);
#endif
}
