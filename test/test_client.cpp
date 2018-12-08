#include <iostream>
#include <string>

#include <simple_rpc/net/SocketLibFunction.h>
#include <simple_rpc/net/TcpService.h>
#include <simple_rpc/net/ListenThread.h>
#include <simple_rpc/net/Socket.h>


using namespace simple_rpc;

void RecvClientData(const DataSocket::PTR& session, const char* buffer, size_t len){
    session->send(buffer, len);
    TotalRecvSize += len;
    total_packet_num++;
    return len;
}
std::string test("test/test");

void ClientEnterCallback(const DataSocket::PTR& session){
    // 接受数据后的恢复
    session->setDataCallback(RecvClientData);
    //发送数据
    session->send(test.c_str(), test.size());
}


void ClientConnectSucc(const TcpService::PTR& server, TcpSocket::PTR socket){
    socket->SocketNodelay();
    server->addDataSocket(std::move(socket), TcpService::addSocketOption::withEnterCallback(ClientEnterCallback),
        TcpService::addSocketOption::withMaxRecvBufferSize(1024*1024));
}

void CilentDisConnect(void){
    std::cout << "error:" << e.what() << std::endl;
}

int main(void){
    int port = 8888;
    int thread_num = 3;
    std::string ip("127.0.0.1");
    auto server = TcpService::Create();
    server->startWorkThread(thread_num);

    auto connector = AsyncConnector::Create();
    connector->startWorkThread();

    auto timeout = std::chrono::seconds(10);

    connector->asycConnect(ip, port, timeout, ClientConnectSucc, CilentDisConnect);



}