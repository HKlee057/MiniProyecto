//-----------------------------------------------------------------------------
// Hector Alejandro Klée González
// Carné 17118
// MiniProyecto
// Diseño e Innovación en Ingeniería 1
//-----------------------------------------------------------------------------
#include "UART.h"                           // Libreria creada para UART

/* ------------------------------     Variables        ---------------------------- */
#define UART0_BAUDRATE  115200              // Macro for UART0 Baud rate

/* -------------------      Inicializacion de UART0     --------------------- */
void
uart_init(void)
{
    /// Reloj del sistema a 80MHz
    SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    // Habilitación de periféricos de Puerto A y UART0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    // Interrupción maestra
    IntMasterEnable();

    // Configurcion de PA0 como UART0_Rx y PA1 como UART0_Tx
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    // Configuracion de baud rate y data setup para UART0
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), UART0_BAUDRATE, UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE );

    // Configuracion y habilitacion de interrupcion para UART0
    IntEnable(INT_UART0);
    UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);

    // Configuracion de UART0 para entradas y salidas estandar
    UARTStdioConfig(0, UART0_BAUDRATE, SysCtlClockGet());
    UARTprintf("Ingrese valores: \n");
}

/* -------------------      Handler de Interrupción UART0     --------------------- */
void
uart_int_handler(void)
{
    // Limpieza de puerto UART previo a comunicacion
    UARTIntClear(UART0_BASE, UARTIntStatus(UART0_BASE, true));

    // Ciclo while que se da mientras haya un caracter disponible en el input
    while(UARTCharsAvail(UART0_BASE))
    {
        // Poner caracter en el puerto UART
        UARTCharPutNonBlocking(UART0_BASE, UARTCharGetNonBlocking(UART0_BASE));
    }
}

