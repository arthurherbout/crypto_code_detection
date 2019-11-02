#include <stdio.h>

int main(void) {
int t, n, nums[1000], a, b, org[1000], truth;
float temp;
scanf("%d", &t);
for(a=0; a<t; a++){
scanf("%d", &n);
for(b=0; b<n; b++){
scanf("%d", &nums[b]);
org[b]=b;
}
truth=0;
for(b=0; b<n; b++){
if(nums[b]==org[b]){
truth++;
}
if((b-1)>0){
if(nums[b]==org[b-1]){
truth++;
}
}
if((b+1)<n){
if(nums[b]==org[b+1]){
truth++;
}
}
}
temp=n;
temp=temp/2;
if(truth>temp){
printf("Case #%d: BAD\n", (a+1));
}
else{
printf("Case #%d: GOOD\n", (a+1));
}
}

return 0;
}
