#include <unordered_map>
using namespace std;

int entry(int x)
{
    static std::unordered_map<int, int> table = {{1, 1}};
    if(table.count(x)) return table[x];

    return (table[x] = 1 + entry(x - entry(entry(x - 1))));
}
