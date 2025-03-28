#pragma once
#if defined(_WIN32)
/* WIN */
#ifndef _WIN32_WINNT /* defined for the Winsock headers to provide all the functions we neeed. */
#define _WIN32_WINNT 0x0600
#endif
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")
  
#else
/* UNIX */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#endif

#if defined (_WIN32)
#define CLOSESOCKET(s) closesocket(s)
#define ISVALIDSOCKET(s) ((s) != INVALID_SOCKET)
#define GETSOCKETERRNO() (WSAGetLastError())
#else
#define CLOSESOCKET(s) close(s)
#define ISVALIDSOCKET(s) ((s) >= 0)
#define SOCKET int
#define GETSOCKETERRNO() (errno)
#endif

#include <string>
#include <stdexcept>

#define STRING_SOCKET_ERRNO() std::to_string(GETSOCKETERRNO())

class SocketException : public std::exception {
private:
    std::string err_str_;
public:
    SocketException() = delete;
    SocketException(const std::string& estr) = delete;

    using exception::exception;

    SocketException(std::string&& estr) : err_str_(std::move(estr)) {}
    virtual const char* what() const noexcept override {
        return err_str_.c_str();
    }
};

class WinsockLib {    
private:
    WinsockLib() {
    #if defined(_WIN32)
            WSADATA d;
            if (WSAStartup(MAKEWORD(2, 2), &d)) {
                fprintf(stderr, "Failed to initialize.\n");
                return 1;
            }
    #endif
    }

    ~WinsockLib() noexcept {
    #if defined(_WIN32)
            WSACleanup();
    #endif
    }

    WinsockLib(const WinsockLib&) = delete;
    WinsockLib& operator=(const WinsockLib&) = delete;

public:
    static WinsockLib& Instance() {
        static WinsockLib instance;
        return instance;
    }
};