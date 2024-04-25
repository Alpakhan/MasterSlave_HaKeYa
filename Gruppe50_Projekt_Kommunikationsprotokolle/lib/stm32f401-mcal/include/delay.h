/* gpio.h is part of the rdf_mcal lib */

#ifndef RDF_MCAL_SYSTICK_H
#define RDF_MCAL_SYSTICK_H

#include <stm32f4xx.h>
#include <inttypes.h>
#include <stdbool.h>



typedef struct {
	uint32_t delay;
	uint32_t timestamp;
} Delay_TypeDef;


// SysTick Delay (only ms)

/* 
	Init the systick timer to call its ISR every ms 
*/
void systick_init();
/*
    Blocking delay_ms function using the SysTick timer.
    Need to call systick_init() once to work correctly.
*/
void systick_delay_ms(uint32_t ms);
/* 
	Settting up a nonblocking timer.
	The delay argument must be in milliseconds.
 	Need to call systick_init() once to work correctly.
	To check if timer is expired use systick_is_timer_expired()
 */
void systick_set_timer(uint32_t *timer, uint16_t delay);
/* 
	Check if a given timer is expired.
	The timer argument should be set by calling systick_set_timer()
	Need to call systick_init() once to work correctly.
*/
bool systick_is_timer_expired(uint32_t *timer);

// DWT Delay (ms, us)
void DWT_init(void);
uint32_t DWT_tick(void);
uint32_t DWT_millis(void);
uint32_t DWT_micros(void);
void DWT_delay_ms(uint32_t delay);
void DWT_delay_us(uint32_t delay);
// DWT Non Blocking Delay (ms, us)
uint8_t DWT_nb_timeout(Delay_TypeDef *dt);
void DWT_nb_delay_ms(Delay_TypeDef *dt, uint32_t delay);
void DWT_nb_delay_us(Delay_TypeDef *dt, uint32_t delay);

#endif // RDF_MCAL_SYSTICK_H