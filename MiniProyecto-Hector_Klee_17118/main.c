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
#include "inc/hw_ints.h"                    // Libreria de interrupciones
#include "driverlib/timer.h"                // Libreria de Timer

#include "Servo.h"                          // Libreria creada para control de servo en PWM1, Gen0
#include "Sensor.h"                         // Libreria creada para control de sensor de interferencia/ seguidor de linea
#include "UART.h"                           // Libreria creada para UART
#include "delay_timer.h"
/* -----------------------              Main            --------------------- */
int main(void)
{
    uart_init();
    pwm_init();
    sens_init();

    while (1)
    {
        colors();
        delay(100);
        angle_get();
    }
}
