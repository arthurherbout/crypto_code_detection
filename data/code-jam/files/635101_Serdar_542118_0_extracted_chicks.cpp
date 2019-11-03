#include <iostream>

using namespace std;

struct Chick{
	int speed;
	int pos;
};

Chick chicks[50];
int pas[50];

int main()
{

	int C;
	cin>>C;

	for( int testCase = 1 ; testCase <= C ; testCase++ )
	{
		int gecen = 0;
		for( int i = 0 ; i < 50 ; i++ )
			pas[i] = 0;
		int N,K,B,T;
		cin >> N >> K >> B >> T;
	
		for( int i = 0 ; i < N ; i++ )
			cin >> chicks[i].pos;
		for( int i = 0 ; i < N ; i++ )
			cin >> chicks[i].speed;
	
		for( int i = 0 ; i < N ; i++ )
		{
			int length = B - chicks[i].pos;
			int pos = chicks[i].pos;
			int speed = chicks[i].speed;
			int t = T;
			
			if( length > speed * t )
			{
				pas[i] = -1;
				continue;
			}
			
			// it can reach
			for( int j = i+1 ; j < N ; j++ )
			{
				int pos2 = chicks[j].pos;
				int speed2 = chicks[j].speed;
				if( speed2 >= speed )
					continue;
				
				if( speed*(pos2-pos) < length*(speed - speed2) )
					pas[i]++;
			}
			
			gecen++;
		}
	
		if( gecen < K )
		{
			cout << "Case #"<< testCase << ": IMPOSSIBLE" << endl;
			continue;
		}
	
		//cout << "---" << gecen << endl;
		for( int i = 0 ; i < N ; i++ )
			for( int j = i+1 ; j < N ; j++ )
				if ( pas[j] < pas[i] )
				{
					int temp = pas[j];
					pas[j] = pas[i];
					pas[i] = temp;
				}
		int k = 0;
		while( pas[k] < 0 )
			k++;
		int total = 0;
		for( int i = 0 ; i < K ; i++ )
			total += pas[k+i];
	
		cout << "Case #"<< testCase << ": " << total << endl;
		//cout << total << endl << endl;
	}




	return 0;
}
