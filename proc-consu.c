/***************************************************************/
/*         PROGRAM NAME:         PRODUCER_CONSUMER             */
/*    This program simulates two processes, producer which     */
/* continues  to produce message and  put it into a buffer     */
/* [implemented by PIPE], and consumer which continues to get  */
/* message from the buffer and use it.                         */
/*    The program also demonstrates the synchronism between    */
/* processes and uses of PIPE.                                 */
/***************************************************************/
//原始的版本
#include <stdio.h>
#include <stdlib.h>
#define PIPESIZE 8

#define PRODUCER 0
#define CONSUMER 1

#define RUN 0   /* statu of process */
#define WAIT 1  /* statu of process */
#define READY 2 /* statu of process */

#define NORMAL 0
#define SLEEP 1
#define AWAKE 2

#include <stdio.h>

struct pcb
{
  char *name;
  int statu;
  int time;
}; /* times of execution */
struct pipetype
{
  char type;
  int writeptr;
  int readptr;
  struct pcb *pointp; /* write wait point */
  struct pcb *pointc;
};                      /* read wait point  */
int pipe[PIPESIZE];     //管道数组
struct pipetype pipetb; //管道的状态
struct pcb process[2];

main()
{
  int output, ret, i;
  char in[2];

  int runp(), runc(), prn(); //分别是运行生产者 消费者 显示缓冲区

  pipetb.type = 'c';

  pipetb.writeptr = 0; //初始态=0
  pipetb.readptr = 0;

  pipetb.pointp = pipetb.pointc = NULL;
  process[PRODUCER].name = "Producer\0";
  process[CONSUMER].name = "Consumer\0";
  process[PRODUCER].statu = process[CONSUMER].statu = READY;
  process[PRODUCER].time = process[CONSUMER].time = 0;
  output = 0;

  printf("Now starting the program!\n");
  printf("Press 'p' to run PRODUCER, press 'c' to run CONSUMER.\n");
  printf("Press 'e' to exit from the program.\n");

  for (i = 0; i < 1000; i++)
  {
    //用于检测是否输入的是epc三个字符的一个
    in[0] = 'N';
    while (in[0] == 'N')
    {
      scanf("%s", in);
      if (in[0] != 'e' && in[0] != 'p' && in[0] != 'c')
        in[0] = 'N';
    }

    //开始运行生产者进程
    if (in[0] == 'p' && process[PRODUCER].statu == READY) //输入的是P 生产者进程是就绪
    {
      output = (output + 1) % 100;
      if ((ret = runp(output, process, pipe, &pipetb, PRODUCER)) == SLEEP)
        pipetb.pointp = &process[PRODUCER]; //管道状态的生成者进程指针指向
      if (ret == AWAKE)
      {
        (pipetb.pointc)->statu = READY;
        pipetb.pointc = NULL;
        runc(process, pipe, &pipetb, CONSUMER);
      }
    }
    //消费者进程
    if (in[0] == 'c' && process[CONSUMER].statu == READY)
    {
      if ((ret = runc(process, pipe, &pipetb, CONSUMER)) == SLEEP)
        pipetb.pointc = &process[CONSUMER];
      if (ret == AWAKE)
      {
        (pipetb.pointp)->statu = READY;
        pipetb.pointp = NULL;
        runp(output, process, pipe, &pipetb, PRODUCER);
      }
      // //只要输入C 就应该生产者->ready
      //  pipetb.writeptr =0;
      //  process[CONSUMER].statu=READY;//设置就绪态
    }
    if (in[0] == 'p' && process[PRODUCER].statu == WAIT)
      printf("PRODUCER is waiting, can't be scheduled.\n");
    if (in[0] == 'c' && process[CONSUMER].statu == WAIT)
      printf("CONSUMER is waiting, can't be scheduled.\n");
    if (in[0] == 'e')
      exit(1);
    prn(process, pipe, pipetb);
    in[0] = 'N';
  }
}
/* run producer */
runp(out, p, pipe, tb, t) int out, pipe[], t;
struct pcb p[];
struct pipetype *tb;
{
  p[t].statu = RUN;
  printf("run PRODUCER. product %d     ", out);
  if (tb->writeptr >= PIPESIZE) //出错这里当一旦有C 就应该回滚到
  {
    p[t].statu = WAIT;
    return (SLEEP);
  }
  pipe[tb->writeptr] = out;
  tb->writeptr++;
  p[t].time++;
  p[t].statu = READY;
  if ((tb->pointc) != NULL)
    return (AWAKE);
  return (NORMAL);
}

runc(p, pipe, tb, t) /* run consumer */
    int pipe[],
    t;
struct pcb p[];
struct pipetype *tb;
{
  int c;
  p[t].statu = RUN;
  printf("run CONSUMER. ");

  if (tb->readptr >= tb->writeptr) //这里！
  {
    //回滚到0
    p[t].statu = WAIT;
    return (SLEEP);
  }

  c = pipe[tb->readptr];
  tb->readptr++;
  printf(" use %d      ", c);
  if (tb->readptr >= tb->writeptr)
    tb->readptr = tb->writeptr = 0;
  p[t].time++;
  p[t].statu = READY;
  if ((tb->readptr) == 0 && (tb->pointp) != NULL)
    return (AWAKE);
  return (NORMAL);
}

prn(p, pipe, tb) int pipe[];
struct pipetype tb;
struct pcb p[];
{
  int i;
  printf("\n         ");
  for (i = 0; i < PIPESIZE; i++)
    printf("------ ");
  printf("\n        |");
  for (i = 0; i < PIPESIZE; i++)
    if ((i >= tb.readptr) && (i < tb.writeptr))
      printf("  %2d  |", pipe[i]);
    else
      printf("      |");
  printf("\n         ");
  for (i = 0; i < PIPESIZE; i++)
    printf("------ ");
  printf("\nwriteptr = %d, readptr = %d,  ", tb.writeptr, tb.readptr);
  if (p[PRODUCER].statu == WAIT)
    printf("PRODUCER wait ");
  else
    printf("PRODUCER ready ");
  if (p[CONSUMER].statu == WAIT)
    printf("CONSUMER wait ");
  else
    printf("CONSUMER ready ");
  printf("\n");
}
