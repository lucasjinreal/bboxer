//
// Created by jintian on 17-3-23.
//
#include "utils.h"

template <typename Out>
void split(const std::string &s, char delimiter, Out result){
    std::stringstream ss;
    ss.str(s);
    std::string item;
    while(std::getline(ss, item, delimiter)){
        *(result++) = item;
    }
}
std::vector<std::string> split(const std::string &s, char delimiter) {
    std::vector<std::string> elems;
    split(s, delimiter, std::back_inserter(elems));
    return elems;
}