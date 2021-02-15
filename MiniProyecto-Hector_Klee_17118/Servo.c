//-----------------------------------------------------------------------------
// Hector Alejandro Klée González
// Carné 17118
// MiniProyecto
// Diseño e Innovación en Ingeniería 1
//-----------------------------------------------------------------------------

#include "Servo.h"                          // Libreria creada para control de servo en PWM1, Gen0
#include "UART.h"                           // Libreria creada para UART
/* -----------------------------     Definiciones        ---------------------------- */
#define Period  320000 //(16000000/50) 50Hz
#define SERVO_STEPS         180     // Maximum amount of steps in degrees (180 is common)
#define SERVO_MIN           11500//9500     // The minimum duty cycle for this servo
#define SERVO_MAX           40000//35100    // The maximum duty cycle

unsigned int servo_lut[SERVO_STEPS+1];

/* ------------------------------     Variables        ---------------------------- */
volatile uint32_t ui8Adjust;            // Variable el angulo del Servo

/* -------------------      Handler de Interrupción en Puerto F     --------------------- */
void InitServo(void) //PC5
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    GPIOPinConfigure(GPIO_PC5_WT0CCP1);
    GPIOPinTypeTimer(GPIO_PORTC_BASE, GPIO_PIN_5);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER0);
    TimerConfigure(WTIMER0_BASE, TIMER_CFG_SPLIT_PAIR|TIMER_CFG_B_PWM);
    TimerLoadSet(WTIMER0_BASE, TIMER_B, (Period-1));
    TimerMatchSet(WTIMER0_BASE, TIMER_B, (Period-9600));

    TimerMatchSet(WTIMER0_BASE, TIMER_B, 1);
    TimerEnable(WTIMER0_BASE, TIMER_B);
}

void ConfigureServo(void)
{
    unsigned int servo_stepval, servo_stepnow;
    unsigned int i;

    servo_stepval   = ( (SERVO_MAX - SERVO_MIN) / SERVO_STEPS );
    servo_stepnow   = SERVO_MIN;

    for (i = 0; i < (SERVO_STEPS+1); i++)
    {
        servo_lut[i] = (Period-servo_stepnow);
        servo_stepnow += servo_stepval;
    }
}

void SetServoPosition(uint32_t position)
{
    TimerMatchSet(WTIMER0_BASE, TIMER_B, position);
}

void SetServoAngle(uint32_t angle)
{
    SetServoPosition(servo_lut[angle]);
}

void
portF_int_handler(void)
{
    // ISR de Puerto F manejo de interrupciones
    GPIOIntClear(GPIO_PORTF_BASE , GPIO_INT_PIN_4 | GPIO_INT_PIN_0);

    // If SW1 se presiona, aumentar a la variable de control de Duty Cycle
    if(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4 | GPIO_PIN_0)== GPIO_PIN_0){
        //ui8Adjust++;
        ui8Adjust = ui8Adjust + 3;
        if (ui8Adjust > 180){
            ui8Adjust = 180;
        }
        SetServoAngle(ui8Adjust);
    }
    // If SW2 se presiona, disminuir a la variable de control de Duty Cycle
    if(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4 | GPIO_PIN_0)== GPIO_PIN_4){
        //ui8Adjust--;
        ui8Adjust = ui8Adjust - 3;
        if (ui8Adjust < 0){
            ui8Adjust = 0;
        }
        SetServoAngle(ui8Adjust);
    }
}

/* -------------------      Inicializacion de modulo PWM     --------------------- */
void
pwm_init(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC |   SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_0);
    GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0, 0x00);
/*
    // Habilitación de periféricos de Puerto F
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    // Desbloqueo de SW2
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= GPIO_PIN_0;
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;

    // PF_0 y PF_4 (SW1 y SW2) configurados como Input y Pulled-up
    GPIODirModeSet(GPIO_PORTF_BASE, GPIO_PIN_4|GPIO_PIN_0, GPIO_DIR_MODE_IN);
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4 | GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    // Configuración y habilitación de the Interrupción para PF_0 y PF_4 (SW1 y SW2)
    IntEnable(INT_GPIOF);
    GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_4 | GPIO_PIN_0, GPIO_FALLING_EDGE);
    GPIOIntEnable(GPIO_PORTF_BASE, GPIO_INT_PIN_0 | GPIO_INT_PIN_4);
    // Interrupción maestra
    IntMasterEnable();
*/
    timerInit();

    InitServo();
    ConfigureServo();

    SetServoAngle(90);

    // Iniciliazacion de UART
    //uart_init();
}

void
angle_get(void)
{
    // Limpieza de puerto UART previo a comunicacion
    UARTIntClear(UART0_BASE, UARTIntStatus(UART0_BASE, true));

    if(UARTCharsAvail(UART0_BASE))
    {
        // Poner caracter en el puerto UART
        UARTCharPut(UART0_BASE, UARTCharGet(UART0_BASE));
        // Igualar valor a varaible
        ui8Adjust = UARTCharGet(UART0_BASE);
    }
    SetServoAngle(ui8Adjust);
}
