/*
 * 
 * Typical pin layout used:
 * -----------------------------------------------------------------------------------------
 *             MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
 *             Reader/PCD   Uno           Mega      Nano v3    Leonardo/Micro   Pro Micro
 * Signal      Pin          Pin           Pin       Pin        Pin              Pin
 * -----------------------------------------------------------------------------------------
 * RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
 * SPI SS      SDA(SS)      10            53        D10        10               10
 * SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
 * SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
 * SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15
 */

#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         5          // Configurable, see typical pin layout above
#define SS_PIN          53         // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

int amountOfUsers = 3; //Totale mængde brugere
String names[3] = {"John","Simon","Kasper"}; //Navne på brugere. Indexet skal passe overens med deres kode
String pincodes[3] = {"3ad93b56", "6aa54d56", "3ad93b54"}; //Brugernes koder
String hexstring = ""; //Denne streng indeholder koden fra det scannede kort

void setup() {
	Serial.begin(9600);		// Initialize serial communications with the PC
	while (!Serial);		// Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
	SPI.begin();			// Init SPI bus
	mfrc522.PCD_Init();		// Init MFRC522
	mfrc522.PCD_DumpVersionToSerial();	// Show details of PCD - MFRC522 Card Reader details
	Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));
}

void loop() {

	// Look for new cards
	if ( ! mfrc522.PICC_IsNewCardPresent()) {
		return;
	}

	// Select one of the cards
	if ( ! mfrc522.PICC_ReadCardSerial()) {
		return;
	}

  convertString();

  Serial.println(hexstring);

  Serial.println(getName());

}

//Konverterer vores byte array to en string skrevet i hex
void convertString(){
  hexstring = ""; //Resetter strengen for hvert nyt kort.

  for(int i = 0; i < mfrc522.uid.size; i++) {
    if(mfrc522.uid.uidByte[i] < 0x10) {
      hexstring += '0';
    }

    hexstring += String(mfrc522.uid.uidByte[i], HEX);
  }
}

//Retunerer det navn der hører til den kode man har scannet. 
String getName(){
  for(int k = 0; k < amountOfUsers; k++){
     if(pincodes[k] == hexstring){
      //Indkommenter denne linje kode for at finde kode fra et kort.
      // Serial.println(names[k]); //Printer navnet der hører til koden
      return names[k];
     }
  }
}
