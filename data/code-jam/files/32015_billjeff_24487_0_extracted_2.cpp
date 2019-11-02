#include <iostream>
using namespace std ;

int cases = 1 ;

long long re = 0 ;
long long num = 0 ;
int N ;
int len ;
string Num ;

bool IsOk( int *arr )
{
  int ptr = 1 ;
  long long val = Num[0]-'0' ;
  for ( int i = 0 ; i < len ; ++i ){
    //cout << val << endl ; // debug
    if ( arr[i] == -1 && ptr < len ){
      long long tmp = Num[ptr++]-'0' ;
      ++i ;
      while ( i < len && arr[i] == 0 && ptr < len ){
	tmp *= 10 ;
	tmp += Num[ptr++]-'0' ;
	++i ;
      }
      --i ;
      val -= tmp ;
    }
    else if ( arr[i] == 1 && ptr < len ){
      long long tmp = Num[ptr++]-'0' ;
      ++i ;
      while ( i < len && arr[i] == 0 && ptr < len){
	tmp *= 10 ;
	tmp += Num[ptr++]-'0' ;
	++i ;
      }
      --i ;
      val += tmp ;
    }
    else if ( ptr < len )// is zero
    {
      val *= 10 ;
      val += Num[ptr++]-'0' ;
    }
    //cout << val << endl ; // debug
  }
  /*
  for ( int i = 0 ; i < len ; ++i )
    cout << arr[i] << " " ;
  cout << endl ;
  cout << Num << ": " << val << endl ; // debug
  */
  if ( val == 0 ) return true ;
  if ( val % 2 == 0 ) return true ;
  if ( val % 3 == 0 ) return true ;
  if ( val % 5 == 0 ) return true ;
  if ( val % 7 == 0 ) return true ;
  return false ;
}

void search( int d, int *arr )
{
  if ( d+1 == len ){
    if ( IsOk( arr ) )  ++re ;
    return ;
  }

  arr[d] = -1 ; // minus ;
  search( d+1, arr ) ;
  arr[d] = 0 ; // zero ;
  search( d+1, arr ) ;
  arr[d] = 1 ; // add ;
  search( d+1, arr ) ;
  
}

int main( void )
{
  //freopen( "in.txt", "r", stdin ) ;
  freopen( "in2-s", "r", stdin ) ;
  freopen( "out2-s", "w", stdout ) ;

  cin >> N ;
  while ( N-- ){
    cout << "Case #" << cases++ << ": " ;
    cin >> Num ;
    re = 0 ;
    len = Num.length() ;
    //long long tmp = num ;
    int arr[14] ;
    search( 0, arr ) ;
    cout << re << endl ;
  }

  return 0 ;
}
