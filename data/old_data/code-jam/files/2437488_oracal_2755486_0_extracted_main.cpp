#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <sstream>

// Usage:
// cat input.txt | ./main > output.txt
// ./main input.txt > output.txt
// ./main input.txt output.txt

void processAllCases(std::istream &is, std::ostream &os);
void processCase(const int iteration, std::istream &is, std::ostream &os);

int main(int argc, char *argv[]) {
    std::ostream* os;
    std::istream* is;
    std::ofstream fout;
    std::ifstream fin;
    switch(argc) {
        case 1:
            os = &std::cout;
            is = &std::cin;
            break;
        case 2:
            fin.open(argv[1]);
            is = &fin;
            os = &std::cout;
            break;
        case 3:
            fin.open(argv[1]);
            is = &fin;
            fout.open(argv[2]);
            os = &fout;
            break;
        default:
            std::cout
                << "Usage:" << std::endl
                << "cat input.txt | ./main > output.txt" << std::endl
                << "./main input.txt > output.txt" << std::endl
                << "./main input.txt output.txt" << std::endl;
            return 1;
    }
    processAllCases(*is, *os);
    return 0;
}

// process all cases
void processAllCases(std::istream &is, std::ostream &os)
{
    int totalCases;
    is >> totalCases;
    for(int caseNumber = 1; caseNumber <= totalCases; ++caseNumber)
    {
        processCase(caseNumber, is, os);
    }
}

// process individual case, caseNumber = 1..totalCases
void processCase(const int caseNumber, std::istream &is, std::ostream &os)
{

    int N;
    is >> N;
    std::vector<std::vector<long> > tribes;

    for(int i =0; i<N; ++i)
    {
        std::vector<long> tribe;
        for(int j =0; j < 8; ++j)
        {
            long temp;
            is >> temp;
            tribe.push_back(temp);
        }
        tribes.push_back(tribe);

    }

    int day = 0;
    std::map<long, long> wall;
    int count = 0;

    while(true)
    {
        std::map<long, long> attack;
        bool moreAttacks = false;

        for(int i =0; i < tribes.size(); ++i)
        {
            bool attackSucceeded = false;
            if(tribes[i][1] !=0)
            {
                moreAttacks = true;
                if(tribes[i][0] == day)
                {
                    //do the attack;
                    for (long position = tribes[i][2]; position < tribes[i][3]; ++position)
                    {
                        if(tribes[i][4] > wall[position])
                        {
                            attackSucceeded = true;
                        }
                        if(tribes[i][4] > attack[position])
                        {
                            attack[position] = tribes[i][4];
                        }
                    }

                    tribes[i][1]--;


                    tribes[i][0] += tribes[i][5];
                    tribes[i][2] += tribes[i][6];
                    tribes[i][3] += tribes[i][6];
                    tribes[i][4] += tribes[i][7];
                }
            }
            if (attackSucceeded)
            {
                count++;
            }
        }

        for(std::map<long, long>::iterator it = attack.begin(); it !=attack.end(); ++it)
        {
            if(wall[it->first] < it->second)
            {
                wall[it->first] = it->second;
            }
        }

        if (!moreAttacks)
        {
            break;
        }


        day++;
    }

    os << "Case #" << caseNumber << ": " << count << std::endl;
}




