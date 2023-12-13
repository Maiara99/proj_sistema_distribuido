INSTRUÇÕES - TRABALHO PRÁTICO DE SDA

Desenvolvido por: Helen da Silva Ikeda (2019021360)
		  Matheus José Fonseca Franklin (2018014212)

Antes de iniciar, é necessário que algumas bibliotecas que foram utilizadas no trabalho estejam 
instaladas na máquina. Caso não estejam, basta digitar no prompt de comando: 

pip install simple_pid
pip install numpy
pip install time
pip install threading
pip install scipy
pip install socket
pip install opc

Garanta que o prosys está rodando para que funcione corretamente.

É importante que os programas rodem na seguinte ordem: o tanque_conico.py, o clp.py e depois o synoptic_process.py.

Ao rodar, note que o terminal possui a interface e simula uma tela de controle do tanque. 
Assim que a conexão entre os dois processos for estabelecida, o terminal pedirá uma entrada do teclado. 
Insira um valor inteiro de 0 a 10, que corresponde à referência de altura do tanque. A tela irá mostrar 
o andamento da execução e assim que finalizar, mostrará no prompt de comando o último valor de altura 
registrado e informará o encerramento do programa.

Abra o arquivo historiador.txt para ver todos os valores de altura, vazão de entrada e vazão de saída 
obtidos durante a execução do processo.