#include <iostream>
#include <fstream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define PORT 80 // Порт для HTTP

int main() {
    std::string hostname = "httpforever.com";
    std::string ip_address = "146.190.62.39";
    
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error: socket creation failed");
        return 1;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(ip_address.c_str());

    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error: connection failed");
        close(sockfd);
        return 1;
    }
   
    std::string request = "GET / HTTP/1.1\r\nHost: " + hostname + "\r\nConnection: close\r\n\r\n";
    ssize_t bytes_sent = send(sockfd, request.c_str(), request.size(), 0);
    if (bytes_sent < 0) {
        perror("Error: failed to send request");
        close(sockfd);
        return 1;
    }

    char buffer[4096];
    std::ofstream outfile("httpforever.txt", std::ios::out); 

    if (!outfile.is_open()) {
        std::cerr << "Error: could not open file for writing." << std::endl;
        close(sockfd);
        return 1;
    }

    ssize_t bytes_received;
    while ((bytes_received = recv(sockfd, buffer, sizeof(buffer), 0)) > 0) {
        outfile.write(buffer, bytes_received); 
    }

    if (bytes_received < 0) {
        perror("Error: failed to receive data");
    }

    outfile.close();
    close(sockfd);

    std::cout << "HTTP response saved to 'httpforever.txt'" << std::endl;

    return 0;
}

