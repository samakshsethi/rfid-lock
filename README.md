# rfid-lock

Lock system using RFID

At least 2 cards are required for this system, 1st will be the master card which will authorize and unauthorize all other cards.


Execution procedure:
1. The first card to be recognized gets assigned 0 index and is termed as the "master card"
2. The following cards have to be used after first scanning the master card, after which 2 things can happen
   (i) Add-> If the card does not exist in the index already, it is added and authorized for future scans
   (ii) Delete-> If the card does exists in the index already, it is deleted and unauthorized for future scans
NOTE- The master card cannot be deleted without using the RST pin on the board.
