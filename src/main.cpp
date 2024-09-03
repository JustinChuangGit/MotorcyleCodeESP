#include <SPI.h>
#include <MFRC522.h>
#include "CardChecker.h" // Include the header file

#define RST_PIN 23  // Reset pin
#define SS_PIN 5    // Slave Select pin
#define LED_PIN 2   // Onboard LED pin
#define MOSI_PIN 18 // MOSI pin
#define SCK_PIN 21  // SCK pin

MFRC522 rfid(SS_PIN, RST_PIN);

bool bikeOn = false; // Variable to track if the bike is on

// The specific card UID to match
byte allowedUID[] = {0xA3, 0x44, 0x80, 0x0E};

void setup()
{
  Serial.begin(115200);

  // Initialize SPI bus with default pins
  SPI.begin(SCK_PIN, MISO, MOSI_PIN, SS_PIN);

  rfid.PCD_Init(); // Init MFRC522

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW); // Ensure LED is off at startup

  Serial.println("RFID Reader Initialized");
}

void loop()
{
  // Check if the correct card is tapped
  if (checkCard(rfid, allowedUID, sizeof(allowedUID)))
  {
    // Toggle the bikeOn state
    bikeOn = !bikeOn;

    // Update LED state based on bikeOn
    if (bikeOn)
    {
      digitalWrite(LED_PIN, HIGH); // Turn on the LED
      Serial.println("Bike is On");
    }
    else
    {
      digitalWrite(LED_PIN, LOW); // Turn off the LED
      Serial.println("Bike is Off");
    }

    // Add a small delay to debounce the card detection
    delay(1000);
  }
}
