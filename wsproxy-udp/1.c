#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char target_host[256];
int target_port;

typedef struct {
    int listen_port;
}settings_t;

settings_t settings;

void configfile()
{
    FILE *myfile = fopen("Config.txt", "r");
    fscanf(myfile, "listen_port=%d target_port=%d target_host=%s", &settings.listen_port, &target_port, target_host);
    fclose(myfile);
} 

int  main() {
    
    configfile();
 
    printf("Listen Port: %d", settings.listen_port);
    printf("Target Port: %d", target_port);
    printf("Target Host: %s", target_host);



}