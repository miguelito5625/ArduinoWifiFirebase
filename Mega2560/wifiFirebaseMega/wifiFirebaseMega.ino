
//FirebaseESP8266.h debe ser incluido antes de ESP8266WiFi.h
#include "FirebaseESP8266.h"
#include <ESP8266WiFi.h>

#define FIREBASE_HOST "dauntless-karma-277202.firebaseio.com" //Without http:// or https:// schemes
#define FIREBASE_AUTH "psZV9bBmVORAk5RsJlK1Fkm7uxX25Q9i4k0yC9s5"
#define WIFI_SSID "Repetidor NXY300"
#define WIFI_PASSWORD "a1a2a3a4a5"

//Difinir FirebaseESP8266 como un objeto de datos
FirebaseData firebaseData;
FirebaseJson json;

String path = "/proyecto/arduino";
int personTemp = 0;
String firebaseEstado = "espera";
//String dataSerial = "";

void obtenerEstado();
void cambiarEstado(String estado);
void mandarTemperatura(String serialData);
void leerYMandarEstado();
void serialDisponible();


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

  obtenerEstado();
  leerYMandarEstado();
  serialDisponible();

}

void serialDisponible() {
  if (Serial.available() > 0) // Si el puerto serial esta disponible
  {
    delay(100);
    String serialData;
    serialData = Serial.readString();
    serialData.trim();
    mandarTemperatura(serialData);
    comprobacionTalanqueras(serialData);
  }
}



void obtenerEstado() {
  delay(1000);
  if (Firebase.getString(firebaseData, path + "/estado"))
  {
    //Serial.println("firebase estado: " + firebaseData.stringData());
    firebaseEstado = firebaseData.stringData();
  }
  else
  {
    //Serial.println("Error Firebase: " + firebaseData.errorReason());
    //Serial.print("error");
  }

}

void cambiarEstado(String estado) {
  firebaseEstado = estado;
  if (Firebase.setString(firebaseData, path + "/estado", estado))
  {
    //Serial.println("firebase cambio estado: " + firebaseData.stringData());
  }
  else
  {
    //Serial.print("error cambiar estado firebase");
  }
}


void leerYMandarEstado() {
  if (firebaseEstado == "pedTemp") {
    Serial.println(firebaseEstado);
  } else if (firebaseEstado == "eleEnt") {
    Serial.println(firebaseEstado);
  } else if (firebaseEstado == "bajEnt") {
    Serial.println(firebaseEstado);
  } else if (firebaseEstado == "eleSal") {
    Serial.println(firebaseEstado);
  } else if (firebaseEstado == "bajSal") {
    Serial.println(firebaseEstado);
  }
}

//Metodo que obtiene la temperatura enviada por el sensor y actualiza la variable temperatura en firebase
void mandarTemperatura(String serialData) {
  String dato = serialData.substring(0, 10);

  if (dato == "personTemp") {
    String personTemp =  serialData.substring(11, 13);
    if (Firebase.setInt(firebaseData, path + "/temperatura", personTemp.toInt()))
    {
      //Serial.println("firebase cambio temperatura: " + firebaseData.stringData());
      cambiarEstado("espera");
    }
    else
    {
      //Serial.print("error cambiar temperatura firebase");
    }
  }
}


//Metodo para actualizar en firebase la variable de estado
void comprobacionTalanqueras(String serialData) {
  String datos = serialData.substring(0,6);
  if (datos == "eleEnt" || datos == "bajEnt" || datos == "eleSal" || datos == "bajSal") {
    cambiarEstado("espera");
  } 
}

/*
 
//Metodo para actualizar en firebase que la talanquera de entrada fue bajada
void entrabaBajada(String serialData) {
  if (serialData == "bajEnt") {
    cambiarEstado("espera");
  } 
}

//Metodo para actualizar en firebase que la talanquera de salida fue elevada
void salidaElevada(String serialData) {
  if (serialData == "eleSal") {
    cambiarEstado("espera");
  } 
}

//Metodo para actualizar en firebase que la talanquera de entrada fue bajada
void salidaBajada(String serialData) {
  if (serialData == "bajSal") {
    cambiarEstado("espera");
  } 
}

*/
