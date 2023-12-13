import threading
import time
import socket
from opcua import Client

global mutex 
global h, qin, qout
mutex = threading.Lock()
h = 0

def controlador():
    global mutex 
    global h, qin, qout
    print("CLP iniciado")

    HOST = "127.0.0.1"
    PORT = 1024

    i = 0
    v = ','
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.bind((HOST, PORT))

    s.listen()
    print("Esperando conexoes...")

    conn, addr = s.accept()
    with conn:        
        print(f"Conectado por {addr}")

        # cria o client
        client = Client("opc.tcp://localhost:52520/OPCUA/SimulationServer")

        client.connect()

        node_h = client.get_node("ns=3;i=1008")
        node_q_in = client.get_node("ns=3;i=1009")
        node_h_ref = client.get_node("ns=3;i=1010")

        mutex.acquire()
        while True:
            href = conn.recv(1024)
            href = float(href.decode())
            node_h_ref.set_value(href)

            h = node_h.get_value()
            q_in = node_q_in.get_value()

            #condições para acionamento do alarme
            #a altura do tanque é 10m
            h_tanque = 10
            if (h/h_tanque)<0.05:
                alarme = 1
            elif (h/h_tanque)>0.95:
                alarme = 2
            else:
                alarme = 0

            aux = str(h)+str(v)+str(q_in)+str(v)+str(alarme)
            data = aux.encode()
            conn.sendall(data)
            time.sleep(0.5)
        mutex.release()

softPLC_thread = threading.Thread(target=controlador, args=())
softPLC_thread.start()
