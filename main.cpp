#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <string>


std::string IP = "127.0.0.1";
const int port = 5000;
int main() {
    // Criação do socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Erro ao criar o socket do servidor.\n";
        return EXIT_FAILURE;
    }

    // Configuração do endereço do servidor
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(port); // Porta de exemplo

    // Associa o socket à porta
    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        std::cerr << "Erro ao associar o socket à porta.\n";
        close(serverSocket);
        return EXIT_FAILURE;
    }

    // Aguarda conexões
    if (listen(serverSocket, 5) == -1) {
        std::cerr << "Erro ao ouvir conexões.\n";
        close(serverSocket);
        return EXIT_FAILURE;
    }

    std::cout << "*********SERVER IS RUNNING: "<<IP<<":"<<port<<" ********"<<std::endl;

    // Aceita a conexão de um cliente
    sockaddr_in clientAddress;
    socklen_t clientSize = sizeof(clientAddress);
    int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientSize);

    if (clientSocket == -1) {
        std::cerr << "Erro ao aceitar a conexão do cliente.\n";
        close(serverSocket);
        return EXIT_FAILURE;
    }

    std::cout << "Cliente conectado.\n";

    // Loop de comunicação com o cliente
    char buffer[1024];
    while (true) {
        memset(buffer, 0, sizeof(buffer)); // Limpa o buffer

        // Recebe a mensagem do cliente
        if (recv(clientSocket, buffer, sizeof(buffer), 0) == -1) {
            std::cerr << "Erro ao receber mensagem do cliente.\n";
            break;
        }

        if (strlen(buffer) == 0) {
            // Se a mensagem estiver vazia, o cliente encerrou a conexão
            break;
        }

        std::cout << "CLIENTE: " << buffer << std::endl;

        // Envia uma mensagem de volta ao cliente
        std::cout << "Digite a mensagem para o cliente: ";
        std::cin.getline(buffer, sizeof(buffer));
        send(clientSocket, buffer, strlen(buffer), 0);
    }

    // Fecha os sockets
    close(clientSocket);
    close(serverSocket);

    std::cout << "Conexão encerrada.\n";

    return EXIT_SUCCESS;
}
