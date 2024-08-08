#include <iostream>
#include <random>
#include <string>
#include <unordered_map>
#include <vector>

// A markov state contains words with their weights
typedef std::unordered_map<std::string, std::size_t> MarkovState;
typedef std::unordered_map<std::string, MarkovState> MarkovChain;
MarkovChain markovChain;

std::vector<std::string> split_str(const std::string& str, const std::string& delim)
{
    std::size_t current_pos = 0;
    std::size_t prev_pos = 0;
    std::vector<std::string> result{};

    while((current_pos = str.find(delim, prev_pos)) != std::string::npos)
    {
        result.push_back(str.substr(prev_pos, current_pos - prev_pos));
        prev_pos = current_pos + delim.size();
    }

    result.push_back(str.substr(prev_pos, current_pos));
    return result;
}

std::string get_next_word(const std::string& last_word)
{
    if(markovChain.count(last_word) == 0)
    {
        return "";
    }

    // Weighted random shit
    static std::random_device dev;
    static std::mt19937 rng(dev());

    std::size_t total_weight = 0;
    for(auto&[word, weight] : markovChain[last_word])
    {
        total_weight += weight;
    }

    std::uniform_int_distribution<std::size_t> dist(0, total_weight - 1);
    std::size_t random_weight = dist(rng);

    std::size_t sum = 0;
    for(auto&[word, weight] : markovChain[last_word])
    {
        if(sum <= random_weight && random_weight < sum + weight)
        {
            return word;
        }
        sum += weight;
    }

    return "";
}

void train(const std::string& corpus)
{
    std::vector<std::string> splitted_corpus = split_str(corpus, " ");
    // Build Markov chain
    for(std::size_t i = 0; i < splitted_corpus.size() - 1; ++i)
    {
        std::string& current_word = splitted_corpus[i];
        std::string& next_word = splitted_corpus[i + 1];

        markovChain[current_word][next_word]++;
    }
}

std::string complete(const std::string& text_so_far, std::size_t len=1)
{
    auto v = split_str(text_so_far, " ");
    std::string last_word = v[v.size() - 1];
    std::string result{};

    while(len--)
    {
        std::string next_word = get_next_word(last_word);
        if(next_word.empty())
        {
            return result;
        }
        
        result += (next_word + " ");
        last_word = next_word;
    }

    return result;
}

int main()
{
    std::string training_text{};
    std::string text_so_far{};

    std::cout << "Enter training text: ";
    std::getline(std::cin, training_text);
    std::cout << "Enter text so far: ";
    std::getline(std::cin, text_so_far);

    train(training_text);
    std::cout << complete(text_so_far, 99) << std::endl;

    return 0;
}
