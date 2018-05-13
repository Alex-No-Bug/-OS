#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>
#include<wait.h>
int main()
{
    //1.建立管道
    int fd[2];//fd0 为读 fd1 写
    int ret= pipe(fd);
    if(ret==-1)//失败
    {
    printf("pipe error!\n");
    return(0);
    }
    //2.创建子进程 父进程
    pid_t id1=fork();//从0这里开始分叉
    pid_t id3;
    if(id1 == 0)//子进程向管道里写
    {       
           
              close(fd[0]);//关掉读的端口
            char*s="Subprocess3 is sending message!\n";

            lockf(fd[1],1,0);//lock the port of write

             write(fd[1],s,strlen(s));//然后返回计数器值(长度不包含'\0')  

            lockf(fd[1],0,0);

            

    }
    else if(id1>0)
    { 
        
        pid_t id2=fork();
        if(id2==0)//child2
        {
           close(fd[0]);//关掉读的端口

           char*s="Subprocess2 is sending message!\n";

           lockf(fd[1],1,0);
           write(fd[1],s,strlen(s));//然后返回计数器值(长度不包含'\0')  
           lockf(fd[1],0,0);
         }
       else{ 
        //父亲进程读出来
         id3=fork();
           if(id3==0)
            {
            close(fd[0]);//关掉读的端口
            char*s="Subprocess1 is sending message!\n";

            lockf(fd[1],1,0);//lock the port of write

             write(fd[1],s,strlen(s));//然后返回计数器值(长度不包含'\0')  

            lockf(fd[1],0,0);
            }
           else
           {
        close(fd[1]);//关掉写的端口
        char message[150];

        //初始化
	   
           memset(message,'\0',150);   
	   int status = 0;
           //sleep(3);
           //waitpid(id2, &status, 0);
	   //waitpid(id1, &status, 0);
	  //waitpid(id3, &status, 0);
           wait(0);
           wait(0);
           wait(0);
             
           read(fd[0],message,150); 
           printf("%s",message); 
}
      }
    }
    else 
    {
        printf("fork error!\n");
        return(0);
    }
    return(0);
}