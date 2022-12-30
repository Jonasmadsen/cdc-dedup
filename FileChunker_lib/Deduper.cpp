#include <mutex>
#include <condition_variable>
#include <iostream>
#include "Deduper.h"
#include "FileChunker.h"

uint32_t fletcher32(const char* data, size_t length) {
    uint32_t sum1 = 0xffff, sum2 = 0xffff;

    while (length) {
        size_t tlen = length > 360 ? 360 : length;
        length -= tlen;

        do {
            sum2 += sum1 += *data++;
        } while (--tlen);

        sum1 = (sum1 & 0xffff) + (sum1 >> 16);
        sum2 = (sum2 & 0xffff) + (sum2 >> 16);
    }

/* Second reduction step to reduce sums to 16 bits */
    sum1 = (sum1 & 0xffff) + (sum1 >> 16);
    sum2 = (sum2 & 0xffff) + (sum2 >> 16);
    return sum2 << 16 | sum1;
}

std::vector<long long> Deduper::get_splits(const mt_task& task){
    char window[window_size];
    int added = 0;
    std::vector<long long> splits;
    for (int i = 0; i < task.data_size; ++i) {
        if (added != window_size){
            window[added++] = task.data[i];
            continue;
        }
        // check if we split
        auto hash = fletcher32(window, window_size);
        //If the hash is difficult we find it to be worthy
        if (hash % difficulty == 0){
            splits.push_back(task.offset + i);
        }
        //Shift array to right
        for (int j = 0; j < window_size - 1; ++j)
            window[j] = window[j+1];
        //Add new char at end
        window[window_size-1] = task.data[i];
    }
    return splits;
}

void Deduper::compute() {
    while (true) {
        mt_task task;
        {
            std::lock_guard<std::mutex> task_lock(task_mutex);
            if (tasks.empty() && Deduper::all_tasks_processed) return; // no more tasks to process
            if (tasks.empty()) continue; // wait for more tasks to be added
            task = tasks.back();
            tasks.pop_back();
        }
        std::vector<long long> result = get_splits(task);
        if (!silent and !result.empty())
            std::cout << "Found: " << result.size() << " from offset: " << task.offset << std::endl;
        std::lock_guard<std::mutex> result_lock(result_mutex);
        for (long long i: result)
            results.push_back(i);
    }
}


int Deduper::dedupe(){

    //Make a FileChunk object used by all threads.
    FileChunker f = FileChunker(input, Deduper::mt_chunk_size, Deduper::window_size);

    //Ideally we launch the maximum amount of threads requested
    int threads_to_launch = Deduper::max_threads;
    //If there exists fewer chunks than threads we lower our thread count to match the amount of chunks
    if (f.amount_of_chunks() < threads_to_launch){
        threads_to_launch = f.amount_of_chunks();
        //Cannot have 0 threads
        if (threads_to_launch == 0)
            threads_to_launch = 1;
        if (Deduper::max_threads != threads_to_launch && !silent){
            std::cout << "Had lower amount of threads from: " << Deduper::max_threads;
            std::cout << " to: " << threads_to_launch << std::endl;
        }
    }

    std::vector<std::thread> workers; // vector to store the worker threads

    // Create and start the worker threads
    for (int i = 0; i < threads_to_launch; i++) {
        workers.emplace_back(&Deduper::compute, this);
    }

    // Add the tasks to the task vector
    bool sleep_a_little = false;
    while (f.has_more()){
        if (sleep_a_little){
            std::this_thread::yield();
            sleep_a_little = false;
        }

        std::lock_guard<std::mutex> task_lock(task_mutex);
        if (tasks.size() > threads_to_launch){
            sleep_a_little = true;
            continue;
        }else
            tasks.push_back(f.get_next_mt_task(silent));
    }

    //We wait until all tasks are finished
    while(!tasks.empty())
        std::this_thread::yield();

    if (!silent)
        std::cout << "all tasks have been finished" << std::endl;
    all_tasks_processed = true; // set the flag to indicate that all tasks have been added

    // Wait for all the worker threads to finish
    for (std::thread& t : workers){
        if (!silent)
            std::cout << "Waiting for all threads to finish" << std::endl;
        t.join();
    }

    //Clean output for final chunks vector
    if (!silent)
        std::cout << "Sorting: " << results.size() << " values" << std::endl;
    sort(results.begin(), results.end());
    int count = 0;
    int biggest = 0;
    long long last_val = 0;
    int size;
    for (long long i : results) {
        //If the new value appears to soon we don't include it, this removes all the small chunks and duplicates.
        if (i - last_val < difficulty)
            continue;
        chunks.push_back(i);
        size = int(i - last_val);
        if (size > biggest)
            biggest = size;
        last_val = i;
        count++;
    }

    if (!silent){
        std::cout << "amount of splits: " << count << std::endl;
        std::cout << "biggest: " << biggest << std::endl;
    }
    f.input.close();
    return 1;
}

int Deduper::get_found_splits() const{
    return int(chunks.size());
}

void Deduper::write_to_file() {
    std::ofstream res_file;
    res_file.open (output);
    res_file << "index,value,size" << std::endl;
    int count = 0;
    int size;
    long long last_val = 0;
    for (long long i : chunks){
        size = int(i - last_val);
        res_file << count << "," << i << "," << size << std::endl;
        count++;
        last_val = i;
    }
    res_file.close();
}

Deduper::Deduper(const std::string &input, const std::string &output, int difficulty, int max_threads, int window_size, int mt_chunk_size) {
    Deduper::mt_chunk_size = mt_chunk_size;
    Deduper::window_size = window_size;
    Deduper::max_threads = max_threads;
    Deduper::difficulty = difficulty;
    Deduper::input = input;
    Deduper::output = output;
}
