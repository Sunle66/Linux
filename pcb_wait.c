#include<sys/wait.h>
#include<sys/types.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<unistd.h>

int main(){
    pid_t pid;
    pid = fork();
    if(pid < 0){
        printf("%s fork error\n",__FUNCTION__);
        return 1;
    }else if(pid == 0){
        printf("child is run,pid is : %d \n",getpid());
        sleep(5);
        exit(5);
    }else{
        int status = 0;
        pid_t ret = waitpid(-1,&status,0); //阻塞式等待，等待5秒
        printf("this is test for wait\n");
        if(WIFEXITED(status) && ret == pid){
            printf("wait child 5s success,child return code is %d\n",WEXITSTATUS(status));
        }else{
            printf("wait child faild,return.\n");
            return 1;
        }
    }


    return 0;
}