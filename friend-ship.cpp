//user.h User 成员函数 查找在线的好友
vector<int> search_online()
{
    vector<string> online_friend;
    for(int i=0;i<friend.size();i++)
    {
        if(friend[i].onlineFlag)//在线
            online_friend.push_back(friend[i].id);  
    }
   return(online_friend);
}

//创建9个用户1-9
//1和所有人是好友
//2， 3和自己的倍数是好友
void initialize(vector<User>* u)
{   
   
    User user1;
    User user2;
    User user3;
     User user4;
      User user5;
       User user6;
        User user7;
     User user8;
    User user9;

     u->push_back(user1);
      u->push_back(user2);
       u->push_back(user3);
        u->push_back(user4);
     u->push_back(user5);
      u->push_back(user6);
       u->push_back(user7);
        u->push_back(user8);
     u->push_back(user9);
     
    for(int i=1;i<=9;i++)
    {
        u[i].id=i;
        for(int j=0;j<6;j++)
          u[i].cipher[j]='0'+i;//密码6个1
        u[i].cipher[j]='\0';
         u1.IPaddress="";
         onlineFlag=true;
    }
    for(int i=1;i<=9;i++)
    {
        if(i==1)//1和2-9都是好友
        { 
            for(int k=2;k<=9;k++)
             {
                 u[i].friend.push_back(u[k]);
                 u[k].friend.push_back(u[i]);
             }
        }
        else if(i==2)
        {
             for(int k=3;k<=9;k++)
              { 
                  if(k%2==0)//是2的整数倍
                 {
                      u[i].friend.push_back(u[k]);
                      u[k].firend.push_back(u[i]);
                 }
              }
        }
      else if(i==3)
       {
           for(int k=4;k<=9;k++)
           {
               if(k%3==0)//3的倍数
               {
                    u[i].friend.push_back(u[k]);  
                    u[k].firend.push_back(u[i]);
               } 
           }
       }
    }
    // for(int i=0;i<10;i++)
    // {
    //    User u1 ;
    //    u1.IPaddress="";
    //    u1.id=i;
    //    for(int j=0;j<6;j++)
    //    {
    //        u1.cipher[j]=i+'0';
    //    }
    //    u1.cipher[6]=0;

    //    u1.onlineFlag=false;
    //    u -> push_back(u1);
    // }
}




