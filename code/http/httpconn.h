/*
 * @Author       : mark
 * @Date         : 2020-06-15
 * @copyleft Apache 2.0
 */ 

#ifndef HTTP_CONN_H
#define HTTP_CONN_H

#include <sys/types.h>
#include <sys/uio.h>     // readv/writev
#include <arpa/inet.h>   // sockaddr_in
#include <stdlib.h>      // atoi()
#include <errno.h>      

#include "../log/log.h"
#include "../pool/sqlconnRAII.h"
#include "../buffer/buffer.h"
#include "httprequest.h"
#include "httpresponse.h"

class HttpConn {
public:
    HttpConn();

    ~HttpConn();

    void init(int sockFd, const sockaddr_in& addr);

    ssize_t read(int* saveErrno);

    ssize_t write(int* saveErrno);

    void Close();

    int GetFd() const;

    int GetPort() const;

    const char* GetIP() const;
    
    sockaddr_in GetAddr() const;
    
    bool process();

    int ToWriteBytes() { 
        return iov_[0].iov_len + iov_[1].iov_len; 
    }

    bool IsKeepAlive() const {
        return request_.IsKeepAlive();
    }

    // LT 水平触发，ET 边缘触发
    // ET的要求是需要一直读写，直到返回EAGAIN，否则就会遗漏事件。
    // 而LT的处理过程中，直到返回EAGAIN不是硬性要求，
    // 但通常的处理过程都会读写直到返回EAGAIN，
    // 但LT比ET多了一个开关EPOLLOUT事件的步骤。
    static bool isET;
    static const char* srcDir;
    // 对该变量的操作是原子的
    // static表示所有对象共有该变量
    static std::atomic<int> userCount;
    
private:
   
    int fd_;
    // socket地址，ipv4
    struct sockaddr_in addr_;
    // 
    bool isClose_;
    
    int iovCnt_;
    // iovec由基地址和长度组成
    // iov_[0]是响应头，iov_[1]是响应文件
    struct iovec iov_[2];
    
    Buffer readBuff_; // 读缓冲区
    Buffer writeBuff_; // 写缓冲区

    HttpRequest request_;
    HttpResponse response_;
};


#endif //HTTP_CONN_H