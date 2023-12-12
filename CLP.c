#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>


#define PORT 8080
#define MAX_CONNECTIONS 5
#define BUFFER_SIZE 1024

// Thread do Cliente OPC
void *clienteOPC(void *arg) {
    printf("Thread 1 executando\n");
    // Faça o trabalho da primeira thread aqui
    return NULL;
}

// Thread do Servidor TCP
void *servidorTCP(void *arg) {
    printf("Thread Servidor TCP Executando...\n");
    
     int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addrLen = sizeof(struct sockaddr_in);
    char buffer[BUFFER_SIZE] = {0};

    // Criar um socket
    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Erro ao criar o socket");
        exit(EXIT_FAILURE);
    }

    // Configurar a estrutura do endereço do servidor
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    // Associar o socket à porta e endereço local
    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Erro ao fazer bind");
        exit(EXIT_FAILURE);
    }

    // Esperar por conexões
    if (listen(serverSocket, MAX_CONNECTIONS) < 0) {
        perror("Erro ao escutar por conexões");
        exit(EXIT_FAILURE);
    }

    printf("Aguardando por conexões...\n");

    // Aceitar conexões e lidar com os dados
    if ((clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &addrLen)) < 0) {
        perror("Erro ao aceitar a conexão");
        exit(EXIT_FAILURE);
    }

    printf("Conexão estabelecida com sucesso!\n");

    // Troca de dados
    while (1) {
        memset(buffer, 0, sizeof(buffer));
        if (recv(clientSocket, buffer, sizeof(buffer), 0) <= 0) {
            perror("Erro ao receber dados");
            break;
        }
        printf("Cliente: %s", buffer);

        memset(buffer, 0, sizeof(buffer));
        printf("Digite a mensagem para enviar ao cliente: ");
        fgets(buffer, sizeof(buffer), stdin);
        send(clientSocket, buffer, strlen(buffer), 0);
    }

    close(serverSocket);
    return NULL;
}

int main() {
    pthread_t thread1, thread2;
    int result1, result2;

    // Criação da primeira thread
    result1 = pthread_create(&thread1, NULL, clienteOPC, NULL);
    if (result1 != 0) {
        perror("Erro ao criar a Thread 1");
        exit(EXIT_FAILURE);
    }

    // Criação da segunda thread
    result2 = pthread_create(&thread2, NULL, servidorTCP, NULL);
    if (result2 != 0) {
        perror("Erro ao criar a Thread 2");
        exit(EXIT_FAILURE);
    }

    // Aguarda o término das threads
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    printf("Ambas as threads terminaram\n");

    return 0;
}
