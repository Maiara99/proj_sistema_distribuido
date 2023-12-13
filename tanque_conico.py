import numpy as np
from scipy.integrate import odeint
from opcua import Client
import time
from simple_pid import PID

##################
# cria o client
client = Client("opc.tcp://localhost:52520/OPCUA/SimulationServer")

client.connect()

##################
node_h = client.get_node("ns=3;i=1008")
node_q_in = client.get_node("ns=3;i=1009")
node_h_ref = client.get_node("ns=3;i=1010")

def tanque(u):
    u = abs(u)

    # Definição da EDO
    def edo(h, t, cv, r0, a, u):
        return (-cv*np.sqrt(h))/(np.pi*(r0 + a*h)**2) + u/(np.pi*(r0 + a*h)**2)

    # Definição das variáveis da dinâmica do tanque
    cv = 0.75           # coeficiente de descarga da saída do tanque
    r0 = 5              # raio da base do tanque (menor raio)
    r1 = 8              # raio do topo do tanque (raio maior)
    H = 10              # altura do tanque
    a = (r1 - r0)/H     # alpha

    # Solução da EDO para condição inicial
    h0 = 0
    t = np.linspace(0, 10, 1001)
    sol = odeint(edo, h0, t, args=(cv, r0, a, u))

    # Método de integração de Runge-Kutta
    def rungekutta4(f, h0, t, args=()):
        n = len(t)
        h = np.zeros((n, 1))
        h[0] = h0
        for i in range(n - 1):
            height = t[i+1] - t[i]
            k1 = f(h[i], t[i], *args)
            k2 = f(h[i] + k1 * height / 2., t[i] + height / 2., *args)
            k3 = f(h[i] + k2 * height / 2., t[i] + height / 2., *args)
            k4 = f(h[i] + k3 * height, t[i] + height, *args)
            h[i+1] = h[i] + (height / 6.) * (k1 + 2*k2 + 2*k3 + k4)
        return h[i+1]

    # Solução pelo método de Runge-Kutta
    t = np.linspace(0, 10, 1001)

    sol = rungekutta4(edo, h0, t, args=(cv, r0, a, u))
    sol = np.round(sol, 4)

    qout = cv*np.sqrt(sol)
    qout = np.round(qout, 4)

    u = np.round(u, 4)

    return sol, u, qout

href = 0
pid = PID(3, 9, 0.05, setpoint=href)
h, qin, qout = tanque(0)
while True:
    href = node_h_ref.get_value()
    #a finalidade do 'for' é para não enviar muitas requisições ao servidor
    #opc desnecessariamente
    for _ in range(5):
        pid.setpoint = href
        control = pid(h)
        h, qin, qout = tanque(control)
    node_q_in.set_value(qin[0])
    node_h.set_value(h[0])
    time.sleep(0.5)