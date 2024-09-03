#include "CardChecker.h"
#include <Arduino.h>

bool checkCard(MFRC522 &rfid, byte allowedUID[], int uidSize)
{
    // Look for new cards
    if (!rfid.PICC_IsNewCardPresent())
    {
        return false;
    }

    // Select one of the cards
    if (!rfid.PICC_ReadCardSerial())
    {
        return false;
    }

    // Check if the read card's UID matches the allowed UID
    if (rfid.uid.size == uidSize)
    {
        bool match = true;
        for (byte i = 0; i < uidSize; i++)
        {
            if (rfid.uid.uidByte[i] != allowedUID[i])
            {
                match = false;
                break;
            }
        }

        if (match)
        {
            Serial.println("Authorized Card Detected");
            rfid.PICC_HaltA(); // Halt PICC to prevent reading the same card multiple times
            return true;       // Correct card detected
        }
        else
        {
            Serial.println("Unauthorized Card");
        }
    }

    rfid.PICC_HaltA(); // Halt PICC to prevent reading the same card multiple times
    return false;      // No card detected or wrong card
}
