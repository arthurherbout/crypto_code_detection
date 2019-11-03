#include <QtCore/QCoreApplication>

#include <QStringList>
#include <QFile>
#include <QTextStream>
int gcd(int a,int b);
class Test
{
public:
    QList<long int> freq;

    QString res;

    long int f;

    long int N;
    long int L;
    long int H;

};

Test*
  readTest(QTextStream& in)
{
    Test* res = new Test;

    in >> res->N;
    in >> res->L;
    in >> res->H;

    long int buf;
    for (int i =0 ; i < res->N; i++)
    {
        in >> buf;
        res->freq.append(buf);
    }

    return res;
};

void
        solve(Test *test)
{
    bool possible = false;

    //sort
    qSort(test->freq);

    for (int i = test->L;
         i<=test->H;
         i++)
    {
        bool okay = true;
        for (int j = 0; j < test->freq.size(); ++j)
            if ((i % test->freq[j] != 0)
                && (test->freq[j] % i !=0))
                {
            okay = false;
            break;
            }

        if (okay)
        {
            test->f = i;
            possible = true;
            break;
        }
    }


    if (possible)
        test->res = QString::number(test->f);
    else
        test->res.append("NO");


}

int gcd(int a,int b)
{
  int c;
  while(1)
  {
      c = a%b;
      if(c==0)
        return b;
      a = b;
      b = c;
  }
}

int lcm(int a,int b)
 {
   int n;
   for(n=1;;n++)
   {
       if(n%a == 0 && n%b == 0)
         return n;
   }
 }

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QFile file("in.txt");
    QFile outfile("out.txt");

    QTextStream in (&file);
    QTextStream out (&outfile);

    outfile.open(QIODevice::WriteOnly
             | QIODevice::Text);
    if (!file.open(QIODevice::ReadOnly
            | QIODevice::Text))
        out << "no in file";

    int T= 0;
    in >> T;

    for (int i=0; i< T; i++)
    {
        Test *test = readTest(in);
        solve(test);
        out << "Case #" << i+1 << ": " <<  test->res << "\n";
    }

   return 0;
}
