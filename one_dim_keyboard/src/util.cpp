#include <util.h>
#include <unordered_set>
#include <random>
#include <print>
#include <algorithm>
#include <optional>
#include <SQLiteCpp/SQLiteCpp.h>
#include <mutex>

std::mutex db_write_mutex;
ThreadSignal::ThreadSignal(BoolProxy& ref):
    flag(ref)
{
    flag = false;
}
ThreadSignal::~ThreadSignal() {flag = true;}

BoolProxy::BoolProxy(bool initial) : _value(initial) {}
BoolProxy::BoolProxy(bool& b) {_value = b;}
BoolProxy::operator bool() const
{
    return _value;
}

WordFreqMap load_csv(const std::string& filename)
{
    rapidcsv::Document doc(filename);
    std::vector<std::string> words = doc.GetColumn<std::string>("word");
    std::vector<double> freqs = doc.GetColumn<double>("freq");
    WordFreqMap data;
    for(std::size_t i = 0; i < words.size(); ++i)
    {
        [[unlikely]] if(words[i].empty()) continue;
        data.emplace_back((WordPair){
            .word=std::move(words[i]),
            .freq=freqs[i]
        });
    }
    return data;
}

Layout str_to_layout(const std::string &s)
{
    Layout l;
    for(int i = 1; const auto& c : s)
    {
        l[c] = i;
        ++i;
    }
    return l;
}

double cost_fn(const WordFreqMap &words, const Layout &l)
{
    double total_cost = 0.0;
    for(const auto& wp : words)
    {
        double inner_cost = 0.0;
        const std::string& word = wp.word;

        char first_char = word.at(0);
        int last_pos = l.at(first_char);
        for(size_t i = 1; i < word.size(); ++i)
        {
            const char& c = word.at(i);
            int current_pos = l.at(c);
            inner_cost += std::abs(current_pos - last_pos) + 1;
            last_pos = current_pos;
        }

        total_cost += wp.freq * inner_cost;
    }

    return total_cost;
}

void hill_climb_racing(const WordFreqMap& words, sql::Database& db, BoolProxy& flag_ref)
{
    ThreadSignal signal(flag_ref);
    // Add a bloom filter here
    std::unordered_set<std::string> seen_layouts;
    std::string current_str_layout = ascii_lowercase;
    std::random_device dev;
    std::mt19937 prng(dev());
    std::shuffle(current_str_layout.begin(), current_str_layout.end(), prng);

    while(true)
    {
        Layout current_layout = str_to_layout(current_str_layout);
        seen_layouts.insert(current_str_layout);
        double current_cost = cost_fn(words, current_layout);
        // std::println("Current layout: {}. Cost: {}", current_str_layout, current_cost);

        std::optional<std::pair<std::size_t, std::size_t>> best_swap;
        for(std::size_t i = 0; i < current_str_layout.size(); ++i)
        {
            for(std::size_t j = i + 1; j < current_str_layout.size(); ++j)
            {
                std::swap(current_str_layout[i], current_str_layout[j]);
                if(seen_layouts.contains(current_str_layout))
                {
                    std::swap(current_str_layout[i], current_str_layout[j]);
                    continue;
                }
                Layout new_layout = str_to_layout(current_str_layout);
                double new_cost = cost_fn(words, new_layout);
                if(new_cost < current_cost)
                {
                    best_swap = {i, j};
                    current_cost = new_cost;
                }
                seen_layouts.insert(current_str_layout);
                std::swap(current_str_layout[i], current_str_layout[j]);
            }
        }
        if(!best_swap.has_value())
        {
            std::lock_guard<std::mutex> lock(db_write_mutex);
            sql::Statement query(db, R"(
                INSERT INTO keyboards (layout, cost) VALUES (:layout, :cost)
            )");
            query.bind(":layout", current_str_layout);
            query.bind(":cost", current_cost);
            query.exec();
            return;
        }
        auto[i, j] = best_swap.value();
        std::swap(current_str_layout[i], current_str_layout[j]);
    }
}
