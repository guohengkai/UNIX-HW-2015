/*************************************************************************
    > File Name: chat_client.cpp
    > Author: Guo Hengkai
    > Description: Chat client class implementation for homework 9A
    > Created Time: Sat 02 May 2015 08:24:41 PM CST
 ************************************************************************/
#include "chat_client.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>
#include <cstdio>
#include <cstring>
#include <stdint.h>
#include <string>
#include "common.h"

using std::string;

namespace ghk
{
bool ChatClient::Init(const string &ip_addr, uint16_t port,
        const string &nick_name)
{
    fp_ = stdin;
    if ((sock_fd_ = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        printf("Client socket error.\n");
        return false;
    }

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, ip_addr.c_str(), &serv_addr.sin_addr) != 1)
    {
        printf("IP address error.\n");
        return false;
    }

    if (connect(sock_fd_, (struct sockaddr *)&serv_addr,
                sizeof(serv_addr)) == -1)
    {
        printf("Client connect error.\n");
        return false;
    }

    printf("Start to set the name to [%s]...\n", nick_name.c_str());
    nick_name_ = nick_name;
    SendNickName();

    return true;
}

void ChatClient::Loop()
{
    fd_set recv_set;
    char send_line[MAX_LINE];
    const int in_fd = fileno(fp_);
    const int max_fd = std::max(in_fd, sock_fd_);
    int nready;

    FD_ZERO(&recv_set);
    // printf("Start looping...\n");
    while (1)
    {
        FD_SET(in_fd, &recv_set);
        FD_SET(sock_fd_, &recv_set);
        if ((nready = select(max_fd + 1, &recv_set, NULL, NULL, NULL)) < 0)
        {
            printf("Client select error.\n");
            return;
        }
        // printf("The number of response is %d.\n", nready);

        if (FD_ISSET(sock_fd_, &recv_set))  // Socket is readable
        {
            char type, len;
            if (read(sock_fd_, &type, 1) <= 0)
            {
                printf("Client socket read error.\n");
                return;
            }
            readln(sock_fd_, &len, 1);

            char raw_msg[MAX_LINE];
            readln(sock_fd_, raw_msg, len);
            raw_msg[(int)len] = '\0';
            
            switch (type)
            {
                case MsgType::MSG_TEXT:
                    ShowMessage(raw_msg);
                    break;
                case MsgType::MSG_HEART:
                    SendHeartCheck();
                    break;
                case MsgType::MSG_NAME:
                    nick_name_ = string(raw_msg);
                    printf("Hello, your name is [%s].\n", nick_name_.c_str());
                    break;
                default:
                    printf("Client message type error.\n");
                    return;
            }
        }

        if (FD_ISSET(in_fd, &recv_set))  // Input is readable
        {
            if (fgets(send_line, MAX_LINE, fp_) == NULL)
            {
                return;
            }
            send_line[strlen(send_line) - 1] = '\0';
            SendMessage(string(send_line));
        }
    }
}

void ChatClient::SendRawMessage(MsgType type, const string &msg) const
{
    char raw_msg[msg.size() + 2];
    raw_msg[0] = type;
    raw_msg[1] = static_cast<char>(msg.size());
    if (!msg.empty())
    {
        memcpy(raw_msg + 2, msg.c_str(), msg.size());
    }
    writeln(sock_fd_, raw_msg, msg.size() + 2);
    // printf("Sent message: %s\n", msg.c_str());
}

void ChatClient::SendNickName() const
{
    SendRawMessage(MsgType::MSG_NAME, nick_name_);
    // printf("Name [%s] sent.\n", nick_name_.c_str());
}

void ChatClient::SendMessage(const string &msg) const
{
    // Need to add timer for time out
    SendRawMessage(MsgType::MSG_TEXT, msg);
}

void ChatClient::SendHeartCheck() const
{
    SendRawMessage(MsgType::MSG_HEART, "OK");
}

void ChatClient::ShowMessage(const char *raw_msg) const
{
    const int name_len = raw_msg[0];
    const int msg_len = strlen(raw_msg) - 1 - name_len;
    char name[name_len + 1];
    char msg[msg_len + 1];

    memcpy(name, raw_msg + 1, name_len);
    memcpy(msg, raw_msg + 1 + name_len, msg_len);
    name[name_len] = msg[msg_len] = '\0';

    printf("[%s] said: %s\n", name, msg);
}

void ChatClient::ShowTimeOutMessage() const
{
    printf("Time out for sending message, please try again.\n");
}
}  // namespace ghk
