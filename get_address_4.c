#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <ifaddrs.h>

int main(void)
{
	struct ifaddrs *address = (struct ifaddrs *)NULL;
	struct ifaddrs *Next = (struct ifaddrs *)NULL;
	int Ret = -1, i = -1;
	struct sockaddr_in *SockAddr_4 = (struct sockaddr_in *)NULL;
	char ipv4_addr[INET_ADDRSTRLEN];

	Ret = getifaddrs(&address);
	if (Ret == -1) {
		printf("get ip_address error\n");
	}

	for (Next = address; Next != (struct ifaddrs *)NULL; Next = Next->ifa_next){
		if (Next->ifa_addr->sa_family == AF_INET) {

			printf("IPv4 Interface (AF_INET, %d): \n",Next->ifa_addr->sa_family);

		// Interface Name
			printf("pNext->ifa_name: %s \n", Next->ifa_name);

		// Interface Address
			printf("IPv4 Address: \n");
			SockAddr_4 = (struct sockaddr_in *)Next->ifa_addr;
			memset(ipv4_addr, '\0',sizeof(char) * INET_ADDRSTRLEN);

			inet_ntop(AF_INET, (void *)&SockAddr_4->sin_addr,ipv4_addr, sizeof(char) * INET_ADDRSTRLEN);
			printf("SockAddr_4->sin_addr: %s \n", ipv4_addr);
		}
		else if (Next->ifa_addr->sa_family == AF_INET6) { }
		else if (Next->ifa_addr->sa_family == AF_PACKET) { }
		else { }
		printf("\n");
	}
	freeifaddrs((void *)address);

	return 0;
}
