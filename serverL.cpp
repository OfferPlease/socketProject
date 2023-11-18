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
        std::string filename = "./input_files/literature.txt";
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
        return 0;
}