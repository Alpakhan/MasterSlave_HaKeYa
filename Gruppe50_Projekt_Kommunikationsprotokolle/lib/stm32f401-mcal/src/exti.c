#include "exti.h"
#include "gpio.h"

void exti_enable_syscfg_clock()
{
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
}

void exti_set_source(EXTI_LINE_t line, GPIO_TypeDef *port)
{
    uint8_t mask = 0x0;
    if (GPIOA == port)
    {
        mask = 0x0;
    }
    else if (GPIOB == port)
    {
        mask = 0x1;
    }    
    else if (GPIOC == port)
    {
        mask = 0x2;
    }    
    else if (GPIOD == port)
    {
        mask = 0x3;
    } 
    else if (GPIOE == port)
    {
        mask = 0x4;
    } 
    else if (GPIOH == port)
    {
        mask = 0x7;
    } 

    uint8_t reg = line / 4;
    uint8_t shift = (line % 4) * 4;
    SYSCFG->EXTICR[reg] |= (mask << shift);
}

void exti_enable_irq(EXTI_LINE_t line)
{
    EXTI->IMR |= (1 << line);
}

void exti_disable_irq(EXTI_LINE_t line)
{
    EXTI->IMR &= ~(1 << line);
}

void exti_set_trigger_edge(EXTI_LINE_t line, EXTI_TRIGGER_t trigger)
{
    if (EXTI_RISING_EDGE == trigger)
    {
        EXTI->RTSR |= (1 << line);      // Enable rising edge
        EXTI->FTSR &= ~(1 << line);   // Disable falling edge
    }
    else if (EXTI_FALLING_EDGE == trigger)
    {
        EXTI->FTSR |= (1 << line);      // Enable falling edge
        EXTI->RTSR &= ~(1 << line);   // Disable rising edge
    }
    else if (EXTI_RISING_AND_FALLING == trigger)
    {
        EXTI->RTSR |= (1 << line);      // Enable rising edge
        EXTI->FTSR |= (1 << line);      // Enable falling edge
    }

}

bool exti_is_irq_pending(EXTI_LINE_t line)
{
    return (EXTI->PR & (1 << line));
}

void exti_reset_pending_bit(EXTI_LINE_t line)
{
    EXTI->PR |= (1 << line);
}
