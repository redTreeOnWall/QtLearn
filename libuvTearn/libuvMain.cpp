// #include <stdio.h>
// #include <stdlib.h>
// #include "./libuv/include/uv.h"

// // https://luohaha.github.io/Chinese-uvbook/source/introduction.html

// int main() {
//     uv_loop_t *loop = (uv_loop_t *)malloc(sizeof(uv_loop_t));
//     uv_loop_init(loop);

//     printf("Now quitting.\n");
//     uv_run(loop, UV_RUN_DEFAULT);

//     uv_loop_close(loop);
//     free(loop);
//     return 0;
// }

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>

#define MYPORT  8887
#define QUEUE   20
#define BUFFER_SIZE 1024

//lee:普通的的socket
int normalSocket()
{
    //定义sockfd AF_INET（IPv4） AF_INET6（IPv6） AF_LOCAL（UNIX协议） AF_ROUTE（路由套接字） AF_KEY（秘钥套接字）
    // SOCK_STREAM（字节流套接字） SOCK_DGRAM
    int server_sockfd = socket(AF_INET, SOCK_STREAM, 0);

    ///定义sockaddr_in
    struct sockaddr_in server_sockaddr;
    server_sockaddr.sin_family = AF_INET;
    server_sockaddr.sin_port = htons(MYPORT);
    server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    ///bind，成功返回0，出错返回-1
    if(bind(server_sockfd,(struct sockaddr *)&server_sockaddr,sizeof(server_sockaddr))==-1)
    {
        perror("bind");
        exit(1);
    }

    printf("监听%d端口\n", MYPORT);
    ///listen，成功返回0，出错返回-1
    if(listen(server_sockfd, QUEUE) == -1)
    {
        perror("listen");
        exit(1);
    }

    ///客户端套接字
    char buffer[BUFFER_SIZE];
    struct sockaddr_in client_addr;
    socklen_t length = sizeof(client_addr);

    printf("等待客户端连接\n");
    ///成功返回非负描述字，出错返回-1
    int conn = accept(server_sockfd, (struct sockaddr*)&client_addr, &length);
    if(conn<0)
    {
        perror("connect");
        exit(1);
    }
    printf("客户端成功连接\n");

    while(1)
    {
        memset(buffer,0,sizeof(buffer));
        long len = recv(conn, buffer, sizeof(buffer), 0);
        //客户端发送exit或者异常结束时，退出
        ;
        if(strcmp(buffer,"exit\n")==0 || len<=0) {
            printf("出现异常");
            break;
        }

        printf("来自客户端数据：\n");
        fwrite(buffer, len, 1, stdout);
        send(conn, buffer, len, 0);
        printf("发送给客户端数据：\n");
        fwrite(buffer, len, 1, stdout);
    }
    close(conn);
    close(server_sockfd);
    return 0;
}
// 代码一大坨，其实上我们简单拆分一下

// 第一步：创建socket 文件描述符
// 第二步：定义socket addr
// 第三步：绑定文件描述符和地址  bind
// 第四步：监听文件描述符 listen
// 第五步：等待socket返回内容 accept
// 第六步：接收信息 recv





//使用libuv的scoket
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
#include <iostream>
#include <string>
#include "./libuv/include/uv.h"

#define DEFAULT_PORT 7000
#define DEFAULT_BACKLOG 128

uv_loop_t *loop;
struct sockaddr_in addr;

typedef struct {
    uv_write_t req;
    uv_buf_t buf;
} write_req_t;




void free_write_req(uv_write_t *req) {
    write_req_t *wr = (write_req_t*) req;
    free(wr->buf.base);
    free(wr);
}

void alloc_buffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf) {
    buf->base = (char*) malloc(suggested_size);
    buf->len = suggested_size;
}

void on_close(uv_handle_t* handle) {
    free(handle);
}

void echo_write(uv_write_t *req, int status) {
    if (status) {
        fprintf(stderr, "Write error %s\n", uv_strerror(status));
    }
    free_write_req(req);
}



void http_read(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf){
    if (nread > 0) {
        size_t dataSize = sizeof(write_req_t);
        write_req_t *req = (write_req_t*) malloc(dataSize);
        req->buf = uv_buf_init(buf->base, nread);

        int getPost = 0 ;//post/
        int pathSlice[2] = {0,0};
        int postSlice[2] = {0,0};

        for(int i = 0;i < req->buf.len;i++){
            char c = req->buf.base[i];
        }


        char *responseData = "HTTP/1.1 200 OK\nDate: Fri, 22 May 2009 06:07:21 GMT\nContent-Type: text/html; charset=UTF-8\n\n<html> <head></head> <body> this is data <!--body goes here--> </body> </html>";

        // std::cout << "\nbuff size:"<<dataSize << std::endl;
        // fwrite(buf->base, buf->len, 1, stdout);
        //read line by line 
        uv_buf_t *respData =(uv_buf_t*) malloc(sizeof(uv_buf_t));
        respData->base = responseData;
        respData->len = strlen(responseData);

        std::cout << "\nbuff l:"<< req->buf.len << std::endl;
        fwrite(req->buf.base, req->buf.len, 1, stdout);
        // uv_write((uv_write_t*) req, client, &req->buf, 1, echo_write);
        uv_write((uv_write_t*) req, client, respData, 1, echo_write);
        return;
    }
    if (nread < 0) {
        if (nread != UV_EOF)
            fprintf(stderr, "Read error %s\n", uv_err_name(nread));
        uv_close((uv_handle_t*) client, on_close);
    }
    free(buf->base);
}

void echo_read(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf) {
    if (nread > 0) {
        write_req_t *req = (write_req_t*) malloc(sizeof(write_req_t));
        req->buf = uv_buf_init(buf->base, nread);
        std::cout << "\nbuff len:"<<buf->len << std::endl;
        fwrite(buf->base, buf->len, 1, stdout);
        uv_write((uv_write_t*) req, client, &req->buf, 1, echo_write);
        return;
    }
    if (nread < 0) {
        if (nread != UV_EOF)
            fprintf(stderr, "Read error %s\n", uv_err_name(nread));
        uv_close((uv_handle_t*) client, on_close);
    }

    free(buf->base);
}

void on_new_connection(uv_stream_t *server, int status) {
    if (status < 0) {
        fprintf(stderr, "New connection error %s\n", uv_strerror(status));
        // error!
        return;
    }

    uv_tcp_t *client = (uv_tcp_t*) malloc(sizeof(uv_tcp_t));
    uv_tcp_init(loop, client);
    if (uv_accept(server, (uv_stream_t*) client) == 0) {
        std::cout << "new conection:" << client <<std::endl;
        // uv_read_start((uv_stream_t*) client, alloc_buffer, echo_read);
        uv_read_start((uv_stream_t*) client, alloc_buffer, http_read);
    }
    else {
        uv_close((uv_handle_t*) client, on_close);
    }
}

int libUvSocket() {
    loop = uv_default_loop();

    uv_tcp_t server;
    uv_tcp_init(loop, &server);

    uv_ip4_addr("0.0.0.0", DEFAULT_PORT, &addr);

    uv_tcp_bind(&server, (const struct sockaddr*)&addr, 0);
    int r = uv_listen((uv_stream_t*) &server, DEFAULT_BACKLOG, on_new_connection);
    if (r) {
        fprintf(stderr, "Listen error %s\n", uv_strerror(r));
        return 1;
    }
    return uv_run(loop, UV_RUN_DEFAULT);
}


//this is hello world of libuv

int runHello(){
    uv_loop_t * loop =(uv_loop_t *) malloc(sizeof(uv_loop_t));
    uv_loop_init(loop);
    printf("now quiting\n");
    uv_run(loop,UV_RUN_DEFAULT);
    uv_loop_close(loop);
    free(loop);
    return 0;
}





int main(){
    // normalSocket();
    libUvSocket();
    // runHello();
}