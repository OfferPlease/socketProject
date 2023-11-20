#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <unordered_map>
#include "utils.h"
extern unsigned int buffer_size;
int main()
{
        std::cout << "Server H is up and running using UDP on port 43367\n";
        // phrase1, just read in book status
        // read in status from txt file
        std::string filename = "./input_files/history.txt";
        std::unordered_map<std::string, int> books = read_in_books(filename);

        // Create UDP socket
        // phrase2, work as a UDP server to receive requests sent by Mserver
        // Define server address
        int sockfd;
        char buffer[buffer_size];
        char *message = "Hello from UDP server";
        struct sockaddr_in servaddr, cliaddr;

        // Creating socket file descriptor
        if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
        {
                perror("socket creation failed");
                exit(EXIT_FAILURE);
        }

        memset(&servaddr, 0, sizeof(servaddr));
        memset(&cliaddr, 0, sizeof(cliaddr));

        // Filling server information
        servaddr.sin_family = AF_INET; // IPv4
        servaddr.sin_addr.s_addr = INADDR_ANY;
        servaddr.sin_port = htons(43367);

        // Bind the socket with the server address
        if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
        {
                perror("bind failed");
                exit(EXIT_FAILURE);
        }

        while (true)
        {
                int len, n;
                len = sizeof(cliaddr); // len is value/result

                n = recvfrom(sockfd, (char *)buffer, buffer_size, MSG_WAITALL, (struct sockaddr *)&cliaddr, (socklen_t *)&len);
                buffer[n] = '\0';
                std::cout << "Client : " << buffer << std::endl;

                sendto(sockfd, (const char *)message, strlen(message), MSG_CONFIRM, (const struct sockaddr *)&cliaddr, len);
                std::cout << "Hello message sent." << std::endl;
        }
        return 0;
}
