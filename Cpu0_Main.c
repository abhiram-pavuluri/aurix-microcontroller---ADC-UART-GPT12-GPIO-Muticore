/**********************************************************************************************************************
 * \name          Task 3       - Using Multicore for Task 1 and Task 2
 * \version       V1.0
 * \board         APPLICATION KIT TC3X7 V2.0, KIT_A2G_TC397_5V_TFT, TC39xXX_B-Step
 * \Lead          Abhiram Pavuluri                    Z0236953
 * \Team          Swami Venkata Dinesh Dadi           Z0235609
 *                Spandana Kandala                    Z0235599
 *                Sunil Manikanta Kumar Buddiga       Z0235604
 *                Tirumala Devi Malothu               Z0235611
 *                VVS ChandraSekhara Yadavalli        Z0236956
 *                Santhoshini Shivani Kondapally      Z0236630
 *
*********************************************************************************************************************
                                                     cpu0_Main.c
*********************************************************************************************************************
 /*\title         ADC single channel conversion (Task 1)
 * \abstract      The Enhanced Versatile Analog-to-Digital Converter (EVADC) is configured to measure an analog signal using queued request.
 * \description   An analog input channel is continuously converted using the queued mode.
 *                The input value is determined using the microcontroller's supply voltage, ground level or letting
 *                the analog pin open and floating. Two LEDs are used, indicating ADC Running Process and low limit of ADC Value.
 *
 * \documents     <NEED TO BE UPDATED>
 * \lastUpdated   04-08-2023
 *********************************************************************************************************************/
#include "Ifx_Types.h"
#include "IfxCpu.h"
#include "IfxScuWdt.h"
#include "ADC_Single_Channel.h"
#include "UART_VCOM.h"
#include "GPT12_Timer_Interrupt.h"

IFX_ALIGN(4) IfxCpu_syncEvent g_cpuSyncEvent = 0;

void core0_main(void)
{
    IfxCpu_enableInterrupts();
    char volBuff[255];
    int vol =56;
    
    /* !!WATCHDOG0 AND SAFETY WATCHDOG ARE DISABLED HERE!!
     * Enable the watchdogs and service them periodically if it is required
     */
    IfxScuWdt_disableCpuWatchdog(IfxScuWdt_getCpuWatchdogPassword());
    IfxScuWdt_disableSafetyWatchdog(IfxScuWdt_getSafetyWatchdogPassword());
    
    /* Wait for CPU sync event */
    IfxCpu_emitEvent(&g_cpuSyncEvent);
    IfxCpu_waitEvent(&g_cpuSyncEvent, 1);
    
    init_UART();                                    // Initializing UART Module
    initEVADC();                                    // Initializing EVADC
    initializeLEDs();                               // Initializing LEDs 13.0, 13.1, 13.2, 13.3
    initGpt12Timer();                               // Initializing GPT12 Timer

    /* Continuously update the conversion results */
    while(1)
    {
        readEVADC();
        indicateConversionValue();
    }
}
