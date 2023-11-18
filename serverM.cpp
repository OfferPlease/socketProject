#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>
#include "utils.h"
extern unsigned int buffer_size;
int main()
{
        int udpfd;
        char buffer[buffer_size];
        char *message = "Hello from UDP server";
        struct sockaddr_in servaddr, cliaddr;
        std::unordered_map<std::string, int> books;

        // Creating socket file descriptor
        if ((udpfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
        {
                perror("socket creation failed");
                exit(EXIT_FAILURE);
        }

        memset(&servaddr, 0, sizeof(servaddr));
        memset(&cliaddr, 0, sizeof(cliaddr));

        // Filling server information
        servaddr.sin_family = AF_INET; // IPv4
        servaddr.sin_addr.s_addr = INADDR_ANY;
        servaddr.sin_port = htons(44367);

        // Bind the socket with the server address
        if (bind(udpfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
        {
                perror("bind failed");
                exit(EXIT_FAILURE);
        }

        bool S_flag = false, H_flag = false, L_flag = false;
        while (!S_flag || !H_flag || !L_flag)
        {
                int len, n;
                len = sizeof(cliaddr); // len is value/result

                n = recvfrom(udpfd, (char *)buffer, buffer_size, MSG_WAITALL, (struct sockaddr *)&cliaddr, (socklen_t *)&len);
                buffer[n] = '\0';
                std::cout << "Client : " << buffer << std::endl;
                if (buffer[0] == 'S')
                        S_flag = true;
                if (buffer[0] == 'L')
                        L_flag = true;
                if (buffer[0] == 'H')
                        H_flag = true;
                buffer_to_status(buffer, buffer_size, books);

                sendto(udpfd, (const char *)message, strlen(message), MSG_CONFIRM, (const struct sockaddr *)&cliaddr, len);
                std::cout << "Hello message sent." << std::endl;
        }

        printBooks(books);
        close(udpfd);
        return 0;
        // // work as a UDP server to receive all messages from 3 servers to get all the book informations
        // int UDP_FD;
        // char buffer[1024];
        // std::string message = "Hello from UDP server";
        // struct sockaddr_in servaddr, cliaddr;

        // memset(&servaddr, 0, sizeof(servaddr));
        // memset(&cliaddr, 0, sizeof(cliaddr));

        // // Filling server information
        // servaddr.sin_family = AF_INET; // IPv4
        // servaddr.sin_addr.s_addr = INADDR_ANY;
        // servaddr.sin_port = htons(44367);

        //  // Creating UDP socket file descriptor
        // if ((UDP_FD = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        //     perror("socket creation failed");
        //     exit(EXIT_FAILURE);
        // }

        // // Bind the socket with the server address
        // if (bind(UDP_FD, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        //     perror("bind failed");
        //     exit(EXIT_FAILURE);
        // }

        // bool S_flag = false, H_flag = false, L_flag = false;
        // while(!S_flag || !H_flag || !L_flag){
        //     int len, n;
        //     len = sizeof(cliaddr); // len is value/result

        //     n = recvfrom(UDP_FD, (char *)buffer, 1024, MSG_WAITALL, (struct sockaddr *)&cliaddr, (socklen_t *)&len);
        //     buffer[n] = '\0';
        //     std::cout << "Client : " << buffer << std::endl;

        //     sendto(UDP_FD, (const char *)message.c_str(), strlen(message.c_str()), MSG_CONFIRM, (const struct sockaddr *)&cliaddr, len);
        //     std::cout << "Hello message sent." << std::endl;

        //     if(buffer[0] == 'S') S_flag = true;
        //     if(buffer[0] == 'L') L_flag = true;
        //     if(buffer[0] == 'H') H_flag = true;
        // }

        // close(UDP_FD);

        // // work as a TCP server
        // int server_fd, new_socket;
        // struct sockaddr_in address;
        // int opt = 1;
        // int addrlen = sizeof(address);

        // // Creating socket file descriptor
        // if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        //     perror("socket failed");
        //     exit(EXIT_FAILURE);
        // }

        // // Setup some settings
        // if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        //     perror("setsockopt");
        //     exit(EXIT_FAILURE);
        // }
        // address.sin_family = AF_INET;
        // address.sin_addr.s_addr = INADDR_ANY;
        // address.sin_port = htons(45367);

        // // Forcefully bind socket to the port
        // if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
        // {
        //     perror("bind failed");
        //     exit(EXIT_FAILURE);
        // }
        // // listen to the port
        // if (listen(server_fd, 3) < 0) {
        //     perror("listen");
        //     exit(EXIT_FAILURE);
        // }
        // while(1)
        // {
        //     if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0)
        //     {
        //         perror("accept");
        //         exit(EXIT_FAILURE);
        //     }
        //     read(new_socket, buffer, 1024);
        //     std::cout << buffer << std::endl;
        //     send(new_socket, message.c_str(), strlen(message.c_str()), 0);
        //     std::cout << "Hello message sent\n";
        // }

        // return 0;
}
