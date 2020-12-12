
#include <Wire.h>
#include <LiquidCrystal_I2C.h> // Libreria para LCD

LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2);

int cantidadPersonas = 0;
bool cargandoWifi = true;

#define luzVerde 12
#define luzRoja 11


void setup()
{
  Serial.begin(115200);

  pinMode(luzVerde, OUTPUT);
  pinMode(luzRoja, OUTPUT);

  // Inicializar LCD:
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Inicializando...");

}


void loop()
{


  if (Serial.available() > 0) // Si el puerto serial esta disponible
  {
    delay(100);
    //Serial.println("Datos recividos");
    String dataSerial = Serial.readString();
    Serial.println("esp01 dice: " + dataSerial);

    //No se pudo obtener los datos de firebase
    if (dataSerial == "error") {
      return;
    }

    cargandoWifi = false;
    cantidadPersonas = dataSerial.toInt();
    Serial.println("numero personas: " + cantidadPersonas);
    controlLuces();
    pantallaLcd();
  }



}

void controlLuces() {
  if (cantidadPersonas >= 30) {
    digitalWrite(luzVerde, LOW); // enciende
    digitalWrite(luzRoja, HIGH); // enciende
  } else {
    digitalWrite(luzVerde, HIGH); // enciende
    digitalWrite(luzRoja, LOW); // enciende
  }
}

void pantallaLcd() {

  if (cargandoWifi) {
    //    lcd.setCursor(0, 0);
    //    lcd.print("Inicializando...");
  } else {
    if (cantidadPersonas >= 30) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Personas: " + (String)cantidadPersonas);
      lcd.setCursor(0, 1);
      lcd.print("Cerrado");
    } else {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Personas: " + (String)cantidadPersonas);
      lcd.setCursor(0, 1);
      lcd.print("Abierto");
    }

  }


}

//String dividirCadena(String data, char separator, int index)
//{
//  int found = 0;
//  int strIndex[] = { 0, -1 };
//  int maxIndex = data.length() - 1;
//
//  for (int i = 0; i <= maxIndex && found <= index; i++) {
//    if (data.charAt(i) == separator || i == maxIndex) {
//      found++;
//      strIndex[0] = strIndex[1] + 1;
//      strIndex[1] = (i == maxIndex) ? i + 1 : i;
//    }
//  }
//  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
//}
