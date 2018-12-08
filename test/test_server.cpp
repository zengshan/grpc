#include <iostream>
#include <mutex>
#include <atomic>
#include <simple_rpc/net/SocketLibFunction.h>
#include <simple_rpc/net/TcpService.h>
#include <simple_rpc/net/ListenThread.h>
#include <simple_rpc/net/Socket.h>

using namespace simple_rpc;

std::atomic_llong TotalRecvSize = ATOMIC_VAR_INIT(0);
std::atomic_llong total_client_num = ATOMIC_VAR_INIT(0);
std::atomic_llong total_packet_num = ATOMIC_VAR_INIT(0);

void RecvClientData(const DataSocket::PTR& session, const char* buffer, size_t len){
    session->send(buffer, len);
    TotalRecvSize += len;
    total_packet_num++;
    return len;
}

void ClientDisconnec(const DataSocket::PTR& session){
    total_client_num--;
}


void ClientEnterCallback(const DataSocket::PTR& session){
    total_client_num ++;
    auto func = bind(RecvClientData, session)
    session->setDataCallback(func)
    session->setDisConnectedCallback(ClientDisconnec);
}

void AccepSocket(TcpService& service, TcpSocket::PTR socket){
    socket->SocketNodelay();
    server->addDataSocket(std::move(socket), TcpService::addSocketOption::withEnterCallback(ClientEnterCallback),
        TcpService::addSocketOption::withMaxRecvBufferSize(1024*1024));
}


int main(void){
    int port = 8888;
    int thread_num = 3;
    std::string ip("127.0.0.1")
    auto server = TcpService::Create();
    auto thread = ListenThread::Create();
    func = bind(AccepSocket, server)
    thread->startListen(false, ip, port, AccepSocket);
    server->startWorThread(thread_num);
    EventLoop mainLoop;
    while(true){
        mainLoop.loop(100);
        std::cout << "total recv : " << (TotalRecvSize / 1024) / 1024 << " M /s, of client num:" << total_client_num << std::endl;
        std::cout << "packet num:" << total_packet_num << std::endl;
        total_packet_num = 0;
        TotalRecvSize = 0;
    }
}
