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
        std::string filename = "./history.txt";
        std::unordered_map<std::string, int> books = read_in_books(filename);

        // Create UDP socket
        // phrase2, work as a UDP server to receive requests sent by Mserver
        // Define server address
        int sockfd;
        char buffer[buffer_size];
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
                memset(buffer, 0, buffer_size);
                n = recvfrom(sockfd, (char *)buffer, 1024, MSG_WAITALL, (struct sockaddr *)&cliaddr, (socklen_t *)&len);
                buffer[n] = '\0';

                // check if the book code exits
                std::string code_str(buffer);
                char buffer_send[buffer_size] = {0};
                bool isAdmin = false;
                unsigned long int pos = code_str.find("+Admin");
                if (pos != std::string::npos)
                {
                        isAdmin = true;
                        code_str = code_str.substr(0, pos);
                }
                if (isAdmin)
                        std::cout << "ServerS reveived an inventory status request for code " << code_str << ".\n";
                else
                        std::cout << "ServerS received " << code_str << " code from the Main Server\n";
                if (!books.count(code_str))
                {
                        strcpy(buffer_send, message_book_not_exit.c_str());
                }
                else if (books[code_str] == 0)
                {
                        strcpy(buffer_send, message_book_not_available.c_str());
                        if (isAdmin)
                                strcpy(buffer_send + message_book_not_available.size(), ":0");
                }
                else
                { // books[code_str] > 0
                        strcpy(buffer_send, message_book_available.c_str());
                        // if it is admin mode, return book number instead of decrease number
                        if (isAdmin)
                        {
                                strcpy(buffer_send + message_book_available.size(), ":");
                                strcpy(buffer_send + message_book_available.size() + 1, std::to_string(books[code_str]).c_str());
                        }
                        else
                        {
                                books[code_str]--;
                        }
                }
                sendto(sockfd, buffer_send, strlen(buffer_send), MSG_CONFIRM, (const struct sockaddr *)&cliaddr, len);

                if (isAdmin)
                        std::cout << "ServerS finished sending the inventory status to thee Main Server using UDP on port 43367.\n";
                else
                        std::cout << "ServerS finished sending the availability status of code " << code_str << " using UDP on port 43367.\n";
        }
        return 0;
}
