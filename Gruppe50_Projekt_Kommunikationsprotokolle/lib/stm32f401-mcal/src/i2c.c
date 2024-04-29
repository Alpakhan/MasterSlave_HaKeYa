#include "i2c.h"
#include "gpio.h"

// Macros to check status register
// adopted from https://gitlab.com/rjesse/mcal-stm
#define I2C_WAIT_BUSY(i2c) ({ while (i2c->SR2 & I2C_SR2_BUSY) ; })
#define I2C_START_COMPLETED(i2c) ({ while (!(i2c->SR1 & I2C_SR1_SB)) ; })
#define I2C_STOPP_COMPLETED(i2c) ({ while (!(i2c->SR1 & I2C_SR1_STOPF)) ; })
#define I2C_ADDRESS_COMPLETED(i2c, timeout) ({ while ((!(i2c->SR1 & I2C_SR1_ADDR))  && --timeout ) ; })
#define I2C_DUMMY_READ_SR1(i2c) ({ i2c->SR1; })
#define I2C_DUMMY_READ_SR2(i2c) ({ i2c->SR2; })
#define I2C_CHECK_TXBUF_EMPTY(i2c) ({ while(!(i2c->SR1 & I2C_SR1_TXE)) ; })
#define I2C_CHECK_RXBUF_NOT_EMPTY(i2c) ({ while(!(i2c->SR1 & I2C_SR1_RXNE)) ; })
#define I2C_BYTE_TRANSFER_FINISHED(i2c) ({ while(!(i2c->SR1 & I2C_SR1_BTF)) ; })
#define I2C_RESET_ACK(i2c) ({ i2c->CR1 &= ~I2C_CR1_ACK_Msk; })
#define I2C_SET_ACK(i2c) ({ i2c->CR1 |= I2C_CR1_ACK; })
#define I2C_SET_POS(i2c) ({ i2c->CR1 |= I2C_CR1_POS; })
#define I2C_RESET_POS(i2c) ({ i2c->CR1 &= ~I2C_CR1_POS_Msk; })
#define I2C_START(i2c) ({ i2c->CR1 |= I2C_CR1_START; })
#define I2C_STOP(i2c) ({ i2c->CR1 |= I2C_CR1_STOP; })

// only I2C1 supported yet.
void i2c_pin_setup(I2C_TypeDef *i2c)
{
    if (I2C1 == i2c)
    {
        // activate GPIO Port B, because I2C1 uses PB8/PB9
        gpio_enable_port_clock(GPIOB);
        gpio_set_mode(GPIOB, 8, ALTFUNC);
        gpio_set_af(GPIOB, 8, AF4);
        gpio_set_mode(GPIOB, 9, ALTFUNC);
        gpio_set_af(GPIOB, 9, AF4);
        // use opendrain output for I2C
        gpio_set_output_type(GPIOB, 8, OPENDRAIN);
        gpio_set_output_type(GPIOB, 9, OPENDRAIN);

        gpio_set_speed(GPIOB, 8, HIGH_SPEED);
        gpio_set_speed(GPIOB, 9, HIGH_SPEED);
    }
}

void i2c_enable_clock(I2C_TypeDef *i2c)
{
    // Activate bus clock
    if (I2C1 == i2c)
    {
        RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
    }
    else if (I2C2 == i2c)
    {
        RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;
    }
    else if (I2C3 == i2c)
    {
        RCC->APB1ENR |= RCC_APB1ENR_I2C3EN;
    }
}

void i2c_set_freq(I2C_TypeDef *i2c, uint32_t freq)
{
    i2c->CR2 = freq; // set peripherie clock frequency // can't be higher then APB1
}

void i2c_set_mode(I2C_TypeDef *i2c, I2C_Mode_Type mode)
{

    i2c->CCR = 0x00; // Reset Clock Control Register
    if (I2C_STANDARD == mode)
    {
        i2c->CCR &= ~I2C_CCR_FS_Msk; // Select 100 kHz bus clock
        i2c->CCR |= 0x0050;          // Bei 16 MHz  CCR =
    }
    else
    {
        i2c->CCR |= I2C_CCR_FS; // Select 400 kHz bus clock
    }

    i2c->TRISE = 0x0011; // Set max. rise time
}

inline void i2c_enable(I2C_TypeDef *i2c)
{
    i2c->CR1 |= I2C_CR1_PE;
}

inline void i2c_disable(I2C_TypeDef *i2c)
{
    i2c->CR1 &= ~I2C_CR1_PE_Msk;
}

void i2c_reset(I2C_TypeDef *i2c)
{
    i2c->CR1 |= I2C_CR1_SWRST;      // enter reset state
    i2c->CR1 &= ~I2C_CR1_SWRST_Msk; // back to normal operation
}

void i2c_set_own_address(I2C_TypeDef *i2c, uint8_t address)
{
    i2c->OAR1 = (address << 1) | (1 << 14); // Datasheet: Keep bit 14 at 1 by software
}

// Send the START condition
// Wait for the SB ( Bit 0 in SR1) to set. This indicates that the start condition is generated
void i2c_start(I2C_TypeDef *i2c)
{
    I2C_START(i2c);           // Generate the START Condition
    I2C_START_COMPLETED(i2c); // Wait until START signal has been sent
}

// Wait for the TXE (bit 7 in SR1) to set. This indicates that the DR is empty
// Send the DATA to the DR Register
// Wait for the BTF (bit 2 in SR1) to set. This indicates the end of LAST DATA transmission
void i2c_write(I2C_TypeDef *i2c, uint8_t data)
{
    I2C_CHECK_TXBUF_EMPTY(i2c); // Wait until the transmit buffer is empty
    i2c->DR = data;
    //I2C_BYTE_TRANSFER_FINISHED(i2c); // Wait until BTF Flag is set
}

// Shift addr 1 to the left.
// Set bit 8 of the addr byte to indicate read or write, depending on the I2C_Direction_Type
// Send the Slave Address to the DR Register
// Wait for the ADDR (bit 1 in SR1) to set. This indicates the end of address transmission
// clear the ADDR by reading the SR1 and SR2
I2C_Status_Type i2c_send_address(I2C_TypeDef *i2c, uint8_t address, I2C_Direction_Type direction)
{
    uint32_t timeout = I2C_TIMEOUT;

    address = address << 1;
    if (I2C_TRANSMITTER == direction)
    {
        address &= ~(1 << 0);
    }
    else if (I2C_RECEIVER == direction)
    {
        address |= (1 << 0);
    }

    i2c->DR = address;                   // Send address
    I2C_ADDRESS_COMPLETED(i2c, timeout); // Wait for ADDR ACK

    I2C_DUMMY_READ_SR1(i2c); // Reset SR1
    I2C_DUMMY_READ_SR2(i2c); // Reset SR2

    // check ack error: example: if slave not available.
    if ((I2C1->SR1 & I2C_SR1_AF) || timeout <= 0)
    {
        return I2C_ERROR;
    }

    return I2C_OK;
}

void i2c_stop(I2C_TypeDef *i2c)
{
    I2C_STOP(i2c);
}

void i2c_enable_isr(I2C_TypeDef *i2c)
{
    i2c->CR2 |= I2C_CR2_ITBUFEN;
    i2c->CR2 |= I2C_CR2_ITEVTEN;
}

void i2c_disable_isr(I2C_TypeDef *i2c)
{
    i2c->CR2 &= ~I2C_CR2_ITBUFEN_Msk;
    i2c->CR2 &= ~I2C_CR2_ITEVTEN_Msk;
}

void i2c_master_setup(I2C_TypeDef *i2c, I2C_Mode_Type mode)
{
    i2c_pin_setup(i2c); // only I2C1
    i2c_enable_clock(i2c);
    i2c_disable(i2c); // disable i2c component
    i2c_set_freq(i2c, I2C_CR2_FREQ_4);
    i2c_set_mode(i2c, mode);
    i2c_enable(i2c); // enable i2c component
}

void i2c_slave_setup(I2C_TypeDef *i2c, I2C_Mode_Type mode, uint8_t addr)
{
    i2c_pin_setup(i2c); // only I2C1
    i2c_enable_clock(i2c);
    i2c_disable(i2c); // disable i2c component
    i2c_set_freq(i2c, I2C_CR2_FREQ_4);
    i2c_set_mode(i2c, mode);
    i2c_set_own_address(i2c, addr);
    i2c_enable(i2c); // enable i2c component
}

I2C_Status_Type i2c_master_transmit(I2C_TypeDef *i2c, uint8_t addr, uint8_t *data, uint16_t size)
{
    I2C_WAIT_BUSY(i2c); // Checks whether the I2C bus is busy
    i2c_start(i2c);

    if (I2C_ERROR == i2c_send_address(i2c, addr, I2C_TRANSMITTER))
    {
        i2c_stop(i2c);
        return I2C_ERROR;
    }

    while (size-- > 0)
    {
        i2c_write(i2c, *data++);
    }

    i2c_stop(i2c); // Send STOP signal

    return I2C_OK;
}

I2C_Status_Type i2c_master_receive(I2C_TypeDef *i2c, uint8_t addr, uint8_t *data, uint16_t size)
{
    I2C_WAIT_BUSY(i2c); // Checks whether the I2C bus is busy

    i2c_start(i2c); // Generate I2C RESTART
    if (I2C_ERROR == i2c_send_address(i2c, addr, I2C_RECEIVER))
    {
        i2c_stop(i2c);
        return I2C_ERROR;
    }

    I2C_SET_ACK(i2c); // Enable Acknowledge

    while (size > 0) // Start reading multiple values
    {
        if (size == 1U) // If there is only one byte left...
        {
            I2C_RESET_ACK(i2c);             // Disable acknowledge
            I2C_STOP(i2c);                  // Generate STOP signal
            I2C_CHECK_RXBUF_NOT_EMPTY(i2c); // Wait until receive buffer is no longer empty
            *data++ = i2c->DR;              // Read data from data register
            break;
        }
        else // More than one byte left
        {
            I2C_CHECK_RXBUF_NOT_EMPTY(i2c); // Wait until receive buffer is no longer empty
            (*data++) = i2c->DR;            // Read data from data register
            size--;
        }
    }
    return I2C_OK;
}

I2C_Status_Type i2c_slave_transmit(I2C_TypeDef *i2c, uint8_t *data, uint16_t size)
{
    // TODO;
    return I2C_OK;
}

// blocking
I2C_Status_Type i2c_slave_receive(I2C_TypeDef *i2c, uint8_t *data, uint16_t size)
{
    uint32_t timeout = I2C_TIMEOUT;

    I2C_SET_ACK(i2c); // Enable addr acknowledge

    I2C_ADDRESS_COMPLETED(i2c, timeout);

    if (timeout <= 0)
    {
        return I2C_ERROR;
    }

    // clear addr bit
    I2C_DUMMY_READ_SR1(i2c);
    I2C_DUMMY_READ_SR2(i2c);

    while (size > 0)
    {

        I2C_CHECK_RXBUF_NOT_EMPTY(i2c);

        (*data++) = i2c->DR; // read data from DR
        size--;

        // If RxNE is set and the data in the DR register is not read before the end of the next data reception,
        // the BTF bit is set and the interface waits until BTF is cleared by a read from the I2C_DR register, stretching SCL low
        if ((i2c->SR1 & I2C_SR1_BTF) && (0 != size))
        {
            (*data++) = i2c->DR; // read data from DR
            size--;
        }
    }

    I2C_STOPP_COMPLETED(i2c);

    // clear stop flag
    I2C_DUMMY_READ_SR1(i2c);
    i2c->CR1 |= I2C_CR1_PE;

    // disable addr acknowledge
    I2C_RESET_ACK(i2c);

    return I2C_OK;
}

I2C_Status_Type i2c_mem_write(I2C_TypeDef *i2c, uint8_t dev_addr, uint8_t mem_addr, uint8_t *data, uint16_t size)
{
    I2C_WAIT_BUSY(i2c); // Checks whether the I2C bus is busy
    i2c_start(i2c);

    if (I2C_ERROR == i2c_send_address(i2c, dev_addr, I2C_TRANSMITTER))
    {
        i2c_stop(i2c);
        return I2C_ERROR;
    }

    i2c_write(i2c, mem_addr);

    while (size-- > 0)
    {
        i2c_write(i2c, *data++);
    }

    i2c_stop(i2c); // Send STOP signal

    return I2C_OK;
}

I2C_Status_Type i2c_mem_read(I2C_TypeDef *i2c, uint8_t dev_addr, uint8_t mem_addr, uint8_t *data, uint16_t size)
{
    I2C_WAIT_BUSY(i2c); // Checks whether the I2C bus is busy

    i2c_start(i2c); // Generate I2C START
    if (I2C_ERROR == i2c_send_address(i2c, dev_addr, I2C_TRANSMITTER))
    {
        i2c_stop(i2c);
        return I2C_ERROR;
    }
    i2c_write(i2c, mem_addr);

    i2c_start(i2c); // RESTART;

    if (I2C_ERROR == i2c_send_address(i2c, dev_addr, I2C_RECEIVER))
    {
        i2c_stop(i2c);
        return I2C_ERROR;
    }

    I2C_SET_ACK(i2c); // Enable Acknowledge

    while (size > 0) // Start reading multiple values
    {
        if (size == 1U) // If there is only one byte left...
        {
            I2C_RESET_ACK(i2c);             // Disable acknowledge
            I2C_STOP(i2c);                  // Generate STOP signal
            I2C_CHECK_RXBUF_NOT_EMPTY(i2c); // Wait until receive buffer is no longer empty
            *data++ = i2c->DR;              // Read data from data register
            break;
        }
        else // More than one byte left
        {
            I2C_CHECK_RXBUF_NOT_EMPTY(i2c); // Wait until receive buffer is no longer empty
            (*data++) = i2c->DR;            // Read data from data register
            size--;
        }
    }
    return I2C_OK;
}