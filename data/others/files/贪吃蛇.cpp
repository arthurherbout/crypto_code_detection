#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include<conio.h>
#include<time.h>
char x[10][20];
int H[20];
int Z[20];
int h, v;//�ֱ�����������
char ch = 'd';
int len = 3;
char f;
void getFood()//ʳ��
{
	h = rand() % 10;
	v = rand() % 20;
	if (x[h][v] == 1)
	{
		getFood();
	}
	else
		x[h][v] = 3;//����ASCII��Ϊ3
}
void print()
{
	for (int i = 0; i <len; i++)//�������м�¼����������ȫ�����ΪЦ������ASCII��Ϊ1��
	{
		x[H[i]][Z[i]] = 1;
	}
	for (int i = 0; i < 10; i++)//�ñ�������ʽ����
	{
		for (int j = 0; j < 20; j++)
		{
			printf("%c", x[i][j]);
		}
		printf("\n");
	}
	Sleep(500);//����0.5�� �൱����0.5�붯һ��
	system("cls");//�������������Ա㽫�������»���
}
void getkey()//�ж��Ƿ��¼���
{
	if (_kbhit())//������¼��̣��ͻ�ִ���������
	{
		ch = _getch();//��ȡ����ֵ
		if (ch == f)//�����ΰ����������ߵķ���һ����Ϊ��Ч��������������ȡ���µļ���ֵ
		{
			getkey();
		}
		else if ((ch == 100 && f != 97) || ch == 119 && f != 115 || ch == 97 && f != 100 || ch == 115 && f != 119) //�����ж�������Ч��������������ʱ�������䳯���ߵȵȣ�
			f = ch;//������Чʱ��ch��ֵΪf
		else
			getkey();//������Чʱ���²������
	}
}
void left()
{
	x[H[0]][Z[0]] = '-';
	for (int i = 0; i <len - 1; i++)//����ȫ��ǰ��һ��
	{
		Z[i] = Z[i + 1];
		H[i] = H[i + 1];
	}
	Z[len-1]--;
}
void right()
{
	x[H[0]][Z[0]] = '-';
	for (int i = 0; i <len - 1; i++)//����ȫ��ǰ��һ��
	{
		Z[i] = Z[i + 1];
		H[i] = H[i + 1];
	}
	Z[len-1]++;
}
void up()
{
	x[H[0]][Z[0]] = '-';
	for (int i = 0; i <len-1; i++)//����ȫ��ǰ��һ��
	{
		Z[i] = Z[i + 1];
		H[i] = H[i + 1];
	}
	H[len-1]--;
}
void down()
{
	x[H[0]][Z[0]] = '-';
	for (int i = 0; i <len - 1; i++)//����ȫ��ǰ��һ��
	{
		Z[i] = Z[i + 1];
		H[i] = H[i + 1];
	}
	H[len-1]++;
}
int main()
{
	f = ch;//����ʼ���ƶ�����ֵ��f
	for (int i = 0; i < 10; i++) //���ַ�����ȫ����'-'��䣬���г�ʼ��
	{
		for (int j = 0; j < 20; j++)
		{
			x[i][j] = '-';
		}
	}
	for (int i = 0; i <10; i++)//���������������ȫ������
	{
		Z[i] = 0;
		H[i] = 0;
	}
	for (int i = 0; i <len; i++)//��¼ǰ3����������
		Z[i] = i;
	getFood();
	while (len<=19)//�����ߵ���󳤶���Ӯ����Ϸ
	{
		getkey();
		if (x[h][v]!=3)//ʳ��û���˲Ż�ȡʳ��
		{
          getFood();
		}
		switch (f)
		{
		case 'w':
			  if(H[len-1]-1<0)
			 {
			  printf("������ǽ��\n");
		      return 0;
             }
			else if (H[len-1]-1==h&&Z[len-1]==v)//�ж���Ҫ�ߵ��¸����Ƿ���ʳ��ĵ�
			 {
				 H[len]=h;
				 Z[len]=v;
				 len++;
				 break;
			 }
			up();
			break;
		case 'a':
			if(Z[len-1]-1<0)
			{
			printf("������ǽ��\n");
		    return 0;
			}
            else if (H[len-1]==h&&Z[len-1]-1==v)
			 {
				 H[len]=h;
				 Z[len]=v;
				 len++;
				 break;
			 }
			left();
			break;
		case 's':
			if(H[len-1]+1>=10)
			{
			printf("������ǽ��\n");
		    return 0;
			}
			if (H[len-1]+1==h&&Z[len-1]==v)
			 {
				 H[len]=h;
				 Z[len]=v;
				 len++;
				 break;
			 }
			down();
			break;
		case 'd':
			if(Z[len-1]+1>=20)
			{
			printf("������ǽ��\n");
		    return 0;
			}
			if (H[len-1]==h&&Z[len-1]+1==v)
			 {
				 H[len]=h;
				 Z[len]=v;
				 len++;
				 break;
			 }
			right();
			break;
		}
		print();
	}
    printf("You win!\n");
	system("pause");
	return 0;
}

