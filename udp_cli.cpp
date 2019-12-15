/*
 * 封装实现一个udpsocket类,向外实现更加容易使用的udp接口
 * 来实现udo通信流程
 */
#include<iostream>                                                
#include<string>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<unistd.h>
using namespace std;

class UdpSocket{

    public:
        UdpSocket()
            :_sockfd(-1)
        {}
        ~UdpSocket(){
            Close();
        }
        //创建套接字
        bool Socket(){
            _sockfd = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
            if(_sockfd < 0){
                cout<<"socket error"<<endl;
                return false;
            }
            return true;
        }
        //绑定地址信息
        bool Bind(const std::string &ip,uint16_t port){
            struct sockaddr_in addr;    
            addr.sin_family = AF_INET;         
            addr.sin_port = htons(port);      
            addr.sin_addr.s_addr = inet_addr(ip.c_str());

            socklen_t len = sizeof(struct sockaddr_in);                            
            int ret = bind(_sockfd,(struct sockaddr*)&addr,len);               
            if(ret < 0){                    
                std::cerr<<"bind error\n";                       
                return false;                
            }                
            return true;     
        }
        //发送数据
        bool Send(string &data,string &ip,uint16_t port){
            struct sockaddr_in addr;
            addr.sin_family = AF_INET;
            addr.sin_port = htons(port);
            addr.sin_addr.s_addr = inet_addr(ip.c_str());

            socklen_t len = sizeof(struct sockaddr_in);
            int ret = sendto(_sockfd,&data[0],data.size(),0,(struct sockaddr*)&addr,len);
            if(ret < 0){
                std::cerr<<"sendto error\n";
                return false;
            }
            return true;                                                                     
        }
        //接收数据
        bool Recv(std::string &buf,std::string &ip,uint16_t &port){
            char tmp[4096];                             
            struct sockaddr_in peeraddr;  //定义一个对端地址
            socklen_t len = sizeof(peeraddr);
            int ret = recvfrom(_sockfd,tmp,4096,0,(struct sockaddr*)&peeraddr,&len);
            //返回实际接受数据长度
            if(ret < 0){                                       
                std::cerr<<"recvfrom error"<<std::endl;        
                return false;                                  
            }                                 

            buf.assign(tmp,ret);  //从tmp拷贝ret长度的数据到buf
            port = ntohs(peeraddr.sin_port);  //网络字节序转换
            //inet_ntoa将网络字节序的整数的IP地址转换为字符串IP
            //返回缓冲区首地址,内部实现使用了静态成员变量
            //非线程安全
            ip = inet_ntoa(peeraddr.sin_addr);
            return true; 
        }
        //关闭套接字
        bool Close(){
            if(_sockfd >= 0){
                close(_sockfd);
                _sockfd = -1;
            }
            return true;
        }
    private:
        int _sockfd;  //套接字描述符
};
#define CHECK_RET(q) if((q) == false) {return -1;}
int main(int argc,char* argv[]){
    if(argc != 3){
        std::cout<<"./udp_cli severip severport\n";
        return -1;
    }
    std::string srv_ip = argv[1];
    uint16_t srv_port = atoi(argv[2]);

    UdpSocket sock;

    CHECK_RET(sock.Socket());
    CHECK_RET(sock.Bind("192.168.168.132",8000));
    while(1){
        string buf;
        std::cin>>buf;
        CHECK_RET(sock.Send(buf,srv_ip,srv_port));
        buf.clear();

        CHECK_RET(sock.Recv(buf,srv_ip,srv_port));
        std::cout<<"sever say: "<< buf<<"\n";
    }
    CHECK_RET(sock.Close());

    return 0;
}                                                        