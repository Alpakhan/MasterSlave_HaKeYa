#ifndef RDF_MCAL_I2C_H
#define RDF_MCAL_I2C_H

#include <stm32f4xx.h>
#include <stdbool.h>


#define I2C_TIMEOUT 10000

typedef enum {
	I2C_STANDARD,
	I2C_FAST
} I2C_Mode_Type;

typedef enum {
	I2C_TRANSMITTER,
	I2C_RECEIVER
} I2C_Direction_Type;

typedef enum {
	I2C_ACK,
	I2C_NACK
} I2C_Acknowledge_Type;

typedef enum {
	I2C_ERROR = 0x0,
	I2C_OK
} I2C_Status_Type;


void i2c_enable_clock(I2C_TypeDef *i2c);
void i2c_enable(I2C_TypeDef *i2c);
void i2c_disable(I2C_TypeDef *i2c);
void i2c_reset(I2C_TypeDef *i2c);
void i2c_enable_isr(I2C_TypeDef *i2c);
void i2c_disable_isr(I2C_TypeDef *i2c);
void i2c_set_own_address(I2C_TypeDef *i2c, uint8_t address);

void i2c_start(I2C_TypeDef *i2c);
void i2c_write(I2C_TypeDef *i2c, uint8_t data);
I2C_Status_Type i2c_send_address(I2C_TypeDef *i2c, uint8_t address, I2C_Direction_Type direction);
void i2c_stop(I2C_TypeDef *i2c);


void i2c_pin_setup(I2C_TypeDef *i2c);
void i2c_master_setup(I2C_TypeDef *i2c, I2C_Mode_Type mode);
void i2c_slave_setup(I2C_TypeDef *i2c, I2C_Mode_Type mode, uint8_t addr);

I2C_Status_Type i2c_master_transmit(I2C_TypeDef *i2c, uint8_t dev_addr, uint8_t* data, uint16_t size);
I2C_Status_Type i2c_master_receive(I2C_TypeDef *i2c, uint8_t dev_addr, uint8_t* data, uint16_t size);
I2C_Status_Type i2c_slave_transmit(I2C_TypeDef *i2c, uint8_t* data, uint16_t size);
I2C_Status_Type i2c_slave_receive(I2C_TypeDef *i2c,uint8_t* data, uint16_t size);

I2C_Status_Type i2c_mem_write(I2C_TypeDef *i2c, uint8_t dev_addr, uint8_t mem_addr, uint8_t* data, uint16_t size);
I2C_Status_Type i2c_mem_read(I2C_TypeDef *i2c,uint8_t dev_addr, uint8_t mem_addr, uint8_t* data, uint16_t size);

#endif //RDF_MCAL_I2C_H