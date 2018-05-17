#include <stdio.h>
#include <string.h>
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
        printf("%d ", Access_Series[index]);
    printf("\n");
}

//输出在内存的页面
void print_M_Frame()
{
    for (int i = 0; i < frame_num; i++)
    {
        if (M_Frame[i].flag == IN)
            printf("%d ", M_Frame[i].page_no);
    }
    printf("\n");
}
//输出缺页次数和缺页率
void print_disaffect(int dis)
{
    float disaffect_rate=(float)dis/Total_instruction;
    printf("disaffect=%d\ndisafffect_rate=%f",disaffect,disaffect_rate);
}
//LRU命中函数 页表M_Frame0优先级最高 [0]-[index-1]一次向后移动一位
void hit(int index)
{
    printf("运行命中函数\n");
    int temp; //用于交换
    temp = M_Frame[index].page_no;
    for (int i = index; i >0; i--)
    {
        M_Frame[i].page_no = M_Frame[i-1].page_no;
    }
    M_Frame[0].page_no = temp;
    printf("\n");
}
//LRU 未命中函数 调入
void miss_call_in(int page)
{
    if (loc < frame_num) //此时的内存未满
    {
        for (int i=loc; i>0; i--)
           {
               M_Frame[i].flag=IN;
               M_Frame[i].page_no=M_Frame[i-1].page_no;//下移动
           }
        M_Frame[0].flag = IN;
        M_Frame[0].page_no = page;//移动完赋值
        loc++;
    }
    else
    {
        for (int i = frame_num-1; i>0; i--)
            M_Frame[i].page_no= M_Frame[i-1].page_no;
        M_Frame[0].page_no = page;
    }
}
int main()
{
   // for (int i = 0; i < Total_instruction; i++) //随机产生访问页面序列
     //   Access_Series[i] = rand() % Total_instruction;
    Access_Series[ 0]=0 ;
    Access_Series[ 1]= 1;
    Access_Series[2 ]= 2;
    Access_Series[ 3]= 0;
    Access_Series[ 4]= 1;
    Access_Series[ 5]= 4;
        printf("run LRU:\n");
        init();
        print_Access_Series();
        for (int cur = 0; cur < Total_instruction; cur++)
        {
            is_hit=MISS;
            int index;
            for ( index = 0; index < frame_num; index++) //遍历内存 M_Frame
            {
                if (M_Frame[index].flag == IN && M_Frame[index].page_no == Access_Series[cur]) //在内存 且 页号命中
                   {
                    // printf(" M_Frame[index].page_no == Access_Series[cur]%d   %d   ",M_Frame[index].page_no,Access_Series[cur]);
                    is_hit = HIT;
                    break;
                   }
            }
            if (is_hit == HIT)
            {

                printf("hit_index=%d " ,index);
                hit(index);
                printf("hit   ");
                print_M_Frame();
            }
            else //未命中
            {
                miss_call_in(Access_Series[cur]);
                printf("miss  ");
                print_M_Frame();
                disaffect++;
            }
        }
        print_disaffect(disaffect);
    return (0);
}
