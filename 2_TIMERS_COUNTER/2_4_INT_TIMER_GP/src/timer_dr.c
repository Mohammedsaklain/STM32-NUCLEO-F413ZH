#include "rcc.h"
#include "timer_dr.h"
#include "stm32f0xx.h"
#include "gpio.h"


TIM_TypeDef* Get_Timer_No(uint8_t timer_no)
{
    TIM_TypeDef * TIMER; 
    switch(timer_no)
    {
        case T1:     TIMER = TIM1;break;
        case T2:     TIMER = TIM2;break;
        case T3:     TIMER = TIM3;break;
        case T6:     TIMER = TIM6;break;
        case T14:    TIMER = TIM14;break;
        case T15:    TIMER = TIM15;break;
        case T16:    TIMER = TIM16;break;
        case T17:    TIMER = TIM17;break;
    }
    return TIMER;
}

/* Delay in Micro Seconds*/
void Delay_uS(uint8_t timer_no, uint16_t us)
{
    TIM_TypeDef * TIMER = Get_Timer_No(timer_no);
    TIMER->CNT = 0;                  // First reset the count
    while(TIMER->CNT <us);           // Wait till desired count value reached
}

/* Delay in milli Seconds*/
void Delay_mS(uint8_t timer_no,uint16_t ms)
{
    TIM_TypeDef * TIMER = Get_Timer_No(timer_no);
    for(uint16_t i=0;i<ms;i++)
    {
        Delay_uS(timer_no,1000);             // Delay for 1ms
    }
}

/* TO initialize Timer: 
Arguments : timer_no = TIM2 - Timer Number
            INT      = TRUE - Enables the Interrupt*/

void Timer_Start(uint8_t timer_no, uint8_t INT)
{
    /* When we need Interrupt based Timer Operation*/
    if(INT==TRUE)
    {
        TIM_TypeDef * TIMER = Get_Timer_No(timer_no);
        TIMER->PSC = 7999;                      // Prescaling by 8000, 8MHz/8000 = 1KHz => gives us to 1mS
        TIMER->ARR = 1000;                      // Timer Reload value based on Calculation ((1k x 8k)/8M)=1Sec
        TIMER->CNT = 0;                         // Reset the count
        TIMER->CR1 |= TIM_CR1_CEN;              // Enable Tim Counter
        TIMER->DIER |= TIM_DIER_UIE;            // Enable the Timer Interrupt
        Timer_Irq_Start(timer_no);   
    }
    /* Default, 1uS*/
    else
    {
        TIM_TypeDef * TIMER = Get_Timer_No(timer_no);
        TIMER->PSC = 7;                      // Prescaling by 8, 8MHz/8 = 1MHz => gives us to 1uS
        TIMER->ARR = 0xFFFF;                 // Timer Reload value based on Calculation ((1uSec x 8M)/8)=1uSec
        TIMER->CNT = 0;                      // Reset the count
        TIMER->CR1 |= TIM_CR1_CEN;           // Enable Tim Counter
        TIMER->DIER &= ~TIM_DIER_UIE;        // Disable the Timer Interrupt
    }
}

/* To stop Timer*/
void Timer_Stop(uint8_t timer_no)
{
    TIM_TypeDef * TIMER = Get_Timer_No(timer_no);
    TIMER->CR1 &= ~(TIM_CR1_CEN);            // Disable the timer
}

/* To enable the Timer Interrupt */
void Timer_Irq_Start(uint8_t timer_no)
{
    TIM_TypeDef * TIMER = Get_Timer_No(timer_no);
    __disable_irq();
    switch(timer_no)
    {
        case T1:     NVIC_EnableIRQ(TIM1_BRK_UP_TRG_COM_IRQn);break;
        case T2:     NVIC_EnableIRQ(TIM2_IRQn);break;
        case T3:     NVIC_EnableIRQ(TIM3_IRQn);break;
        case T6:     NVIC_EnableIRQ(TIM6_DAC_IRQn);break;
        case T14:    NVIC_EnableIRQ(TIM14_IRQn);break;
        case T15:    NVIC_EnableIRQ(TIM15_IRQn);break;
        case T16:    NVIC_EnableIRQ(TIM16_IRQn);break;
        case T17:    NVIC_EnableIRQ(TIM17_IRQn);break;
    }
    __enable_irq();
}

/* To Disable the Timer Interrupt */
void Timer_Irq_Stop(uint8_t timer_no)
{
    TIM_TypeDef * TIMER = Get_Timer_No(timer_no);
    TIMER->DIER &= ~(TIM_DIER_UIE);       //Disable the Interrupt
    __disable_irq();
    switch(timer_no)
    {
        case T1:     NVIC_DisableIRQ(TIM1_BRK_UP_TRG_COM_IRQn);break;
        case T2:     NVIC_DisableIRQ(TIM2_IRQn);break;
        case T3:     NVIC_DisableIRQ(TIM3_IRQn);break;
        case T6:     NVIC_DisableIRQ(TIM6_DAC_IRQn);break;
        case T14:    NVIC_DisableIRQ(TIM14_IRQn);break;
        case T15:    NVIC_DisableIRQ(TIM15_IRQn);break;
        case T16:    NVIC_DisableIRQ(TIM16_IRQn);break;
        case T17:    NVIC_DisableIRQ(TIM17_IRQn);break;
    }
    __enable_irq();                       // Turnig On, inorder to prevent other IRQ's to be stopped
}
