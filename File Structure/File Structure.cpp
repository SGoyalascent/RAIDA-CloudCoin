#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include <direct.h>
#include <vector>


using namespace std;

bool ValidCredentials(int id)
{
    return (id >= 1 && id <= 16777215);
}


int main()
{   
    int Userid;

    cout << "Enter User_id:";    // get user_id
    cin >> Userid;

    while (!ValidCredentials(Userid))  // check if User_id in Range
    {
        cout << "User_id Invalid" <<endl;

        cout << "Enter User_id:  ";    
        cin >> Userid;
        // cout << endl;
    }

    string User_id = to_string(Userid);

    // E:\AscentSparks_Work\Tasks\Secrets

    string dirpath = "E:/Secrets/Users/"+User_id;  //Assume the path C:/root/Secrets/  exists
    int status = mkdir(dirpath.c_str());         // mkdir(const char *path, mode_t mode)
    //cerr << "Error :  " << strerror(errno) << endl;

    string Users_Folders_name[] = {
        "/Files",
        "/Passwords",
        "/Crypto_keys",
        "/Calenders",
        "/Diary",
        "/Emails",
        "/CloudCoin",
        "/Contacts"
        };

    if(status == 0)
    {
        // cout <<"New User_id Dir created"<<endl;

        for(int i = 0; i < 8; i++)
        {
            status = mkdir((dirpath+Users_Folders_name[i]).c_str()); 
        }
    }
    else
    {
       // cout <<"User Dir not created"<<endl;
    }

    string Group_id;

    cout << "Enter Group_id:  ";  // get Group_id
    cin >> Group_id;
   // cout << endl;

    dirpath = "E:/Secrets/Groups/"+Group_id; 
    status = mkdir(dirpath.c_str());
    //cerr << "Error :  " << strerror(errno) << endl;

    string Groups_Folders_name[] = {
        "/Files",
        "/Journal",
        "/Messages", 
        "/Group_profiles", 
        "/User_profiles" 
        };

    if(status == 0)
    {
        //cout <<"New Group_id Dir created"<<endl;

        for(int i = 0; i < 5; i++)
        {
            status = mkdir((dirpath+Groups_Folders_name[i]).c_str()); 
        }
    }
    else
    {
        //cout <<"Group Dir not created"<<endl;     
    }

    return 0;
}