#ifndef CARDCHECKER_H
#define CARDCHECKER_H

#include <MFRC522.h>

// Function declaration for checkCard
bool checkCard(MFRC522 &rfid, byte allowedUID[], int uidSize);

#endif
