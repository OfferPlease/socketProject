#ifndef UTILS_H
#define UTILS_H
#include <string>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>

std::unordered_map<std::string, int> read_in_books(std::string filename);
void status_to_buffer(char* buffer, int size, std::unordered_map<std::string, int>& books);
#endif




