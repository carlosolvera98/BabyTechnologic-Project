
//---------    Librerías   ------------//
#include <FirebaseESP32.h>
#include <FirebaseESP32HTTPClient.h>
#include <FirebaseJson.h>
#include <jsmn.h>
#include <analogWrite.h>
#include <Servo_ESP32.h>
#include <DHT.h>   

// ------------- Conexión a base de datos ------------------------//
// Definimos la direccion host de nuestro servidor
#define FIREBASE_HOST "lightbulb-eaabd.firebaseio.com"
// Definimos el token de nuestro servidor
#define FIREBASE_AUTH "Q4LjMNOvvioKOV1sORvlezyiInJI80GnVIAolwQ9"
// Definimos la conexion a punto de acceso wifi
#define WIFI_SSID "ARRIS-3682"
#define WIFI_PASSWORD "E34B71FE1CDC946E"
WiFiClient client;
FirebaseData firebaseData;
FirebaseData firebaseData2;
FirebaseData firebaseData3;
FirebaseData firebaseData4;
//FirebaseData firebaseData5;

//----------- Definiciones -------------//
//Pin para el led
#define LED 2

//Pin para el SERVO1
static const int servoPin = 14;
Servo_ESP32 SERVO1;

//Pin para el SERVO2
static const int servoPin2 = 15;
Servo_ESP32 SERVO2;

int freq = 2000;
int channel = 0;
int resolution = 8;

#define DHTPIN 33
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE); 

//------------ Funciones ---------------//
void setup() {

  // Indico conexión
  Serial.begin(9600);
  WiFi.begin (WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println ("");
  Serial.println ("Se conectó al wifi!");
  Serial.println(WiFi.localIP());
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH); 

  dht.begin();  
}

void loop() {
  mov(); 
  ilum();
  mov2();
  temp();
  sound();
  sound2();
  
  /*Firebase.readStream(firebaseData4);    
  if (firebaseData4.streamAvailable()) 
  {  
      Serial.println(firebaseData4.intData()); // El tipo de dato debe coincidir con el dato cargado en la nube
      delay(50);                                 // El delay es para que solo se haga una lectura, caso contrario repetira la lectura varias veces,
  }*/

}

// ----------- Funciones propias ------------ //
void ilum() {

  pinMode(LED, OUTPUT);
  
  // Código para iluminación
  Firebase.getInt(firebaseData,"/led");
  int dato=firebaseData.intData();
  digitalWrite(LED, dato);
}


void mov() {

  SERVO1.attach(servoPin);
  
  // Código para movimiento de movil
  Firebase.getInt(firebaseData2,"/servo1");
  int dato2=firebaseData2.intData();
  if(dato2 == 1){
    int i;
    for(i=0; i<180; i++){
      SERVO1.write(i);
      delay(10);
    }
    delay(200);
    for(i=180; i>0; i--){
      SERVO1.write(i);
      delay(10);
    }
    delay(200);
  }
  else{
    SERVO1.write(0);
  }
}


void mov2() {

  SERVO2.attach(servoPin2);
  
  // Código para movimiento de cuna
  Firebase.getInt(firebaseData3,"/servo2");
  int dato3=firebaseData3.intData();
  if(dato3 == 1){
    int i;
    for(i=0; i<180; i++){
      SERVO2.write(i);
      delay(3);
    }
    delay(70);
    for(i=180; i>0; i--){
      SERVO2.write(i);
      delay(3);
    }
    delay(70);
  }
  else{
    SERVO2.write(0);
  }
}

void temp() {

  float h = dht.readHumidity();                                              // Reading temperature or humidity takes about 250 milliseconds!
  float t = dht.readTemperature();                                           // Read temperature as Celsius (the default)
    
  if (isnan(h) || isnan(t)) {                                                // Check if any reads failed and exit early (to try again).
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  
  /*String fireHumid = String(h);
  String fireTemp = String(t);    */                                                 //convert integer temperature to string temperature
  delay(4000);
  
  Firebase.setFloat(firebaseData4,"/humedad", h);                                  //setup path and send readings
  Firebase.setFloat(firebaseData4,"/temperatura", t);                                //setup path and send reading

  delay(100);
  
}

void sound(){

  ledcSetup(channel, freq, resolution);
  ledcAttachPin(12, channel);

  Firebase.getFloat(firebaseData,"/temperatura");
  float dator=firebaseData.floatData();


  if(dator >= 32.0)
  {
    ledcWriteTone(channel, 2000);
  
    for (int dutyCycle = 0; dutyCycle <= 150; dutyCycle=dutyCycle+10){
    
      Serial.println(dutyCycle);
    
      ledcWrite(channel, dutyCycle);
      delay(1000);
    }
    
    ledcWrite(channel, 125);
    
    for (int freq = 255; freq < 500; freq = freq + 250){
    
       Serial.println(freq);
    
       ledcWriteTone(channel, freq);
       delay(1000);
    }
  }
  Serial.println("DatoR");
  Serial.println(dator);
}



void sound2() {

  ledcSetup(channel, freq, resolution);
  ledcAttachPin(12, channel);
  
  // Código para movimiento de movil
  Firebase.getInt(firebaseData2,"/sound");
  int datog=firebaseData2.intData();
  
  if(datog == 1){
  
    ledcWriteTone(channel, 2000);
  
      for (int dutyCycle = 0; dutyCycle <= 150; dutyCycle=dutyCycle+10){
      
        Serial.println(dutyCycle);
      
        ledcWrite(channel, dutyCycle);
        delay(1000);
      }
      
      ledcWrite(channel, 125);
      
      for (int freq = 255; freq < 500; freq = freq + 250){
      
         Serial.println(freq);
      
         ledcWriteTone(channel, freq);
         delay(1000);
      }
  }
  else
  {
    ledcWriteTone(channel, 0);
  }
 
}
