/*************************************************************************
    > File Name: chat_server.cpp
    > Author: Guo Hengkai
    > Description: Chat server class implementation for homework 9A 
    > Created Time: Sat 02 May 2015 08:29:19 PM CST
 ************************************************************************/
#include "chat_server.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>
#include <cstdio>
#include <cstring>
#include <stdint.h>
#include <sstream>
#include <string>
#include "common.h"

using std::string;

namespace ghk
{
bool ChatServer::Init(uint16_t port)
{
    if ((listen_fd_ = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        printf("Server socket error.\n");
        return false;
    }

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(port);

    if (bind(listen_fd_, (struct sockaddr *)&serv_addr,
                sizeof(serv_addr)) == -1)
    {
        printf("Server bind error.\n");
        return false;
    }

    if (listen(listen_fd_, LISTENQ) == -1)
    {
        printf("Server listen error.\n");
        return false;
    }
    printf("Server listening on the port %d...\n", port);

    memset(client_fd_, -1, MAX_CONNECT);
    max_client_ = -1;
    FD_ZERO(&all_set_);
    FD_SET(listen_fd_, &all_set_);
    max_fd_ = listen_fd_;

    return true;
}

void ChatServer::Loop()
{
    fd_set recv_set;
    int nready;

    // printf("Start looping...\n");
    while (1)
    {
        recv_set = all_set_;
        if ((nready = select(max_fd_ + 1, &recv_set, NULL, NULL, NULL)) < 0)
        {
            printf("Server select error.\n");
            return;
        }
        // printf("The number of response is %d.\n", nready);

        if (FD_ISSET(listen_fd_, &recv_set))  // New client connection
        {
            struct sockaddr_in c_addr;
            socklen_t c_len = sizeof(c_addr);
            int conn_fd;
            if ((conn_fd = accept(listen_fd_, (struct sockaddr *)&c_addr,
                            &c_len)) < 0)
            {
                printf("Server accept error.\n");
                return;
            }
            
            if (!AddClient(conn_fd))
            {
                return;
            }

            if (--nready <= 0)
            {
                continue;
            }
        }
        
        // Check all the clients for data
        for (int i = 0; i <= max_client_; ++i)
        {
            int sock_fd;
            if ((sock_fd = client_fd_[i]) < 0)
            {
                continue;
            }

            if (FD_ISSET(sock_fd, &recv_set))
            {
                char type, len;
                if (read(sock_fd, &type, 1) <= 0)
                {
                    RemoveClient(i);
                }
                else
                {
                    readln(sock_fd, &len, 1);

                    char raw_msg[MAX_LINE];
                    if (len > 0)
                    {
                        readln(sock_fd, raw_msg, len);
                    }
                    raw_msg[(int)len] = '\0';
                    printf("Received raw message from client %d: %s\n", i, raw_msg);

                    switch (type)
                    {
                        case MsgType::MSG_TEXT:
                            SendMessage(i, raw_msg);
                            break;
                        case MsgType::MSG_NAME:
                            SetNickName(i, raw_msg);
                            break;
                        default:
                            printf("Server message type error.\n");
                            return;
                    }
                }

                if (--nready <= 0)
                {
                    break;
                }
            }
        }
    }
}

bool ChatServer::AddClient(int conn_fd)
{
    int i;
    for (i = 0; i < MAX_CONNECT; ++i)
    {
        if (client_fd_[i] < 0)
        {
            client_fd_[i] = conn_fd;
            break;
        }
    }
    if (i == MAX_CONNECT)
    {
        printf("Too many clients.\n");
        return false;
    }

    FD_SET(conn_fd, &all_set_);
    if (conn_fd > max_fd_)
    {
        max_fd_ = conn_fd;
    }
    if (i > max_client_)
    {
        max_client_ = i;
    }

    std::stringstream ss;
    ss << i;
    client_names_[i] = "Client " + ss.str();
    printf("Client %d connected.\n", i);
    
    return true;
}

void ChatServer::RemoveClient(int index)
{
    printf("Client %d offline..\n", index);
    int sock_fd = client_fd_[index];
    close(sock_fd);
    FD_CLR(sock_fd, &all_set_);
    client_fd_[index] = -1;
}

void ChatServer::SendRawMessage(int index, MsgType type,
        const char *raw_msg) const
{
   char final_msg[strlen(raw_msg) + 2];
   final_msg[0] = type;
   final_msg[1] = strlen(raw_msg);
   memcpy(final_msg + 2, raw_msg, strlen(raw_msg));
   writeln(client_fd_[index], final_msg, strlen(raw_msg) + 2);
}
void ChatServer::SetNickName(int index, const char *raw_msg)
{
    int len = strlen(raw_msg);
    if (len > 0)
    {
        client_names_[index] = string(raw_msg);
    }
    SendRawMessage(index, MsgType::MSG_NAME, client_names_[index].c_str());
    printf("Name of client %d is: %s\n", index, client_names_[index].c_str());
}

void ChatServer::SendMessage(int index, const char *msg) const
{
    char raw_msg[MAX_LINE];
    raw_msg[0] = static_cast<char>(client_names_[index].size());
    memcpy(raw_msg + 1, client_names_[index].c_str(),
            client_names_[index].size());
    memcpy(raw_msg + 1 + raw_msg[0], msg, strlen(msg));
    raw_msg[raw_msg[0] + 1 + strlen(msg)] = '\0';
    for (int i = 0; i <= max_client_; ++i)
    {
        if (i != index && client_fd_[i] >= 0)
        {
            SendRawMessage(i, MsgType::MSG_TEXT, raw_msg);
            printf("Sended to Client %d.\n", i);
        }
    }
}
}  // namespace ghk
