#MiniProyecto - Diseño e Innovación en Ingeniería
#Hector A. Klée González
#17118


from tkinter import *
import serial
import time
import sys

from tkinter import messagebox

#Definición de variables
volt_sens = "OFF"
real_sens = 0

#Configuración del Puerto Serial
puerto = 'COM8'
ser = serial.Serial(port = puerto, baudrate = 115200,
                    parity = serial.PARITY_NONE,
                    stopbits = serial.STOPBITS_ONE,
                    bytesize = serial. EIGHTBITS, timeout = 0)


#Configuración de la ventana
ventana = Tk()
ventana.title("Mini Proyecto - Diseño e Innovación en Ingeniería 1")
ventana.geometry("500x285")
ventana.config(bg='firebrick4')

name = Label(ventana, text="Hector A. Klée González \n17118", background = 'gold3', font=('BolsterBold', 16)).place(x=100,y=10)
title =Label(ventana, text="Ángulo de Servo", background = 'gold3', font=('BolsterBold', 16)).place(x=150,y=80)
answer = Entry(ventana)
answer.place(x=100, y=130)
sens1 = Label(ventana, text="Valor de Sensor", background = 'gold3', font=('BolsterBold', 16)).place(x=150,y=170)


#Configuración del botón
def sendCount():
    ser.flushInput()
    ser.flushOutput()
    time.sleep(.05)
    answer_write = answer.get()
    answer_send = chr(int(answer_write))
    ser.write(answer_send) 

#def helloCallBack():
#   messagebox.showinfo( "Hello Python", "Hello World")
   
send = Button(ventana, text = "Enviar", command=sendCount)
send.place(x=340,y=130)

#Configración de casillas de información
sens = Label(ventana,text=str(volt_sens) ,fg="gray18",font=(18))
sens.place(x=245,y=230)

#While loop en el que se mostraran los datos de los potenciometros enviados del PIC en la computadora
#while True:
#    dato = ser.read()
#    try:
#        if dato == '':
#            pass
#        else:

            #dato_sens = ser.read()
            #ser.flushOutput()

            #real_sens = ord(dato_sens)

            #print(str(real_sens) 
            #ser.flushInput()
            #time.sleep(.05)
            
    #except:
    #    print('#')
    #volt_sens = float((real_sens)/float (51))
    #sens.config(text = str(volt_POT1)[0:4])
    #ventana.update_idletasks()
    #ventana.update() 
    #time.sleep(.2)

ventana.mainloop()
