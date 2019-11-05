/*
 *Describe:共享内存的基本操作
    1.创建
    2.建立映射
    3.内存操作
    4.解除映射
    5.删除
 */

#include<stdio.h>
#include<errno.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/shm.h>
#define IPC_KEY 0x12345678
#define PROJ_ID 0x112233
int main(int argc,char* argv){
    //1.创建共享内存
    //key_t ftok(const char *pathname,int proj_id);
    //通过文件inode节点号与proj_id生成一个key值
    //int shmget(key_t key,size_t size,int shmflg);
    // key       共享内存标识符 
    // size      共享内存大小
    // shmflg ： 选项标志
    //      IPC_CREAT   共享内存不存在则创建，存在则打开
    //      IPC_EXCL    与IPC_CREAt同用，则共享内存时报错
    //      shm_mode    权限
    //      返回值：标识符(代码中的操作句柄)    失败：-1
    key_t key = ftok(".",PROJ_Id);
    int shmid = shmget(IPC_KEY,32,IPC_CREAT | 0664);
    //0664所有者可读可写，其他人只读
    if(shmid < 0){
        perror("shmget error");
        return -1;
    }
    //建立映射
    //void *shmat(int shmid,const void *shmaddr,int shmflg)
    //  shmid:创建共享内存，返回的句柄
    //  shmaddr:置空-映射首地址由操作系统分配
    //  shmflg:映射成功后的操作权限
    //      SHM_RDONLY  只读
    //      0           默认-可读可写
    //  返回值：映射首地址          失败：(void*)-1
    char *shm_start = shmat(shmid,NULL,0);
    if(shm_start == (void*) -1){
        perror("shmat error");
        return -1;
    }
    while(1){
        sprintf(shm_start,"今天天气好晴朗～～+%d\n",i++);
        sleep(1);
    }
    //  解除映射
    //  int shmdt(const void *shmaddr);
    //      shmaddr:映射首地址
    shmdt(shm_start);
    //删除
    //int shmctl(int shmid,int cmd,struct shmid_ds *buf);
    //  shmid:操作句柄
    //  cmd:操作类型
    //      IPC_RMID  删除共享内存
    //  buf:设置/获取共享内存信息
    //
    shmctl(shmid,IPC_RMID,NULL);

    
    return 0;
}
