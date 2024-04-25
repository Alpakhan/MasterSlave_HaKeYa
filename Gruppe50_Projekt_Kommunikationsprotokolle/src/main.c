#include <stm32f401xe.h>
#include <i2c.h>
#include <gpio.h>
#include <stdio.h>
#include <uart.h>

uint8_t puffer[10];
uint8_t x;

uint8_t r_data[1];
uint8_t t_data[1];

//Adresse 
//Slave 1(weiß,grün): 0x08
//Slave 2(blau,rot):  0x09


int main()
{
    __disable_irq();
    gpio_enable_port_clock(GPIOA);

    uart_setup(USART2 ,9600);
    i2c_master_setup(I2C1,I2C_STANDARD);
    __enable_irq();

    printf("Gruppe50\n");
    printf("Master\n");
    printf("t_data: %1c\n", t_data[0]);
    printf("r_data: %1c\n", t_data[0]);
    while(1){   }
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
        printf("%c\n", byte);

        if (puffer[0] == 'g' && puffer[1] == 'r' && byte == '\n')
        {   
            printf("grün");
            t_data[0] = 1;
            i2c_master_transmit(I2C1,0x08,t_data,1);
            printf("I2C master transmit: grün toggeln\n");
            puffer[0]=0;
            puffer[1]=0;
            t_data[0] = 0;
            x=0;
        }

        if (puffer[0] == 'w'&& puffer[1] == 's' && byte == '\n')
        {   
            printf("weiß");
            t_data[0] = 2;
            i2c_master_transmit(I2C1,0x08,t_data,1);
            printf("I2C master transmit: weiß toggeln\n");
            puffer[0]=0;
            puffer[1]=0;
            puffer[2]=0;
            t_data[0]=0;
            x=0;
        }

        if (puffer[0] == 'b' && puffer[1] == 'l' && byte == '\n')
        {
            printf("blau");
            t_data[0] = 3;
            i2c_master_transmit(I2C1,0x08,t_data,1);
            printf("I2C master transmit: blau toggeln\n");
            puffer[0]=0;
            puffer[1]=0;
            puffer[2]=0;
            t_data[0]=0;
            x=0;
        }

        if (puffer[0] == 'r'&& puffer[1] == 't' && byte == '\n')
        {
            printf("rot");
            t_data[0] = 4;
            i2c_master_transmit(I2C1,0x08,t_data,1);
            printf("I2C master transmit: rot toggeln\n");
            puffer[0]=0;
            puffer[1]=0;
            puffer[2]=0;
            t_data[0]=0;
            x=0;
        }

        if (puffer[0] == '1'&& byte == '\n')
        {
            printf("grün weiß");
            t_data[0] = 5;
            i2c_master_transmit(I2C1,0x08,t_data,1);
            printf("I2C master transmit: grün & weiß toggeln\n");
            puffer[0]=0;
            puffer[1]=0;
            puffer[2]=0;
            t_data[0]=0;
            x=0;
        }

        if (puffer[0] == '2' && byte == '\n')
        {
            printf("rot blau");
            t_data[0] = 6;
            i2c_master_transmit(I2C1,0x08,t_data,1);
            printf("I2C master transmit: rot & blau toggeln\n");
            puffer[0]=0;
            puffer[1]=0;
            puffer[2]=0;
            t_data[0]=0;
            x=0;
        }

        if (puffer[0] == 'c' && puffer[1] == 'h' && puffer[2] == 'c' && byte == '\n')
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

                case 3: //ALLE TOT!!!!11!!"
                    printf("I2C 0x08 master receive: ROT ist AUS BLAU ist AUS\n");
                break;

                case 4: //Rot ist an , Blau ist an
                    printf("I2C 0x08 master receive: ROT ist AN BLAU ist AN\n");
                break;

                default:  // Fehler
                    printf("I2C 0x08 master receive: Ungültiger Zustand\n");
                break;
            }


            i2c_master_receive(I2C1,0x09,r_data,1);

            printf("I2C master receive 0x09\n");

            switch(r_data[0])
            {
                case 1: //Weiß ist an , Grün ist aus
                    printf("I2C 0x09 master receive: WEIß ist AN GRÜN ist AUS\n");
                break;
                
                case 2: //Weiß ist aus , Grün ist an
                    printf("I2C 0x09 master receive: WEIß ist AUS GRÜN ist AN\n");
                break;

                case 3: //ALLE TOT !!!11!!1!!!!
                printf("I2C 0x09 master receive: WEIß ist AUS GRÜN ist AUS\n");
                break;

                case 4: //Weiß ist an , Grün ist an
                printf("I2C 0x09 master receive: WEIß ist AN GRÜN ist AN\n");
                break;

                default:  // Fehler
                    printf("I2C 0x09 master receive: Ungültiger Zustand\n");
                break;

            }

            r_data[0]=0;
            puffer[0]=0;
            puffer[1]=0;
            puffer[2]=0;
            puffer[3]=0;
            puffer[4]=0;
            x=0;
        }

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

            printf("0:%1c\n", puffer[0]);
            printf("1:%1c\n", puffer[1]);
            printf("2:%1c\n", puffer[2]);
            printf("3:%1c\n", puffer[3]);
            printf("3:%1c\n", puffer[4]);
            printf("%1i\n", x);




    }
}
