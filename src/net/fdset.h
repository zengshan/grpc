#pragma once

#include <stdbool.h>
#include <simple_rpc/net/SocketLibTypes.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

extern "C" {

// struct pollfd {
//     int   fd; socket文件描述符
//     short events;  等待的事件
//     short revents;  实际发生的事件
// };

enum CheckType
{   
    // 001
    ReadCheck = 0x1,
    // 010
    WriteCheck = 0x2,
    // 100
    ErrorCheck = 0x4,
};

struct fdset_s{
    struct pollfd* poll_fds;
    int nfds;
    int limit_size;
};

void _extandPollfd(struct fdset_s* p_set, int size_num){
    int num = size_num + p_set->limit_size;
    // 内存增加
    struct pollfd* new_fd_s = (struct pollfd*) mallac(sizeof(struct pollfd) * num);
    if(!new_fd_s){
        return;
    }
    // 有可能原来的数据有问题
    if(p_set->poll_fds != NULL){
        memcpy(new_fd_s, p_set->pollfds, sizeof(struct pollfd) * p_set.nfds);
        free(p_set->poll_fds);
        p_set->poll_fds = NULL;
    }
    
    p_set->poll_fds = new_fd_s;
    p->limit_size += size_num;
}

struct fdset_s* ox_fdset_new(void){
    struct fdset_s* ret = (struct fdset_s*) malloc(sizeof(struct fdset_s));
    if(!ret){
        return ret;
    }

    ret->poll_fds = NULL;
    ret->limit_size = 0;
    ret->nfds = 0;
    _extandPollfd(ret, 1024);

    return ret;
}
void ox_fdset_delete(struct fdset_s* self){
    free(self->poll_fds);

    self->poll_fds = NULL;
    self->nfds = 0;
    self->limit_size = 0;
    
    free(self);
    self = NULL;
}

struct pollfd* _findPollfd(struct fdset_s* self, sock fd){

    for (int i=0; i < self->nfds; i++)
    {
        if(self->poll_fds[i].fd == fd)
        {
            return self->poll_fds+i;
        }
    }

    return NULL;

}


void TryRemovePollFd(struct fdset_s* self, sock fd)
{
    int i = 0;
    for (; i < self->nfds; i++)
    {
        if(self->pollFds[i].fd == fd)
        {   
            //将后面的往前移动
            memmove(self->pollFds + i, self->pollFds + i + 1, sizeof(struct pollfd)*(self->nfds - i - 1));
            self->nfds--;
            assert(self->nfds >= 0);
            break;
        }
    }
}

void ox_fdset_add(struct fdset_s* self, sock fd, int type)
{
    //
    if(self->limit_size == self->nfds){
        _extandPollfd(self, 128);
    }
    if(self->limit_size <= self->nfds){
        return;
    }
    struct pollfd* p_fd = _findPollfd(self, fd);

    //一个新的事件，在dict中不存在
    if(p_fd == NULL){
        p_fd = self->poll_fds + self->nfds;
        p_fd->events = 0;
        p_fd->fd = fd;
        
        self->nfds ++;
    }

    if (type & ReadCheck)
    {
        pf->events |= POLLIN ;
    }

    if (type & WriteCheck)
    {
        pf->events |= POLLOUT;
    }

    // 添加epoll之类的事件
    if(pf->events == 0){
        // 添加没有成功
        _removePollfd(self, fd);
    }


}
//移除关注事件
void ox_fdset_del(struct fdset_s* self, sock fd, int type){
    struct pollfd* pfd = _findPollfd(self, fd);
    if(pfd == NULL){
        return;
    }
    if(type && ReadCheck){
        pfd->events &= ~POLLIN;
    }
    if(type && WriteCheck){
        pdf-events &= ~POLLOUT;
    }
    
    // 没有需要监听的事件了
    if(pfd->events == 0){
        _removePollfd(self, fd);
    }
}
void ox_fdset_remove(struct fdset_s* self, sock fd){
    _removePollfd(self, fd);
}
int ox_fdset_poll(struct fdset_s* self, long overtime){
    int ret = poll(self->poll_fds, self->nfds, overtime);
    if(ret== SOCKET_ERROR){
        // 系统阻塞与系统调用
        return (sErrno != EINTR) ? -1: 0;
    }
    return ret;
}

}