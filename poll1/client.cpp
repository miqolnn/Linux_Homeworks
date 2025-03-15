#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define SERVER_ADDRESS "127.0.0.1"
#define SERVER_PORT 8080

int main() {
    int sock;
    struct sockaddr_in server_addr;
    char buffer[1024];

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        std::cerr << "Socket creation failed\n";
        return 1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_ADDRESS);

    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Connection failed\n";
        return 1;
    }

    std::cout << "Connected to server\n";

    while (true) {
        std::cout << "Enter message (or 'exit' to quit): ";
        std::cin.getline(buffer, sizeof(buffer));

        if (strcmp(buffer, "exit") == 0) {
            break;
        }

        send(sock, buffer, strlen(buffer), 0);

        int read_size = recv(sock, buffer, sizeof(buffer) - 1, 0);
        if (read_size > 0) {
            buffer[read_size] = '\0';
            std::cout << "Server reply: " << buffer << std::endl;
        } else {
            std::cerr << "Error receiving data or client disconnected\n";
            break;
        }
    }

    close(sock);
    std::cout << "Disconnected from server\n";
    return 0;
}

