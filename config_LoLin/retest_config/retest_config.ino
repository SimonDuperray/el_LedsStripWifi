#include <ESP8266WiFi.h>
#include <Adafruit_NeoPixel.h>

const char* ssid = "Bbox-3B2C6180";
const char* password = "A972453E546E7C31A67CD353454C63";

#define PIN 4

WiFiServer server(80);

Adafruit_NeoPixel strip = Adafruit_NeoPixel(37, PIN, NEO_GRB + NEO_KHZ800);

void setup(){

  strip.begin();
  connecter();
  strip.show();
  
  // initialisation de la communication série
  Serial.begin(115200);
  delay(100);

  // connexion wifi
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  // connection en cours...
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }

  // WiFi connecté
  Serial.println("WiFi conneté");

  // Démarrage du serveur
  server.begin();
  Serial.println("Serveur démarré");

  // Affichage de l'adresse IP
  Serial.print("Utiliser cette adresse URL pour la connexion: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("\n");
}

void connecter(){
  for(int i=37; i>=0; i--){
    strip.setPixelColor(i, 0, 255, 0);
    strip.show();
  }
  for(int i=0; i<38; i++){
    strip.setPixelColor(i, 0, 0, 0);
    strip.show();
  }
}

void allumer(){
  for(int i=0; i<38; i++){
    strip.setPixelColor(i, 255, 255, 255);
    strip.show();
  }
}

void eteindre(){
  for(int i=0; i<38; i++){
    strip.setPixelColor(i, 0, 0, 0);
    strip.show();
  }
}

void rouge(){
  for(int i=0; i<38; i++){
    strip.setPixelColor(i, 255, 0, 0);
    strip.show();
  }
}

void vert(){
  for(int i=0; i<38; i++){
    strip.setPixelColor(i, 0, 255, 0);
    strip.show();
  }
}

void bleu(){
  for(int i=0; i<38; i++){
    strip.setPixelColor(i, 0, 0, 255);
    strip.show();
  }
}

void notif(){
  for(int i=0; i<5; i++){
    for(int j=0; j<38; j++){
      strip.setPixelColor(j, 0, 0, 255);
      delay(10);
      strip.show();
    }
    for(int k=37; k>=0; k--){
      strip.setPixelColor(k, 0, 0, 0);
      delay(10);
      strip.show();
    }
  }
}

void pale(){
  for(int i=0; i<38; i++){
    strip.setPixelColor(i, 218, 165, 32);
    strip.show();
  }
}

void loop() {  
  WiFiClient client;

  // Vérification si le client est connecté
  client = server.available();
  if(!client){
    return;
  }

  // Attendre si le client envoie des données...
  Serial.println("Nouveau Client");
  while(!client.available()){
    delay(1);
  }
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

  int value = LOW;
  if(request.indexOf("LED=ON") != -1){
    allumer();
    value=HIGH;
  }
  if(request.indexOf("LED=OFF") != -1){
    eteindre();
    value=LOW;
  }
  if(request.indexOf("LED=ROUGE") != -1){
    rouge();
    value=LOW;
  }
  if(request.indexOf("LED=BLEU") != -1){
    bleu();
    value=LOW;
  }
  if(request.indexOf("LED=VERT") != -1){
    vert();
    value=LOW;
  }
  if(request.indexOf("LED=NOTIF") != -1){
    notif();
    value=LOW;
  }
  if(request.indexOf("LED=PALE") != -1){
    pale();
    value=HIGH;
  }
  
  // réponse
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("");
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");

  client.print("Etat de la led: ");

  if(value==HIGH){
    client.print("On");
  } else {
    client.print("Off");
  }
  client.println("<br><br>");
  client.println("<a href=\"/LED=ON\"\"><button>Allumer </button></a>");
  client.println("<a href=\"/LED=OFF\"\"><button>Eteindre </button></a><br />");
  client.println("<a href=\"/LED=ROUGE\"\"><button>Rouge </button></a>");
  client.println("<a href=\"/LED=BLEU\"\"><button>Bleu </button></a>");
  client.println("<a href=\"/LED=VERT\"\"><button>Vert </button></a>");
  client.println("<a href=\"/LED=PALE\"\"><button>Pale </button></a>");
  client.println("<a href=\"/LED=NOTIF\"\"><button>Notif </button></a>");
  client.println("</html>");

  delay(1);
  Serial.println("Client déconnecté");
  Serial.println("");
}
