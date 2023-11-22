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
        char buffer[buffer_size];
        // phrase 1
        // Creating socket file descriptor
        std::cout << "Main Server is up and running" << std::endl;

        // phrase 2
        // read in encrypted member information
        std::unordered_map<std::string, std::string> members = read_in_members("./member.txt");
        // printMembers(members);
        std::cout << "Main server loaded the member list" << std::endl;
        // reuse the buffer
        memset(buffer, 0, buffer_size);
        // prepare a sending buffer
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

                const long unsigned int pos = recv_str.find(':');
                if (pos == std::string::npos)
                {
                        std::cout << "invalid transmitted data" << std::endl;
                }
                std::string username_entryped = recv_str.substr(0, pos);
                std::string password_entryped = recv_str.substr(pos + 1);
                std::cout << "Main server received the username amd password from the clientt using TCP over port 45367\n";

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
                        break;
                }
                else
                { // password != members[username]
                        std::cout << password_entryped << " does not match the password. Send a reply to the client\n";
                        send(new_socket, message_password_mismatch.c_str(), strlen(message_password_mismatch.c_str()), 0);
                }
                close(new_socket);
        }

        // phrase 3
        // prepare a UDP client/server
        int sock_udp;
        struct sockaddr_in serverS_addr;
        struct sockaddr_in serverH_addr;
        struct sockaddr_in serverL_addr;
        struct sockaddr_in serverM_addr;

        memset(&serverM_addr, 0, sizeof(serverM_addr));
        serverM_addr.sin_family = AF_INET;
        serverM_addr.sin_port = htons(44367);
        serverM_addr.sin_addr.s_addr = INADDR_ANY;
        // Creating socket file descriptor
        if ((sock_udp = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
        {
                perror("socket creation failed");
                exit(EXIT_FAILURE);
        }

        // bind the socket with the specific port number
        if (bind(sock_udp, (const struct sockaddr *)&serverM_addr, sizeof(serverM_addr)) < 0)
        {
                perror("bind failed");
                exit(EXIT_FAILURE);
        }

        memset(&serverS_addr, 0, sizeof(serverS_addr));
        memset(&serverH_addr, 0, sizeof(serverH_addr));
        memset(&serverL_addr, 0, sizeof(serverL_addr));

        // Filling server information
        serverS_addr.sin_family = AF_INET;
        serverS_addr.sin_port = htons(41367);
        serverS_addr.sin_addr.s_addr = INADDR_ANY;

        serverL_addr.sin_family = AF_INET;
        serverL_addr.sin_port = htons(42367);
        serverL_addr.sin_addr.s_addr = INADDR_ANY;

        serverH_addr.sin_family = AF_INET;
        serverH_addr.sin_port = htons(43367);
        serverH_addr.sin_addr.s_addr = INADDR_ANY;

        int serverLen = sizeof(serverS_addr);

        // accept TCP connection, this time outside of while loop
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
        {
                perror("accept");
                exit(EXIT_FAILURE);
        }

        while (1)
        {
                // reuse buffer
                memset(buffer, 0, buffer_size);
                // read client query
                read(new_socket, buffer, buffer_size);
                std::cout << buffer << std::endl;
                std::cout << "Main Server received the book request from client using TCP over port 45367\n";
                // check the received message to see if  it is received
                char recv_buffer[buffer_size] = {0};
                if (buffer[0] == 'S')
                {
                        // forward the massage to serverS
                        std::cout << "Found " << buffer << " located at ServerS. Send to ServerS\n";
                        sendto(sock_udp, (const char *)buffer, strlen(buffer), MSG_CONFIRM, (const struct sockaddr *)&serverS_addr, sizeof(serverS_addr));
                        int n = recvfrom(sock_udp, recv_buffer, buffer_size, MSG_WAITALL, (struct sockaddr *)&serverS_addr, (socklen_t *)&serverLen);
                        recv_buffer[n] = '\0';
                        // std::cout << "serverS: " << recv_buffer << std::endl;
                }
                else if (buffer[0] == 'L')
                {
                        // forward the massage to serverL
                        std::cout << "Found " << buffer << " located at ServerL. Send to ServerL\n";
                        sendto(sock_udp, (const char *)buffer, strlen(buffer), MSG_CONFIRM, (const struct sockaddr *)&serverL_addr, sizeof(serverL_addr));
                        int n = recvfrom(sock_udp, recv_buffer, buffer_size, MSG_WAITALL, (struct sockaddr *)&serverL_addr, (socklen_t *)&serverLen);
                        recv_buffer[n] = '\0';
                        // std::cout << "serverL: " << recv_buffer << std::endl;
                }
                else if (buffer[0] == 'H')
                {
                        // forward the massage to serverH
                        std::cout << "Found " << buffer << " located at ServerH. Send to ServerH\n";
                        sendto(sock_udp, (const char *)buffer, strlen(buffer), MSG_CONFIRM, (const struct sockaddr *)&serverH_addr, sizeof(serverL_addr));
                        int n = recvfrom(sock_udp, recv_buffer, buffer_size, MSG_WAITALL, (struct sockaddr *)&serverH_addr, (socklen_t *)&serverLen);
                        recv_buffer[n] = '\0';
                        // std::cout << "serverH: " << recv_buffer << std::endl;
                }
                else
                { // not a valid code, send to client
                        std::cout << "Did not find " << buffer << " in the book code list.\n";
                        send(new_socket, message_book_not_available.c_str(), strlen(message_book_not_available.c_str()), 0);
                        continue;
                }
                // send the response message from serverS, H, L to the
                std::cout << "Main Server reveived from server" << buffer[0] << " the book status result using UDP over port 44367\n";
                if (strcmp(recv_buffer, message_book_available.c_str()) == 0)
                {
                        std::cout << "The requested book is available\n";
                }
                if (strcmp(recv_buffer, message_book_not_available.c_str()) == 0)
                {
                        std::cout << "The requested book is not available\n";
                }
                if (strcmp(recv_buffer, message_book_not_exit.c_str()) == 0)
                {
                        std::cout << "Not able to find the book\n";
                }
                send(new_socket, recv_buffer, strlen(recv_buffer), 0);
                std::cout << "Main server sent the book status to the client\n";
        }
        close(new_socket);
        return 0;
}
