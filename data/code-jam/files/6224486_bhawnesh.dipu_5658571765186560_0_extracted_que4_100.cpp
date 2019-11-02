#include<iostream>
#include<fstream>
using  namespace std;
typedef struct t
{
    public :
    int r,c,x;
}ttt;



int main()
{


    freopen("D-small-attempt0.in","r",stdin);
    freopen("D-small-attempt0.out","w",stdout);










    ttt t_4[5],t_3[2],t_2[1];
t_4[0].r=2;t_4[0].c=2;t_4[0].x=4;
t_4[1].r=3;t_4[1].c=2;t_4[1].x=4;
t_4[2].r=3;t_4[2].c=2;t_4[2].x=4;
t_4[3].r=3;t_4[3].c=2;t_4[3].x=4;
t_4[4].r=4;t_4[4].c=1;t_4[4].x=4;

t_3[0].r=3;t_3[0].c=1;t_3[0].x=3;
t_3[1].r=2;t_3[1].c=2;t_3[0].x=3;

t_2[0].r=2;t_2[0].c=1;t_2[0].x=2;







    int tt;
    cin>>tt;
    for(int test=1;test<=tt;test++)
    {
        int x,r,c;
        cin>>x>>r>>c;
        int ans=0;
        int flag=0;

      if(x==1)
      {
          flag=1;

      }
      else if(x==2)
      {

          if(x>r&&x>c)flag=0;

          else if((x<=r&&x>=c)||(x>=r&&x<=c)||(x<=r&&x<=c))
          {
              if(r*c%2==0) flag=1;


          }


      }
      else if(x==3)
      {

          if(x>r&&x>c)flag=0;

          else if((x<=r&&x>=c)||(x>=r&&x<=c)||(x<=r&&x<=c))
          {
              if(r*c%3==0) flag=1;


          }
    }
    else if(x==4)
    {
        if(r==1||c==1)flag=0;

        else if(x>r&&x>c)flag=0;

        else if(r==2||c==2)
        {
            if(r%5==0||c%5==0) flag=1;
        }
         else if((x<=r&&x>=c)||(x>=r&&x<=c)||(x<=r&&x<=c))
          {
              if(r*c%4==0) flag=1;


          }



    }


        cout<<"Case #"<<test<<": ";
        (flag)?cout<<"GABRIEL"<<endl:cout<<"RICHARD"<<endl;

    }

}
