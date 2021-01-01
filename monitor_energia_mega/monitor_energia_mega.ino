//Escrit i desenvolupat per Damià Sintes a partir d'una idea d'Anna Acosta a 25-06-2020.

//++++++++Edició a 26-06-2020, s'afeigeix lectura de potència real i alarma de color quan potència >= potència contractada.++++++++++
//++++++++Edició a 24/09/2020, s'afeigeix alarma de colors voltatge i smoothed_average voltatge passa de 20 a 25. +++++++++
//++++++++Edició a 17/12/2020, s'afeigeix sensor Temp, Pressió i humitat. +++++++++
//++++++++Edició a 23/12/2020, s'afeigeix sensor CO2 y TVOC. +++++++
//++++++++Edició a 29/12/2020, s'afeigeix llibreria "arduinojson" per enviar dades a NodeMCU i publicar a servidor a través de WiFi.+++++++++++
#include <EmonLib.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Adafruit_CCS811.h>
#include <Smoothed.h>
#include <ArduinoJson.h>
#define SENSOR_PIN A0,A1;



// ++++++Variables Medidor energia++++++
Smoothed <float> mySensor;
Smoothed <float> mySensor2;
Smoothed <float> mySensor3;
Smoothed <float> mySensor4;
Smoothed <float> mySensor5;
EnergyMonitor emon1;


Adafruit_BME280 bme; // I2C
Adafruit_CCS811 ccs;

StaticJsonBuffer<1000> jsonBuffer;
JsonObject& root = jsonBuffer.createObject();




void setup()
{
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial2.begin(115200);


  bme.begin();
  ccs.begin();

  mySensor.begin(SMOOTHED_AVERAGE, 25);
  mySensor2.begin(SMOOTHED_AVERAGE, 40);
  mySensor3.begin(SMOOTHED_AVERAGE, 40);
  mySensor4.begin(SMOOTHED_AVERAGE, 40);
  mySensor5.begin(SMOOTHED_AVERAGE, 40);

  emon1.voltage(1, 755, 1.7);  // Voltage: (pin, calibrar, canvi de fase) 
  emon1.current(0, 48);       // Current: (pin, calibrar) 
}


void loop()
{
  energia();
  clima();
  aire();
  serialTX();
}

void energia() {

  emon1.calcVI(20, 500);        // Calcula tot. No.de longitud de ones, temps de espera.

  float realPower       = emon1.realPower;        //extreu Potència real a variable
  float apparentPower   = emon1.apparentPower;    //extreu Potència aparent a variable
  float powerFActor     = emon1.powerFactor;      //extreu factor de potència a Variable
  float supplyVoltage   = emon1.Vrms;             //extreu Voltatge a Variable
  float Irms            = emon1.Irms;             //extreu Intensitat a Variable
  int voltatge_per_calcul = 23500;
  
  mySensor.add(supplyVoltage);
  mySensor2.add(realPower);
  mySensor3.add(apparentPower);
  mySensor4.add(powerFActor);
  mySensor5.add(Irms);
  
  
 float voltatge_smoothed = mySensor.get();
  float potencia_real_smoothed = mySensor2.get();
  float potencia_aparent_smoothed = mySensor3.get();
  float factor_potencia_smoothed = mySensor4.get();
  float Irms_smoothed = mySensor5.get();
  int potencia_real = potencia_real_smoothed * (-100);
  int potencia_aparent = potencia_aparent_smoothed * 1000;
  int factor_potencia = factor_potencia_smoothed * (-100);
  int voltatge = voltatge_smoothed * 100;
  int intensitat = Irms_smoothed * 100;
  int irms_waveform = map(Irms_smoothed, 0, 50, 0, 255);
  int potencia_aparent1;
  
  potencia_aparent1 = potencia_aparent_smoothed;



  Serial1.print("x0.val=" + String (voltatge)); //Envia voltatge a pantalla Nextion a n'es requadre x0.
  Serial1.write(0xff);
  Serial1.write(0xff);
  Serial1.write(0xff);

  if ((voltatge <= 22900) && (voltatge >= 24100)) { //Voltatge menor que 230V o major que 241V, canvia a color vermell
    Serial1.print("x0.pco=63488");
    Serial1.write(0xff);
    Serial1.write(0xff);
    Serial1.write(0xff);

  } else {
    Serial1.print("x0.pco=65520"); //Si es voltatge està entre 230 y 240V color de x0 verd.
    Serial1.write(0xff);
    Serial1.write(0xff);
    Serial1.write(0xff);
  }


  Serial1.print("n0.val=" + String (potencia_aparent1)); //Envia wattatge a pantalla Nextion a n'es requadre n0.
  Serial1.write(0xff);
  Serial1.write(0xff);
  Serial1.write(0xff);

  if (potencia_aparent1 >= 3450) {  //Si potència aparent per damunt o igual a 3450watts canvia color n0 a vermell.
    Serial1.print("n0.pco=63488");
    Serial1.write(0xff);
    Serial1.write(0xff);
    Serial1.write(0xff);

  } else {
    Serial1.print("n0.pco=2047"); //Si potència menor q 3400watts color de n0 verd.
    Serial1.write(0xff);
    Serial1.write(0xff);
    Serial1.write(0xff);
  }


  if (intensitat <= 40) { //Si intensitat per devall 0,40A mostra 0.
    intensitat = 0.00;
  }
  Serial1.print("x2.val=" + String (intensitat)); // Envia intensitat a pantalla nextion a requadre x2.
  Serial1.write(0xff);
  Serial1.write(0xff);
  Serial1.write(0xff);

  if (intensitat >= 1468) {          //
    Serial1.print("x2.pco=63488");    //Si intensitat per major o igual 15.5A canvia color de x2 a vermell.
    Serial1.write(0xff);
    Serial1.write(0xff);
    Serial1.write(0xff);

  } else {
    Serial1.print("x2.pco=2047");    //Si intensitat menor que 15.5A color de x2 verd
    Serial1.write(0xff);
    Serial1.write(0xff);
    Serial1.write(0xff);
  }


  Serial1.print("x3.val=" + String (factor_potencia)); // Envia factor de potencia a requadre x3 de pantalla Nextion.
  Serial1.write(0xff);
  Serial1.write(0xff);
  Serial1.write(0xff);

  if (factor_potencia <= 70) {       //
    Serial1.print("x3.pco=63488");    // Si factor de potència per devall de 0.70kVA canvia color x3 a vermell
    Serial1.write(0xff);
    Serial1.write(0xff);
    Serial1.write(0xff);
  } else {
    Serial1.print("x3.pco=65520"); // Per damunt de 0.70kVA color x3 verd
    Serial1.write(0xff);
    Serial1.write(0xff);
    Serial1.write(0xff);

  }
}

void clima() {
  int temp = ((bme.readTemperature() * 100) - 100);
  int pressure = (bme.readPressure() / 10.0F);
  int humitat = (bme.readHumidity() * 100);

  Serial1.print("x4.val=" + String (temp)); // Envia temperatura a requadre x4 de pantalla Nextion.
  Serial1.write(0xff);
  Serial1.write(0xff);
  Serial1.write(0xff);

  Serial1.print("x6.val=" + String (pressure)); // Envia pressió a requadre x6 de pantalla Nextion.
  Serial1.write(0xff);
  Serial1.write(0xff);
  Serial1.write(0xff);

  Serial1.print("x5.val=" + String (humitat)); // Envia humitat a requadre x5 de pantalla Nextion.
  Serial1.write(0xff);
  Serial1.write(0xff);
  Serial1.write(0xff);
}
void aire() {
  if (ccs.available()) {
    if (!ccs.readData()) {
      Serial1.print("n3.val=" + String (ccs.geteCO2()));
      Serial1.write(0xff);
      Serial1.write(0xff);
      Serial1.write(0xff);

      Serial1.print("n1.val=" + String (ccs.getTVOC()));
      Serial1.write(0xff);
      Serial1.write(0xff);
      Serial1.write(0xff);
    }
  }
  delay(0);
}


void serialTX() {

  float p = (emon1.realPower * (-1));

  float a = emon1.apparentPower;

  float f = emon1.powerFactor;

  float v = emon1.Vrms;

  float i = emon1.Irms;

  float t = (bme.readTemperature() - 1);

  float m = (bme.readPressure() / 100.0F);

  float h = bme.readHumidity();

  int g = ccs.geteCO2();

  int o = ccs.getTVOC();


  if (isnan(p) || isnan(a) || isnan(f) || isnan(v) || isnan(i) || isnan(t) || isnan(m) || isnan(h) || isnan(g) || isnan(o)) {
    return;
  }

  root["pow"] = p;
  root["apow"] = a;
  root["pf"] = f;
  root["volt"] = v;
  root["inte"] = i;
  root["temp"] = t;
  root["pres"] = m;
  root["hum"] = h;
  root["gas"] = g;
  root["voc"] = o;


  if (Serial2.available() > 0)
  {
    root.printTo(Serial2);
  }
}
