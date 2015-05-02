/*************************************************************************
    > File Name: chat_client.h
    > Author: Guo Hengkai
    > Description: Chat client class definition for homework 9A
    > Created Time: Sat 02 May 2015 07:10:52 PM CST
 ************************************************************************/
#ifndef NINE_A_CHAT_CLIENT_H_
#define NINE_A_CHAT_CLIENT_H_

#include <stdint.h>
#include <string>

namespace ghk
{
class ChatClient
{
public:
    bool Init(const std::string &ip_addr, uint16_t port,
            const std::string &nick_name);
    void Loop();

    const std::string &nick_name() const { return nick_name_; }

private:
    void SendNickName() const;
    void RequestNickName() const;
    void ShowMessage(const char *raw_msg) const;
    void SendMessage(const std::string &msg) const;
    void SendHeartCheck() const;
    void ShowTimeOutMessage() const;

    int sock_fd_;
    std::string nick_name_;
};
}  // namespace ghk

#endif  // NINE_A_CHAT_CLIENT_H_
