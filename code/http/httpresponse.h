/*
 * @Author       : mark
 * @Date         : 2020-06-25
 * @copyleft Apache 2.0
 */ 
#ifndef HTTP_RESPONSE_H
#define HTTP_RESPONSE_H

#include <unordered_map>
#include <fcntl.h>       // open
#include <unistd.h>      // close
#include <sys/stat.h>    // stat
#include <sys/mman.h>    // mmap, munmap

#include "../buffer/buffer.h"
#include "../log/log.h"

class HttpResponse {
public:
    HttpResponse();
    ~HttpResponse();

    // Init不像通常类放在构造函数中，用于设置srcDir, path, isKeepAlive, code
    // srcDir, 资源存放目录
    // srcDir + path，资源的url
    // 状态码
    void Init(const std::string& srcDir, std::string& path, bool isKeepAlive = false, int code = -1);
    void MakeResponse(Buffer& buff);
    // 析构函数中调用，同时对外留了接口
    void UnmapFile();
    char* File();
    size_t FileLen() const;
    void ErrorContent(Buffer& buff, std::string message);
    int Code() const { return code_; }

private:
    void AddStateLine_(Buffer &buff);
    void AddHeader_(Buffer &buff);
    void AddContent_(Buffer &buff);

    void ErrorHtml_();
    std::string GetFileType_();

    int code_;
    bool isKeepAlive_;

    std::string path_;
    std::string srcDir_;
    // 请求的
    char* mmFile_; 
// 保存文件状态信息的结构体
// dev_t     st_dev     文件所在设备的标识
// ino_t     st_ino     文件结点号
// mode_t    st_mode    文件保护模式
// nlink_t   st_nlink   硬连接数
// uid_t     st_uid     文件用户标识
// gid_t     st_gid     文件用户组标识
// dev_t     st_rdev    文件所表示的特殊设备文件的设备标识
// off_t     st_size    总大小，字节为单位
// time_t    st_atime   最后访问时间
// time_t    st_mtime   最后修改时间
// time_t    st_ctime   最后状态改变时间
// blksize_t st_blksize 文件系统的块大小
// blkcnt_t  st_blocks  分配给文件的块的数量，512字节为单元

// st_mode的使用方式比较特殊，需要和宏做&得到指定信息
// 文件类型标志包括：
// S_IFBLK：文件是一个特殊的块设备
// S_IFDIR：文件是一个目录
// S_IFCHR：文件是一个特殊的字符设备
// S_IFIFO：文件是一个FIFO设备
// S_IFREG：文件是一个普通文件
// S_IFLNK：文件是一个符号链接
// 其他模式标志包括：
// S_ISUID：文件设置了SUID位
// S_ISGID：文件设置了SGID位
// S_ISVTX：文件设置了sticky位
// 用于解释st_mode标志的掩码包括：
// S_IFMT：文件类型
// S_IRWXU：属主的读/写/执行权限，可以分成S_IXUSR, S_IRUSR, S_IWUSR
// S_IRWXG：属组的读/写/执行权限，可以分成S_IXGRP, S_IRGRP, S_IWGRP
// S_IRWXO：其他用户的读/写/执行权限，可以分为S_IXOTH, S_IROTH, S_IWOTH
// 还有一些用于帮助确定文件类型的宏定义，这些和上面的宏不一样，这些是带有参数的宏，类似与函数的使用方法：
// S_ISBLK：测试是否是特殊的块设备文件
// S_ISCHR：测试是否是特殊的字符设备文件
// S_ISDIR：测试是否是目录
// S_ISFIFO：测试是否是FIFO设备
// S_ISREG：测试是否是普通文件
// S_ISLNK：测试是否是符号链接
// S_ISSOCK：测试是否是socket
    struct stat mmFileStat_;

    static const std::unordered_map<std::string, std::string> SUFFIX_TYPE;
    static const std::unordered_map<int, std::string> CODE_STATUS;
    static const std::unordered_map<int, std::string> CODE_PATH;
};


#endif //HTTP_RESPONSE_H