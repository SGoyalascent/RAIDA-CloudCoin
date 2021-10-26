#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<sstream>
#include<cstring>
#include<stdio.h>
#include<cstdio>

using namespace std;

int main() {

    int listen_port;
    int target_port;
    char target_host[256];
    
    FILE* myfile = fopen("Config.txt","r");

        fscanf(myfile, "listen_port=%d target_port=%d target_host=%s", &listen_port, &target_port, target_host);
        cout << "listen_port: " << listen_port <<endl;
        cout << "target_port: " << target_port <<endl;
        cout << "target_host: " << target_host <<endl;
    
    
    fclose(myfile);

    return 0;
}