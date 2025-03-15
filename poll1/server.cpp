#include <iostream>
#include <vector>
#include <thread>
#include <unistd.h>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <poll.h>
#include <pthread.h>

#define PORT 8080
#define MAX_CLIENTS 10

void* handle_client(void* arg) {
    int client_socket = *(int*)arg;
    char buffer[1024];
    
    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int read_size = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
        
        if (read_size <= 0) {
            std::cout << "Client disconnected or error\n";
            break;
        }

        std::cout << "Received from client: " << buffer << std::endl;
        send(client_socket, "Message received", 17, 0);
    }

    close(client_socket);
    pthread_exit(nullptr);
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        std::cerr << "Socket creation failed\n";
        return 1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Bind failed\n";
        return 1;
    }

    if (listen(server_socket, 3) < 0) {
        std::cerr << "Listen failed\n";
        return 1;
    }

    std::cout << "Server is running on port " << PORT << "\n";

    std::vector<struct pollfd> fds;
    struct pollfd server_poll_fd;
    server_poll_fd.fd = server_socket;
    server_poll_fd.events = POLLIN;
    fds.push_back(server_poll_fd);

    while (true) {
        int poll_result = poll(fds.data(), fds.size(), -1);
        if (poll_result == -1) {
            std::cerr << "Poll error\n";
            break;
        }

        for (size_t i = 0; i < fds.size(); ++i) {
            if (fds[i].revents & POLLIN) {
                if (fds[i].fd == server_socket) {
                    client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_addr_len);
                    if (client_socket < 0) {
                        std::cerr << "Client accept failed\n";
                        continue;
                    }

                    std::cout << "New client connected\n";

                    struct pollfd client_poll_fd;
                    client_poll_fd.fd = client_socket;
                    client_poll_fd.events = POLLIN;
                    fds.push_back(client_poll_fd);

                    pthread_t client_thread;
                    pthread_create(&client_thread, nullptr, handle_client, &client_socket);
                    pthread_detach(client_thread);
                }
            }
        }
    }

    close(server_socket);
    return 0;
}

