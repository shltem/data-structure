#include <stdio.h>/*printf*/
#include "dhcp.h"

int main()
{
    int index = 0;
    uint32_t subnet_IP = 3232263936;
    char *ip_str = "192.168.150.1";
    size_t subnet_length = 24;
    dhcp_t *dhcp = NULL; 
    char buffer[20] = {0};
    uint32_t result_ip = 0;
    printf("test convertion\n");

    printf("str to ip 192.168.150.1 : %u\n", DHCPStringToIp(ip_str));
    DHCPIpToString(3232273921, buffer);
    printf("ip to str 3232273921 :%s\n", buffer);
    printf("\n");

    dhcp = DHCPCreate(subnet_IP, subnet_length);
    printf("dhcp after create\n");
    PrintDhcp(dhcp);
    printf("\n");

    DHCPAllocateIp(dhcp,  0, &result_ip); 
    printf("result when requested ip is 0 : %s\n", DHCPIpToString(result_ip, buffer));

    DHCPAllocateIp(dhcp,  subnet_IP +64, &result_ip); 
    printf("result after request is subnet_IP + 64 : %s\n", DHCPIpToString(result_ip, buffer));

   /* DHCPAllocateIp(dhcp,  subnet_IP -256 + (index * 2), &result_ip); 
    printf("result after illegal IP1 %u\n", result_ip);*/

    for(index = 12; index < 15; ++index)
    {
        DHCPAllocateIp(dhcp,  subnet_IP + (index * 2), &result_ip); 
        printf("result IP1 %s\n", DHCPIpToString(result_ip, buffer));
        DHCPAllocateIp(dhcp,  subnet_IP + (index * 2), &result_ip);
        printf("result IP2 %s\n", DHCPIpToString(result_ip, buffer));
    }
    printf("\n\n\n");
    printf("dhcp after allocate\n");
    PrintDhcp(dhcp);
    printf("\n");
    printf("Count befor %lu \n", DHCPCountFree(dhcp));
    printf("\n");
    printf("remove %s\n", DHCPIpToString(3232263962, buffer));
    printf("\n");
    DHCPFreeIp(dhcp, 3232263962) ;
    printf("Count after %lu \n", DHCPCountFree(dhcp));
    printf("dhcp after remove %s\n", DHCPIpToString(3232263962, buffer));
    printf("\n");
    PrintDhcp(dhcp);
    DHCPDDestroy(dhcp);
    return 0;
}