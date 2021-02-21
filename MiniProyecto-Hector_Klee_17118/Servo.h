//-----------------------------------------------------------------------------
// Hector Alejandro Klée González
// Carné 17118
// MiniProyecto
// Diseño e Innovación en Ingeniería 1
//-----------------------------------------------------------------------------

#ifndef SERVO_H_
#define SERVO_H_
/* -----------------------------        Librerias       ----------------------------- */
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "inc/hw_gpio.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "driverlib/fpu.h"
#include "driverlib/debug.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "driverlib/pwm.h"
#include "driverlib/timer.h"
#include "driverlib/adc.h"

#include "utils/uartstdio.h"
#include "utils/cmdline.h"
#include "driverlib/uart.h"
#include "driverlib/interrupt.h"

#include "delay_timer.h"

// ------------------------      Funciones Prototipo     --------------------- */
void
InitServo(void);

void
ConfigureServo(void);

void
SetServoPosition(uint32_t position);

void
SetServoAngle(uint32_t angle);

void
pwm_init(void);                       // Iniciliazacion de modulo PWM

void
angle_get(void);

#endif /* SERVO_H_ */
