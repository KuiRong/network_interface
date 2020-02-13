#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/wait.h>
#include <string.h>

#define BUFFER_SIZE 65536

void parse_interface(char * data)
{
	printf("data is \n%.10s\n",data);
	char chunk_str[20];
	char *pch = NULL, *pch1 = NULL, *pch2 = NULL;
	char *virtual_str = "virtual";
	char *net_str = "net/";
	memset(chunk_str, 0, sizeof(chunk_str));
	sprintf(chunk_str, "device/");

	
	memcpy(pch, data, strlen(data));
	pch = pch + 8;
	printf("pch + 8 is %.10s",pch);
	pch1 = strstr(pch, chunk_str);

	printf("pch1 is %.10s\n",pch1-10);
	char *interface_name;

	while(1){
		if (pch1 == NULL){
			return 0;
		}else if ((pch1 + 1) == 'v'){
			continue;
		}else if ((pch1 + 1) == 'p'){
			pch2 = strstr(pch1, net_str);
			pch1 = pch2 + strlen(net_str);
			pch2 = strstr(pch1, "\r\n");
			strncpy(interface_name, pch1, (pch2 - pch1));
			printf("interface name is %s",interface_name);
			pch1 += strlen(interface_name)+2;
		}
	}
}

void get_popen(){
	FILE *pf;
	char command[25];
	char *data;
	//   char *ret;
	int ret;
	// Execute a process listing
	sprintf(command, "ls -l /sys/class/net"); 

	// Setup our pipe for reading and execute our command.
	pf = popen(command,"r"); 
	// Error handling

	// Get the data from the process execution
	data = (char*)malloc(BUFFER_SIZE);
	//ret = fgets(data, BUFFER_SIZE, pf);
	ret = fread(data,1 , BUFFER_SIZE , pf);
	printf("return value of fread is %d\n",ret);
	printf("data is \n%s", data);

	char* data2 = data +8;
	printf("data2 is \n%s", data2);
	char chunk_str[20];
	char *pch = NULL, *pch1 = NULL, *pch2 = NULL;
	char *virtual_str = "virtual";
	char *net_str = "net/";
	char *end_char = "\/";

	memset(chunk_str, 0, sizeof(chunk_str));
	sprintf(chunk_str, "devices/");
	printf("\n\n\n");
//	pch1 = strstr(data2, chunk_str);
	
	//	parse_interface(data);
	char *interface_name;
	interface_name = (char*)malloc(BUFFER_SIZE);

	pch1 += strlen(chunk_str);

//	pch2 = strstr(pch1, end_char);

	int size = pch2 - pch1;
	pch = data2;
	while(1){
		pch1 = strstr(pch, chunk_str);
//		printf("pch1 is %.10s\n",pch1);
		if (pch1 == NULL){
			return 0;
		}
		pch2 = strstr(pch1+strlen(chunk_str), end_char);
		pch1 += strlen(chunk_str);
//		printf("pch2 is %.10s\n",pch2);

		size = pch2 - pch1;
//		printf("size is %d\n",size);
//		printf("strlen is %d\n",strlen(virtual_str));

		if (size == strlen(virtual_str)){
			pch = pch1 + strlen(virtual_str);
			continue;
		}else if (size > strlen(virtual_str)){
			pch1 = strstr(pch2, net_str);
			pch2 = pch1 + strlen(net_str);
	
			pch1 = pch2;
			pch2 = strstr(pch1, "\n");
	
			//strncpy(interface_name, pch1, (pch2 - pch1));
			memset(interface_name,'\0', BUFFER_SIZE);
			strncpy(interface_name, pch1, 6);

			printf("interface name is %s\n",interface_name);
			pch1 += strlen(interface_name)+1;
			pch = pch1;
		}
	
	}

	// the data is now in 'data'

	if (pclose(pf) != 0)
		fprintf(stderr," Error: Failed to close command stream \n");

	return;
}

int main(void){

	get_popen();
	return 0;
}
