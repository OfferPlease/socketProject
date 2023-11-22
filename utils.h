#ifndef UTILS_H
#define UTILS_H
#include <string>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>

static unsigned int buffer_size = 1024;
static std::string message_not_registered = "-1";
static std::string message_password_match = "0";
static std::string message_password_mismatch = "-2";
static std::string message_book_not_exit = "-3";
static std::string message_book_available = "1";
static std::string message_book_not_available = "-4";

std::unordered_map<std::string, int> read_in_books(std::string filename);
void status_to_buffer(char *buffer, int size, std::unordered_map<std::string, int> &books);
void buffer_to_status(char *buffer, int size, std::unordered_map<std::string, int> &books);
void printBooks(std::unordered_map<std::string, int> &books);
void printMembers(std::unordered_map<std::string, std::string> &members);
std::unordered_map<std::string, std::string> read_in_members(std::string filename);
std::string encryptedStr(std::string str);
std::string unencrypStr(std::string &str);
#endif
