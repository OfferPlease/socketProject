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
std::unordered_map<std::string, int> read_in_books(std::string filename);
void status_to_buffer(char *buffer, int size, std::unordered_map<std::string, int> &books);
void buffer_to_status(char *buffer, int size, std::unordered_map<std::string, int> &books);
void printBooks(std::unordered_map<std::string, int> &books);
#endif
