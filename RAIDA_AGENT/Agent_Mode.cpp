#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
namespace fs = std::filesystem;

using namespace std;

#define PRIMARY_MODE          1
#define WITNESS_MODE          3
#define MIRROR_MODE           2

void CheckRaidaConfig() {
    
    std::string path = "opt/raida/";
    for (const auto & entry : fs::directory_iterator(path)) {
        std::cout << "Entry_Path: " <<entry.path() << std::endl;
        std::cout << "Entry: " <<entry << std::endl;

    }

    fs::path filePath("opt/raida/agent.primary.config");
    std::error_code ec;
    if(fs::exists(filepath, ec)) {
        cout <<"File exists"<<endl;
        std::cerr <<ec.message() ;
    }

}


int main() {

    cout << "Hello User! \n Welcome to the RAIDA Agent." <<endl;

    CheckRaidaConfig();

}