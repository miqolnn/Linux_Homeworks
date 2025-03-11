#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <vector>
#include <climits>
struct client{
        int socket;
        sockaddr_in address;
};

void parse_and_send(char* message, int sender)
{
        //Splitting
        int res= INT_MAX;
        char* token = strtok(message, " ");
        char* oken = token;
        token = strtok(NULL, " ");

        int first = atoi(token);
        token = strtok(NULL, "\0");
        int second = atoi(token);


        if(strcmp(oken, "SUB")==0)
        { res = first - second;}
        else
        if(strcmp(oken, "SUM")==0)
        { res = first + second; }
        else
        if(strcmp(oken, "DIV")==0)
        { res = first / second; }
        else
        if(strcmp(oken, "MUL")==0)
        { res = first * second; }
        if(res == INT_MAX)
        {
                message = "Specify right command";
                send(sender, message, strlen(message), 0);
                return;
        }
        std::string ss = std::to_string(res);
        char const *pchar = ss.c_str();
        send(sender, pchar, strlen(pchar), 0);

}




void* handle_client(void* arg)
{
        client* client = (struct client*)arg;
        char buffer[3001];
        int rs;
        while((rs = recv(client->socket, buffer, 3000, 0)) > 0)
        {
                buffer[rs] = '\0';
                parse_and_send(buffer, client->socket);

        }
        close(client->socket);
        delete client;
        client = nullptr;
        pthread_exit(NULL);
}


int main() {
  // create a socket  
  int server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket == -1) {
      perror("socket creation error");
      exit(errno);
  }

  // create an endpoint

  // socket address
  struct sockaddr_in server_address;
  // internet protocol = AF_INET
  server_address.sin_family = AF_INET;
  // accept or any address (bind the socket to all available interfaces)
  server_address.sin_addr.s_addr = htonl(INADDR_ANY);
  // port
  server_address.sin_port = htons(8887);

  // Bind server_socket to server_address
  if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
      perror("bind failed");
      exit(errno);
  }

  // Listen for incoming connections
  if (listen(server_socket, 24) < 0) {
    perror("listen failed");
    exit(errno);
  }
  std::cout << "Waiting for connection\n";

  while(true) {
      int client_socket;
      struct sockaddr_in client_address;
      unsigned int client_addr_len = sizeof(client_address);

      // Accept incoming connection
      if ((client_socket = accept(server_socket, (struct sockaddr*) &client_address, &client_addr_len)) < 0) {
          perror("accept failed");
          exit(errno);

      }

      std::cout << "Connected client with address: " << inet_ntoa(client_address.sin_addr) << "\n";
      client *client = new struct client();
      client->socket = client_socket;
      client->address = client_address;
      pthread_t thread;
      pthread_create(&thread, 0, handle_client, (void*)client);
      pthread_detach(thread);
  }

  // close
  close(server_socket);
  return 0;
}
