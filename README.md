# Dokumentation Microcontroller Projekt Gruppe 50 
(Für bessere Darstellung READMNE.md öffnen und Code als Ansicht wählen)

Master wurde im CMSIS Framekwork und mit der mcal Bibliothek von jomaway geschrieben. 
https://codeberg.org/jomaway/stm32f401-mcal/src/branch/main

Die beiden Slaves wurden im Arduino Framework geschrieben.


Befehle werden über UART2 angenommen und über I2C1 an die Slaves gesendet, dort wird überprüft wie weiter mit dem Befehl zu verfahren ist.


UART-Eingabe:

gn   =>   t_data[0] = 1 & Adresse 0x08 -> grüne LED an
ws   =>   t_data[0] = 2 & Adresse 0x08 -> weiße LED an
bl   =>   t_data[0] = 1 & Adresse 0x09 -> blaue LED an
rt   =>   t_data[0] = 2 & Adresse 0x09 -> rote LED an
1    =>   t_data[0] = 5 & Adresse 0x08 -> Beide LED an Slave 1 0x08 anschalten (Grüne und Weiße LED)
2    =>   t_data[0] = 5 & Adresse 0x09 -> Beide LED an Slave 2 0x09 anschalten (Blaue und Rote LED)
chc  =>   Abfrage an beide Slaves welche LED an sind.

Bei jeder Erfolgreichen Eingabe wird eine Meldung über UART ausgegeben die aussagt was jetzt passieren soll.
Nach Eingabe des 4. Zeichens wird die Eingabe zurückgesetzt und es erscheint als Rückmeldung über UART: "-------------Neue--Eingabe-!---------------------".


Implementiertes I2C Protokoll:

Der Master spricht die verschiedenen Slaves an über die Adressen: 
Slave 1 0x08 
Slave 2 0x09

Hier wird auf die Ziffern eingegangen die über I2C gesendet werden, nicht welche über UART eingegeben werden.

Master Transmit:
1 mit Adresse 0x08 => Slave 1 überprüft welchen Zustand LED grün hat und switched ihn.
2 mit Adresse 0x08 => Slave 1 überprüft welchen Zustand LED weiß hat und switched ihn.
1 mit Adresse 0x09 => Slave 2 überprüft welchen Zustand LED blau hat und switched ihn.
2 mit Adresse 0x09 => Slave 2 überprüft welchen Zustand LED rot hat und switched ihn.

5 mit Adresse 0x08 0> Slave 1 schaltet LED grün & LED weiß an.
5 mit Adresse 0x08 0> Slave 1 schaltet LED grün & LED weiß an.

Master Receive:
Wenn der Master nachfragt werden verschiedene Ziffern übertragen die der Master als Status interprtiert.

Slave 1 0x08:
0.Bit =0 => LED rot aus 
0.Bit =1 => LED rot an
1.Bit =0 => LED blau aus
1.Bit =1 => LED blau an


Slave 2 0x09:#
0.Bit =0 => LED weiß aus
0.Bit =1 => LED weiß an
1.Bit =0 => LED grün aus
1.Bit =1 => LED grün an

Das wird in einer Statusmeldung über UART ausgegeben.

Eine Statusmeldung sieht so aus:
I2C 0x08 master receive: ROT ist AN BLAU ist AUS
Diese sagt aus das über I2C eine Nachricht von 0x08 (Slave1) empfangen wurde die aussagt, das LED rot leuchtet und LED blau aus ist. 

Sollte etwas anderes als die erwarteten Ziffern zurückkommen wird eine Fehlermeldung über UART ausgegeben.

Installation des Projekts ging über PlatformIO

[Hier befindet sich der physikalischer Aufbau als Schaltplan](Schaltplan.pdf)
