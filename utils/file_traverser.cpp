//
// Created by jintian on 17-3-23.
//
#include "utils.h"


using namespace std;

vector<string> traverse_file(char* dir){
    DIR *dp;
    struct dirent *dirP;
    vector<string> files;
    if ((dp = opendir(dir)) == NULL){
        cout << "dir not exist." << endl;
    }

    while ((dirP = readdir(dp)) != NULL){
        if(dirP->d_type == DT_REG){
            files.push_back(dir + string("/") + string(dirP->d_name));
        }
    }

    closedir(dp);
    cout << "done!" << endl;
    cout << "get all files number: " << files.size() << endl;
    return files;
}
