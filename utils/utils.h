//
// Created by jintian on 17-3-23.
//
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <dirent.h>
#include <ctime>
#include <string>
#include <getopt.h>
#include <sstream>
#include <iterator>
#ifndef BBOXER_UTILS_H
#define BBOXER_UTILS_H

std::vector<std::string> traverse_file(char* dir);
void echo_test();

class Time{
public:
    int get_year();
    int get_month();
    int get_day();
    int get_hour();
    int get_min();
    int get_sec();

private:
    time_t now = time(0);
    tm* ltm=localtime(&now);
};

std::vector<std::string> split(const std::string &s, char delimiter);
#endif //BBOXER_UTILS_H
