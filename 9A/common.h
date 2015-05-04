/*************************************************************************
    > File Name: common.h
    > Author: Guo Hengkai
    > Description: Common variable and function definition for homework 9A
    > Created Time: Sat 02 May 2015 07:07:04 PM CST
 ************************************************************************/
#ifndef NINE_A_COMMON_H_
#define NINE_A_COMMON_H_

#include <stdint.h>
#include <sys/types.h>

namespace ghk
{
const uint16_t SERV_PORT = 12345;
const char DEFAULT_SERV_IP[] = "127.0.0.1";
const int MAX_CONNECT = FD_SETSIZE;
const int MAX_LINE = 255;
const int LISTENQ = 20;

enum MsgType
{
    MSG_TEXT = 0,
    MSG_HEART,
    MSG_NAME
};

ssize_t readln(int fd, void *vptr, size_t n);
ssize_t writeln(int fd, const void *vptr, size_t n);
}  // namespace ghk

#endif  // NINE_A_COMMON_H_

