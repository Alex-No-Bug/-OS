#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int loc;                    //ջ��ָ��
#define frame_num 3         //�����С
#define Total_instruction 6 //�ܵĶ����ҳ��

//���к�δ����
int is_hit;
#define HIT 1
#define MISS 0

//ҳ���Ƿ����ڴ�ı�־
#define IN '1'
#define OUT '0'

//���ҳ�����е�����
int Access_Series[Total_instruction]; //(1)

//ҳ��
struct one_frame
{
    int page_no; //��Ӧ��ҳ��
    char flag;   //�Ƿ����ڴ�ı�־
};
struct one_frame M_Frame[frame_num]; //(2)

int disaffect; //ȱҳ������
//��ʼ������ ÿ�����̿�ʼ������
void init()
{
    loc = 0;
    is_hit = MISS; //��ʼδ����
    disaffect = 0;
    for (int index = 0; index < frame_num; index++)
        M_Frame[index].flag = OUT; //ҳ���ʼ�� ������ҳ����
}

//�����������
void print_Access_Series()
{
    printf("Access Series:\n");
    for (int index = 0; index < Total_instruction; index++)
        printf("%d ", Access_Series[index]);
    printf("\n");
}

//������ڴ��ҳ��
void print_M_Frame()
{
    for (int i = 0; i < frame_num; i++)
    {
        if (M_Frame[i].flag == IN)
            printf("%d ", M_Frame[i].page_no);
    }
    printf("\n");
}
//���ȱҳ������ȱҳ��
void print_disaffect(int dis)
{
    float disaffect_rate=(float)dis/Total_instruction;
    printf("disaffect=%d\ndisafffect_rate=%f",disaffect,disaffect_rate);
}
//LRU���к��� ҳ��M_Frame0���ȼ���� [0]-[index-1]һ������ƶ�һλ
void hit(int index)
{
    printf("�������к���\n");
    int temp; //���ڽ���
    temp = M_Frame[index].page_no;
    for (int i = index; i >0; i--)
    {
        M_Frame[i].page_no = M_Frame[i-1].page_no;
    }
    M_Frame[0].page_no = temp;
    printf("\n");
}
//LRU δ���к��� ����
void miss_call_in(int page)
{
    if (loc < frame_num) //��ʱ���ڴ�δ��
    {
        for (int i=loc; i>0; i--)
           {
               M_Frame[i].flag=IN;
               M_Frame[i].page_no=M_Frame[i-1].page_no;//���ƶ�
           }
        M_Frame[0].flag = IN;
        M_Frame[0].page_no = page;//�ƶ��긳ֵ
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
   // for (int i = 0; i < Total_instruction; i++) //�����������ҳ������
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
            for ( index = 0; index < frame_num; index++) //�����ڴ� M_Frame
            {
                if (M_Frame[index].flag == IN && M_Frame[index].page_no == Access_Series[cur]) //���ڴ� �� ҳ������
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
            else //δ����
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
