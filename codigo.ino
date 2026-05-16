#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#define btdisplay 6
#define tmp A2
#define ldr A1
#define umid A3
#define lvd 2
#define lam 3
#define lve 4
#define bz 5

int alert;
int ldrvalue;
int tmpvalue;
int umidvalue;
int alertAnterior;
bool buzzer;
unsigned long tempo;

int vetorTemp[5];
int vetorUmid[5];
int vetorLdr[5];

int mediaTemp;
int mediaUmid;
int mediaLdr;

int indice = 0;

int tela = 0;
unsigned long trocaTela = 0;

LiquidCrystal_I2C lcd(0x27,16,2);

void setup()
{
 pinMode(ldr, INPUT);
 pinMode(bz, OUTPUT);
 pinMode(lvd, OUTPUT);
 pinMode(lam, OUTPUT);
 pinMode(lve, OUTPUT);
 pinMode(btdisplay, INPUT_PULLUP);
 
 lcd.init();
 lcd.backlight();
 lcd.setCursor(0,0);
  
 Serial.begin(9600);
  
 for(int i = 0; i < 5; i++)
{
  vetorTemp[i] = 0;
  vetorUmid[i] = 0;
  vetorLdr[i] = 0;
}
 lcd.print("iniciando...");
}

void loop()
{
  calcMedia();
  verificar();
  
  digitalWrite(lvd, (alert == 1));
  digitalWrite(lam, (alert == 2));
  digitalWrite(lve, (alert == 3));
  digitalWrite(bz, (alert == 3));
  
  if(millis() - trocaTela > 5000){
  trocaTela = millis();

  tela++;

  if(tela > 2)
  {
    tela = 0;
  }

  lcd.clear();
  selet_tela();
}
}
void verificar(){
  ldrvalue = mediaLdr;
  tmpvalue = mediaTemp;
  umidvalue = mediaUmid;
  
  if(ldrvalue > 30 || tmpvalue > 22 || tmpvalue < 5 || umidvalue < 50 || umidvalue > 80){
    alert = 3;
  }
  else if((ldrvalue > 10 && ldrvalue < 30) || (tmpvalue > 16 && tmpvalue < 22 ) || (tmpvalue > 5 && tmpvalue < 10 ) || (umidvalue > 50 && umidvalue < 60) || (umidvalue > 70 && umidvalue < 80)){
    alert = 2;
  }
  else if(ldrvalue < 10 || (tmpvalue >= 10 && tmpvalue <= 16) || (umidvalue > 60 && umidvalue < 70)){
    alert = 1;
  }
 
}

void dp_umidity(){
  if(umidvalue < 50) {
    lcd.print("Umidade Baixa");
  }
  else if(umidvalue > 70) {
    lcd.print("Umidade Alta");
  }
  else{
    lcd.print("Umidade OK");
  }

  lcd.setCursor(0,1);
  lcd.print("Umidade:");
  lcd.print(umidvalue);
  lcd.print("%");
}

void dp_tmp(){
  if(tmpvalue < 10) {
    lcd.print("Temp Baixa");
  }
  else if(tmpvalue > 16) {
    lcd.print("Temp Alta");
  }
  else{
    lcd.print("Umidade OK");
  }

  lcd.setCursor(0,1);
  lcd.print("Temperatura: ");
  lcd.print(tmpvalue);
  lcd.print("°");

}

void dp_luminosity(){
  if(ldrvalue > 10 && ldrvalue < 30){
    lcd.print("Meia Luz");
  }
  else if(ldrvalue > 30) {
    lcd.print("Luz Forte");
  }
  else{
    lcd.print("Luz Ideal");
  }

  lcd.setCursor(0,1);
  lcd.print("Luminosidade:");
  lcd.print(ldrvalue);
  lcd.print("%");

}

void calcMedia()
{
  vetorTemp[indice] = map(analogRead(tmp), 20, 358, -40, 125);

  vetorUmid[indice] = map(analogRead(umid), 0, 876, 0, 100);

  vetorLdr[indice] = map(analogRead(ldr), 0, 679, 0, 100);

  int somaTemp = 0;
  int somaUmid = 0;
  int somaLdr = 0;

  for(int i = 0; i < 5; i++)
  {
    somaTemp += vetorTemp[i];
    somaUmid += vetorUmid[i];
    somaLdr += vetorLdr[i];
  }

  mediaTemp = somaTemp / 5;
  mediaUmid = somaUmid / 5;
  mediaLdr = somaLdr / 5;

  indice++;

  if(indice >= 5)
  {
    indice = 0;
  }
}
void selet_tela(){
  switch(tela)
{
  case 0:
    dp_umidity();
    break;

  case 1:
    dp_tmp();
    break;

  case 2:
    dp_luminosity();
    break;
  }  
}