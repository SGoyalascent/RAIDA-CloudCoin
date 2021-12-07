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
    
    std::string path = "opt/RAIDA-CloudCoin/RAIDA_AGENT/Testing/raida/";
    for (const auto & entry : fs::directory_iterator(path)) {
        std::cout << "Entry_Path: " <<entry.path() << std::endl;
        std::cout << "Entry: " <<entry << std::endl;

    }

    fs::path filePath("opt/RAIDA-CloudCoin/RAIDA_AGENT/Testing/raida/agent.primary.config");
    std::error_code ec;
    if(fs::exists(filePath, ec)) {
        cout <<"File exists"<<endl;
        std::cerr <<ec.message() ;
    }

    fs::path filePath("opt/RAIDA-CloudCoin/RAIDA_AGENT/Testing/raida/agent.mirror.config");
    std::error_code ec;
    if(fs::exists(filePath, ec)) {
        cout <<"File not exists"<<endl;
        std::cerr <<ec.message() ;
    }

}


int main() {

    cout << "Hello User! \n Welcome to the RAIDA Agent." <<endl;

    CheckRaidaConfig();

}