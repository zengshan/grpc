
#include <cstdio>
#include <cstring>

namespace simple_rpc { namespace net { namespace base {
    bool initSocket(void){
        singal(SIGPIPE, SIG_IGN);
        return true;
    }

    void destrySocket(void){
        ;
    }

    int socetNoDelay(sock fd){
        const int flag = 1;
        return setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, (char*)&flag, sizeof(flag));
    }

    int SocketNonBlock(sock fd){
        int error;
        unsigned long ul = true;
        error = ioctl(fd, FIONBIO, &ul);
        return error != SOCKET_ERROR;
    }

    int SocketSetSendSize(sock fd, int socket_size){
        return setsockopt(fd, SOL_SOCKET, SO_SNDBUF, (const char*)&socket_size, sizeof(socket_size));
    }

    int SocketSetRecvSizd(sock fd, int socket_size){
        return setsockopt(fd, SOL_SOCKET, SO_RECVBUF, (const char*)&socket_size, sizeof(socket_size));
    }

    sock Connect(const std::string& server_ip, int port){
        initSocket();
        struct sockaddr_in ipaddr = {0};
        struct sockaddr_in* paddr = ipaddr;

        sock clientfd = SocketCreate(AF_INET, SOCK_STREAM, 0);
        if(clientfd == -1){
            return clientfd;
        }
        ipaddr.sin_family = AF_INET;
        ipaddr.sin_port = port;
        bool result = inet_pton(AF_INET, server_ip.c_str(), &ipaddr.sin_addr);
        if(!result){
            SocketClose(clientfd);
            return -1;
        }
        int addrlen = sizeof(ipaddr);
        while(connect(clientfd, (stuct sockaddr*)paddr, addrlen) < 0){
            if (EINTR == sErrno){
                continue;
            }
            SocketClose(clietnfd);
            return -1;
        }
        return clientfd;
    }

    sock Listen(const char*ip, int port, int back_num){
        initSocket();
        struct socketaddr_in ipaddr ={0};
        struct socketaddr_in* paddr = &ipaddr;

        sock sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd == -1){
            return -1;
        }
        boot ptonResult = False;
        ipaddr.sin_family = AF_INET;
        ipaddr.sin_port = port;
        ipaddr.sin_addr.s_addr = INADDR_ANY;
        ptonResult = inet_pton(AF_INET, ip, &ipaddr.sin_adr) > 0;
        // 端口不能复用
        int resuseaddr_value = 1;
        if (!ptonResult){
            if (setsocketopt(socketfd, SOL_SOCKET, SO_REUSEADDR, (const char*)*resuseaddr_value, sizeof(int))<0){
                SocketClose(sockfd);
                return -1;
            }
        }
        int bindRet = bind(sockfd, (stuct sockaddr*)paddr, addrlen);
        if (bindRet == SOCKET_ERROR){
            SocketClose(sockfd);
            return -1;
        }
        bindRet = listen(sockfd, back_num);
        if(bindRet == SOCKET_ERROR){
            SocketClose(sockfd);
            return -1;
        }
        return sockfd;

    }

    void SocketClose(sock fd){
        close(fd);
    }

    void SocketSend(sock fd, const char* buffer, int len){
        int send_num = send(fd, buffer, len, 0);
        //缓冲区已满
        if (send_num < 0 && S_EWOULDBLOCK==sErrno){
            send_num = 0;
        }
        return send_num;
    }

    void SocketAccept(sock sockfd, struct sockaddr* addr, socklen_t* addrlen){
        return accept(sockfd, addr, addrlen);
    }

}}}