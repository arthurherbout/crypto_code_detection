    /******************************
    *    �ļ���twelve_Spring.c   *
    *    ��ɣ�CN-2082-2         *
    *    ���ߣ�W0ng Y1R2n        *
    *    ���ţ�PE-362��UG        *
    *    ԭ����: TI-352132��һһ *
    *    ����ʮ����:Spring 12    *
    *******************************
/** #env "planet_engine" - target engine
                -- ��������3��ת�򷢶���**/

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
//#include <self_check_striker.c>
//#include <engine_start.c>
#include <unistd.h>

#ifndef CURVE_NATURAL
#define CURVE_NATURAL
#endif // CURVE_NATURAL

/*
int engine_init() {
    set_engine_number_mask(ENGINE_ALL);
    set_funeral_level(FUNERAL_FULL);

    ready_state = self_check_striker(checked_array_iterator_time, number_engine, temperature);
                //ײ���Լ����
    if ready_state == true
        return engine_start(main);
    //�������10������
    else if unix_time() < make_unix_time(2082��1��28��23��59��60 - 10))
        return ERR_ENGIN_ENV;
    else
        return  engine_check_init();
                //�ڱ���֮���ʵ������֮ǰ
}
int main(){
    set_curve��CURVE_NATURAL��; //��Ȼ���ߺķ�ȼ������
    for(explicit i: range(0��12��1)){
        engine_start();
        wait_engine��ENGINE_STATE_CHAGNE��;
        sleep(2000); //����������

        engin_stop();
        wait_engine��ENGINE_STATE_CHAGNE��;//��������ȴ
        sleep(4000);  //���ʱ����ģ��������������������
    }
    return 0 ;
}
int  final(){
    engine_ensure_shutdown����;

}
**/
/***---------demoģ����Գ���------------ ***/

int main()
{	register int c;
    register int k;
c = 12;
k = 1;
printf("����ʮ������������\n");

//sleep(3);

	int i;
    for(i=1;i<101;i++)
    {
              printf("[");
              printf("������ؽ���>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>[\b\b\b%d%%]\r",i);
              fflush(stdout);
              usleep(10000);
            //usleep(1000*1000);

        if(i % 10 == 0){
            sleep(1);

        }
    }
    printf("\n");
    printf("���������ϣ���������������...\n");

    sleep(1);
        printf("Ӳ��ϵͳ�Լ����...\n");
            sleep(1);
            printf("ײ��ϵͳ�Լ����...\n");
                sleep(1);
                    printf("��������������...\n");
    sleep(1);


while(k <= 10){

    printf("����ת�򷢶�������������..............%d/100\r",k * 10 );
    k += 1;

    sleep(1);
}

while( c > 0)
{
    Sleep(5);
    system("color 46");

    Sleep(100);
    system("color 04");

        c=c-1;
        Sleep(10);
        system("cls");
}
system("color 04");
int a; a=12;

while(a>=1){

    printf("%*s%s%*s",52,"","Biu!!!\n",0,"");
    a -= 1;
    Sleep(100);
}
int j = 12;

printf("%*s%s%*s",51,"","\\ ^^ /\n");

while(j >= 1)
{
    printf("%*s%s%*s",51,"","| ^^ |\n");
    j -= 1;
    Sleep(100);
}

//���Ƿ�������״
    printf("%*s%s%*s",51,"","/----\\\n");
    printf("%*s%s%*s",49,""," / \\  / \\ \n");
    Sleep(100);
    printf("%*s%s%*s",48,"","| -------- | \n");
    Sleep(100);
    printf("%*s%s%*s",45,"","/| ------------- |\\ \n");
    Sleep(100);
    printf("%*s%s%*s",43,"","__________________\n");
    Sleep(100);
    Sleep(1000);
int d;
d=12;
while(d>0)
{
    system("color 46");
    Sleep(100);

    system("color 04");
    d = d - 1;
    Sleep(10);
    }
return 0;

    }
