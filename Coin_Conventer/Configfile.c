#include <stdio.h>
#include <string.h>

int main() {

    char Host_ip[256];
    char Database_name[256];
    char Username[256];
    char User_password[256];
    int listen_port;
    char Encryption_key[256];
    char Mode[256];

    printf("Hello User\n");

    FILE *myfile = fopen("Coin_Converter.config", "r");
    if(myfile == NULL) {
        printf("Config file not found\n");
    }
    fscanf(myfile, "Host = %255s Database = %255s Username = %255s Password = %255s listenport = %d encryption_key = %255s mode = %255s", Host_ip, Database_name,
                                                  Username, User_password, &listen_port, Encryption_key, Mode);
    fclose(myfile);

    printf("Host = %s\n Database = %s\n Username = %s\n Password = %s\n listenport = %d\n encryption_key = %s\n mode = %s\n", Host_ip, Database_name, Username, User_password, &listen_port, Encryption_key, Mode);

    return 0;
}
