/*
 *
 * 使用信号量实现生产者与消费者模型
 *
 */

#include<iostream>
#include<thread>
#include<mutex>
#include<cond_variable>
#include<semaphore.h>
#include<vector>

#define MAXQ 10;

using namespace std;


class RingQueue{
public:
    RingQueue(int maxq = MAXQ)
    :_capacity(maxq)
    ,_queue(maxq)
    {
        sem_init(&_lock,0,1);
        sem_init(&_idle_space,0,maxq);
        sem_init(&_data_space,0,0);
    }
    ~RingQUeue(){
        sem_destroy(&_lock);
        sem_destroy(&_data_space);
        sem_destroy(&_idle_space);
    }
    bool QueuePush(int data){
        sem_wait(&_idle_space);
        sem_wait(&_lock);
        _queue[_step_write] = data;
        _step_write = (_step_write + 1) % _capacity;
        sem_wait(&_lock);
        sem_post(&_data_space);
        return true;
    }

    bool QueuePop(int &data){
        sem_wait(&_data_space);
        sem_wait(&_lock);
        data = _queue[_step_read];
        _step_read = (_step_read + 1) % _capacity;
        sem_post(&_lock);
        sem_post(&_idle_space);

        return true;
    }

private:
    vector<int> _queue;
    int _capacity;  //环形队列节点数
    int _step_read; //读指针
    int _step_write;//写指针

    sem_t _lock;    //实现线程安全，互斥锁
    sem_t _idle_space;  //空闲空间计数
    sem_t _data_space   //数据空间计数
};

void thr_producer(RingQueue *q){
    int data = 0;
    while(1){
        q->QueuePush(data);
        cout<<"put data------"<<data++<<endl;
    }
    return;
}

void thr_consumer(RingQueue *q){
    int data = 0;
    while(1){
        q->QueuePop(data);
        cout<<"get data----"<<data<<endl;
    }
    return;
}
int main(){
    RingQueue q; 
    vector<thread> list_con(4);
    vector<thread> list_pro(4);
    for(int i = 0;i < 4;i++){
        list_pro[i] = thread(thr_producer,&q);
    }
    for(int i = 0;i < 4;i++){
        list_con[i] = thread(thr_consumer,&q);
    }
    for(int i = 0;i < 4;i++){
        list_pro[i].join();
        list_con[i].join();
    }
    return 0;

}
