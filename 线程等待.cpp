/*
 * 这个demo，演示线程退出返回值的获取，以及线程等待
 *
 */
#include<stdio.h>
#include<unistd.h>
#include<pthread.h>

void *thr_start(void *arg){
    //char buf[] = "nihao";
    char *buf = "nihao";
    sleep(5);
    return buf;
}

int main(){
    pthread_t tid;
    int ret = pthread_create(&tid,NULL,thr_start,NULL);
    if(ret != 0){
        printf("creat thread error\n");
        return -1;
    }
    //pthread_detach(tid);
    void *retval;
    pthread_join(tid,&retval); //线程等待，阻塞函数

    printf("thread exit val:%s\n",(char*)retval);
    return 0;
}
