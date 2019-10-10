    #include<stdio.h>
    #include<iostream>
    #include<math.h>
    using namespace std;
    int main()
    {
    // freopen("C-small-attempt01.in","r",stdin);
    // freopen("output.txt","w",stdout);
    int t,i;
    scanf("%d",&t);
    int initial=0,count=0;

    int n,m,k;
    int sqr;
    int ans=0;
    while(t--)
    {
    initial++;
    ans=0;
    cin>>n;
    cin>>m;
    cin>>k;
    sqr=sqrt(k);
    //cout<<"sqr "<<sqr;
    if((sqr*sqr>k/2) && (sqr<=n)&& (sqr<=m))
    {
    ans=k-(sqr*sqr-(sqr*2+2*(sqr-2)));
    //cout<<endl;
    //cout<<ans<<endl;

    ans=ans-(k-(sqr*sqr));
    //cout<<ans<<endl;
    }
    else
    {
    int l,b;

    l=sqr;
    b=sqr;
    bool con=true;
    do
    {
    if(con)
    {l++;
    con=false;
    }
    else{
    b++;
    con=true;
    }

    }while(l*b<=k);
    if(con)
    b--;
    else
    l--;
    ans=k-(l*b-(b*2+2*(l-2)));
    //cout<<endl;
    //cout<<ans<<endl;

    ans=ans-(k-(l*b));
    //cout<<ans<<endl;
    }
    printf("Case #%d: %d\n",initial,ans);


    }


    }
