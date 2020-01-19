#include <Adafruit_LiquidCrystal.h>
#include<LiquidCrystal.h>
LiquidCrystal lcd(13,12,11,10,9,8);
#include <SoftwareSerial.h>
SoftwareSerial fingerPrint(2, 3);
#include<Servo.h>

Servo myServo;

#include <Adafruit_Fingerprint.h>
uint8_t id;
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&fingerPrint);

#define enroll A0
#define del A1
#define up A2
#define down A3
#define LampeVerte 7
#define LampeRouge 6
#define servoPin 5

void setup(){
  myServo.attach(servoPin);
  myServo.write(135);
  pinMode(enroll, INPUT_PULLUP);
  pinMode(up, INPUT_PULLUP);
  pinMode(down, INPUT_PULLUP);
  pinMode(del, INPUT_PULLUP);
  pinMode(LampeVerte, OUTPUT);
  pinMode(LampeRouge, OUTPUT);
  lcd.begin(16,2);
  lcd.clear();
  finger.begin(57600);
  Serial.begin(9600);
  lcd.clear();
  lcd.print("VeuillezPatienter");
  lcd.setCursor(0,1);
  delay(1000);
  if (finger.verifyPassword()){
    Serial.println("Fingerprint sensor Trouve!");
    lcd.clear();
    lcd.print("Module Trouve");
    delay(2000);
  }
  else{
    Serial.println("Fingerprint sensor non Trouve!");
    lcd.clear();
    lcd.print("Module Non Trouve");
    lcd.setCursor(0,1);
    lcd.print("Verifier la Connection");
    while (1);
  }
}

void loop(){
  lcd.setCursor(0,0);
  lcd.print("  Cliquer Match ");
  lcd.setCursor(0,1);
  lcd.print("Pour lancer!");
  digitalWrite(LampeRouge, HIGH);
  if(digitalRead(up)==0 || digitalRead(down)==0){
    for(int i=0;i<5;i++){
      lcd.clear();
      lcd.print("Placer le doigt");
      delay(2000);
      int resultat=getFingerprintIDez();
      if(resultat>=0){
        digitalWrite(LampeVerte, HIGH);
        digitalWrite(LampeRouge, LOW);
        lcd.clear();
        lcd.print(" Autorise ");
        lcd.setCursor(0,1);
        lcd.print(" Portail Ouvert ");
        myServo.write(0);
        delay(3500);
        myServo.write(135);
        digitalWrite(LampeRouge, HIGH);
        digitalWrite(LampeVerte, LOW);
        lcd.setCursor(0,1);
        lcd.print(" Portail Ferme ");
        return;
      }
    }
  }
  checkKeys();
  delay(1000);
}

void checkKeys(){
  if(digitalRead(enroll) == 0){
    lcd.clear();
    lcd.print("Veuillez patienter!");
    delay(1000);
    while(digitalRead(enroll) == 0);
      Enroll();
  }
  
  else if(digitalRead(del) == 0){
    lcd.clear();
    lcd.print("Veuillez patienter!");
    delay(1000);
    delet();
  }
}

void Enroll(){
  int count=0;
  lcd.clear();
  lcd.print("Enroll Empreinte");
  lcd.setCursor(0,1);
  lcd.print("Position:");
  while(1){
    lcd.setCursor(9,1);
    lcd.print(count);
    if(digitalRead(up) == 0){
      count++;
      if(count>25)
      count=0;
      delay(500);
    }
    
    else if(digitalRead(down) == 0){
      count--;
      if(count<0)
      count=25;
      delay(500);
    }
    else if(digitalRead(del) == 0){
      id=count;
      getFingerprintEnroll();
      return;
    }
    
    else if(digitalRead(enroll) == 0){
      return;
    }
  }
}

void delet(){
  int count=0;
  lcd.clear();
  lcd.print("Supprimer Empreinte");
  lcd.setCursor(0,1);
  lcd.print("Position:");
  while(1){
    lcd.setCursor(9,1);
    lcd.print(count);
    if(digitalRead(up) == 0){
      count++;
      if(count>25)
      count=0;
      delay(500);
    }
    
    else if(digitalRead(down) == 0){
      count--;
      if(count<0)
      count=25;
      delay(500);
    }
    else if(digitalRead(del) == 0){
      id=count;
      deleteFingerprint(id);
      return;
    }
    
    else if(digitalRead(enroll) == 0){
      return;
    }
  }
}

uint8_t getFingerprintEnroll(){
  int p = -1;
  lcd.clear();
  lcd.print("Empreinte ID:");
  lcd.print(id);
  lcd.setCursor(0,1);
  lcd.print("Placer le doigt");
  delay(1000);
  while (p != FINGERPRINT_OK){
    p = finger.getImage();
    switch (p){
      case FINGERPRINT_OK:
        Serial.println("Image capture");
        lcd.clear();
        lcd.print("Image capture");
        break;
      case FINGERPRINT_NOFINGER:
        Serial.println("No Empreinte");
        lcd.clear();
        lcd.print("No Empreinte");
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication erreur");
        lcd.clear();
        lcd.print("Comm Erreur");
        break;
      case FINGERPRINT_IMAGEFAIL:
        Serial.println("Imaging erreur");
        lcd.clear();
        lcd.print("Imaging Erreur");
        break;
      default:
        Serial.println("Unknown erreur");
        lcd.clear();
        lcd.print("Unknown Erreur");
        break;
    }
  }
  
  // OK success!
  
  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image convertee");
      lcd.clear();
      lcd.print("Image convertee");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image floue");
      lcd.clear();
      lcd.print("Image floue");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication erreur");
      lcd.clear();
      lcd.print("Comm Erreur");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Caracterestique de l'empreinte non trouvees");
      lcd.clear();
      lcd.print("Caracterestique Non Trouvee");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Caracterestique de l'empreinte non trouvees");
      lcd.clear();
      lcd.print("Caracterestique Non Trouvee");
      return p;
    default:
      Serial.println("Erreur inconnue");
      lcd.clear();
      lcd.print("Erreur inconnue");
      return p;
  }
  
  Serial.println("Enlever le doigt");
  lcd.clear();
  lcd.print("Enlever le doigt");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER){
    p = finger.getImage();
  }
  Serial.print("ID "); Serial.println(id);
  p = -1;
  Serial.println("Placer le meme doigt encore");
  lcd.clear();
  lcd.print("Placer le doigt");
  lcd.setCursor(0,1);
  lcd.print("Encore");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        Serial.println("Image capturee");
        break;
      case FINGERPRINT_NOFINGER:
        Serial.print(".");
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication erreur");
        break;
      case FINGERPRINT_IMAGEFAIL:
        Serial.println("Image erreur");
        break;
      default:
        Serial.println("Erreur inconnue");
        return;
    }
  }
  
  // OK success!
  
  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image convertee");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image floue");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication erreur");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Caracterestique de l'empreinte non trouvees");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Caracterestique de l'empreinte non trouvees");
      return p;
    default:
      Serial.println("Erreur inconnue");
      return p;
  }
  
  // OK converted!
  Serial.print("Creer le modele pour #"); Serial.println(id);
  
  p = finger.createModel();
  if (p == FINGERPRINT_OK){
    Serial.println("Empreinte identiques");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication erreur");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Empreintes non identiques");
    return p;
  } else {
    Serial.println("Erreur inconnue");
    return p;
  }
  
  Serial.print("ID "); Serial.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println("Enregitree!");
    lcd.clear();
    lcd.print("Enregistree!");
    delay(2000);
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication erreur");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not store in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Erreur");
    return p;
  }
  else{
    Serial.println("Erreur inconnue");
    return p;
  }
}

int getFingerprintIDez()
{
  uint8_t p = finger.getImage();
  
  if (p != FINGERPRINT_OK)
    return -1;
  
  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)
    return -1;
  
  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK){
    lcd.clear();
    lcd.print("Empreinte NonTrouvee!");
    lcd.setCursor(0,1);
    delay(3000);
    lcd.print("Veuillez reessayer!");
    delay(3000);
    return -1;
  }
  // un match trouve!
  Serial.print("Found ID #");
  Serial.print(finger.fingerID);
  return finger.fingerID;
}
//fonction de suppresion d'empreinte
uint8_t deleteFingerprint(uint8_t id){
  uint8_t p = -1;
  lcd.clear();
  p = finger.deleteModel(id);
  if (p == FINGERPRINT_OK){
    Serial.println("Supprimée!");
    lcd.clear();
    lcd.print("Empreinte supprimée!");
    lcd.setCursor(0,1);
    lcd.print("avec succes");
    delay(1000);
  }
  
  else{
    Serial.print("Erreur !");
    lcd.clear();
    lcd.print("Erreur !");
    lcd.setCursor(0,1);
    lcd.print("Veuillez reessayer ! ");
    delay(3000);
    return p;
  }
}
