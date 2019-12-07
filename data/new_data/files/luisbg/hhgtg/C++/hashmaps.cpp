#include <iostream>
#include <unordered_map>

using namespace std;

int
main ()
{
  unordered_map < string, string > capitals;
  capitals = { {"Canada", "Ottawa"},
	  {"Germany", "Berlin"},
	  {"Ireland", "Dublin"},
	  {"Spain", "Sevilla"},
	  {"UK", "London"},
	  {"USA", "Washington DC"} };

  cout << "capitals has size: " << capitals.size () << endl;
  cout << endl << "capitals contains:" << endl;
  for (auto it = capitals.begin (); it != capitals.end (); ++it)
    std::cout << " " << it->first << ":" << it->second << endl;
  cout << endl;

  std::cout << "capitals's buckets contain:\n";
  for (unsigned i = 0; i < capitals.bucket_count (); ++i) {
    std::cout << "bucket #" << i << " contains:";
    for (auto local_it = capitals.begin (i); local_it != capitals.end (i);
        ++local_it)
      std::cout << " - " << local_it->first << ":" << local_it->second;
    std::cout << std::endl;
  }

  cout << endl << "Capital of Spain: "  << capitals["Spain"] << endl;

  cout << endl << "erase Ireland" << endl;
  capitals.erase ("Ireland");
  cout << "insert South Korea" << endl;
  capitals.insert ( {"South Korea", "Seoul"} );

  cout << "update the capital of Spain" << endl;
  capitals["Spain"] = "Madrid";

  cout << endl << "capitals now contains:" << endl;
  for (auto it = capitals.begin (); it != capitals.end (); ++it)
    std::cout << " " << it->first << ":" << it->second << endl;
  cout << endl;

  cout << "Spain is listed " << capitals.count("Spain") << " times " << endl;

  return 0;
}
