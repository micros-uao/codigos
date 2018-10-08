
# Se importan las librerias
from ttk import *
from Tkinter import *
from threading import Thread
import serial
import time
import collections
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg, NavigationToolbar2TkAgg
import Tkinter as Tk
from ttk import Frame





class serialPlot(Frame):
    #Metodo que inicializa la clase , recibe unos argumentos necesarios para establecer la comunicacion
    def __init__(self, master, figure, serialPort , serialBaud , plotLength , dataNumBytes ):
        global ax
        #Se inicia la clase Frame para usar sus widgets posteriormente
        Frame.__init__(self, master)
        #Las siguientes son Variables que sirven para la ejecucion de los metodos de la clase serialPlot
        self.port = serialPort
        self.baud = serialBaud
        self.plotMaxLength = plotLength
        self.dataNumBytes = dataNumBytes
        #Se crea un arreglo de bytes para recibir los datos leidos por el serial
        self.rawData = bytearray(dataNumBytes)
        self.data = collections.deque([0] * plotLength, maxlen=plotLength)
        self.isRun = True
        self.isReceiving = False
        self.thread = None
        self.plotTimer = 0
        self.previousTimer = 0
        self.dtemp=0
        self.dsp=0
        self.dpulse=0
        self.decg=0
        self.demg=0
        self.dgsr=0
        self.valor1 = 0
        self.valor2 = 0
        self.valor3 = 0
        self.numero = 0
        #Se agregan los wigets a la interfaz
        self.pack()
        #Se crean los widgets
        self.createWidgets()
        self.entry = None
        self.setPoint = None
        self.master = master  # a reference to the master window
        #SE activa la caracteristica del fig para que cuando se presiones Esc se cierre el programa
        self.master.bind("<Escape>", exit)
        #Se guardan el ancho y alto del monitor
        w, h = self.master.winfo_screenwidth(), self.master.winfo_screenheight()
        # use the next line if you also want to get rid of the titlebar
        # self.master.overrideredirect(1)
        #Se le dan las medidas a la ventana del programa
        self.master.geometry("%dx%d+0+0" % (w, h))
        #Se inicializa la ventana y se le pasa por parametro la grafica
        self.initWindow(figure)  # initialize the window with our settings

        print('Trying to connect to: ' + str(serialPort) + ' at ' + str(serialBaud) + ' BAUD.')
        #SE crea la intancia para el puerto Serial
        try:
            self.serialConnection = serial.Serial(serialPort, serialBaud, timeout=4)
            print('Connected to ' + str(serialPort) + ' at ' + str(serialBaud) + ' BAUD.')
        except:
            print("Failed to connect with " + str(serialPort) + ' at ' + str(serialBaud) + ' BAUD.')
    #Metodo encargado de crear un hilo para la lectura por serial
    def readSerialStart(self):

        if self.thread == None:
            #Se crea el hilo y se relaciona con el metodo backgroundThread
            self.thread = Thread(target=self.backgroundThread)
            #Se inicia el hilo
            self.thread.start()
            # Espera a recibir valores
            while self.isReceiving != True:
                time.sleep(0.1)
    #Metodo para iniciar el canvas donde se va a poner la grafica
    def initWindow(self, figure):
        self.master.title("Real Time Plot")
        canvas = FigureCanvasTkAgg(figure, master=self.master)
        #toolbar = NavigationToolbar2TkAgg(canvas, self.master)
        #Posicion del canvas en la interfaz
        canvas.get_tk_widget().place( relx=0.35, rely=0.55, anchor=CENTER)

    #Metodo para graficar los datos
    def getSerialData(self, frame, lines, lineValueText, lineLabel, timeText):
        #Tiempo actual del programa
        currentTimer = time.clock()
        #Se restan el instante actual con el anterior y se multiplica por 1000
        self.plotTimer = int((currentTimer - self.previousTimer) * 1000)     # the first reading will be erroneous
        #Se guarda el instante actual en el instante anterior
        self.previousTimer = currentTimer
        #Se imprime el periodo con que se grafican los datos
        timeText.set_text('Plot Interval = ' + str(self.plotTimer) + 'ms')
        #Se guarda la data leida en la variable hola
        hola=self.rawData.decode()
        print("raw data : ")
        print(hola)
        #Se llama al objeto diferenciar el cual necesita de los datos leidos por serial
        self.diferenciar(hola)
        #Se muestra el valor de temperatura en la interfaz
        self.tempv["text"]=str('%.1f'%(self.dtemp*50))
        # Se muestra el valor del pulso en la interfaz
        self.pulsov["text"]=str(self.dpulse*200)
        # Se muestra el valor de la  concentracion de oxigeno en la interfaz
        self.oconv["text"]=str(self.dsp*100)
        #Se plotea ECG , EMG o GAlvanico dependiendo del resultado del metodo diferencia
        if (self.valor1==1):
            self.data.append(self.decg)  # we get the latest data point and append it to our array
            lines.set_data(range(self.plotMaxLength), self.data)
            lineValueText.set_text('[' + lineLabel[0] + '] = ' + str(self.decg))
        if(self.valor2==1):
            self.data.append(self.demg)  # we get the latest data point and append it to our array
            lines.set_data(range(self.plotMaxLength), self.data)
            lineValueText.set_text('[' + lineLabel[0] + '] = ' + str(self.demg))
        if(self.valor3==1):
            self.data.append(self.dgsr)  # we get the latest data point and append it to our array
            lines.set_data(range(self.plotMaxLength), self.data)
            lineValueText.set_text('[' + lineLabel[0] + '] = ' + str(self.dgsr*10))

    #Metodo para verificar que el dato llego bien y separar el string en sus variables
    def diferenciar(self,dato):
        #ndato=dato[1:5]
        #print(dato[24])
        if dato[0] == "a":
            self.dtemp=float(dato[1:5])
            print("a")
        if dato[6] == "b":
            self.dsp=float(dato[7:11])
            print(dato[6])
        if dato[12] == "c":
            self.dpulse=float(dato[13:17])
            print("c")
        if dato[18] == "d":
            self.decg=float(dato[19:23])
            print("d")
        if dato[24] == "e":
            self.demg=float(dato[25:29])
            print("e")
        if dato[30] == "f":
           self.dgsr=float(dato[31:35])
           print("f")
        if dato[18] == "D":
               self.decg =-1* float(dato[19:23])
               print("D")
        if dato[24] == "E":
               self.demg =-1* float(dato[25:29])
               print("E")
        if dato[30] == "F":
               self.dgsr = -1*float(dato[31:35])
               print("F")

    #Metodo para la lectura de los datos que recibe el puerto serial
    def backgroundThread(self):    # retrieve data
        #print("value : ")
        #Tiempo para que el buffer reciba la data
        time.sleep(1.0)
        #Limpiar buffer
        self.serialConnection.reset_input_buffer()
        #Lectura de datos
        while (self.isRun):
            #Se guardan los bytes leidos en rawdata
            self.serialConnection.readinto(self.rawData)
            #Bandera de lectura de datos
            self.isReceiving = True

    #Metodo para Cerrar la interfaz y el puerto serial
    def close(self):
        #Bandera de lectura de datos
        self.isRun = False
        print(self.isRun)
        #Se une el hilo creado al hilo principal
        self.thread.join()
        #Se cierra la conexion serial
        self.serialConnection.close()
        print('Disconnected...')
        # df = pd.DataFrame(self.csvData)
        # df.to_csv('/home/rikisenia/Desktop/data.csv')

    def exit(self):
        self.close()
        self.quit()
        print("salir")
        #sys.exit()



    #Metodo asociado al boton para que se grafique la señal ECG
    def ECG(self):
        self.valor1= 1
        self.valor2= 0
        self.valor3= 0

    # Metodo asociado al boton para que se grafique la señal EMG
    def EMG(self):
        self.valor1 = 0
        self.valor2 = 1
        self.valor3 = 0

    # Metodo asociado al boton para que se grafique la señal GSR
    def GSR(self):
        self.valor1 = 0
        self.valor2 = 0
        self.valor3 = 1

    def createWidgets(self):
        #Estilo para los botonos
        but1font = ('Helvetica', 10, 'bold')
        #Se crea el boton del ECG , se le dan las caracteristicas , metodo asociado y posicion en la interfaz
        self.ecg = Tk.Button(command=lambda : self.ECG())
        self.ecg["text"] = "ECG"
        self.ecg["bg"] = "white"
        self.ecg["height"] = 5
        self.ecg["width"] = 5
        self.ecg["fg"] = "black"
        self.ecg["font"] = but1font
        #self.ecg["command"] = print("ecg")
        self.ecg.place( relx=0.80, rely=0.7, anchor=CENTER)

        # Se crea el boton del EMG , se le dan las caracteristicas , metodo asociado y posicion en la interfaz
        self.emg = Tk.Button(command=lambda : self.EMG())
        self.emg["text"] = "EMG"
        self.emg["bg"] = "white"
        self.emg["height"] = 5
        self.emg["width"] = 5
        self.emg["fg"] = "black"
        self.emg["font"] = but1font
        self.emg.place(relx=0.87, rely=0.7, anchor=CENTER)

        # Se crea el boton del GSR , se le dan las caracteristicas , metodo asociado y posicion en la interfaz
        self.gsr = Tk.Button(command=lambda : self.GSR())
        self.gsr["text"] = "GSR"
        self.gsr["bg"] = "white"
        self.gsr["height"] = 5
        self.gsr["width"] = 5
        self.gsr["fg"] = "black"
        self.gsr["font"] = but1font
        self.gsr.place(relx=0.94, rely=0.7, anchor=CENTER)

        # Se crea el boton de SALIR , se le dan las caracteristicas , metodo asociado y posicion en la interfaz
        self.salir = Tk.Button(command=lambda : self.exit())
        self.salir["text"] = "X"
        self.salir["bg"] = "white"
        self.salir["height"] = 2
        self.salir["width"] = 3
        self.salir["fg"] = "black"
        self.salir["font"] = but1font
        #self.salir.bind("<Button-1>",self.sumarnumero())
        self.salir.place(relx=0.99, rely=0.01, anchor=CENTER)

        #Estilo para las etiquetas
        labelfont=('Helvetica',60,'bold')
        #Label para el Titulo
        hola = Label(self.master, bg='black', fg='white', font=labelfont ,text="B-learning Tools", pady=5,padx=5, relief=FLAT)
        hola.place(relx=0.5, rely=0.1, anchor=CENTER)

        labelfont1 = ('Helvetica', 20, 'bold')
        # Label con la palabra PRbpm
        pulso = Label(self.master, bg='black', fg='white', font=labelfont1, text="PRbpm♥", pady=5, padx=5,relief=FLAT)
        pulso.place(relx=0.82, rely=0.25, anchor=CENTER)

        #Label para poner el dato de pulso transmitido
        lf1 = ('Helvetica', 20, 'bold')
        self.pulsov = Label(self.master, bg='black', fg='white', font=lf1, text=str(self.numero), pady=5, padx=5,relief=FLAT)
        self.pulsov.place(relx=0.92, rely=0.25, anchor=CENTER)

        labelfont2 = ('Helvetica', 20, 'bold')
        # Label con la palabra SpO2
        ocon = Label(self.master, bg='black', fg='white', font=labelfont2, text="%SpO2", pady=5, padx=5,relief=FLAT)
        ocon.place(relx=0.82, rely=0.35, anchor=CENTER)

        lf2 = ('Helvetica', 20, 'bold')
        #Label para poner el dato de conocentracion de oxigeno transmitido
        self.oconv = Label(self.master, bg='black', fg='white', font=lf2, text="####", pady=5, padx=5,relief=FLAT)
        self.oconv.place(relx=0.92, rely=0.35, anchor=CENTER)

        labelfont3 = ('Helvetica', 20, 'bold')
        # Label con la palabra View signals
        signs = Label(self.master, bg='black', fg='white', font=labelfont3, text="View Signals", pady=5, padx=5,relief=FLAT)
        signs.place(relx=0.87, rely=0.55, anchor=CENTER)

        # Label con la palabra Temp
        temp = Label(self.master, bg='black', fg='white', font=labelfont3, text="Temp °:", pady=5, padx=5,relief=FLAT)
        temp.place(relx=0.82, rely=0.45, anchor=CENTER)

        lf3 = ('Helvetica', 20, 'bold')
        #Label para poner el dato de temperatura transmitido
        self.tempv = Label(self.master, bg='black', fg='white', font=lf3, text="#####", pady=5, padx=5,relief=FLAT)
        self.tempv.place(relx=0.92, rely=0.45, anchor=CENTER)





#Main del programa
def main():

    #Se inicializan los parametros para crear el puerto Serie
    portName = '/dev/ttyACM0'
    baudRate = 115200
    maxPlotLength = 100
    #Numero de bytes que va a leer el puerto
    dataNumBytes = 36       # number of bytes of 1 data point
    #Se crea una instancia de Tk
    root = Tk.Tk()
    #Se configura el fondo de la interfaz
    root.config(background='black')
    #Se configura el tamaño de el elemento graficador
    fig = plt.figure(figsize=(8, 5))
    #Periodo en el cual la animacion grafica cada dato
    pltInterval = 50
    #Limites de la grafica
    xmin = 0
    xmax = 100
    ymin = -(1)
    ymax = 1
    #Grafica donde se van a observar los datos y su configuracion
    ax = plt.axes(xlim=(xmin,xmax), ylim=(float(ymin - (ymax - ymin) / 10), float(ymax + (ymax - ymin) / 10)))
    #Se instancia la clase serialPlot con sus respectivos parametros
    s = serialPlot(root,fig,portName,  baudRate, maxPlotLength, dataNumBytes)
    #Se inicia la comunicacion serial
    s.readSerialStart()
    #Se configrua el color de los ejes y el marco de la grafica
    ax.spines['bottom'].set_color('white')
    ax.spines['top'].set_color('white')
    ax.spines['left'].set_color('white')
    ax.spines['right'].set_color('white')
    ax.tick_params(axis='x',colors='white')
    ax.tick_params(axis='y', colors='white')
    #Labels
    ax.set_title('e-HEALTH Monitor', color="white",fontsize=18)
    ax.set_xlabel("Time", color="white",fontsize=18)
    ax.set_ylabel("Output Value", color="white",fontsize=18)
    #
    lineLabel = ['X', 'Y', 'Z']
    style = ['w-', 'c-', 'b-']  # linestyles for the different plots

    timeText = ax.text(0.50, 0.95, '', transform=ax.transAxes, color="white")
    #Variable donde se va a guardar el dato que llegue por serial
    lines = ax.plot([], [], label=lineLabel,color="blue")[0]
    lineValueText = ax.text(0.50, 0.90, '', transform=ax.transAxes,color="blue")
    #Se crea el Canvas animation y se le pasa los argumentos
    anim = animation.FuncAnimation(fig, s.getSerialData, fargs=(lines, lineValueText, lineLabel, timeText),interval=pltInterval)  # fargs has to be a tuple
    #Se inicia la interfaz en un loop
    root.mainloop()
    #Si se cierra la interfaz el puerto serie se cierra
    s.close()

# Llamada al Main del programa
if __name__ == '__main__':
    main()
