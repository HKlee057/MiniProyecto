//-----------------------------------------------------------------------------
// Hector Alejandro Klée González
// Carné 17118
// MiniProyecto
// Diseño e Innovación en Ingeniería 1
//-----------------------------------------------------------------------------
/* ----------------------------     Referencias     ---------------------------
 * https://github.com/YashBansod/ARM-TM4C-CCS/tree/master/TM4C123G%20LaunchPad%20PWM%20Servo
 * https://github.com/ntua-cslep/Motor-Control-with-Tiva-EK-TM4C123GXL
 * https://forum.43oh.com/topic/7171-using-hardware-pwm-on-tiva-launchpad/
 * https://github.com/mathmagson/microservo9g_tm4c123g/blob/master/main.c
 * https://www.ti.com/seclit/ml/ssqu015/ssqu015.pdf
 * https://www.digikey.com/eewiki/display/microcontroller/I2C+Communication+with+the+TI+Tiva+TM4C123GXL
 */
/* ----------------------------     Librería PWM        --------------------------------- */

#include <stdint.h>                         // Librería de Standard Integer Types
#include <stdbool.h>                        // Librería de  Standard Boolean Types
#include "inc/tm4c123gh6pm.h"               // Definiciones para interrupciones y registros en Tiva C
#include "inc/hw_memmap.h"                  // Macros definiendo la memory map de la Tiva C
#include "inc/hw_types.h"                   // Define types comunes y macros
#include "inc/hw_gpio.h"                    // Define Macros para hardware GPIO
#include "inc/hw_pwm.h"                     // Defines y Macros para puerto PWM
#include "driverlib/debug.h"                // Macros para asistir el debug del driverLib
#include "driverlib/pwm.h"                  // Funciones prototipo API function para puertos PWM
#include "driverlib/sysctl.h"               // Defines y macros para System Control API del driverLib
#include "driverlib/interrupt.h"            // Defines y macros para NVIC Controller API del driverLib
#include "driverlib/gpio.h"                 // Defines y macros para GPIO API del driverLib
#include "driverlib/pin_map.h"              // Mapeo de periféricos a pines
#include "driverlib/rom.h"                  // Defines y macros para ROM API del driverLib

#include "inc/hw_ints.h"
#include "driverlib/timer.h"

#define PWM_FREQUENCY 50                    // Define macro para la frecuencia del PWM en Hz

// -----------------------        Variable Globales       --------------------- */
uint32_t ui32Period;                        // Variable para el periodo de PWM
volatile uint8_t ui8Adjust = 75;            // Variable el Duty Cycle del Servo

uint8_t sens_data = 1;                      // Variable de control de sensor
uint8_t action;                             // Varaible de verificación de detección
uint8_t count;                              // Variable contadora de caso
// -----------------------      Funciones Prototipo     --------------------- */
void PortFIntHandler(void);                 // Prototipo ISR de GPIO PortF

/* -----------------------              Main            --------------------- */
int main(void){
    // Reloj del sistema a 80MHz y reloj de PWM a 1.25MHz
    SysCtlClockSet(SYSCTL_SYSDIV_2_5 |SYSCTL_USE_PLL |SYSCTL_OSC_MAIN |SYSCTL_XTAL_16MHZ);
    SysCtlPWMClockSet(SYSCTL_PWMDIV_64);

    // Habilitación de periféricos de Puerto F, Puerto D y PWM1
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    // Configurarción de PD_0 para la señal de PWM (modulo PWM 1, generador 0)
    GPIOPinTypePWM(GPIO_PORTD_BASE, GPIO_PIN_0);
    GPIOPinConfigure(GPIO_PD0_M1PWM0);

    // Desbloqueo de SW2
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= GPIO_PIN_0;
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;

    // PF_0 y PF_4 (SW1 y SW2) configurados como Input y Pulled-up
    GPIODirModeSet(GPIO_PORTF_BASE, GPIO_PIN_4|GPIO_PIN_0, GPIO_DIR_MODE_IN);
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4 | GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    // Habilitación de las entradas y salidas de Puertos
    GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_1);
    GPIOPadConfigSet(GPIO_PORTE_BASE, GPIO_PIN_1, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);

    // Configuración y habilitación de the Interrupción para PF_0 y PF_4 (SW1 y SW2)
    IntEnable(INT_GPIOF);
    GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_4 | GPIO_PIN_0, GPIO_FALLING_EDGE);
    GPIOIntEnable(GPIO_PORTF_BASE, GPIO_INT_PIN_0 | GPIO_INT_PIN_4);
    // Interrupción maestra
    IntMasterEnable();

    // Cálculo de periodo de Timer de PWM
    uint32_t ui32PWMClock = SysCtlClockGet() / 64;
    ui32Period = (ui32PWMClock / PWM_FREQUENCY) - 1;
    // Configuración del PWM_1 Genereador_0 como Count Down Mode
    PWMGenConfigure(PWM1_BASE, PWM_GEN_0, PWM_GEN_MODE_DOWN);
    // Carga del periodo calculado al módulo
    PWMGenPeriodSet(PWM1_BASE, PWM_GEN_0, ui32Period);

    // Cambio del Duty Cycle de PWM a valor específico (obtenido al presionar SW_1 o SW_2)
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_0, ui8Adjust * ui32Period / 1000);
    // Habilitaación de del pin PWM_0 de módulo PWM 1 como salida
    PWMOutputState(PWM1_BASE, PWM_OUT_0_BIT, true);
    // Habilitación del Generador PWM
    PWMGenEnable(PWM1_BASE, PWM_GEN_0);

    while (1){
        sens_data = GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_1);
        while(GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_1) == sens_data)
            {
                action = 1;
            }
        if (action == 1)
            {
                SysCtlDelay(500000);
                count++;
                if (count == 1)
                {
                    // Azul
                    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0x00);
                    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);
                    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3);
                }  else if (count == 2)
                {
                    // Negro
                    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0x00);
                    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0x00);
                    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0x00);
                    count = 0;
                }
                action = 0;
            };
    }
}

/* -------------------      Handler de Interrupción en Puerto F     --------------------- */
void PortFIntHandler(void){
    // ISR de Puerto F manejo de interrupciones
    GPIOIntClear(GPIO_PORTF_BASE , GPIO_INT_PIN_4 | GPIO_INT_PIN_0);

    // If SW1 se presiona, aumentar a la variable de control de Duty Cycle
    if(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4 | GPIO_PIN_0)== GPIO_PIN_0){
        //ui8Adjust++;
        ui8Adjust = ui8Adjust + 3;
        if (ui8Adjust > 125){
            ui8Adjust = 125;
        }
        PWMPulseWidthSet(PWM1_BASE, PWM_OUT_0, ui8Adjust * ui32Period / 1000);
    }
    // If SW2 se presiona, disminuir a la variable de control de Duty Cycle
    if(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4 | GPIO_PIN_0)== GPIO_PIN_4){
        //ui8Adjust--;
        ui8Adjust = ui8Adjust - 3;
        if (ui8Adjust < 35){
            ui8Adjust = 35;
        }
        PWMPulseWidthSet(PWM1_BASE, PWM_OUT_0, ui8Adjust * ui32Period / 1000);
    }
}
