#include <Wire.h>
#include <SPI.h>
#include <MFRC522.h>
#include <MPU6050.h>
#include "CardChecker.h" // Include the header file

#define RST_PIN 23   // Reset pin
#define SS_PIN 5     // Slave Select pin
#define LED_PIN 2    // Onboard LED pin
#define RELAY_PIN 13 // Relay control pin (updated)
#define MOSI_PIN 18  // MOSI pin
#define SCK_PIN 21   // SCK pin

// I2C pins for MPU6050
#define SDA_PIN 16 // SDA pin for MPU6050
#define SCL_PIN 17 // SCL pin for MPU6050

MFRC522 rfid(SS_PIN, RST_PIN);
MPU6050 mpu;

bool bikeOn = false; // Variable to track if the bike is on

// The specific card UID to match
byte allowedUID[] = {0xA3, 0x44, 0x80, 0x0E};

// Variables for non-blocking timing
unsigned long previousMillis = 0;
const long interval = 1000; // Interval at which to check MPU6050 (milliseconds)

void setup()
{
  Serial.begin(115200);

  // Initialize SPI bus for RFID
  SPI.begin(SCK_PIN, MISO, MOSI_PIN, SS_PIN);
  rfid.PCD_Init(); // Init MFRC522

  // Initialize I2C for MPU6050
  Wire.begin(SDA_PIN, SCL_PIN);
  mpu.initialize();

  // Check if MPU6050 is connected
  if (!mpu.testConnection())
  {
    Serial.println("Failed to connect to MPU6050");
    while (1)
    {
      delay(10);
    }
  }

  pinMode(LED_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);

  digitalWrite(LED_PIN, LOW);   // Ensure LED is off at startup
  digitalWrite(RELAY_PIN, LOW); // Ensure relay is open at startup

  Serial.println("RFID Reader and MPU6050 Initialized");
}

void loop()
{
  unsigned long currentMillis = millis();

  // Check if the correct card is tapped
  if (checkCard(rfid, allowedUID, sizeof(allowedUID)))
  {
    // Toggle the bikeOn state
    bikeOn = !bikeOn;

    // Update LED and relay state based on bikeOn
    if (bikeOn)
    {
      digitalWrite(LED_PIN, HIGH);   // Turn on the LED
      digitalWrite(RELAY_PIN, HIGH); // Close the relay
      Serial.println("Bike is On - Relay Closed");
    }
    else
    {
      digitalWrite(LED_PIN, LOW);   // Turn off the LED
      digitalWrite(RELAY_PIN, LOW); // Open the relay
      Serial.println("Bike is Off - Relay Open");
    }
  }

  // Only read MPU6050 data if the bike is on
  if (!bikeOn)
  {
    // Check if the interval has passed
    if (currentMillis - previousMillis >= interval)
    {
      previousMillis = currentMillis;

      int16_t gx, gy, gz;
      mpu.getRotation(&gx, &gy, &gz);

      // Print out the gyroscope values
      Serial.print("Gyro X: ");
      Serial.print(gx);
      Serial.print(", Y: ");
      Serial.print(gy);
      Serial.print(", Z: ");
      Serial.print(gz);
      Serial.println(" rad/s");
    }
  }
}
