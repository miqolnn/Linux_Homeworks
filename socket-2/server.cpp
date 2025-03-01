#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unordered_map>
#include <string>

std::mutex cout_mutex;
std::unordered_map<int, std::string> client_names;  // Мапа для хранения имен клиентов
#define MAX_CLIENTS 100  // Максимальное количество клиентов
int clients[MAX_CLIENTS];  // Массив для хранения сокетов клиентов
int client_count = 0;  // Количество подключенных клиентов

void broadcast_message(const std::string& message, int sender_socket) {
    std::lock_guard<std::mutex> lock(cout_mutex);
    for (int i = 0; i < client_count; i++) {
        int client_socket = clients[i];
        if (client_socket != sender_socket) {
            send(client_socket, message.c_str(), message.size(), 0);
        }
    }
}

void handle_client(int client_socket, std::string client_ip) {
    char buffer[1001];
    std::string client_name;

    // Запрашиваем имя клиента
    ssize_t received = recv(client_socket, buffer, 1000, 0);
    if (received > 0) {
        buffer[received] = '\0';
        client_name = std::string(buffer);
        client_names[client_socket] = client_name;
    }

    // Добавляем клиента в массив
    {
        std::lock_guard<std::mutex> lock(cout_mutex);
        if (client_count < MAX_CLIENTS) {
            clients[client_count++] = client_socket;
        } else {
            std::cout << "Max client limit reached. Can't add new client.\n";
            close(client_socket);
            return;
        }
    }

    // Слушаем сообщения от клиента
    while (true) {
        received = recv(client_socket, buffer, 1000, 0);
        if (received <= 0) {
            std::lock_guard<std::mutex> lock(cout_mutex);
            std::cout << "Client " << client_ip << " (" << client_name << ") disconnected.\n";
            client_names.erase(client_socket);

            // Удаляем клиента из массива
            {
                std::lock_guard<std::mutex> lock(cout_mutex);
                for (int i = 0; i < client_count; i++) {
                    if (clients[i] == client_socket) {
                        clients[i] = clients[client_count - 1];  // Заменяем текущего клиента последним
                        client_count--;
                        break;
                    }
                }
            }
            break;
        }

        buffer[received] = '\0';
        std::string message(buffer);

        // Обработка команды /list
        if (message == "/list") {
            std::string clients_list = "Connected clients:\n";
            for (const auto& entry : client_names) {
                clients_list += entry.second + "\n";
            }
            send(client_socket, clients_list.c_str(), clients_list.size(), 0);
        } else {
            // Рассылаем сообщение всем подключенным клиентам
            broadcast_message(client_name + ": " + message, client_socket);
        }
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

