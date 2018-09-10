This project contains the main part of codes of my second version of bootloader. It has changed greatly in the following aspects.
1. It uses 2 seperate state machines to handle the process of receiving code from the usart and command processing. Compared to the former 
strategy that the upper machine receive the message by checking the receive buffer for a certain time, this is more efficient and extremely
faster. Now it only takes a few seconds to finish the whole processure.
2. It adds a protocol in usart transmittion and crc parity check for the whole code. It will ensure the safety issues.
