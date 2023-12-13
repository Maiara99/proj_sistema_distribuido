#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "open62541.h"

#define PORT 8080
#define MAX_CONNECTIONS 5
#define BUFFER_SIZE 1024

// Função executada pela primeira thread
void *CLIENTE_OPC(void *arg) {
    printf("Thread CLIENTE OPC On\n");
    
// Inicializar o contexto
    UA_Client* client = UA_Client_new();

    // Configurar o cliente para se conectar ao servidor OPC UA
    UA_ClientConfig* config = UA_Client_getConfig(client);
    config->timeout = 5000; // Tempo limite de conexão em milissegundos
    UA_StatusCode retval = UA_Client_connect(client, "opc.tcp://DESKTOP-I1BUK7M:53530/OPCUA/SimulationServer");

    if(retval != UA_STATUSCODE_GOOD) {
        UA_Client_delete(client);
        return (int)retval;
    }

    // Leitura de um nó do servidor OPC UA
    UA_Variant value;
    UA_Variant_init(&value);
    retval = UA_Client_readValueAttribute(client, UA_NODEID_STRING(1, "MyVariable"), &value);
    if(retval == UA_STATUSCODE_GOOD && UA_Variant_hasScalarType(&value, &UA_TYPES[UA_TYPES_INT32])) {
        printf("Valor lido: %d\n", *(UA_Int32*)value.data);
    }

    // Desconectar e liberar recursos
    UA_Client_disconnect(client);
    UA_Client_delete(client);

    return NULL;
}


void *SERVIDOR_TCP(void *arg) {
    printf("Thread SERVIDOR TCP On\n");

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
    result1 = pthread_create(&thread1, NULL, CLIENTE_OPC, NULL);
    if (result1 != 0) {
        perror("Erro ao criar a Thread 1");
        exit(EXIT_FAILURE);
    }

    // Criação da segunda thread
    result2 = pthread_create(&thread2, NULL, SERVIDOR_TCP, NULL);
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