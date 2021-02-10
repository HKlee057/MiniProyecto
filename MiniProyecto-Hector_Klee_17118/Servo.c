//-----------------------------------------------------------------------------
// Hector Alejandro Kl�e Gonz�lez
// Carn� 17118
// MiniProyecto
// Dise�o e Innovaci�n en Ingenier�a 1
//-----------------------------------------------------------------------------

#include "Servo.h"                          // Libreria creada para control de servo en PWM1, Gen0
/* -----------------------------     Definiciones        ---------------------------- */
#define PWM_FREQUENCY 50                    // Define macro para la frecuencia del PWM en Hz

/* ------------------------------     Variables        ---------------------------- */
uint32_t ui32Period;                        // Variable para el periodo de PWM
volatile uint8_t ui8Adjust = 75;            // Variable el Duty Cycle del Servo

/* -------------------      Handler de Interrupci�n en Puerto F     --------------------- */
void
portF_int_handler(void)
{
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

/* -------------------      Inicializacion de modulo PWM     --------------------- */
void
pwm_init(void)
{
    // Reloj del sistema a 80MHz y reloj de PWM a 1.25MHz
    SysCtlClockSet(SYSCTL_SYSDIV_2_5 |SYSCTL_USE_PLL |SYSCTL_OSC_MAIN |SYSCTL_XTAL_16MHZ);
    SysCtlPWMClockSet(SYSCTL_PWMDIV_64);

    // Habilitaci�n de perif�ricos de Puerto F, Puerto D y PWM1
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    // Configurarci�n de PD_0 para la se�al de PWM (modulo PWM 1, generador 0)
    GPIOPinTypePWM(GPIO_PORTD_BASE, GPIO_PIN_0);
    GPIOPinConfigure(GPIO_PD0_M1PWM0);

    // Desbloqueo de SW2
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= GPIO_PIN_0;
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;

    // PF_0 y PF_4 (SW1 y SW2) configurados como Input y Pulled-up
    GPIODirModeSet(GPIO_PORTF_BASE, GPIO_PIN_4|GPIO_PIN_0, GPIO_DIR_MODE_IN);
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4 | GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    // Configuraci�n y habilitaci�n de the Interrupci�n para PF_0 y PF_4 (SW1 y SW2)
    IntEnable(INT_GPIOF);
    GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_4 | GPIO_PIN_0, GPIO_FALLING_EDGE);
    GPIOIntEnable(GPIO_PORTF_BASE, GPIO_INT_PIN_0 | GPIO_INT_PIN_4);
    // Interrupci�n maestra
    IntMasterEnable();

    // C�lculo de periodo de Timer de PWM
    uint32_t ui32PWMClock = SysCtlClockGet() / 64;
    ui32Period = (ui32PWMClock / PWM_FREQUENCY) - 1;
    // Configuraci�n del PWM_1 Genereador_0 como Count Down Mode
    PWMGenConfigure(PWM1_BASE, PWM_GEN_0, PWM_GEN_MODE_DOWN);
    // Carga del periodo calculado al m�dulo
    PWMGenPeriodSet(PWM1_BASE, PWM_GEN_0, ui32Period);

    // Cambio del Duty Cycle de PWM a valor espec�fico (obtenido al presionar SW_1 o SW_2)
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_0, ui8Adjust * ui32Period / 1000);
    // Habilitaaci�n de del pin PWM_0 de m�dulo PWM 1 como salida
    PWMOutputState(PWM1_BASE, PWM_OUT_0_BIT, true);
    // Habilitaci�n del Generador PWM
    PWMGenEnable(PWM1_BASE, PWM_GEN_0);
}
