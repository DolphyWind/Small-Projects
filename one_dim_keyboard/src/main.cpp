#include <csignal>
#include <cstring>
#include <thread>
#include <array>
#include <chrono>
#include <print>
#include <util.h>
#include <SQLiteCpp/SQLiteCpp.h>
using namespace std::chrono_literals;
namespace sql = SQLite;

bool run = true;

void sigHndl(int signal)
{
    if(signal == SIGINT)
    {
        run = false;
    }
}

int main(int argc, char** argv)
{
    std::size_t N_THREADS = 4;
    for(int i = 1; i < argc; ++i)
    {
        if(std::strncmp(argv[i], "-j", 2) == 0)
        {
            if(i == argc - 1) goto help;
            N_THREADS = std::stoull(argv[i + 1]);
            ++i;
        }
        else
        {
        help:
            std::println("Use -j to specify the number of threads (default 4). -h or --help to see this message.");
            std::exit(0);
        }
    }
    std::println("Starting search. Num threads: {}", N_THREADS);
    signal(SIGINT, sigHndl);
    std::println("Reading the csv file...");
    std::chrono::high_resolution_clock clock;
    auto tbeg = clock.now();
    WordFreqMap words = load_csv("freqs.csv");
    auto tend = clock.now();
    auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(tend - tbeg);
    std::println("Done! Elapsed time: {} ms.", diff.count());

    sql::Database db("./keyboards.db", sql::OPEN_CREATE | sql::OPEN_READWRITE);
    db.exec(R"(
        CREATE TABLE IF NOT EXISTS keyboards (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            layout VARCHAR(50) NOT NULL,
            cost DECIMAL(5,9) NOT NULL
        )
    )");

    std::vector<std::jthread> threads;
    std::vector<BoolProxy> flags;
    threads.reserve(N_THREADS);
    flags.reserve(N_THREADS);
    for(std::size_t i = 0; i < N_THREADS; ++i)
    {
        flags.emplace_back(false);
        threads.emplace_back(hill_climb_racing, std::ref(words), std::ref(db), std::ref(flags[i]));
        std::println("Thread {} started working.", i);
    }

    while(run)
    {
        for(std::size_t i = 0; i < N_THREADS; ++i)
        {
            if(flags[i])
            {
                flags[i] = false;
                threads[i] = std::jthread{hill_climb_racing, std::ref(words), std::ref(db), std::ref(flags[i])};
                std::println("Thread {} finished its task. Attaching a new one...", i);
            }
        }
        std::this_thread::sleep_for(1s);
    }

    std::println("Exiting... Waiting for all tasks to finish.");

    return 0;
}
