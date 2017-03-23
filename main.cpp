//
// Created by jintian on 17-3-23.
//
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <stack>
#include <string>

#include "utils/utils.h"
using namespace std;

// include method from other files must follow behind std using namespace
void bound_box(vector<string> image_files, char* label_save_dir);

// global variables
int year;

int main(int argc, char* argv[]){
    Time t;
    year = t.get_year();
    cout << "=====       welcome to bboxer version 1.0.0 (c) " << year << " JinFagang      =====" << endl;

    if (argc < 3){
        cout << "please specific a path contains images to bbox them. and your bbox label save directory." << endl;
    } else{
        char* dir = argv[1];
        char* label_save_dir = argv[2];
        vector<string> files;
        printf("loading images from %s ...\n" , dir);
        files = traverse_file(dir);
        bound_box(files, label_save_dir);

    }

    return 0;
}
