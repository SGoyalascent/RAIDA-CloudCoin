#include <iostream>
#include <fstream>
#include <experimental/filesystem>
#include <string>

using namespace std;

//using namespace std;

#define PRIMARY_MODE          1
#define WITNESS_MODE          3
#define MIRROR_MODE           2

void CheckRaidaConfig() {
    
    string filepath = "opt/raida/agent.primary.config";
    string dirpath = "opt/raida/";

    if(filepath.find(dirpath) != string::npos) {
        cout << "File exists";
    }
}


int main() {

    std::cout << "Hello User! \n Welcome to the RAIDA Agent." <<std::endl;

    CheckRaidaConfig();

}