#include <stdio.h>

struct dataType{
       int intMe;
       char charMe;
       int sink;
};
typedef struct dataType DT;
DT mapHW[100][100];

struct dataType1{
       int intMe;
       int minh;
       int minw;
};
typedef struct dataType1 Details;

char currentChar = 'a';
int H=0,W=0, casecount=0;
int currenth,currentw;

void labelMe(int h,int w);
Details lowest(int currenth,int currentw);
int isSink(int h, int w);
int minValue();

int main()
{
  FILE *file;
  FILE *fileo;
  char ch;
  int totalCount;
  int i=0,j=0,k=0;
  int h=0,w=0;
  int hwcount = 0;
  int mapSize;
  int allDetails[10000];
  char tempStr[4];

  file = fopen("B-small-attempt1.in", "r"); //B-small-attempt0 //codejam2.txt
  fileo = fopen("output.in", "w"); 
  if(file==NULL) {
    printf("Error: can't open files.\n");getch();
    return 1;
  }
  else {
       
   while(1){
      if((ch=fgetc(file)) == '\n'){
            tempStr[i] = '\0'; i=0;
            mapSize = atoi(tempStr);
            break;
      }else{          
           tempStr[i] = ch;
           i++;
      }
    } 
   i=0;totalCount=1;
    while((ch=fgetc(file)) != EOF) {
      if(ch == ' ' || ch == '\n'){
            tempStr[i] = '\0'; i=0;
            allDetails[totalCount]=atoi(tempStr);
            totalCount++;
      }else{
           tempStr[i] = ch;       
           i++;
      }
    }
    tempStr[i] = '\0';
    allDetails[totalCount]=atoi(tempStr);
    
    fclose(file);
    casecount = 0;
    for(i=1;i<totalCount;){
        hwcount++;
        if(hwcount == 1)
             H=allDetails[i++];
        else if(hwcount == 2)
             W=allDetails[i++];
        else{
            for(j=0;j<H;j++){
               for(k=0;k<W;k++){
                   mapHW[j][k].intMe=allDetails[i];
                   mapHW[j][k].charMe='*';
                   mapHW[j][k].sink=0;
                   
                   i++;
               }
            }

              //labelMaps();
              for(k=0;k<H;k++){
         for(j=0;j<W;j++){
            mapHW[k][j].charMe='*';
            mapHW[k][j].sink=0;
         }
     }
     
     for(k=0;k<H;k++){
         for(j=0;j<W;j++){
               labelMe(k,j);
         }
     }
     casecount++;
     fprintf(fileo,"Case #%d:\n",casecount);
     for(k=0;k<H;k++){
         for(j=0;j<W;j++){
                         //fputc(mapHW[k][j].charMe,fileo);
         if(j==W-1)
         fprintf(fileo,"%c",mapHW[k][j].charMe);
         else fprintf(fileo,"%c ",mapHW[k][j].charMe);
         }fprintf(fileo,"\n");
     }
              /////
              currentChar = 'a';
              hwcount=0;
        }
    }
    fclose(fileo);
    getch();
    return 0;
  }
}

void labelMe(int h,int w){
     DT current;
     Details temp;
     current =  mapHW[h][w];
     currenth = h;currentw = w;
     
         while(!isSink(currenth,currentw)){
          temp = lowest(currenth,currentw);
          currenth = temp.minh;
          currentw = temp.minw;
         }
		int i,j;
        
     if(isSink(currenth,currentw) && mapHW[currenth][currentw].sink == 0){
              mapHW[currenth][currentw].charMe = currentChar; currentChar = currentChar + 1;
              mapHW[currenth][currentw].sink = 1;
              
         }
         
      if(mapHW[currenth][currentw].sink == 1){
              mapHW[h][w].charMe = mapHW[currenth][currentw].charMe;
      }
}

int isSink(int h, int w){
    Details min; 
    min = lowest(h,w);
    if(min.minh == h & min.minw == w)
    return 1; //its sink
    else
    return 0; //not sink
}

Details lowest(int currenth,int currentw){
    Details currentD,min;
    int east=0,west=0,north=0,south=0; //right-east ; left-west
    currentD.intMe = mapHW[currenth][currentw].intMe;
    currentD.minh = currenth;
    currentD.minw = currentw;
    int i;
    min = currentD;
    for(i=0;i<4;i++){
            switch(i){
            case 0: if(currenth-1 >= 0){ //north
            
                        if(min.intMe <= mapHW[currenth-1][currentw].intMe){
                            //dont do anything  
                        }else{   
                            min.intMe = mapHW[currenth-1][currentw].intMe;
                            min.minh = currenth-1;
                            min.minw = currentw;                        
                        }
                    }break;
            case 1:if(currentw-1 >= 0){ //west-left
            
                        if(min.intMe <= mapHW[currenth][currentw-1].intMe){
                            //dont do anything
                        }else{
                            min.intMe = mapHW[currenth][currentw-1].intMe;
                            min.minh = currenth;
                            min.minw = currentw-1;                        
                        }
                   }break;
            case 2:if(currentw+1 < W){ //east-right
           
                        if(min.intMe <= mapHW[currenth][currentw+1].intMe){
                            //dont do anything  
                        }else{
                            min.intMe = mapHW[currenth][currentw+1].intMe;
                            min.minh = currenth;
                            min.minw = currentw+1;                        
                        }
                   } break;
            case 3:if(currenth+1 < H){ //south
            
                        if(min.intMe <= mapHW[currenth+1][currentw].intMe){
                            //do nothing      
                        }else{
                            min.intMe = mapHW[currenth+1][currentw].intMe;
                            min.minh = currenth+1;
                            min.minw = currentw;                        
                        }
                    }break;
            }
            
            //North, West, East, South
            
    }
    return min;
}

int minValue(){
    int i,j,min;
    for(i=0;i<H;i++)
         for(j=0;j<W;j++)
            if(mapHW[i][j].intMe < min)
            min=mapHW[i][j].intMe;
    return min;
}

