#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

std::mutex cout_mutex; 

void handle_client(int client_socket, std::string client_ip) {
    char buffer[1001];

    while (true) {
        ssize_t received = recv(client_socket, buffer, 1000, 0);
        if (received <= 0) {
            std::lock_guard<std::mutex> lock(cout_mutex);
            std::cout << "Client " << client_ip << " disconnected.\n";
            break;
        }

        buffer[received] = '\0';
        std::lock_guard<std::mutex> lock(cout_mutex);
        std::cout << "Message from " << client_ip << ": " << buffer << "\n";
    }

    close(client_socket);
}

int main() {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("socket creation error");
        return errno;
    }

    int opt = 1;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(8888);

    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        perror("bind failed");
        close(server_socket);
        return errno;
    }

    if (listen(server_socket, 10) < 0) {
        perror("listen failed");
        close(server_socket);
        return errno;
    }

    std::cout << "Server is listening on port 8888...\n";

    std::vector<std::thread> threads;

    while (true) {
        struct sockaddr_in client_address;
        socklen_t client_addr_len = sizeof(client_address);
        int client_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_addr_len);
        if (client_socket < 0) {
            perror("accept failed");
            continue;
        }

        std::string client_ip = inet_ntoa(client_address.sin_addr);
        
        {
            std::lock_guard<std::mutex> lock(cout_mutex);
            std::cout << "Client connected: " << client_ip << "\n";
        }

        threads.emplace_back(handle_client, client_socket, client_ip);
    }

    for (auto& t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }

    close(server_socket);
    return 0;
}

