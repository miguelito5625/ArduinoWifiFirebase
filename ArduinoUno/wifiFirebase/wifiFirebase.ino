
// Incluimos la librerias que vamos a utilizar
//FirebaseESP8266.h debe ser incluido antes de ESP8266WiFi.h
#include "FirebaseESP8266.h"
#include <ESP8266WiFi.h>

// Definimos las variables para conectarnos al wifi y a la base de datos de firebase
#define FIREBASE_HOST "dauntless-karma-277202.firebaseio.com"
#define FIREBASE_AUTH "psZV9bBmVORAk5RsJlK1Fkm7uxX25Q9i4k0yC9s5"
#define WIFI_SSID "Repetidor NXY300"
#define WIFI_PASSWORD "a1a2a3a4a5"

//Difinir FirebaseESP8266 como un objeto de datos
FirebaseData firebaseData;

FirebaseJson json;

String path = "/proyecto/arduino";

void imprimirCantidadPersonas();


void setup()
{

  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

  //Establecer el tamaño de los búferes WiFi rx / tx en el caso en el que queramos trabajar con grandes datos.
  firebaseData.setBSSLBufferSize(1024, 1024);

  //Establecer el tamaño de los búferes de respuesta HTTP en el caso en que queramos trabajar con datos grandes.
  firebaseData.setResponseSize(1024);

  //Establezca el tiempo de espera de lectura de la base de datos en 1 minuto (máximo 15 minutos)
  Firebase.setReadTimeout(firebaseData, 1000 * 60);
  //diminuto, pequeño, mediano, grande e ilimitado.
  //Tamaño y su tiempo de espera de escritura, p. Ej. tiny (1s), small (10s), medium (30s) and large (60s).
  Firebase.setwriteSizeLimit(firebaseData, "tiny");

  //opcional, configure los lugares decimales para que los datos flotantes y dobles se almacenen en la base de datos
  Firebase.setFloatDigits(2);
  Firebase.setDoubleDigits(6);

}



void loop()
{

  imprimirCantidadPersonas(); // Obtener cantidad de personas
  delay(1500);

}

void imprimirCantidadPersonas() {

  if (Firebase.getInt(firebaseData, path + "/cantidadPersonas"))
  {
    Serial.print(firebaseData.intData());
  }
  else
  {
    //Serial.println("Error Firebase: " + firebaseData.errorReason());
    Serial.print("error");
  }

}
