#include"library.h"
void md5 (unsigned char *md_input, unsigned char *md_output)
{
	FILE *fp;
	unsigned char inp_cmd[64];
	const char *str;
	int count;
	strcpy(inp_cmd,"echo -n ");
	strcat(inp_cmd, md_input);
	strcat(inp_cmd," | md5sum ");	
	fp = popen(inp_cmd, "r");
	while (fgets(md_output,64, fp) != NULL)
	    printf("\n%s", md_output);
	fclose(fp);
	//Convert hex string to bytes
	str = md_output;
	for ( count = 0; count < 16; count++) {
		sscanf(str, "%2hhx", &md_output[count]);
	        str += 2;
	}
	printf("\n");
	for ( count = 0; count < 16; count++) {
		printf("%0x,",md_output[count]);
	}
	printf("\n");
	return;
}