#ifndef DEDUPER_DEDUPER_H
#define DEDUPER_DEDUPER_H

#include <string>
#include <mutex>
#include <atomic>
#include "FileChunker.h"

class Deduper {
private:
    std::string input;
    std::string output;
    std::mutex task_mutex; // mutex for accessing the task vector
    std::mutex result_mutex; // mutex for accessing the result vector
    std::vector<mt_task> tasks; // vector of tasks to be processed
    std::vector<long long> results; // vector to store the results
    std::atomic<bool> all_tasks_processed = false; // flag to indicate when all tasks have been processed
    std::vector<long long> get_splits(const mt_task &task);
    void compute();

public:
    int dedupe();
    bool silent = true;
    int difficulty;
    int max_threads;
    int window_size;
    int mt_chunk_size;
    [[nodiscard]] int get_found_splits() const;
    std::vector<long long> chunks;
    void write_to_file();
    Deduper(const std::string &input, const std::string &output, int difficulty, int max_threads, int window_size, int mt_chunk_size);
    Deduper(const std::string &input, const std::string &output) : Deduper(input, output, 256, 6, 3, 1024*1024){};
};


#endif //DEDUPER_DEDUPER_H
