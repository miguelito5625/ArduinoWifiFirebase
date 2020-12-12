
// Incluímos la librería para poder utilizar el sensor de temperatura
#include <Adafruit_MLX90614.h>
// Incluímos la librería para poder controlar el servo
#include <Servo.h>

//Variable global para guardar la informacion que viene desde el Serial1
String dataSerial = "";

// Declaramos la variable para controlar los servos
Servo servoEntrada;
Servo servoSalida;

// Pines del sensor ultrasonico de entrada
const int trigPin1 = 8;
const int echoPin1 = 9;
String estadoEntrada = "espera";

// Pines del sensor ultrasonico de salida
const int trigPin2 = 10;
const int echoPin2 = 11;
String estadoSalida = "espera";

//Inicializacion de libreria para el sensor de temperatura y variable para guardar la temperatura obtenida
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
float personTemp = 0;



void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  //Inicializamos los puertos seriales 0 y 1 a 115200 baudios
  Serial.begin(115200);
  Serial1.begin(115200);

  pinMode(trigPin1, OUTPUT); // Definimos el trigPin1 como  Salida
  pinMode(echoPin1, INPUT); // Definimos el echoPin1 como an Entrada

  pinMode(trigPin2, OUTPUT); // Definimos el trigPin1 como  Salida
  pinMode(echoPin2, INPUT); // Definimos el echoPin1 como an Entrada

  //Iniciaizamos la libreria para el sensor de temperatura
  mlx.begin();
  
  // Iniciamos el servo para que empiece a trabajar con el pin 9
  servoEntrada.attach(2);
  servoSalida.attach(3);
  servoEntrada.write(10);
  servoSalida.write(10);

  Serial.println("Arduino iniciado");

}


void loop()
{

  serialDisponible();
  obtenerYMandarTemperatura();
  controlTalanqueras();
  //delay(1000);

  //  int distancia = distanciaSensorEntrada();
  //  Serial.println("Distancia X: " + (String)distancia);
}

int distanciaSensorEntrada() {
  // Definimos las variables para duracion y distancia
  long duration;
  int distance;

  // Limpiar trigPin1
  digitalWrite(trigPin1, LOW);
  delayMicroseconds(2);
  // poner el trigPin1 en estado HIGH por 10 micro segundos
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);
  // Leer el echoPin1, devuelve el tiempo de la onda de sonido en microsegundos
  duration = pulseIn(echoPin1, HIGH);
  // calcula la distancia
  distance = duration * 0.034 / 2;
  return distance;
}

int distanciaSensorSalida() {
  // Definimos las variables para duracion y distancia
  long duration;
  int distance;

  // Limpiar trigPin2
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(2);
  // poner el trigPin2 en estado HIGH por 10 micro segundos
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);
  // Leer el echoPin2, devuelve el tiempo de la onda de sonido en microsegundos
  duration = pulseIn(echoPin2, HIGH);
  // Calcular la distancia
  distance = duration * 0.034 / 2;
  return distance;
}

//Metodo para ejecutar submetodos solo cuando la comunicacion Serial1 esta disponible
void serialDisponible() {
  if (Serial1.available() > 0) // Si el puerto serial esta disponible
  {
    delay(100);
    dataSerial = Serial1.readString();
    dataSerial.trim();
    Serial.println("Serial1: esp01 XX dice: " + dataSerial);
    
  }
}

void controlTalanqueras() {
  elevarEntrada();
  bajarEntrada();
  elevarSalida();
  bajarSalida();
}

//Metodo para elevar la talanquera de la entrada
void elevarEntrada() {

  if (dataSerial.substring(0,6) == "eleEnt") {
    servoEntrada.write(80);
    Serial1.print("eleEnt");
    dataSerial = "";
  }

}

//Metodo para bajar la talanquera de la entrada
void bajarEntrada() {

  int distancia = distanciaSensorEntrada();
  //Serial.println("distancia Entrada: " + (String)distancia);

  if(distancia < 15 && estadoEntrada == "espera"){
    estadoEntrada = "pasando";
    delay(1500);
  }else if(distancia > 15 && estadoEntrada == "pasando"){
    delay(2500);
    servoEntrada.write(10);
    estadoEntrada = "espera";
  }
  
  if (dataSerial.substring(0,6) == "bajEnt") {
    servoEntrada.write(10);
    Serial1.print("bajEnt");
    dataSerial = "";
  }

}

//Emtodo para elevar la talanquera de la salida
void elevarSalida() {

  if (dataSerial.substring(0,6) == "eleSal") {
    servoSalida.write(80);
    Serial1.print("eleSal");
    dataSerial = "";
  }

}

//Metodo para bajar la talanquera de la salida
void bajarSalida() {

  int distancia = distanciaSensorSalida();
  //Serial.println("distancia Salida: " + (String)distancia);

  if(distancia < 15 && estadoSalida == "espera"){
    estadoSalida = "pasando";
    delay(1500);
  }else if(distancia > 15 && estadoSalida == "pasando"){
    delay(2500);
    servoSalida.write(10);
    estadoSalida = "espera";
  }

  if (dataSerial.substring(0,6) == "bajSal") {
    servoSalida.write(10);
    Serial1.print("bajSal");
    dataSerial = "";
  }

}



//Metodo para enviar al modulo wifi la temperatura obtenida desde el sensor
void obtenerYMandarTemperatura() {
  if (dataSerial.substring(0,7) == "pedTemp") {
    personTemp = mlx.readObjectTempC() + 3;
    delay(100);
    Serial1.print("personTemp:" + (String)personTemp);
    dataSerial = "";
  }
}
