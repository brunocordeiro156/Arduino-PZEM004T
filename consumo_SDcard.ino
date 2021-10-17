#include <SPI.h>
#include <SD.h>
#include <PZEM004Tv30.h>


File myFile;
#define PIN_SD_CARD 4
#define W5500_SS 10

/* Use software serial for the PZEM
 * Pin 5 Rx (Connects to the Tx pin on the PZEM)
 * Pin 6 Tx (Connects to the Rx pin on the PZEM)
*/
PZEM004Tv30 pzem(5, 6);
float voltage;
float current;
float power;
float energy;
float frequency;
float pf;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  
  //SD Card
  pinMode(W5500_SS, OUTPUT);
  digitalWrite(W5500_SS, HIGH);
  Serial.print("Initializing SD card...");
  if (SD.begin(PIN_SD_CARD)) {
    Serial.println("initialized SD card.");
  }else{
    Serial.println("initialization failed.");
    return;
  }
}


void loop() {
  voltage = pzem.voltage();
  if( !isnan(voltage) ){
    Serial.print("Voltage: "); Serial.print(voltage); Serial.println("V");
  } else {
    Serial.println("Error reading voltage");
  }
  current = pzem.current();
  if( !isnan(current) ){
    Serial.print("Current: "); Serial.print(current); Serial.println("A");
  } else {
    Serial.println("Error reading current");
  }
  power = pzem.power();
  if( !isnan(power) ){
    Serial.print("Power: "); Serial.print(power); Serial.println("W");
  } else {
    Serial.println("Error reading power");
  }
  energy = pzem.energy();
  if( !isnan(energy) ){
    Serial.print("Energy: "); Serial.print(energy,3); Serial.println("kWh");
  } else {
    Serial.println("Error reading energy");
  }
  frequency = pzem.frequency();
  if( !isnan(frequency) ){
    Serial.print("Frequency: "); Serial.print(frequency, 1); Serial.println("Hz");
  } else {
    Serial.println("Error reading frequency");
  }
  pf = pzem.pf();
  if( !isnan(pf) ){
    Serial.print("PF: "); Serial.println(pf);
  } else {
    Serial.println("Error reading power factor");
  }
  data_sdcard();
  delay(1000);
}

void data_sdcard() {
  String dataString = "";
  dataString = "Voltage;" + String(voltage) + ";" +
               "Current;" + String(current) + ";" +
               "Power;" + String(power) + ";"     +
               "Energy;" + String(energy) + ";"   +
               "Frequency;" + String(frequency) + ";" +
               "PF;" + String(pf) + ";"
                ;
  Serial.println(dataString);
  myFile = SD.open("data.txt", FILE_WRITE); // Cria / Abre arquivo .txt
  if (myFile) { // Se o Arquivo abrir imprime:
    Serial.println("Escrevendo no Arquivo .txt"); // Imprime na tela
    myFile.println(dataString); // Escreve no Arquivo
    myFile.close(); // Fecha o Arquivo após escrever
    Serial.println("Terminado."); // Imprime na tela
  }
 
  else {     // Se o Arquivo não abrir
    Serial.println("Erro ao Abrir Arquivo .txt"); // Imprime na tela
  }
}
