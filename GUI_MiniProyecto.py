#Laboratorio 4 - Electrónica Digital 2
#Hector A. Klée González
#17118


from Tkinter import *
import serial
import time
import sys

#Definición de variables
volt_POT1 = 0.00
volt_POT2 = 0.00
real_POT1 = 0
real_POT2 = 0

#Configuración del Puerto Serial
puerto = 'COM8'
ser = serial.Serial(port = puerto, baudrate = 9600,
                    parity = serial.PARITY_NONE,
                    stopbits = serial.STOPBITS_ONE,
                    bytesize = serial. EIGHTBITS, timeout = 0)


#Configuración de la ventana
ventana = Tk()
ventana.title("Laboratorio 4: Comunicación UART y SPI")
ventana.geometry("500x285")
ventana.config(bg='firebrick4')

name = Label(ventana, text="Hector A. Klée González \n17118", background = 'gold3', font=('BolsterBold', 16)).place(x=100,y=10)
title =Label(ventana, text="Contador", background = 'gold3', font=('BolsterBold', 16)).place(x=185,y=80)
answer = Entry(ventana)
answer.place(x=100, y=130)
pot1 = Label(ventana, text="Potenciómetro 1", background = 'gold3', font=('BolsterBold', 16)).place(x=10,y=170)
pot2 = Label(ventana, text="Potenciómetro 2", background = 'gold3', font=('BolsterBold', 16)).place(x=300,y=170)


#Configuración del botón
def sendCount():
    ser.flushInput()
    ser.flushOutput()
    time.sleep(.05)
    answer_write = answer.get()
    answer_send = chr(int(answer_write))
    ser.write(answer_send) 
    
send = Button(ventana, text = "Enviar", command=sendCount)
send.place(x=315,y=130)

#Configración de casillas de información
POT_1 = Label(ventana,text=str(volt_POT1) ,fg="gray18",font=(18))
POT_1.place(x=110,y=230)

POT_2 = Label(ventana,text=str(volt_POT2) ,fg="gray18",font=(18))
POT_2.place(x=315,y=230)

POT_1_1 = Label(ventana,text="V" ,fg="gray18",font=(18))
POT_1_1.place(x=175,y=230)

POT_2_1 = Label(ventana,text="V" ,fg="gray18",font=(18))
POT_2_1.place(x=380,y=230)

#While loop en el que se mostraran los datos de los potenciometros enviados del PIC en la computadora
while True:
    dato = ser.read()
    try:
        if dato == '':
            pass
        else:

            dato_POT1 = ser.read()
            ser.flushOutput()
            dato_POT2 = ser.read()

            real_POT1 = ord(dato_POT1)
            real_POT2 = ord(dato_POT2)

            print(str(real_POT1) + ',' + str(real_POT2))
            ser.flushInput()
            time.sleep(.05)
            
    except:
        print('#')
    volt_POT1 = float((real_POT1)/float (51))
    volt_POT2 = float((real_POT2)/float (51))
    POT_1.config(text = str(volt_POT1)[0:4])
    POT_2.config(text = str(volt_POT2)[0:4])
    ventana.update_idletasks()
    ventana.update() 
    time.sleep(.2)

ventana.mainloop()
