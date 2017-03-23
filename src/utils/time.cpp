//
// Created by jintian on 17-3-23.
//
#include "utils.h"
using namespace std;

int Time::get_year() {
    return 1900 + ltm->tm_year;
}

int Time::get_month() {
    return 1 + ltm->tm_mon;
}

int Time::get_day() {
    return ltm->tm_mday;
}
int Time::get_hour() {
    return ltm->tm_hour;
}
int Time::get_min() {
    return ltm->tm_min;
}
int Time::get_sec() {
    ltm->tm_sec;
}
