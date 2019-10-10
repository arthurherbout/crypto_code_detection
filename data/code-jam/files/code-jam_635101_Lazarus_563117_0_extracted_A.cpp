#include <iostream>
#include <map>
#include <set>
#include <vector>


class directory_tree
{
public:
    typedef std::map<std::string, directory_tree*> child_map_t;

    child_map_t children;

    directory_tree() :
        children()
    {}

    int mkdir(const std::string& s)
    {
        std::string temp;
        int len = s.length();
        int created = 0;
        directory_tree *node = this;
        for(int i = 1; i <= len; ++i)
        {
            if(i==len || s[i] == '/') {
                child_map_t::const_iterator it = node->children.find(temp);
                if(it == node->children.end()) {
                    ++created;
                    node->children[temp] = new directory_tree();
                    node = node->children[temp];
                } else {
                    node = it->second;
                }
                temp.clear();
            } else {
                temp += s[i];
            }
        }
        return created;
    }
};

int main(int argc, char *argv[])
{
    int ncases;
    std::cin >> ncases;
    for(int i = 1; i <= ncases; ++i)
    {
        int N, M;
        std::cin >> N >> M;

        directory_tree root;
        std::string temp;

        for(int j = 0; j < N; ++j)
        {
            std::cin >> temp;
            root.mkdir(temp);
        }

        int total = 0;
        for(int j = 0; j < M; ++j)
        {
            std::cin >> temp;
            total += root.mkdir(temp);
        }

        std::cout << "Case #" << i << ": " << total;
        std::cout << std::endl;
    }
}
