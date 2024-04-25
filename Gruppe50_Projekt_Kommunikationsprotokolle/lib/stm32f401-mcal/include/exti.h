/* exti.h is part of the rdf_mcal lib */

#ifndef RDF_MCAL_EXTI_H
#define RDF_MCAL_EXTI_H

#include <stm32f4xx.h>
#include <stdbool.h>


typedef enum
{
    EXTI_LINE_0,
    EXTI_LINE_1,
    EXTI_LINE_2,
    EXTI_LINE_3,
    EXTI_LINE_4,
    EXTI_LINE_5,
    EXTI_LINE_6,
    EXTI_LINE_7,
    EXTI_LINE_8,
    EXTI_LINE_9,
    EXTI_LINE_10,
    EXTI_LINE_11,
    EXTI_LINE_12,
    EXTI_LINE_13,
    EXTI_LINE_14,
    EXTI_LINE_15,
} EXTI_LINE_t;

typedef enum
{
    EXTI_RISING_EDGE = 0,
    EXTI_FALLING_EDGE,
    EXTI_RISING_AND_FALLING
} EXTI_TRIGGER_t;


/* Enable the clock for the system configuration controller */
void exti_enable_syscfg_clock();

/* Set the input source (port) which should be connected to the given EXIT*/
void exti_set_source(EXTI_LINE_t line, GPIO_TypeDef *port);

/* Enable EXTI IRQ */
void exti_enable_irq(EXTI_LINE_t line);

/* Disbale EXTI IRQ */
void exti_disable_irq(EXTI_LINE_t line);

/* Set trigger edge */
void exti_set_trigger_edge(EXTI_LINE_t line, EXTI_TRIGGER_t trigger);

/* Check if EXTI IRQ is pending */
bool exti_is_irq_pending(EXTI_LINE_t line);

/* Reset pending IRQ Bit */
void exti_reset_pending_bit(EXTI_LINE_t line);

#endif // RDF_MCAL_EXTI_H