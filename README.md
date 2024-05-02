Dokumentation Microcontroller Projekt Team HAKEYA


Durch das Eingeben in den Serial Monitor des Masters wird der Code von 1-6 über UART 
oder dem Master Code umgewandelt in einen Befehl, welcher man über I2C an die 
Slaves gegeben wird, wodurch die verschiedenen LEDs dementsprechend angemacht 
oder ausgemacht werden, je nachdem ob die LEDs gerade an oder aus sind. Zudem wird 
bei dem Serial Monitor der Slaves die dementsprechenden LEDs ebenfalls per Serial 
Print Befehl ausgegeben.

Installation des Projekts ging über PlatformIO

[Schaltplan.pdf](https://github.com/Alpakhan/MasterSlave_HaKeYa/files/15192253/Schaltplan.pdf)
