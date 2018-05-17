/*每个子进程应能反映出页面置换的过程，
并统计页面置换算法的命中或缺页情况。
设缺页的次数为diseffect。
总的页面访问次数为total_instruction
缺页率 = disaffect/total_instruction 
命中率 = 1- disaffect/total_instruction 
用一个结构数组M_Frame[]记录为进程分配的内存页面的使用情况。
在M_Frame[]中还可记载各页面进入内存或被访问的先后顺序（如可使M_Frame[0]总是最先进入或最久未被访问的页面）。 
   struct one_frame { 
          int page_no; 
          char flag; 
   }; 
   struct one_frame M_Frame[frame_num];//这里frame_num是给一个进程分配的最大的内存页面数 
*/
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <wait.h>
#include <stdlib.h>

int loc;                    //栈底指针
#define frame_num 3         //主存大小
#define Total_instruction 6 //总的额访问页数

//命中和未命中
int is_hit;
#define HIT 1
#define MISS 0

//页面是否在内存的标志
#define IN '1'
#define OUT '0'

//存放页面序列的数组
int Access_Series[Total_instruction]; //(1)

//页表
struct one_frame
{
    int page_no; //对应的页号
    char flag;   //是否在内存的标志
};
struct one_frame M_Frame[frame_num]; //(2)

int disaffect; //缺页的数量
//初始化函数 每个进程开始均调用
void init()
{
    loc = 0;
    is_hit = MISS; //开始未命中
    disaffect = 0;
    for (int index = 0; index < frame_num; index++)
        M_Frame[index].flag = OUT; //页表初始化 均不在页表中
}

//输出访问序列
void print_Access_Series()
{
    printf("Access Series:\n");
    for (int index = 0; index < Total_instruction; index++)
        printf("%d ", Access_series[index]);
    printf("\n");
}

//输出在内存的页面
void print_M_Frame()
{
    for (int i = 0; i < frame_num; i++)
    {
        if (M_Frame[i].flag == IN)
            printf("%d ", M_Frame[i]);
    }
    print("\n");
}
//输出缺页次数和缺页率
void print_disaffect(int dis)
{
    float disaffect_rate=dis/Total_instruction;
    print("disaffect=%d\ndisafffect_rate=%f",disaffect,disaffect_rate);
}
//LRU命中函数 页表M_Frame0优先级最高 [0]-[index-1]一次向后移动一位
void hit(int index)
{
    int temp; //用于交换
    temp = M_frame[index];
    for (int i = 0; i <= (index - 1); i++)
    {
        M_frame[i + 1] = M_frame[i];
    }
    M_Frame[0] = temp;
}

//LRU 未命中函数 调入
void miss_call_in(int page)
{
    if (loc < frame_num) //此时的内存未满
    {
        for (int i = 0; i <loc; i++)
           {
               M_Frame[i].flag=IN;
               M_Frame[i+1]=M_Frame[i];
           }
            M_Frame[loc].flag = IN;
                
        M_Frame[0] = page;//移动完赋值
        loc++;
    }
    else
    {
        for (int i = 0; i <=(frame_num-2); i++)
            M_Frame[i + 1] = M_Frame[i];
        M_Frame[0] = page;
    }
}
int main()
{
    for (int i = 0; i < Total_instruction; i++) //随机产生访问页面序列
        Access_Series[i] = rand() % Total_instruction;
    pid_t id1 = fork();
    if (id1 == 0) //子进程1 LRU int Access_Series[Total_instruction]6;   struct one_frame M_Frame[frame_num];
    {

        printf("run LRU:\n");
        init();
        for (int cur = 0; cur < Total_instruction; cur++)
        {
            for (int index = 0; index < frame_num; index++) //遍历内存 M_Frame
            {
                if (M_Frame[index].flag == IN && M_Frame[index].page_no == Access_Series[cur]) //在内存 且 页号命中
                    is_hit = HIT;   
            }
            if (is_hit == HIT)
            {
                hit();
                print("hit   ");
                print_M_Frame();
            }
            else //未命中
            {
                miss_call_in(Access_Series[cur]);
                print("miss  ");
                print_M_Frame();
                disaffect++;
            }
        }
        print_disaffect(disaffect);
    }
    else
    {
        pid_t id2 = fork(); //子进程2
        if (id2 == 0)//FIFO
        {
            printf("run FIFO:\n");
            init();

        }
        else
        {
        }
    }
    return (0);
}
