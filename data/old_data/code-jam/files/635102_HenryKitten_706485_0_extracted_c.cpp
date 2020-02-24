#include <cstdio>
#include <cstring>

bool Grid[301][301];
bool NewGrid[301][301];

int main(){
	int ncas;
	scanf("%d",&ncas);
	for(int cas=1;cas<=ncas;cas++){
		int nturn = 0,m;
		memset(Grid,0,sizeof(Grid));

		scanf("%d",&m);
		while(m--){
			int x1,x2,y1,y2;
			scanf("%d%d%d%d",&x1,&y1,&x2,&y2);
			for(int i=x1;i<=x2;i++){
				for(int j=y1;j<=y2;j++){
					Grid[i][j] = 1;
				}
			}
		}

		while(1){

			//Check validation
			bool flag = 0;
			for(int i=1;i<=300;i++){
				for(int j=1;j<=300;j++){
					if(Grid[i][j] == 1){
						flag = 1;
						break;
					}
				}
				if(flag) break;
			}
			if(flag == 0) break;

			memset(NewGrid,0,sizeof(NewGrid));
			for(int i=1;i<=300;i++){
				for(int j=1;j<=300;j++){
					NewGrid[i][j] = Grid[i][j];
					if(Grid[i-1][j] == 0 && Grid[i][j-1] == 0){
						NewGrid[i][j] = 0;	
					}
					if(Grid[i-1][j] == 1 && Grid[i][j-1] == 1){
						NewGrid[i][j] = 1;	
					}
				}
			}

			memmove(Grid,NewGrid,sizeof(Grid));

			nturn++;
		}
		
		printf("Case #%d: %d\n",cas,nturn);
	}
	return 0;
}
