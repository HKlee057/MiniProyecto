//-----------------------------------------------------------------------------
// Hector Alejandro Klée González
// Carné 17118
// MiniProyecto
// Diseño e Innovación en Ingeniería 1
//-----------------------------------------------------------------------------

#include "Sensor.h"                         // Libreria creada para control de sensor de interferencia/ seguidor de linea
/* ------------------------------     Variables        ---------------------------- */
uint8_t sens_data = 1;                      // Variable de control de sensor
uint8_t action;                             // Varaible de verificación de detección
uint8_t count;                              // Variable contadora de caso

/* -------------------      Inicializacion de pines de Sensor     --------------------- */
void
sens_init(void)
{
    // Reloj del sistema a 80MHz y reloj de PWM a 1.25MHz
    SysCtlClockSet(SYSCTL_SYSDIV_2_5 |SYSCTL_USE_PLL |SYSCTL_OSC_MAIN |SYSCTL_XTAL_16MHZ);
    SysCtlPWMClockSet(SYSCTL_PWMDIV_64);

    // Habilitación de periféricos de Puerto E, Puerto F
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    // Habilitación de las entradas y salidas de Puertos
    GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_1);
    GPIOPadConfigSet(GPIO_PORTE_BASE, GPIO_PIN_1, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
}

/* ----------------      Cambio de color en LED al haber deteccion     --------------------- */
void
colors(void)
{
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
        }
}
