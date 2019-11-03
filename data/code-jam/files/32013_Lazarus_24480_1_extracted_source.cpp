#include <iostream>
#include <set>
#include <string>

int
main(int argc, char *argv[])
{
    std::set<std::string> queries;
    int ncases;
    std::cin >> ncases;
    std::string unused;
    for(int i = 1; i <= ncases; ++i)
    {
        queries.clear();
        int num_engines;
        std::cin >> num_engines;
        getline(std::cin, unused);
        for(int j = 0; j < num_engines; ++j)
        {
            std::string temp;
            getline(std::cin, temp);
//            std::cerr << "Engine: " << temp << std::endl;
        }

        int num_queries;
        std::cin >> num_queries;
        getline(std::cin, unused);

        int num_switches = 0;
        for(int j = 0; j < num_queries; ++j)
        {
            std::string temp;
            getline(std::cin, temp);
            if(queries.size() < num_engines - 1) {
                queries.insert(temp);
//                std::cerr << "Inserted " << temp << std::endl;
            } else {
                if(queries.find(temp) == queries.end()) {
//                    std::cerr << "Could not find " << temp << std::endl;
                    ++num_switches;
                    queries.clear();
                    queries.insert(temp);
                } else {
//                    std::cerr << "Found " << temp << std::endl;
                }
            }
        }

        std::cout << "Case #" << i << ": " << num_switches << std::endl;
    }
}
