/*************************************************************************
    > File Name: common.cpp
    > Author: Guo Hengkai
    > Description: Common function implementation for homework 9A
    > Created Time: Sat 02 May 2015 10:18:48 PM CST
 ************************************************************************/
#include "common.h"
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>

namespace ghk
{
ssize_t readln(int fd, void *vptr, size_t n)
{
    size_t nleft;
    ssize_t nread;
    char *ptr;

    ptr = static_cast<char*>(vptr);
    nleft = n;
    while (nleft > 0)
    {
        if ((nread = read(fd, ptr, nleft)) < 0)
        {
            if (errno == EINTR)
            {
                nread = 0;
            }
            else
            {
                return -1;
            }
        }
        else if (nread == 0)
        {
            break;
        }

        nleft -= nread;
        ptr += nread;
    }

    return n - nleft;
}

ssize_t writeln(int fd, const void *vptr, size_t n)
{
    size_t nleft;
    ssize_t nwritten;
    const char *ptr;

    ptr = static_cast<const char*>(vptr);
    nleft = n;
    while (nleft > 0)
    {
        if ((nwritten = write(fd, ptr, nleft)) <= 0)
        {
            if (nwritten < 0 && errno == EINTR)
            {
                nwritten = 0;
            }
            else
            {
                return -1;
            }

            nleft -= nwritten;
            ptr += nwritten;
        }
    }
    
    return n;
}
}  // namespace ghk
