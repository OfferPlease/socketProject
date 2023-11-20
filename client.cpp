#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <stdio.h>
#include "utils.h"

int main()
{
        std::cout << "Client is up and running" << std::endl;
        // phrase 2 login
        std::string username;
        std::string password;
        while (1)
        {

                std::cout << "Please enter the username: ";
                getline(std::cin, username);
                std::cout << "Please enter the password: ";
                getline(std::cin, password);

                // entrype the username and the password
                std::string encrypted_username = encryptedStr(username);
                std::string encrypted_password = encryptedStr(password);
                std::cout << encrypted_username << " " << encrypted_password << std::endl;

                // phrase2: send the username and password to the serverM and get the result
                // start a TCP client
                int sock = 0, valread;
                struct sockaddr_in serv_addr;
                char send_buffer[buffer_size] = {0};
                char recv_buffer[buffer_size] = {0};

                if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
                {
                        std::cout << "\n Socket creation error \n";
                        return -1;
                }

                serv_addr.sin_family = AF_INET;
                serv_addr.sin_port = htons(45367);

                // Convert IPv4 and IPv6 addresses from text to binary form
                if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
                {
                        std::cout << "\nInvalid address/ Address not supported \n";
                        return -1;
                }

                if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
                {
                        std::cout << "\nConnection Failed \n";
                        return -1;
                }
                // prepare sending buffer
                strcpy(send_buffer, encrypted_username.c_str());
                strcpy(send_buffer + encrypted_username.size(), ":");
                strcpy(send_buffer + encrypted_username.size() + 1, encrypted_password.c_str());
                send_buffer[encrypted_username.size() + encrypted_password.size() + 1] = '\0';
                send(sock, send_buffer, strlen(send_buffer), 0);
                std::cout << "Message sent: " << send_buffer << std::endl;
                valread = read(sock, recv_buffer, buffer_size);

                // handle received message
                std::string recv_str(recv_buffer);
                if (recv_str == message_not_registered)
                        std::cout << username << " received the result of authentication from Main Server using TCP over port 45367. Authentication failed: User not found.\n";
                else if (recv_str == message_password_mismatch)
                        std::cout << username << " received the result of authentication from Main Server using TCP over port 45367. Authentication failed: Password does not  match.\n";
                else if (recv_str == message_password_match)
                {
                        std::cout << username << " received the result of authentication from Main Server using TCP over port 45367. Authentication is successful.\n";
                        close(sock);
                        break;
                }
        }

        // phrase 3
        // start a new TCP connection after logged in
        int sock = 0, valread;
        struct sockaddr_in serv_addr;
        const char *message = "Hello from client";
        char buffer[buffer_size] = {0};

        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        {
                std::cout << "\n Socket creation error \n";
                return -1;
        }

        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(45367);

        // Convert IPv4 and IPv6 addresses from text to binary form
        if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
        {
                std::cout << "\nInvalid address/ Address not supported \n";
                return -1;
        }

        if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        {
                std::cout << "\nConnection Failed \n";
                return -1;
        }

        while (1)
        {
                std::cout << "Please enter book code to query:\n";
                std::string query;
                std::cin >> query;
                std::cout << username << " sent the request to Main Server.\n";
                send(sock, query.c_str(), strlen(message), 0);
                std::cout << "Message sent\n";
                valread = read(sock, buffer, 1024);
                std::cout << buffer << std::endl;
        }

        return 0;
}
