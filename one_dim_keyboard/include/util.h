#pragma once
#include <unordered_map>
#include <string>
#include <vector>
#include <rapidcsv.h>
#include <cmath>
#include <SQLiteCpp/SQLiteCpp.h>
namespace sql = SQLite;

const std::string ascii_lowercase("abcdefghijklmnopqrstuvwxyz");

struct WordPair
{
    std::string word;
    double freq;
};

class BoolProxy
{
public:
    BoolProxy(bool initial = false);
    BoolProxy(bool& b);
    operator bool() const;
private:
    bool _value;
};

struct ThreadSignal
{
    BoolProxy& flag;
    ThreadSignal(BoolProxy& ref);
    ~ThreadSignal();
};

typedef std::unordered_map<char, int> Layout;
typedef std::vector<WordPair> WordFreqMap;

WordFreqMap load_csv(const std::string& filename);
Layout str_to_layout(const std::string& s);
double cost_fn(const WordFreqMap& words, const Layout& l);
void hill_climb_racing(const WordFreqMap& words, sql::Database& db, BoolProxy& flag_ref);
