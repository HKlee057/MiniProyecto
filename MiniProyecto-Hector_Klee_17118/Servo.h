//-----------------------------------------------------------------------------
// Hector Alejandro Klée González
// Carné 17118
// MiniProyecto
// Diseño e Innovación en Ingeniería 1
//-----------------------------------------------------------------------------

#ifndef SERVO_H_
#define SERVO_H_
/* -----------------------------        Librerias       ----------------------------- */
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

// ------------------------      Funciones Prototipo     --------------------- */
void
portF_int_handler(void);                 // Prototipo ISR de GPIO PortF

void
pwm_init(void);                       // Iniciliazacion de modulo PWM

#endif /* SERVO_H_ */
