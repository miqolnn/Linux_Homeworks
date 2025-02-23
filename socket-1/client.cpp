#include <iostream>
#include <unistd.h>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>

int main() {
    struct sockaddr_in server_address;

    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("socket creation error");
        return errno;
    }

    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8888);

    if (connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        perror("connection failed");
        close(client_socket);
        return errno;
    }

    std::cout << "Connected to server. Type 'exit' to disconnect.\n";

    std::string message;
    while (true) {
        std::cin>>message;

        if (message == "exit") {
            break;
        }

        ssize_t sent = send(client_socket, message.c_str(), message.size(), 0);
        if (sent == -1) {
            exit(errno);
        }
    }

    std::cout << "Closing connection.\n";
    close(client_socket);
    return 0;
}

