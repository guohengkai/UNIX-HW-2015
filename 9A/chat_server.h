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
    void AddClient(int index);
    void SetNickName(int index, const std::string &nick_name);
    void SendNickName(int index) const;
    void SendMessage(int src_index, int tar_index,
            const std::string &msg) const;
    void ReceiveMessage(int src_index, const char *raw_msg) const;
    void CheckHeart(int index);

    int client_fd_[MAX_CONNECT];
    std::string client_names_[MAX_CONNECT];
    int max_client_;
    int listen_fd_;
    fd_set all_set_;
    int max_fd_;
};
}  // namespace ghk

#endif  // NINE_A_CHAT_SERVER_H_

