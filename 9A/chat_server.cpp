/*************************************************************************
    > File Name: chat_server.cpp
    > Author: Guo Hengkai
    > Description: Chat server class implementation for homework 9A 
    > Created Time: Sat 02 May 2015 08:29:19 PM CST
 ************************************************************************/
#include "chat_server.h"
#include <cstdio>
#include <string>
#include <stdint.h>
#include <sstream>
#include <string>

using std::string;

namespace ghk
{
bool ChatServer::Init(uint16_t port)
{
    return true;
}

void ChatServer::Loop()
{

}

void ChatServer::AddClient(int index)
{

}

void ChatServer::SetNickName(int index, const string &nick_name)
{

}

void ChatServer::SendNickName(int index) const
{

}

void ChatServer::SendMessage(int src_index, int tar_index,
        const string &msg) const
{

}

void ChatServer::ReceiveMessage(int src_index, const char *raw_msg) const
{

}

void ChatServer::CheckHeart(int index)
{

}
}  // namespace ghk
