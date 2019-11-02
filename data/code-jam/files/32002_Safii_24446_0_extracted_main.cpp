// This code is devoted to Airi who I love.

#include <iostream>
#include <string>

using namespace std;

bool broken[10][10];
bool field[10][10];
int w, h;

int answer;

bool check(int x, int y)
{
    if(x<0 || x>=w || y<0 || y>=h)
        return true;

    return !field[y][x];
}

void solve(int x, int y, int cnt)
{
    if(cnt + ((w-x+1)+(h-y-1)*(w+1))/2< answer)
        return;
    if(x==w)
    {
        solve(0, y+1, cnt);
        return;
    }
    if(y==h)
    {
        answer = max(cnt, answer);
        return;
    }

    if(!broken[y][x])
    {
        if( check(x-1, y-1) &&
            check(x-1, y) &&
            check(x+1, y-1))
        {
            field[y][x]=true;
            solve(x+1, y, cnt+1);
            field[y][x]=false;
        }
    }
    solve(x+1, y, cnt);
}

int main()
{
    int n;
    cin >> n;
    for(int i=0; i<n; i++)
    {
        cin >> h >> w;

        string buf;
        getline(cin, buf);
        for(int y=0; y<h; y++)
        {
            getline(cin, buf);
            for(int x=0; x<w; x++)
            {
                field[y][x] = false;
                broken[y][x] = (buf[x]=='x');
            }
        }

        answer=0;
        solve(0, 0, 0);
        cout << "Case #" << i+1 << ": "
           << answer
           << endl;
    }
    return 0;
}
