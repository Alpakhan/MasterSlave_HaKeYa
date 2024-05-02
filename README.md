Dokumentation Microcontroller Projekt Team HAKEYA
Implementiertes I2C Protokoll
Der Master spricht die verschiedenen Slaves an über die Adressen 0x08 und 0x09. 
Verschiedene UART-Schnittstellen
gn => t_data[0] =1 ->Grüne LED an
ws => t_data[0] =2 ->weiße LED an
bl => t_data[0] = 3 -> blaue LED an
rt => t_data[0] = 4 -> rote LED an
1 => t_data [0] = 5 -> erste Gruppe an (Grüne und Weiße LED)
2 => t_data[0] = 6 -> zweite Gruppe an (Blaue und Rote LED)

Durch das Eingeben in den Serial Monitor des Masters wird der Code von 1-6 über UART 
oder dem Master Code umgewandelt in einen Befehl, welcher man über I2C an die 
Slaves gegeben wird, wodurch die verschiedenen LEDs dementsprechend angemacht 
oder ausgemacht werden, je nachdem ob die LEDs gerade an oder aus sind. Zudem wird 
bei dem Serial Monitor der Slaves die dementsprechenden LEDs ebenfalls per Serial 
Print Befehl ausgegeben.

Installation des Projekts ging über PlatformIO

[Schaltplan.pdf](https://github.com/Alpakhan/MasterSlave_HaKeYa/files/15192253/Schaltplan.pdf)
