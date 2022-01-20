#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <LiquidCrystal_I2C.h>
#include <PZEM004Tv30.h>
#include<Wire.h>
#define SDA SCL


LiquidCrystal_I2C lcd (0x27, 16, 2);
PZEM004Tv30 pzem(D5, D6); // Software Serial pin 11 (RX) & 12 (TX)
const char* ssid = "BAGOES";
const char* pass = "bagoesaja123";
const char* host = "192.168.1.3";

float harga = 1352;
float biaya;
//Inisialisasi variabel host untuk IP Address server



void setup() {
   Serial.begin(9600);
   Wire.begin(D1, D2);
   //isialisasi host (nodemcu)
   WiFi.hostname("NodeMCU");
   WiFi.begin(ssid, pass);
   lcd.begin();
   Serial.begin(9600);
   delay(10);
   Serial.println("Connecting to ");
   Serial.println(ssid);   
   while (WiFi.status() != WL_CONNECTED)
   {
     Serial.print(".");
     delay(500);
    }
   Serial.println("");
   Serial.println("WiFi connected");
   
}

void loop() {
   float voltage = pzem.voltage();
   if(voltage != NAN){
       Serial.print("Voltage: ");
       Serial.print(voltage,2);
       Serial.println("V");
       lcd.setCursor(0,0);
       lcd.print("V: ");
       lcd.print(voltage);
       delay(500);
    
   } else {
       Serial.println("Error reading voltage");
   }

   float current = pzem.current();
   if(current != NAN){
       Serial.print("Current: ");
       Serial.print(current);
       Serial.println("A");
       lcd.setCursor(0,1);
       lcd.print("I: ");
       lcd.print(current);
       delay(500);
       
   } else {
       Serial.println("Error reading current");
   }

   float power = pzem.power();
   if(current != NAN){
       Serial.print("Power: ");
       Serial.print(power);
       Serial.println("W");
       lcd.setCursor(9,0);
       lcd.print("P: ");
       lcd.print(power);
       delay(500);
       
   } else {
       Serial.println("Error reading power");
   }

   float energy = pzem.energy();
   if(current != NAN){
       Serial.print("Energy: ");
       Serial.print(energy,3);
       Serial.println("kWh");
       lcd.setCursor(9,1);
       lcd.print("E: ");
       lcd.print(energy);
       delay(500);
      
   } else {
       Serial.println("Error reading energy");
   }

   delay(10000);
   lcd.clear();
   

   if(biaya != NAN){
   Serial.print("Biaya:Rp. ");
   biaya = energy*harga;
   Serial.print(biaya);
   lcd.setCursor(0,0);
   lcd.print("Rp: ");
   lcd.print(biaya);
   delay(10000);
   lcd.clear();
      } else {
       Serial.println("Error reading biaya");
   }

Serial.println("==================================");
//   float frequency = pzem.frequency();
//   if(current != NAN){
//       Serial.print("Frequency: ");
//       Serial.print(frequency, 1);
//       Serial.println("Hz");
//   } else {
//       Serial.println("Error reading frequency");
//   }
//
//   float pf = pzem.pf();
//   if(current != NAN){
//       Serial.print("PF: ");
//       Serial.println(pf);
//   } else {
//       Serial.println("Error reading power factor");
//   }

    //kirim data ke database
   WiFiClient client;
//   const int httpPort = 80;
   if (!client.connect(host, 80))
   {
    Serial.println("Gagal koneksi ke web Server");
      return;
    }

    //apabila terkoneksi ke web server, maka kirim data
    String Link;
    HTTPClient http;
    Link = "http://192.168.1.3/web/kirimdata.php?tegangan=" + String(voltage);
    //eksekusi Link
    http.begin(client, "http://192.168.1.3/web/kirimdata.php?tegangan=" + String(voltage));
    //mode GET
    http.GET();
    http.end();


    
//    HTTPClient http;
//    //siapkan variabel Link URL untuk kirim data
//    String Link ="http://" + String(server) + "/web/kirimdata.php?voltage=" + String(voltage); //+"&current=" + String(current);
//    //eksekusi link url
//    http.begin(client, "http://");
//    http.GET();
//    //tangkap respon kirim data
//    String respon = http.getString();
//    Serial.println(respon);
   
   Serial.println();
   delay(2000);
}
