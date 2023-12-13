import socket
import time

HOST = "127.0.0.1"
PORT = 1024

with open('historiador.txt','w') as arquivo:
    arquivo.write("--------------------------------------\n")
    arquivo.write("            HISTORICO DA\n")
    arquivo.write("     TELA DE CONTROLE DO TANQUE\n")

    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((HOST, PORT))

    arquivo.write("--------------------------------------\n")
    arquivo.write("Altura desejada: ")
    print("\nInsira a altura desejada: ")

    ref = input()
    arquivo.write(f"{ref} metros\n")
    arquivo.write("--------------------------------------\n")
    arquivo.write("Dados do controle realizado:\n")
    print("\nIniciando controle")

    while True:
        s.sendall(ref.encode())
        hist = s.recv(1024)

        if not hist:
            arquivo.write("--------------------------------------\n")
            arquivo.write("Sistema finalizado.\n")
            arquivo.write("--------------------------------------\n")
            arquivo.close()
            break

        h, qin, alarme = hist.split(b',')
        h = h.decode()
        qin = qin.decode()
        alarme = alarme.decode()

        h = str(h).replace("[", " ")
        h = str(h).replace("]", " ")

        qin = str(qin).replace("[", " ")
        qin = str(qin).replace("]", " ")

        alarme = str(alarme).replace("[", " ")
        alarme = str(alarme).replace("]", " ")

        if alarme == '1':
            print("PERIGO: NIVEL BAIXO\n")
        elif alarme == '2':
            print("PERIGO: NÍVEL ALTO\n")

        print(f"Altura atual ={h}\nVazao de entrada ={qin}\n")
        arquivo.write(f"--------------------------------------\nAltura atual ={h}\nVazao de entrada ={qin}\n")
        time.sleep(0.5)
