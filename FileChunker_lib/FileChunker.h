#ifndef DEDUPER_FILECHUNKER_H
#define DEDUPER_FILECHUNKER_H

#include <string>
#include <queue>
#include <fstream>
#include <chrono>

struct mt_task {
    long long offset;
    int data_size;
    char* data;
};

class FileChunker {

public:
    mt_task get_next_mt_task(bool silent);
    bool has_more() const;
    int amount_of_chunks() const;
    std::ifstream input;

    explicit FileChunker(const std::string& path, int mt_chunk_size, int window_size);
    ~FileChunker(){
        input.close();
    }

private:
    long long remaining_bytes;
    long long file_size;
    int window_size;
    char* overlap{};
    int mt_chunk_size;

    //For debug printing
    double total_megabytes_processed = 0.0;
    double megabytes_processed_this_round = 0.0;
    double progress = 0;
    std::chrono::time_point<std::chrono::high_resolution_clock> print_time;
    std::vector<double> speed_avg;

    void print_stats();
};


#endif //DEDUPER_FILECHUNKER_H
