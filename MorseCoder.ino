#include <queue>
#include <WiFi.h>
#include <WebServer.h>

std::queue<char> letters;
const char* ssid = "TP-LINK_EXT";
const char* psw = "Ismail@2008";
unsigned long start = 0;
int b = 0;
bool c = 0;


WebServer server(80);
void handleRoot() {
   // Simple HTML page with input form
  server.send(200, "text/html", R"rawliteral(
    <form action="/send" method="GET">
      <input type="text" name="msg">
      <input type="submit" value="Send">
    </form>
  )rawliteral");
}
void handleSend() {
 if (server.hasArg("msg")) {
    //letters.push(server.arg("msg"));//
    for (unsigned int i = 0; i < server.arg("msg").length(); i++) {
    letters.push(server.arg("msg")[i]);  // Push each character into the queue
    }
  }
  server.sendHeader("Location", "/"); // tells browser to go to "/"
  server.send(302, "text/plain", ""); // 302 = temporary redirect
}

int a[26][4] = {
  {1, 2, 0 ,0},
  {2, 1, 1, 1},
  {2, 1, 2, 1},
  {2, 1, 1, 0},
  {1, 0, 0, 0},
  {1, 1, 2, 1},
  {2, 2, 1, 0},
  {1, 1, 1, 1},
  {1, 1, 0, 0},
  {1, 2, 2, 2},
  {2, 1 ,2, 0},
  {1, 2, 1, 1},
  {2, 2, 0, 0},
  {2, 1, 0, 0},
  {2, 2, 2, 0},
  {1, 2, 2, 1},
  {2, 2, 1, 2},
  {1, 2, 1, 0},
  {1, 1, 1, 0},
  {2, 0, 0, 0},
  {1, 1, 2, 0},
  {1, 1, 1, 2},
  {1, 2, 2, 0},
  {2, 1, 1, 2},
  {2, 1, 2, 2},
  {2, 2, 1, 1}
};

#define now millis()
void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, psw);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  pinMode(2, OUTPUT);
  pinMode(23, OUTPUT);
  pinMode(22, OUTPUT);
  Serial.println(WiFi.localIP());

   server.on("/", handleRoot);
  server.on("/send", handleSend);

  server.begin();
}
char letter = ' ';
int i = 4;
int ls = 0;
void loop() {
  //unsigned long now = millis();
  server.handleClient();
  if((!letters.empty() && !b) || (letters.empty() && i < 4 && !b)) {
    if(i == 4) {
      if(c) {
        start = now;
        c = 0;
      }
      if(now - start >= 800) { 
        i = 0;
        letter = letters.front();
        letters.pop();
        c = 1;
      }
    }
    if(letter != ' ' && i < 4) {
      int key = a[int(letter - 'A')][i];
      if(key == 1) {
        digitalWrite(2, HIGH);
        digitalWrite(22, HIGH);
        b = 1;
        start = now;
      }
      else if(key == 2) {
        digitalWrite(22, HIGH);
        digitalWrite(23,HIGH);
        b = 2;
        start = now;
      }
      else {
        b = 4;
        start = now;
      }

    } 
  }
  if(b == 4 && now - start >= 300) {
    i++;
    b = 0;
  }
  if(b == 1 && now - start >= 300) {
    digitalWrite(2, LOW);
     digitalWrite(22, LOW);
     if(now - start >= 600) {
      i++;
      b = 0;
    }
  }
  if(b == 2 && now - start >= 800) { 
     digitalWrite(22, LOW);
    digitalWrite(23, LOW);
    if(now - start >= 1100) {
      i++;
      b = 0;
    }
  } 
}
