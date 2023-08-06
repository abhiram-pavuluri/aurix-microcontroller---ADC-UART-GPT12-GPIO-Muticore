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
                                                     cpu1_Main.c
*********************************************************************************************************************
 /*\title         Switching two LEDs 13.0 & 13.3 using two switches connected to 14.6 & 14.8 (Task 2)
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
#include "ifxport.h"
#include <stdint.h>
extern IfxCpu_syncEvent g_cpuSyncEvent;

void core1_main(void)
{
    uint32_t i;
    IfxCpu_enableInterrupts();
    
    /* !!WATCHDOG0 AND SAFETY WATCHDOG ARE DISABLED HERE!!
     * Enable the watchdogs and service them periodically if it is required
     */
    IfxScuWdt_disableCpuWatchdog(IfxScuWdt_getCpuWatchdogPassword());
    IfxScuWdt_disableSafetyWatchdog(IfxScuWdt_getSafetyWatchdogPassword());
    
    /* Wait for CPU sync event */
    IfxCpu_emitEvent(&g_cpuSyncEvent);
    IfxCpu_waitEvent(&g_cpuSyncEvent, 1);
    Ifx_P *port=&MODULE_P13;
    Ifx_P *port1=&MODULE_P14;
    
    port->IOCR0.U=0X000;
    port->IOCR0.U=0x80000080;   // Configure Port13 0&3 as input lines
    port1->IOCR8.U=0x0;
    port1->IOCR4.U=0x0;
    port1->IOCR8.U=(1<<4);      // Configure Port14 8 as output line
    port1->IOCR4.U=(1<<20);     // Configure Port14 6 as output line
    while(1)
    {
        port -> OMR.U = 0x0;
        if((port1->IN.U)&(1<<8)){       // Read P8(SW1) from IN
            for(i=0;i<1000000;i++);
            port->OMR.U = (1<<0);       // Set PS0(LED 13.0 ON) on OMR
        }
        else{
            for(i=0;i<1000000;i++);
            port->OMR.U = (1<<16);      // Set PCL0(LED 13.0 OFF) on OMR
        }

        if((port1->IN.U)& (1<<6)){      // Read P6(SW2) from IN
            for(i=0;i<1000000;i++);
            port->OMR.U = (1<<3);       // Set PCL3(LED 13.3 ON) on OMR
        }
        else{
            for(i=0;i<1000000;i++);
            port->OMR.U = (1<<19);      // Set PCL3(LED 13.3 OFF) on OMR
        }
    }
}

