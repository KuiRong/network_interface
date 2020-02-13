#include <stdio.h>
#include <string.h>
#include <errno.h>

int GetNetCardCount()
{
    int nCount = 0;
    FILE* f = fopen("/proc/net/dev", "r");
    if (!f)
    {
        fprintf(stderr, "Open /proc/net/dev failed!errno:%d\n", errno);
        return nCount;
    }
    char szLine[512];
    fgets(szLine, sizeof(szLine), f);    /* eat line */
    fgets(szLine, sizeof(szLine), f);
    while(fgets(szLine, sizeof(szLine), f))
    {
        char szName[128] = {0};
        sscanf(szLine, "%s", szName);
        int nLen = strlen(szName);
        if (nLen <= 0)continue;
        if (szName[nLen - 1] == ':') szName[nLen - 1] = 0;
        if (strcmp(szName, "lo") == 0)continue;
        nCount++;
    }
    fclose(f);
    f = NULL;
    return nCount;
}
void DispNetInfo(const char* szDevName)
{
    int s = socket(AF_INET, SOCK_DGRAM, 0);
    if (s < 0)
    {
        fprintf(stderr, "Create socket failed!errno=%d", errno);
        return;
    }

    struct ifreq ifr;
    unsigned char mac[6];
    unsigned long nIP, nNetmask, nBroadIP;

    printf("%s:\n", szDevName);
    strcpy(ifr.ifr_name, szDevName);
    if (ioctl(s, SIOCGIFHWADDR, &ifr) < 0)
    {
        return;
    }
    memcpy(mac, ifr.ifr_hwaddr.sa_data, sizeof(mac));
    printf("\tMAC: %02x-%02x-%02x-%02x-%02x-%02x\n",
            mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

    strcpy(ifr.ifr_name, szDevName);
    if (ioctl(s, SIOCGIFADDR, &ifr) < 0)
    {
        nIP = 0;
    }
    else
    {
        nIP = *(unsigned long*)&ifr.ifr_broadaddr.sa_data[2];
    }
    printf("\tIP: %s\n", inet_ntoa(*(in_addr*)&nIP));

    strcpy(ifr.ifr_name, szDevName);
    if (ioctl(s, SIOCGIFBRDADDR, &ifr) < 0)
    {
        nBroadIP = 0;
    }
    else
    {
        nBroadIP = *(unsigned long*)&ifr.ifr_broadaddr.sa_data[2];
    }
    printf("\tBroadIP: %s\n", inet_ntoa(*(in_addr*)&nBroadIP));
    strcpy(ifr.ifr_name, szDevName);
    if (ioctl(s, SIOCGIFNETMASK, &ifr) < 0)
    {
        nNetmask = 0;
    }
    else
    {
        nNetmask = *(unsigned long*)&ifr.ifr_netmask.sa_data[2];
    }
    printf("\tNetmask: %s\n", inet_ntoa(*(in_addr*)&nNetmask));
    close(s);
}
int main(int argc, char* argv[])
{
    printf("NetCardCount: %d\n", GetNetCardCount());
    return 0;
}
