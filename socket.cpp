/*
 * 封装一个tcpsocket类,向
 *    1.创建套接字
 *    2.绑定地址信息
 *    3.开始监听/发起连接
 *    4.获取已完成连接
 *    5.发送数据
 *    6.接收数据
 *    7.关闭套接字
 =========================
 */           
#include<iostream>    
#include<string>     
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/socket.h>   

using namespace std;
       
class TcpSocket{
    public:
        TcpSocket()
            :_sockfd(-1)
        {
        }
        ~TcpSocket(){
        }

        bool Socket(){
            _sockfd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
            if(_sockfd < 0){
                cerr<<"socket error"<<endl;
                return false;
            }
            return true;
        }
        bool Bind(string &ip,uint16_t port){
            struct sockaddr_in addr;
            addr.sin_family = AF_INET;
            addr.sin_port = htons(port);
            addr.sin_addr.s_addr = inet_addr(&ip[0]);

            socklen_t len = sizeof(struct sockaddr);
            int ret = bind(_sockfd,(struct sockaddr*)&addr,len);
            if(ret < 0){
                cerr<<"bind error"<<endl;                              
                return false;
            }
            return true;
        }
        //服务端进行监听  
         bool Listen(int backlog){
             int ret = listen(_sockfd,backlog);
             if(ret < 0){
                 cerr<<"listen error"<<endl;
                 return false;
             }
             return true;
         }
         //客户端发起请求
         bool Connect(string &ip,uint16_t port){
             struct sockaddr_in addr;
             addr.sin_family = AF_INET;
             addr.sin_port = htons(port);
             addr.sin_addr.s_addr = inet_addr(&ip[0]);
             socklen_t len = sizeof(struct sockaddr_in);
 
             int ret = connect(_sockfd,(struct sockaddr*)&addr,len);
             if(ret < 0){
                 cerr<<"connect error"<<endl;
                 return false;
             }
             return true;
         }
         //获取已完成链接
         bool Accept(TcpSocket &newsock);
         //发送数据                                                               
         bool Send(string &buf);
         //接受数据
         bool Recv(string &buf);
         bool Close();
private:
    int _sockfd;
};
