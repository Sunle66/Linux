/*
 * 这个demo用来实现一个线程池
 */

#include<iostream>
#include<thread>
#include<queue>
#include<vector>
#include<pthread.h>
#include<sstream>
#include<unistd.h>
#include<stdlib.h>
#include<time.h>


#define MAX_THREAD 5
#define MAX_QUEUE  10

using namespace std;
typedef void(*handler_t)(int);

//任务类,有两个成员：
//  1.要处理的数据
//  2.数据的处理方法
//这两个成员都需要用户传入
//  一个线程中用什么方法处理什么数据都有用户自己决定
class MyTask{
public:
    MyTask(int data,handler_t handle)
        :_data(data)
        ,_handler(handle)
    {}

    ~MyTask(){}

    //设置任务
    void SetTask(int data,handler_t handle){
        _data = data;
        _handler = handle;
    }
    //向外提供一个接口
    void Run(){
        return _handler(_data);
    }
private:
    int _data;  //数据
    handler_t _handler;  //数据处理方式
};

//线程池类
class ThreadPool{
public:
    ThreadPool(int maxq = MAX_QUEUE,int maxt = MAX_THREAD)
        :_capacity(maxq)
        ,_thr_list(maxt)
        ,_thr_max(maxt)
        ,_thr_cur(0)
        ,_quit_falg(false)
    {
        pthread_mutex_init(&_mutex, NULL);
        pthread_cond_init(&_cond_con, NULL);
        pthread_cond_init(&_cond_pro, NULL);
    }
    ~ThreadPool(){
        pthread_mutex_destroy(&_mutex);
        pthread_cond_destroy(&_cond_con);
        pthread_cond_destroy(&_cond_pro);
    }
    //线程初始化，创建指定数量的线程
    bool PoolInit(){
        for(int i = 0;i < _thr_max;i++){
            _thr_list[i] = thread(&ThreadPool::thr_start,this);
            _thr_cur++;
            _thr_list[i].detach();  //将线程分离
        }
        return true;
    }

    //任务队列的数据入队，
    bool AddTask(MyTask &tt){
        pthread_mutex_lock(&_mutex);
        while(_queue.size() == _capacity){
            pthread_cond_wait(&_cond_pro,&_mutex);
        }
        _queue.push(tt);
        pthread_mutex_unlock(&_mutex);
        pthread_cond_signal(&_cond_con);

        return true;
    }
    //线程安全的出队操作
    bool TaskPop(MyTask &tt){
        return true;
    }
    //终止一个线程池
    bool PoolStop(){
        pthread_mutex_lock(&_mutex);
        _quit_falg = true;
        pthread_mutex_unlock(&_mutex);

        while(_thr_cur > 0){
            pthread_cond_broadcast(&_cond_con); //唤醒所有线程,非阻塞函数
            usleep(1000);
        }
        return true;
    }
private:
    //线程安全的出队
    void thr_start(){
        while(1){
            pthread_mutex_lock(&_mutex);
            while(_queue.empty()){
                if(_quit_falg == true){
                    cout<<"thread exit:"<<pthread_self()<<endl;
                    pthread_mutex_unlock(&_mutex);
                    _thr_cur--;
                    return;
                }
                pthread_cond_wait(&_cond_con,&_mutex);
            }
            MyTask tt = _queue.front();
            _queue.pop();
            pthread_mutex_unlock(&_mutex);
            pthread_cond_signal(&_cond_pro);
            //任务处理应该放在解锁之后，否则会造成同一时间只有一个线程在处理任务
            tt.Run();
        }
        return;
    }

private:
    //-------用于实现线程安全的任务队列---------
    queue<MyTask> _queue; //任务
    int _capacity;  //任务最大数量
    pthread_mutex_t _mutex;  //互斥锁
    pthread_cond_t _cond_pro;   //生产者等待队列
    pthread_cond_t _cond_con;   //消费者等待队列

    //--------用于实现线程池当中线程的控制-----------
    int _thr_cur;   //线程退出时，描述当前还有多少个线程
    bool _quit_falg;  //定义一个标志位，初始化为false，控制线程的退出
    vector<thread> _thr_list; //存储线程的操作句柄
    int _thr_max;  //线程池中线程数量上限
};

void test(int data){
    srand(time(NULL));
    int nsec = rand() % 5;
    stringstream ss;
    ss <<"thread: "<<pthread_self()<<"processing data";
    ss << data << "and sleep"<<nsec<<"sec\n";

    cout<<ss.str();
    sleep(nsec);
}
int mian(){
    ThreadPool pool;
    pool.PoolInit();
    for(int i = 0;i < 10;i++){
        MyTask tt(i,test);
        pool.AddTask(tt);
    }
    return 0;
}
