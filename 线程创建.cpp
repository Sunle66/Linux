/*
 * 这个demo用于实现基本的线程创建，体会接口的使用
 *  int pthread_creat(pthread_t *thread,const pthread_arr_t *attr,
 *              void *(start_routine)(void*),void *arg);
 *
 * pthread_cancel(pthread_t tid)  取消一个指定线程
 *
 */

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

//线程入口函数
void *thr_start(void *arg){
    pthread_t tid = pthread_self();

    while(1){
        //printf("----ordinary------%s-%p\n",(char*)arg,tid);
        printf("----ordinary------%s-%lu\n",(char*)arg,tid);
        sleep(1);
    }

    return NULL;
}

int main(){
    int ret;  //用来接收线程创建的返回值
    pthread_t tid;  //用来接收线程id

    pthread_t mtid = pthread_self();  //返回所调用线程的id

    //char tmp[] = "lihou";
    //ret = pthread_create(&tid,NULL,thr_start,(void*)tmp);
    ret = pthread_create(&tid,NULL,thr_start,(void*)"nihao"); //创建一个线程

    if(ret != 0){
        printf("thread creat error\n");
        return -1;
    }

    while(1){
        //printf("-----main------mtid:%p----ctid:%p\n",mtid,tid);
        printf("-----main------mtid:%lu----ctid:%lu\n",mtid,tid);
        sleep(1);
    }


    return 0;
}                                                                               
