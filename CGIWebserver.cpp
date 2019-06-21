#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int main(int argc, char *argv[])
{
    if(argv<=2)
    {
        printf("usage: %s ip_address port number\n", basename(argv[0]));
        return 1;
    }
    const char *ip = argv[1];
    int port = atoi(argv[2]);
    struct sockaddr_in address;
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &address.sin_addr);
    address.sin_port = htons(port);

    int sock = socket(PF_INET, SOCK_STREAM, 0);
    assert(sock >= 0);

    int ret = bind(sock, (struct sockaddr *)&address, sizeof(address));
    assert(ret != -1);

    ret = listen(sock, 5);
    assert(ret != -1);

    struct sockaddr_in client;
    socklen_t client_addrlength = sizeof(client);
    int connfd = accept(sock, (struct sockaddr *)&client, &client, &client_addrlength);
    if(connfd < 0)
    {
        printf("errno is: %d\n", errno);
    }
    else
    {
        close(STDOUT_FILENO);
        dup(connfd);//返回系统中最小的可用文件描述符；由于上一句关闭了STD_OUT，因此这里会返回1.
        printf("abcd\n");//导致了这里的printf会直接print到连接的客户端。
        close(connfd);
    }
    close(sock);
    return 0;
    
}