#include <Keypad.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include "HX711.h"
#include <stdio.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
Servo berasbiasa,berasmerah;
//RFID
#define pinSDA 53
#define pinRST 49
 
MFRC522 RFID(pinSDA, pinRST);
String rfid1="481459726";
String rfid2="1138315039";
const int fan = 35;
LiquidCrystal_I2C lcd_besar(0x27, 20,4);
LiquidCrystal_I2C lcd_kecil(0x26, 16,2);
#define DOUT 46
#define CLK 47
int Status;
String JenisKeluar ="";
HX711 scale;
float calibration_factor = 211.3; //Hasil Kalibrasi
float units;
const byte ROWS = 4; 
const byte COLS = 4;
char keys[ROWS][COLS] = {
 {'1','2','3','A'},
 {'4','5','6','B'},
 {'7','8','9','C'},
 {'*','0','#','D'}
};
byte colPins[COLS] = {7, 6, 5, 4};
byte rowPins[ROWS] = {17, 16, 15, 14};
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS 
);
//button
const int buttonkilo = 39;
const int buttonliter = 38;
const int buttonbiasa = 36;
const int buttonmerah = 37;
int red = 13;
int counter = 0; 
int attempts = 0; 
int max_attempts = 3; 
//MODE
String kilo;
String liter;
int hKilo = 0;
int hLiter = 0;
int buzzer = 4;
//MODE
int hkilo,hliter;
//KILO
float cetakliter,cetakkilo;
String mysecs; 
int seconds = 0; 
long int total_seconds = 0; 
int secflag = 0; 
int timer_started_flag = 0; 
//BERAS
String jenis_beras = "";
// Tracks the time since last event fired
unsigned long previousMillis=0;
unsigned long int previoussecs = 0; 
unsigned long int currentsecs = 0; 
unsigned long currentMillis = 0;
int interval= 1 ; // updated every 1 second
int tsecs = 0; 
//ultrasonic 1
int echoPin1 =30;
int initPin1 =31;
int distance1 =0; 
//ultrasonic 2
int echoPin2 =32;
int initPin2 =33;
int distance2 =0;
 void setup(){
 Serial.begin(9600); 
 lcd_besar.init();
 lcd_besar.backlight();
 lcd_kecil.init();
 lcd_kecil.backlight();
 SPI.begin();
 RFID.PCD_Init();
 scale.begin(DOUT, CLK);
 scale.set_scale();
 scale.tare(); //Reset the scale to 0
 long zero_factor = scale.read_average(); //Get a baseline reading
 pinMode(red, OUTPUT);
 pinMode(fan, OUTPUT);
 pinMode(buttonkilo, INPUT_PULLUP);
 pinMode(buttonliter, INPUT_PULLUP);
 pinMode(buttonbiasa, INPUT_PULLUP);
 pinMode(buttonmerah, INPUT_PULLUP);
 pinMode(initPin1, OUTPUT);
 pinMode(echoPin1, INPUT);
 pinMode(buzzer, OUTPUT);
 pinMode(initPin2, OUTPUT);
 pinMode(echoPin2, INPUT); 
 digitalWrite(red, HIGH);
 digitalWrite(fan,LOW);
 berasbiasa.attach(10); //servo 1 
 berasbiasa.write(50);
 berasmerah.attach(11); //servo 2
 berasmerah.write(117);
 lcd_besar.setCursor(0,0);
 lcd_besar.print("Tempelkan Kartu : ");
 lcd_besar.setCursor(0,1);
 lcd_besar.print(".................");
}
void loop(){
 rfid();
 distance1 = getDistance(initPin1, echoPin1);
 distance2 = getDistance(initPin2, echoPin2);
 Serial.print("Merah 1 :");
 Serial.println(distance1);
 Serial.print("Biasa 2 :");
 Serial.println(distance2);
 timbangan();
 int bKilo = digitalRead(buttonkilo);
 int bLiter = digitalRead(buttonliter);
 int bBiasa = digitalRead(buttonbiasa);
 int bMerah = digitalRead(buttonmerah);
 // Serial.print("KILO : " );
 // Serial.println(bKilo);
 // Serial.print("Liter : " );
 // Serial.println(bLiter);
 // Serial.print("Biasa : ");
 // Serial.println(bBiasa);
 // Serial.print("Merah : ");
 // Serial.println(bMerah);
 // Serial.print("Status : ");
 // Serial.println(Status); 
 if(bMerah == 0 && jenis_beras == "" && Status > 0 && distance1 <= 23){
 lcd_besar.clear();
 jenis_beras = "Merah";
 lcd_besar.setCursor(0,0);
 lcd_besar.print("BERAS MERAH");
 delay(1500);
 lcd_besar.clear();
 lcd_besar.setCursor(0,0);
 lcd_besar.print("KELUARAN BERAS");
 lcd_besar.setCursor(0,1);
 lcd_besar.print("KILO ATAU LITER");
 }else if(bMerah == 0 && jenis_beras != "" && Status > 0){
 lcd_besar.clear();
 lcd_besar.setCursor(0,0);
 lcd_besar.print("SUDAH MEMILIH");
 lcd_besar.setCursor(0,1);
 lcd_besar.print(jenis_beras);
 delay(2000);
 lcd_besar.clear();
 }else if(bMerah == 0 && jenis_beras == "" && Status > 0 && distance1 >= 23){
 lcd_besar.clear();
 digitalWrite(buzzer, HIGH);
 delay(100);
 digitalWrite(buzzer, LOW);
 delay(100);
 digitalWrite(buzzer, HIGH);
 delay(100); 
 digitalWrite(buzzer, LOW);
 lcd_besar.setCursor(0,0);
 lcd_besar.print("BERAS HABIS");
 delay(1500);
 lcd_besar.clear();
 lcd_besar.setCursor(0,0);
 lcd_besar.print("PILIH BERAS");
 lcd_besar.setCursor(0,1);
 lcd_besar.print("MERAH ATAU BIASA");
 }
 if(bBiasa == 0 && jenis_beras == "" && Status > 0 && distance2 <= 24){
 lcd_besar.clear();
 jenis_beras = "Biasa";
 lcd_besar.setCursor(0,0);
 lcd_besar.print("BERAS BIASA");
 delay(1500);
 lcd_besar.clear();
 lcd_besar.setCursor(0,0);
 lcd_besar.print("KELUARAN BERAS");
 lcd_besar.setCursor(0,1);
 lcd_besar.print("KILO ATAU LITER");
 }else if(bBiasa == 0 && jenis_beras != "" && Status > 0 ){
 lcd_besar.clear();
 lcd_besar.setCursor(0,0);
 lcd_besar.print("SUDAH MEMILIH");
 lcd_besar.setCursor(0,1);
 lcd_besar.print(jenis_beras);
 delay(2000);
 lcd_besar.clear();
 }else if(bBiasa == 0 && jenis_beras == "" && Status > 0 && distance2 >= 25){
 lcd_besar.clear();
 digitalWrite(buzzer, HIGH);
 delay(100);
 digitalWrite(buzzer, LOW);
 delay(100);
 digitalWrite(buzzer, HIGH);
 delay(100);
 digitalWrite(buzzer, LOW);
 lcd_besar.setCursor(0,0);
 lcd_besar.print("BERAS HABIS");
 delay(1500);
 lcd_besar.clear();
 lcd_besar.setCursor(0,0);
 lcd_besar.print("PILIH BERAS");
 lcd_besar.setCursor(0,1);
 lcd_besar.print("MERAH ATAU BIASA");
 }
 
 if(Status == 0){
 lcd_besar.setCursor(0,0);
 lcd_besar.print("HARAP TEMPELKAN ");
 lcd_besar.setCursor(0,1);
 lcd_besar.print(".................");
 }
 if(Status == 1 && jenis_beras == ""){
 lcd_besar.setCursor(0,0);
 lcd_besar.print("PILIH BERAS");
 lcd_besar.setCursor(0,1);
 lcd_besar.print("MERAH ATAU BIASA");
 }
 if(bKilo == 0 && jenis_beras != ""){
 hkilo += 1;
 }
 if(bLiter == 0 && jenis_beras != ""){
 hliter += 1;
 }
//MODE KILO BERAS
if(hkilo >= 1 && hliter == 0 && jenis_beras != "" && Status > 0){
 JenisKeluar = "KG";
 kilos();
 }
 else if(bKilo == 0 && jenis_beras == ""){
 lcd_besar.setCursor(0,0);
 lcd_besar.print("HARAP PILIH");
 lcd_besar.setCursor(0,1);
 lcd_besar.print("JENIS BERAS");
 delay(2000);
 lcd_besar.clear();
 }
//MODE LITER BERAS
 if(hliter >= 1 && hkilo == 0 && jenis_beras != "" && Status > 0){
 JenisKeluar = "Liter";
 liters();
 }
 else if(bLiter == 0 && jenis_beras == ""){
 lcd_besar.setCursor(0,0);
 lcd_besar.print("HARAP PILIH");
 lcd_besar.setCursor(0,1);
 lcd_besar.print("JENIS BERAS");
 delay(2000);
 lcd_besar.clear();
 }
 cetakberas();
 lcd_kecil.setCursor(11,0);
 lcd_kecil.print(jenis_beras);
 lcd_kecil.setCursor(5,0);
 lcd_kecil.print(JenisKeluar);
}
void rfid(){
 if ( ! RFID.PICC_IsNewCardPresent()) 
 {
 return;
 }
 if ( ! RFID.PICC_ReadCardSerial()) 
 {
 return;
 }
 String idtag="";
 for (byte i = 0; i < RFID.uid.size; i++)
 {
 idtag+=RFID.uid.uidByte[i];
 }
 
 if(idtag==rfid1 || idtag==rfid2 && Status == 0)
 {
 Status += 1;
 lcd_besar.setCursor(0,0);
 lcd_besar.print("PILIH JENIS BERAS ");
 lcd_besar.setCursor(0,1);
 lcd_besar.print("BIASA ATAU MERAH ");
 }else{ 
 digitalWrite(buzzer, HIGH);
 delay(100);
 digitalWrite(buzzer, LOW);
 delay(100);
 digitalWrite(buzzer, HIGH);
 delay(100);
 digitalWrite(buzzer, LOW);
 lcd_besar.setCursor(0,0);
 lcd_besar.print("KARTU TIDAK DIKENAL");
 lcd_besar.setCursor(0,1);
 lcd_besar.print(" SILAHKAN DAFTAR ");
 delay(1500);
 lcd_besar.clear();
 lcd_besar.setCursor(0,0);
 lcd_besar.print(" TEMPELKAN KARTU");
 lcd_besar.setCursor(0,1);
 lcd_besar.print(" YANG TERDAFTAR");
 delay(1500);
 lcd_besar.clear();
 } 
}
void timbangan(){
 scale.set_scale(calibration_factor);
 units = scale.get_units(), 1;
 if (units < 0)
 {
 units = 0.00;
 } 
 if(units <= 2){
 lcd_kecil.setCursor(10,1);
 lcd_kecil.print("0.0 ");
 }else
 if(units >=2){
 lcd_kecil.setCursor(10,1);
 lcd_kecil.print(units);
 }
}
void kilos(){ 
 lcd_besar.setCursor(0,0);
 lcd_besar.print("Banyak beras: ");
 lcd_besar.setCursor(0,1);
 lcd_besar.print(" ");
char key = keypad.getKey();
 if (key){
 counter = counter + 1; 
 int lcds = counter + 2; 
 lcd_besar.setCursor(counter, 2);
 lcd_besar.print(key); 
 lcd_besar.setCursor(lcds, 2);
 lcd_besar.print("KG");
 }
 if (key == '1')
 {
 kilo = kilo + 1; 
 }
 if (key == '2')
 {
 kilo = kilo + 2; 
 }
 if (key == '3')
 {
 kilo = kilo + 3; 
 }
 if (key == '4')
 {
 kilo = kilo + 4; 
 }
 if (key == '5')
 {
 kilo = kilo + 5;
 }
 if (key == '6')
 {
 kilo = kilo + 6; 
 }
 if (key == '7')
 {
 kilo = kilo + 7; 
 }
 if (key == '8')
 {
 kilo = kilo + 8; 
 }
 if (key == '9')
 {
 kilo = kilo + 9;
 } 
 if (key == '0')
 {
 kilo = kilo + 0; 
 }
 if (key == '#')
 {
 key == NULL;
 kilo == NULL;
 counter = 0;
 lcd_besar.clear();
 lcd_besar.print(key);
 }
 if(key == '*'){
 lcd_besar.clear();
 hKilo = kilo.toInt();
 lcd_besar.print("Banyak: "); 
 lcd_besar.setCursor(0,1); 
 lcd_besar.print(hKilo); 
 lcd_besar.setCursor(2,1); 
 lcd_besar.print("KG"); 
 kilo = "";
 cetakkilo = 1000 * hKilo;
 }
}
void liters(){ 
 lcd_besar.setCursor(0,0);
 lcd_besar.print("Banyak beras: ");
 lcd_besar.setCursor(0,1);
 lcd_besar.print(" ");
char key = keypad.getKey();
 if (key){
 counter = counter + 1; 
 int lcds = counter + 2; 
 lcd_besar.setCursor(counter, 2);
 lcd_besar.print(key);
 lcd_besar.setCursor(lcds, 2);
 lcd_besar.print("Liter");
 }
 if (key == '1')
 {
 liter = liter + 1; 
 }
 if (key == '2')
 {
 liter = liter + 2; 
 }
 if (key == '3')
 {
 liter = liter + 3; 
 }
 if (key == '4')
 {
 liter = liter + 4; 
 }
 if (key == '5')
 {
 liter = liter + 5;
 }
 if (key == '6')
 {
 liter = liter + 6; 
 }
 if (key == '7')
 {
 liter = liter + 7; 
 }
 if (key == '8')
 {
 liter = liter + 8; 
 }
 if (key == '9')
 {
 liter = liter + 9;
 } 
 if (key == '0')
 {
 liter = liter + 0; 
 }
 if (key == '#')
 {
 key == NULL;
 liter == NULL;
 counter = 0;
 lcd_besar.clear();
 lcd_besar.print(key);
 }
 if(key == '*'){
 lcd_besar.clear();
 hLiter = liter.toInt();
 lcd_besar.print("Banyak: "); 
 lcd_besar.setCursor(0,1); 
 lcd_besar.print(hLiter); 
 lcd_besar.setCursor(2,1); 
 lcd_besar.print("Liter"); 
 liter = "";
 cetakliter = 750 * hLiter ;
 } 
}
void cetakberas(){
 if(cetakkilo > 1 && hKilo >= 1 && hKilo <= 4 && jenis_beras == "Merah"){
 // Serial.println("MENCETAK");
 lcd_besar.setCursor(0,2);
 lcd_besar.print("BERAS KELUAR");
 lcd_besar.setCursor(0,1);
 lcd_besar.print(hKilo);
 lcd_besar.setCursor(2,1);
 lcd_besar.print("KG");
 berasmerah.write(145);
 digitalWrite(fan,HIGH);
 if(units >= (cetakkilo-140
 )){
 /\ v2
 lcd_besar.clear();
 berasmerah.write(117);
 digitalWrite(fan,LOW);
 cetakkilo = 0;
 counter = 0;
 lcd_besar.setCursor(0,2);
 lcd_besar.print("AMBIL BERAS ");
 lcd_besar.setCursor(0,3);
 lcd_besar.print("TERIMA KASIH");
 delay(3000);
 jenis_beras = "";
 lcd_kecil.clear();
 lcd_besar.clear();
 Status -= Status;
 hkilo -= hkilo;
 hliter -= hliter;
 JenisKeluar ="";
 }
 } 
 
 if(cetakkilo > 1 && hKilo >= 1 && hKilo <= 4 && jenis_beras == "Biasa"){
 // Serial.println("MENCETAK");
 lcd_besar.setCursor(0,2);
 lcd_besar.print("BERAS KELUAR");
 lcd_besar.setCursor(0,1);
 lcd_besar.print(hKilo);
 lcd_besar.setCursor(2,1);
 lcd_besar.print("KG");
 berasbiasa.write(20);
 digitalWrite(fan,HIGH);
 if(units >= cetakkilo - 280){
 lcd_besar.clear();
 cetakkilo = 0;
 counter = 0;
 digitalWrite(fan,LOW);
 berasbiasa.write(48);
 lcd_besar.setCursor(0,2);
 lcd_besar.print("AMBIL BERAS ");
 lcd_besar.setCursor(0,3);
 lcd_besar.print("TERIMA KASIH");
 delay(3000);
 jenis_beras = "";
 lcd_kecil.clear();
 lcd_besar.clear();
 Status -= Status;
 hkilo -= hkilo;
 hliter -= hliter;
 JenisKeluar ="";
 }
 }
 if(cetakliter > 1 && hLiter >= 1 && hLiter <= 4 && jenis_beras == "Merah"){
 // Serial.println("MENCETAK");
 lcd_besar.setCursor(0,2);
 lcd_besar.print("BERAS KELUAR");
 lcd_besar.setCursor(0,1);
 lcd_besar.print(hLiter);
 lcd_besar.setCursor(2,1);
 lcd_besar.print("Ltr");
 berasmerah.write(145);
 digitalWrite(fan,HIGH);
 if(units >= (cetakliter-120)){
 lcd_besar.clear();
 cetakliter = 0;
 counter = 0;
 digitalWrite(fan,LOW);
 berasmerah.write(117);
 lcd_besar.setCursor(0,2);
 lcd_besar.print("AMBIL BERAS");
 lcd_besar.setCursor(0,3);
 lcd_besar.print("TERIMA KASIH");
 delay(3000);
 jenis_beras = "";
 lcd_kecil.clear();
 lcd_besar.clear();
 Status -= Status;
 hkilo -= hkilo;
 hliter -= hliter;
 JenisKeluar ="";
 }
 } 
 
 if(cetakliter > 1 && hLiter >= 1 && hLiter <= 4 && jenis_beras == "Biasa"){
 // Serial.println("MENCETAK");
 lcd_besar.setCursor(0,3);
 lcd_besar.print("BERAS KELUAR");
 lcd_besar.setCursor(0,1);
 lcd_besar.print(hLiter);
 lcd_besar.setCursor(2,1);
 lcd_besar.print("Ltr");
 berasbiasa.write(20);
 digitalWrite(fan,HIGH);
 if(units >= cetakliter - 250){
 lcd_besar.clear();
 cetakliter = 0;
 counter = 0;
 digitalWrite(fan,LOW);
 berasbiasa.write(48);
 lcd_besar.setCursor(0,2);
 lcd_besar.print("AMBIL BERAS");
 lcd_besar.setCursor(0,3);
 lcd_besar.print("TERIMA KASIH");
 delay(3000);
 jenis_beras = "";
 lcd_kecil.clear();
 lcd_besar.clear();
 Status -= Status;
 hkilo -= hkilo;
 hliter -= hliter;
 JenisKeluar = "";
 }
 }
 if(units >= cetakkilo){
 // Serial.println("BERHENTI MENCETAK");
 cetakkilo = 0;
 }
 if(units >= cetakliter){
 // Serial.println("BERHENTI MENCETAK");
 cetakliter = 0;
 }
 if(hKilo > 4){
 lcd_besar.setCursor(0,0);
 lcd_besar.print("MAKSIMAL 4KG");
 delay(1000);
 lcd_besar.clear();
 hKilo = 0;
 kilo == NULL;
 counter = 0;
 }
 if(hLiter > 4){
 lcd_besar.setCursor(0,0);
 lcd_besar.print("MAKSIMAL 4Ltr");
 delay(1000);
 lcd_besar.clear();
 hLiter = 0;
 liter == NULL;
 counter = 0;
 }
}
int getDistance (int initPin, int echoPin){
digitalWrite(initPin, HIGH);
delayMicroseconds(10);
digitalWrite(initPin, LOW);
unsigned long pulseTime = pulseIn(echoPin, HIGH);
int distance = pulseTime/58;
return distance;
}