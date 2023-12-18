#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>


const int port = 5000;
int main() {
    // Criação do socket do cliente
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        std::cerr << "Erro ao criar o socket do cliente.\n";
        return EXIT_FAILURE;
    }

    // Configuração do endereço do servidor
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1"); // Endereço do servidor
    serverAddress.sin_port = htons(port); // Porta de exemplo

    // Conecta ao servidor
    if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        std::cerr << "Erro ao conectar ao servidor.\n";
        close(clientSocket);
        return EXIT_FAILURE;
    }

    std::cout << "Conectado ao servidor.\n";

    // Loop de comunicação com o servidor
    char buffer[1024];
    while (true) {
        std::cout << "Digite a mensagem para o servidor: ";
        std::cin.getline(buffer, sizeof(buffer));

        // Envia a mensagem para o servidor
        send(clientSocket, buffer, strlen(buffer), 0);

        if (strlen(buffer) == 0) {
            // Se a mensagem estiver vazia, encerra a conexão
            break;
        }

        // Limpa o buffer
        memset(buffer, 0, sizeof(buffer));

        // Recebe a resposta do servidor
        if (recv(clientSocket, buffer, sizeof(buffer), 0) == -1) {
            std::cerr << "Erro ao receber mensagem do servidor.\n";
            break;
        }

        if (strlen(buffer) == 0) {
            // Se a resposta estiver vazia, o servidor encerrou a conexão
            break;
        }

        std::cout << "Servidor: " << buffer << std::endl;
    }

    // Fecha o socket do cliente
    close(clientSocket);

    std::cout << "Conexão encerrada.\n";

    return EXIT_SUCCESS;
}
