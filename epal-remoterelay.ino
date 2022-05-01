#include <WiFi.h>
#include "params.h"
 
WiFiServer server(80); // Δημιουργία web server στην πόρτα 80

String info = "";

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  pinMode(RELAY, OUTPUT);

  if (!WiFi.config(LOCAL_IP, GATEWAY, SUBNET, PRIDNS)) {
    Serial.println("Παρουσιάστηκε σφάλμα με τις ρυθμίσεις δικτύου.");
  }
  
  Serial.println();
  Serial.print("Σύνδεση στο WiFi ");
  Serial.println(WIFI_SSID);
   
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
   
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Συνδέθηκε στο WiFi.");
   
  server.begin();
}
 
void loop() {
  // Σε περίπτωση αποσύνδεσης από το WiFi, προσπάθειας επανασύνδεσης σε 15 δευτερόλεπτα
  if ((WiFi.status() != WL_CONNECTED) && (millis() > 15000)) {
    Serial.println("Σύνδεσηση στο WiFi...");
    WiFi.disconnect();
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  } 
  
  // Έλεγχος σύνδεσης της εφαρμογής
  WiFiClient client = server.available();
  if (!client) {
    return;   // Αν δεν έχει συνδεθεί η εφαρμογή δεν εκτελείται ο κώδικας που βρίσκεται παρακάτω
  }
   
  Serial.print("Σύνδεσης της εφαρμογής από τη διεύθυνση: ");
  Serial.println(client.remoteIP());
   
  String req = client.readStringUntil('\r');  // Ανάγνωση της εισόδου που έστειλε η εφαρμογή
  Serial.println(req);

  // Ανάλογα με την είσοδο εκτελείται η αντίστοιχη ενέργεια
  if (req.indexOf("ledon") != -1) {
    digitalWrite(, HIGH);
    info = "LED ON";
  }
  if (req.indexOf("ledoff") != -1) {
    digitalWrite(LED, LOW);
    info = "LED OFF";
  }
  if (req.indexOf("relayon") != -1) { 
    digitalWrite(RELAY, HIGH);
    info = "RELAY ON";
  }
  if (req.indexOf("relayoff") != -1){
    digitalWrite(RELAY, LOW);
    info = "RELAY OFF";
  }
           
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("");
  client.println(info);

  client.flush();
  client.stop();
  Serial.println("Αποσύνδεση της εφαρμογής.");
}
