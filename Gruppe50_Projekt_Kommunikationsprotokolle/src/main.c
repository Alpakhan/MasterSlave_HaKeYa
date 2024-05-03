//Master

//Adressen
//Slave 1(LED: weiß,grün): 0x08
//Slave 2(LED: blau,rot):  0x09

//Funktion nach Änderung (Entfernen und wiedereinsetzen der Pull-Up Widerstände oder ändern der Herkunft der Bibliothek link oder lokal vorhanden) manchmal gegeben. 
//Sonst "Aufhängen" bei erreichen der "i2c_master_transmit" Funktion.

#include<stm32f401xe.h>
#include <i2c.h>
#include <gpio.h>
#include <stdio.h>
#include <uart.h>


uint8_t puffer[10];
uint8_t x = 0;

uint8_t r_data[1];
uint8_t t_data[1];


int main()
{
    __disable_irq();
    gpio_enable_port_clock(GPIOA);

    uart_setup(USART2 ,9600);
    i2c_master_setup(I2C1,I2C_STANDARD);
    __enable_irq();

    printf("Gruppe50\n");
    printf("Master\n");

    while(1){

            // printf("\n0:%1c ", puffer[0]);
            // printf("1:%1c ", puffer[1]);
            // printf("2:%1c ", puffer[2]);
            // printf("3:%1c ", puffer[3]);
            // printf("4:%1c ", puffer[4]);
            // printf("%1i\n", x);

        if (puffer[0] == 'g' && puffer[1] == 'r')       //LED grün soll bei Slave 1 0x08 toggeln
        {   
            printf("grün");
            t_data[0] = 1;
            printf("%1i\n",i2c_master_transmit(I2C1,0x08,t_data,1));
            printf("I2C master transmit: grün toggeln\n");
            puffer[0]=0;
            puffer[1]=0;
            puffer[2]=0;
            puffer[3]=0;
            puffer[4]=0;
            r_data[0]=0;
            t_data[0]=0;
            x=0;
        }

        if (puffer[0] == 'w'&& puffer[1] == 's')        // LED weiß soll bei Slave 1 0x08 toggeln
        {   
            printf("weiß");
            t_data[0] = 2;
            printf("%1i\n",i2c_master_transmit(I2C1,0x08,t_data,1));
            printf("I2C master transmit: weiß toggeln\n");
            puffer[0]=0;
            puffer[1]=0;
            puffer[2]=0;
            puffer[3]=0;
            puffer[4]=0;
            r_data[0]=0;
            t_data[0]=0;
            x=0;
        }

        if (puffer[0] == 'b' && puffer[1] == 'l')         // LED blau soll bei Slave 2 0x09 toggeln
        {
            printf("blau");
            t_data[0] = 1;
            printf("%1i\n",i2c_master_transmit(I2C1,0x09,t_data,1));
            printf("t_data: %1i\n", t_data[0]);
            printf("I2C master transmit: blau toggeln\n");
            puffer[0]=0;
            puffer[1]=0;
            puffer[2]=0;
            puffer[3]=0;
            puffer[4]=0;
            r_data[0]=0;
            t_data[0]=0;
            x=0;
        }

        if (puffer[0] == 'r'&& puffer[1] == 't')   // LED rot soll bei Slave 2 0x09 toggeln
        {
            printf("rot");
            t_data[0] = 2;
            i2c_master_transmit(I2C1,0x08,t_data,1);
            printf("I2C master transmit: rot toggeln\n");
            puffer[0]=0;
            puffer[1]=0;
            puffer[2]=0;
            puffer[3]=0;
            puffer[4]=0;
            r_data[0]=0;
            t_data[0]=0;
            x=0;
        }

        if (puffer[0] == '1')                                // LED grün & weiß an Slave 1 0x08 toggeln
        {
            printf("grün weiß");
            t_data[0] = 5;
            printf("%1i\n",i2c_master_transmit(I2C1,0x08,t_data,1));
            printf("I2C master transmit: grün & weiß toggeln\n");

            puffer[0]=0;
            puffer[1]=0;
            puffer[2]=0;
            puffer[3]=0;
            puffer[4]=0;
            r_data[0]=0;
            t_data[0]=0;
            x=0;
        }

        if (puffer[0] == '2')                               // LED rot & blau an Slave 2 0x09 toggeln
        {
            printf("rot blau");
            t_data[0] = 5;
            printf("%1i\n",i2c_master_transmit(I2C1,0x09,t_data,1));
            printf("I2C master transmit: rot & blau toggeln\n");

            puffer[0]=0;
            puffer[1]=0;
            puffer[2]=0;
            puffer[3]=0;
            puffer[4]=0;
            r_data[0]=0;
            t_data[0]=0;
            x=0;
        }

        if (puffer[0] == 'c' && puffer[1] == 'h' && puffer[2] == 'c')       // Slave 1 & 2 abfrage welche LED ist an
        {
            i2c_master_receive(I2C1,0x08,r_data,1);

            printf("I2C master receive 0x08\n");

            switch(r_data[0])
            {
                case 1: //Rot ist an , Blau ist aus
                    printf("I2C 0x08 master receive: ROT ist AN BLAU ist AUS\n");
                break;
                
                case 2: //Rot ist aus , Blau ist an
                    printf("I2C 0x08 master receive: ROT ist AUS BLAU ist AN\n");
                break;

                case 0: //Rot ist aus , Blau ist aus
                    printf("I2C 0x08 master receive: ROT ist AUS BLAU ist AUS\n");
                break;

                case 3: //Rot ist an , Blau ist an
                    printf("I2C 0x08 master receive: ROT ist AN BLAU ist AN\n");
                break;

                default:  // Fehler
                    printf("I2C 0x08 master receive: Ungültiger Zustand\n");
                break;
            }


            i2c_master_receive(I2C1,0x09,r_data,2);

            printf("I2C master receive 0x09\n");

            switch(r_data[0])
            {
                case 1: //Weiß ist an , Grün ist aus
                    printf("I2C 0x09 master receive: WEIß ist AN GRÜN ist AUS\n");
                break;
                
                case 2: //Weiß ist aus , Grün ist an
                    printf("I2C 0x09 master receive: WEIß ist AUS GRÜN ist AN\n");
                break;

                case 0: //Weiß ist aus , Grün ist aus
                printf("I2C 0x09 master receive: WEIß ist AUS GRÜN ist AUS\n");
                break;

                case 3: //Weiß ist an , Grün ist an
                printf("I2C 0x09 master receive: WEIß ist AN GRÜN ist AN\n");
                break;

                default:  // Fehler
                    printf("I2C 0x09 master receive: Ungültiger Zustand\n");
                break;
            }
        
            puffer[0]=0;
            puffer[1]=0;
            puffer[2]=0;
            puffer[3]=0;
            puffer[4]=0;
            r_data[0]=0;
            t_data[0]=0;
            x=0;
        }

            // printf("\n0:%1c ", puffer[0]);
            // printf("1:%1c ", puffer[1]);
            // printf("2:%1c ", puffer[2]);
            // printf("3:%1c ", puffer[3]);
            // printf("3:%1c ", puffer[4]);
            // printf("%1i\n", x);

        if (puffer[0] == '\n' || puffer[1] == '\n' || puffer[3] == '\n' || puffer[4] == '\n' || x > 4)
            {
                puffer[0]=0;
                puffer[1]=0;
                puffer[2]=0;
                puffer[3]=0;
                puffer[4]=0;            
                x=0;
                printf("-------------Neue--Eingabe-!---------------------\n");
            }


      }
}
 
int _write(int fd, char *ptr, int len)
{
    uint32_t size = len;
    while (len-- > 0)
    {
        uart_write_byte(USART2, *(ptr++));
    }
 
    return size;
}

 
void USART2_IRQHandler()
{
    if (uart_data_available(USART2))
    {   
        char byte = USART2->DR & 0x1FF;
        puffer[x] = byte;
        x = x+1;
        printf("\nbyte:%c\n", byte);
    }
}