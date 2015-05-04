/*************************************************************************
    > File Name: chat_server.h
    > Author: Guo Hengkai
    > Description: Chat server class definition for homework 9A
    > Created Time: Sat 02 May 2015 07:34:57 PM CST
 ************************************************************************/
#ifndef NINE_A_CHAT_SERVER_H_
#define NINE_A_CHAT_SERVER_H_

#include <stdint.h>
#include <string>
#include "common.h"

namespace ghk
{
class ChatServer
{
public:
    bool Init(uint16_t port);
    void Loop();

private:
    bool AddClient(int conn_fd);
    void RemoveClient(int index);
    void SetNickName(int index, const char *raw_msg);

    void SendRawMessage(int index, MsgType type, const char *raw_msg) const;
    void SendMessage(int index, const char *msg) const;

    int client_fd_[MAX_CONNECT];
    std::string client_names_[MAX_CONNECT];
    int max_client_;
    int listen_fd_;
    int max_fd_;
    fd_set all_set_;
};
}  // namespace ghk

#endif  // NINE_A_CHAT_SERVER_H_

