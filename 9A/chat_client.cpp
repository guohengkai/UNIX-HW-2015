/*************************************************************************
    > File Name: chat_client.cpp
    > Author: Guo Hengkai
    > Description: Chat client class implementation for homework 9A
    > Created Time: Sat 02 May 2015 08:24:41 PM CST
 ************************************************************************/
#include "chat_client.h"
#include <cstdio>
#include <string>
#include <stdint.h>
#include <string>

using std::string;

namespace ghk
{
bool ChatClient::Init(const string &ip_addr, uint16_t port,
        const string &nick_name)
{
    return false;
}

void ChatClient::Loop()
{

}

void ChatClient::SendNickName() const
{

}

void ChatClient::RequestNickName() const
{

}

void ChatClient::ShowMessage(const char *raw_msg) const
{

}

void ChatClient::SendMessage(const string &msg) const
{

}

void ChatClient::SendHeartCheck() const
{

}

void ChatClient::ShowTimeOutMessage() const
{

}
}  // namespace ghk
