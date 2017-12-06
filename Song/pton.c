#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define IPADDR	"192.168.1.102"

// 0x66		01	a8		c0
// 102		1	168		192
// 网络字节序，其实就是大端模式


int main(void)
{
	struct in_addr addr = {0};
	char buf[50] = {0};
	
	addr.s_addr = 0x6601a8c0;
	
	inet_ntop(AF_INET, &addr, buf, sizeof(buf));

	printf("ip addr = %s.\n", buf);
	
	return 0;
}
