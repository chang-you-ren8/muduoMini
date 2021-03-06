#include "SocketOps.h"
#include "InetAddress.h"

#include <fcntl.h>
#include <assert.h>
#include <arpa/inet.h>          //for struct sockaddr_in
#include <unistd.h>             //for close

void sockets::bindOrDie(int sockfd, const InetAddress &localAddr)
{
    struct sockaddr_in addr = localAddr.getSockaddr();

    int ret = ::bind(sockfd, (struct sockaddr *)&addr, sizeof(addr));
    assert(ret == 0);
}

void sockets::listenOrDie(int listenfd)
{
    int ret = ::listen(listenfd, SOMAXCONN);
    assert(ret == 0);
}

int sockets::accept(int listenfd, InetAddress *peerAddr)
{
    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);

    int conn = ::accept(listenfd, (struct sockaddr *)&addr, &len);
    peerAddr->setSockaddr(addr);

    return conn;
}

int sockets::createNonBlockOrDie()
{
    int sockfd = ::socket(AF_INET, SOCK_STREAM, 0);
    assert(sockfd >= 0);

    int oldFlag = ::fcntl(sockfd, F_GETFL);
    int newFlag = oldFlag | O_NONBLOCK;
    ::fcntl(sockfd, F_SETFL, newFlag);
    return sockfd;
}

void sockets::close(int sockfd)
{
    ::close(sockfd);
}

InetAddress sockets::getLocalAddr(int sockfd)
{
    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);
    int ret = ::getsockname(sockfd, (struct sockaddr *)&addr, &len);
    assert(ret == 0);
    return InetAddress(addr);
}

InetAddress sockets::getPeerAddr(int sockfd)
{
    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);
    int ret = ::getpeername(sockfd, (struct sockaddr *)&addr, &len);
    assert(ret == 0);
    return InetAddress(addr);
}

ssize_t sockets::write(int fd, const void *buf, size_t count)
{
    return ::write(fd, buf, count);
}

ssize_t sockets::read(int fd, void *buf, size_t count)
{
    return ::read(fd, buf, count);
}