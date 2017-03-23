//
// Created by jintian on 17-3-23.
//
#include "utils.h"

using namespace std;
void echo_test(){
    cout << "echo, just for test." << endl;
    char a[] = "hello, nihao";
    printf("%s \n", a);
    string b = "I am Jin tian";
    cout << a + b << endl;

    ofstream myfile("test.txt");
    if (myfile.is_open()){
        myfile << "what the fuck?!!!!\n";
        myfile << "another line!!!\n";
        myfile.close();
    }

}
