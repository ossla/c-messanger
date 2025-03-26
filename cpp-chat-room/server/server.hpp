
#pragma once
#include "../chat-room.hpp"

#include <cstring>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <unordered_map>
#include <deque>
#include <memory>
#include <charconv>


class ChatServer {
public:
    explicit ChatServer(std::ostream& log_ostream) : log_os_(log_ostream) {
        InitializeServerSocket();
    }
    ~ChatServer() {
        CLOSESOCKET(listen_socket_);
    }

    void HandleConnections();

private:
    SOCKET listen_socket_;
    std::ostream& log_os_;

    std::deque<std::string>             addresses_;
    std::unordered_map<SOCKET
        , std::unique_ptr<std::string>> connections_; /* stores SOCKET and ptr to the string with its address */

private:
    void InitializeServerSocket();
    void AcceptNewConnection(fd_set& master, int& max_socket);
    void ProcessClientData(fd_set& master, SOCKET curr_socket);
};


SOCKET InitializeServerSocket();
