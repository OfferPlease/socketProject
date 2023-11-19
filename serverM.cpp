#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>
#include "utils.h"
// extern unsigned int buffer_size;
int main()
{
        // int udpfd;
        char buffer[buffer_size];
        // char *message = "Hello from UDP server";
        // struct sockaddr_in servaddr, cliaddr;
        // std::unordered_map<std::string, int> books;
        // // phrase 1
        // // Creating socket file descriptor
        // std::cout << "Main Server is up and running" << std::endl;
        // if ((udpfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
        // {
        //         perror("socket creation failed");
        //         exit(EXIT_FAILURE);
        // }

        // memset(&servaddr, 0, sizeof(servaddr));
        // memset(&cliaddr, 0, sizeof(cliaddr));

        // // Filling server information
        // servaddr.sin_family = AF_INET; // IPv4
        // servaddr.sin_addr.s_addr = INADDR_ANY;
        // servaddr.sin_port = htons(44367);

        // // Bind the socket with the server address
        // if (bind(udpfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
        // {
        //         perror("bind failed");
        //         exit(EXIT_FAILURE);
        // }

        // bool S_flag = false, H_flag = false, L_flag = false;
        // while (!S_flag || !H_flag || !L_flag)
        // {
        //         int len, n;
        //         len = sizeof(cliaddr); // len is value/result

        //         n = recvfrom(udpfd, (char *)buffer, buffer_size, MSG_WAITALL, (struct sockaddr *)&cliaddr, (socklen_t *)&len);
        //         buffer[n] = '\0';
        //         std::cout << "Client : " << buffer << std::endl;
        //         if (buffer[0] == 'S')
        //         {
        //                 S_flag = true;
        //                 std::cout << "Main Server received the book code list from server S using UDP over port 44367" << std::endl;
        //         }
        //         if (buffer[0] == 'L')
        //         {
        //                 L_flag = true;
        //                 std::cout << "Main Server received the book code list from server L using UDP over port 44367" << std::endl;
        //         }
        //         if (buffer[0] == 'H')
        //         {
        //                 H_flag = true;
        //                 std::cout << "Main Server received the book code list from server H using UDP over port 44367" << std::endl;
        //         }
        //         buffer_to_status(buffer, buffer_size, books);

        //         sendto(udpfd, (const char *)message, strlen(message), MSG_CONFIRM, (const struct sockaddr *)&cliaddr, len);
        //         std::cout << "Hello message sent." << std::endl;
        // }

        // // printBooks(books);

        // close(udpfd);

        // phrase 2
        // read in encrypted member information
        std::unordered_map<std::string, std::string> members = read_in_members("./input_files/member.txt");
        printMembers(members);
        std::cout << "Main server loaded the member list" << std::endl;
        // reuse the buffer
        memset(buffer, 0, buffer_size);
        // prepare a sending buffer
        char buffer_send[buffer_size];
        memset(buffer, 0, buffer_size);
        // work as a TCP server
        int server_fd, new_socket;
        struct sockaddr_in address;
        int opt = 1;
        int addrlen = sizeof(address);

        // Creating socket file descriptor
        if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
        {
                perror("socket failed");
                exit(EXIT_FAILURE);
        }

        // Setup some settings
        if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
        {
                perror("setsockopt");
                exit(EXIT_FAILURE);
        }
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(45367);

        // Forcefully bind socket to the port
        if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
        {
                perror("bind failed");
                exit(EXIT_FAILURE);
        }
        // listen to the port
        if (listen(server_fd, 3) < 0)
        {
                perror("listen");
                exit(EXIT_FAILURE);
        }
        while (1)
        {
                if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
                {
                        perror("accept");
                        exit(EXIT_FAILURE);
                }
                read(new_socket, buffer, buffer_size);
                std::cout << buffer << std::endl;
                // check the received message to see if  it is received
                std::string recv_str(buffer);

                int pos = recv_str.find(':');
                if (pos == std::string::npos)
                {
                        std::cout << "invalid transmitted data" << std::endl;
                }
                std::string username_entryped = recv_str.substr(0, pos);
                std::string password_entryped = recv_str.substr(pos + 1);

                // std::cout << "received username is: " << username_entryped << ", reveived password is:" << password_entryped << std::endl;
                // check if it is a registered user
                if (!members.count(username_entryped))
                {
                        std::cout << username_entryped << "is not registed. Send a reply to the client\n";
                        send(new_socket, message_not_registered.c_str(), strlen(message_not_registered.c_str()), 0);
                }
                // check the password ==
                else if (password_entryped == members[username_entryped])
                {
                        std::cout << password_entryped << " matches the username. Send a reply to the client\n";
                        send(new_socket, message_password_match.c_str(), strlen(message_password_match.c_str()), 0);
                }
                else
                { // password != members[username]
                        std::cout << password_entryped << " does not match the password. Send a reply to the client\n";
                        send(new_socket, message_password_mismatch.c_str(), strlen(message_password_mismatch.c_str()), 0);
                }
        }

        return 0;
}
