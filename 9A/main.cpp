/*************************************************************************
    > File Name: main.cpp
    > Author: Guo Hengkai
    > Description: Main program of chatroom for homework 9A
    > Created Time: Sat 02 May 2015 05:46:29 PM CST
 ************************************************************************/
#include <cstdio>
#include <cstring>
#include <string>
#include "common.h"
#include "chat_client.h"
#include "chat_server.h"

using std::string;
using ghk::ChatClient;
using ghk::ChatServer;

void PrintUsage(char **argv)
{
    printf("usage: %s <(s)erver or (c)lient> [parameters] \n", argv[0]);
    printf("  [parameters](for client): [IPaddress] [nick name]\n");
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        PrintUsage(argv);
        return -1;
    }
    else
    {
        if (argv[1][0] == 's')
        {
            ChatServer server;
            if (server.Init(ghk::SERV_PORT))
            {
                printf("The server is listening on Port %d...\n", ghk::SERV_PORT);
                server.Loop();
            }
            else
            {
                printf("Fail to initialize the server.\n");
                return -1;
            }
        }
        else if (argv[1][0] == 'c')
        {
            string ip_addr = string(ghk::DEFAULT_SERV_IP);
            if (argc >= 3)
            {
                ip_addr = string(argv[2]);
            }
            string nick_name = "";
            if (argc >= 4)
            {
                nick_name = string(argv[3]);
            }
            
            ChatClient client;
            if (client.Init(ip_addr, ghk::SERV_PORT, nick_name))
            {
                printf("The client %s is running...\n", client.nick_name().c_str());
                client.Loop();
            }
            else
            {
                printf("Fail to initialize the client.\n");
                return -1;
            }

        }
        else
        {
            PrintUsage(argv);
            return -1;
        }
    }

    return 0;
}
