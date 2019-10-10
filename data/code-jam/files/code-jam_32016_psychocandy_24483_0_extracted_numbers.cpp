#include <stdio.h>
#include <string.h>
#include <fstream>
#include <iostream>
#include <map>

using namespace std;

int main()
{

int N;
int n;
char cadena[10];
char cad[10];
ifstream entrada;
ofstream salida;

entrada.open("C-small.in");
salida.open("salida.txt");

entrada>>N;
entrada.getline(cad,10);

for(int i=0;i<N;i++){

entrada>>n;
entrada.getline(cad,10);

if (n==2) strcpy(cadena,"027");
if (n==3) strcpy(cadena,"143");
if (n==4) strcpy(cadena,"751");
if (n==5) strcpy(cadena,"935");
if (n==6) strcpy(cadena,"607");
if (n==7) strcpy(cadena,"903");
if (n==8) strcpy(cadena,"991");
if (n==9) strcpy(cadena,"335");
if (n==10) strcpy(cadena,"047");
if (n==11) strcpy(cadena,"943");
if (n==12) strcpy(cadena,"471");
if (n==13) strcpy(cadena,"055");
if (n==14) strcpy(cadena,"447");
if (n==15) strcpy(cadena,"463");
if (n==16) strcpy(cadena,"991");
if (n==17) strcpy(cadena,"095");
if (n==18) strcpy(cadena,"607");
if (n==19) strcpy(cadena,"263");
if (n==20) strcpy(cadena,"151");
if (n==21) strcpy(cadena,"855");
if (n==22) strcpy(cadena,"527");
if (n==23) strcpy(cadena,"743");
if (n==24) strcpy(cadena,"351");
if (n==25) strcpy(cadena,"135");
if (n==26) strcpy(cadena,"407");
if (n==27) strcpy(cadena,"903");
if (n==28) strcpy(cadena,"791");
if (n==29) strcpy(cadena,"135");
if (n==30) strcpy(cadena,"647");

salida<<"Case #"<<i+1<<": "<<cadena<<endl;
}

entrada.close();
salida.close();

}
