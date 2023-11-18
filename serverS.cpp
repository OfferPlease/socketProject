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
        int sock;
        struct sockaddr_in server_addr;
        char buffer[buffer_size];
        char message[buffer_size];
        int server_len = sizeof(server_addr);
        int bytes_received;

        // read in status from txt file
        std::string filename = "./input_files/science.txt";
        std::unordered_map<std::string, int> books = read_in_books(filename);
        status_to_buffer(message, 1024, books);

        // Creating socket file descriptor
        if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
        {
                perror("socket creation failed");
                exit(EXIT_FAILURE);
        }

        memset(&server_addr, 0, sizeof(server_addr));

        // Filling server information
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(44367);
        server_addr.sin_addr.s_addr = INADDR_ANY;

        // Send message to server
        sendto(sock, (const char *)message, strlen(message), MSG_CONFIRM, (const struct sockaddr *)&server_addr, sizeof(server_addr));
        std::cout << "Message sent." << std::endl;

        // Receive response from server
        bytes_received = recvfrom(sock, (char *)buffer, buffer_size, MSG_WAITALL, (struct sockaddr *)&server_addr, (socklen_t *)&server_len);
        buffer[bytes_received] = '\0';
        std::cout << "Server : " << buffer << std::endl;

        // Close the socket
        close(sock);

        // // Create UDP socket
        // int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        // if (sockfd < 0) {
        //     std::cerr << "Error creating socket!" << std::endl;
        //     return 1;
        // }

        // // once booted, send book message to the main server
        // struct sockaddr_in main_addr;
        // main_addr.sin_family = AF_INET;
        // main_addr.sin_port = htons(45367);

        // // Convert IPv4 and IPv6 addresses from text to binary form
        // if (inet_pton(AF_INET, "127.0.0.1", &main_addr.sin_addr) <= 0) {
        //     std::cout << "\nInvalid address/ Address not supported \n";
        //     return -1;
        // }

        // if (connect(sockfd, (struct sockaddr *)&main_addr, sizeof(main_addr)) < 0) {
        //     std::cout << "\nConnection Failed \n";
        //     return -1;
        // }

        // // send message
        // send(sockfd, (const char*)buffer, strlen(buffer), 0);
        // char read_buffer[1024] = {0};
        // int read_ret = read(sockfd, read_buffer, 1024);
        // std::cout << read_buffer << std::endl;

        // // end sending
        // close(sockfd);

        // // Define server address
        // struct sockaddr_in servaddr;
        // memset(&servaddr, 0, sizeof(servaddr));
        // servaddr.sin_family = AF_INET;
        // servaddr.sin_addr.s_addr = INADDR_ANY; // Bind to any address
        // servaddr.sin_port = htons(41367); // Port number

        // std::cout << "UDP Server listening on port 41367" << std::endl;

        // while (1) {
        //     char buffer[1024];
        //     struct sockaddr_in cliaddr;
        //     unsigned int len = sizeof(cliaddr);

        //     // Receive message from client
        //     int n = recvfrom(sockfd, (char *)buffer, 1024, 0, (struct sockaddr *)&cliaddr, &len);
        //     buffer[n] = '\0'; // Null-terminate the string

        //     std::cout << "Received: " << buffer << std::endl;
        // }

        // // Close the socket
        // close(sockfd);
        return 0;
}
