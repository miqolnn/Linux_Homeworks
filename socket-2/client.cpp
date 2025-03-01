#include <iostream>
#include <unistd.h>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>
#include <thread>

void receive_messages(int client_socket) {
    char buffer[1001];
    while (true) {
        ssize_t received = recv(client_socket, buffer, 1000, 0);
        if (received <= 0) {
            std::cout << "Disconnected from server.\n";
            break;
        }
        buffer[received] = '\0';
        std::cout << buffer << "\n";  // Выводим полученное сообщение на экран
    }
}

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

    std::cout << "Connected to server.\n";

    // Запрашиваем имя пользователя
    std::string name;
    std::cout << "Enter your name: ";
    std::cin >> name;

    // Отправляем имя серверу
    ssize_t sent = send(client_socket, name.c_str(), name.size(), 0);
    if (sent == -1) {
        perror("name sending failed");
        close(client_socket);
        return errno;
    }

    // Создаем поток для получения сообщений от сервера
    std::thread receiver_thread(receive_messages, client_socket);

    std::cout << "Type 'exit' to disconnect.\n";
    std::cout << "Type '/list' to see the list of connected users.\n";
    std::cout << "Type any message to send it to all users.\n";

    std::string message;
    while (true) {
        std::cin >> message;

        if (message == "exit") {
            break;
        }

        // Отправляем команду или сообщение
        sent = send(client_socket, message.c_str(), message.size(), 0);
        if (sent == -1) {
            perror("message sending failed");
            break;
        }

        // Если это команда /list, то выводим список подключенных клиентов
        if (message == "/list") {
            char buffer[1001];
            ssize_t received = recv(client_socket, buffer, 1000, 0);
            if (received > 0) {
                buffer[received] = '\0';
                std::cout << buffer << "\n";
            }
        }
    }

    std::cout << "Closing connection.\n";
    receiver_thread.join();  // Ожидаем завершения потока для получения сообщений
    close(client_socket);
    return 0;
}

