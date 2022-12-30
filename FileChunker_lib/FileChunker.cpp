#include <iostream>
#include <string>
#include <chrono>
#include "FileChunker.h"


FileChunker::FileChunker(const std::string &path, int mt_chunk_size, int window_size) {
    FileChunker::window_size = window_size;
    FileChunker::mt_chunk_size = mt_chunk_size;
    overlap = new char[window_size*2];
    std::ifstream in_file(path, std::ios::binary);
    in_file.seekg(0, std::ios::end);
    file_size = in_file.tellg();
    in_file.close();
    remaining_bytes = file_size;
    input = std::ifstream( path, std::ios::binary );
    print_time = std::chrono::high_resolution_clock::now();
}

mt_task FileChunker::get_next_mt_task(bool silent) {
    if (remaining_bytes == 0)
        return {};

    mt_task task{};
    task.offset = file_size - remaining_bytes;
    int overlap_size = window_size * 2;

    //If it is the first task
    if (task.offset == 0){
        //if we have less data than a full mt_chunk we only allocate for what we need
        if(mt_chunk_size > remaining_bytes){
            task.data_size = int(remaining_bytes);
        } else {
            task.data_size = mt_chunk_size;
        }

        task.data = new char[task.data_size];
        remaining_bytes -= task.data_size;

        // We only get data from file
        input.read(task.data, task.data_size);
        // and fill the overlap for next time
        memcpy(overlap, task.data + task.data_size - overlap_size, overlap_size);
        megabytes_processed_this_round = double(task.data_size) / 1024 / 1024;
    } else {
        if(mt_chunk_size > remaining_bytes){
            task.data_size = int(remaining_bytes) + overlap_size;
            remaining_bytes -= remaining_bytes;
        } else {
            task.data_size = mt_chunk_size + overlap_size;
            remaining_bytes -= mt_chunk_size;
        }
        task.data = new char[task.data_size];

        // Copy the overlap data into the beginning of the data field
        memcpy(task.data, overlap, overlap_size);
        //We just moved the offset by overlap_size and we must reflect this
        task.offset -= overlap_size;
        // Read the rest of the data from the input file
        input.read(task.data + overlap_size,task.data_size - overlap_size);
        // Update the overlap for the next mt_task
        memcpy(overlap, task.data + task.data_size - overlap_size, overlap_size);
        megabytes_processed_this_round = double(task.data_size - overlap_size) / 1024 / 1024;
    }

    total_megabytes_processed += megabytes_processed_this_round;

    if (!silent)
        print_stats();
    return task;
}


bool FileChunker::has_more() const {
    return remaining_bytes != 0;
}

void FileChunker::print_stats() {

    progress = double(file_size - remaining_bytes)/double(file_size)*100;
    double millies_since_last = double(duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - print_time).count());
    speed_avg.push_back(megabytes_processed_this_round / double(millies_since_last / 1000.0));
    double total = 0.0;

    if (speed_avg.size() > 20)
        speed_avg.erase(speed_avg.begin());
    for(double i : speed_avg)
        total += i;

    print_time = std::chrono::high_resolution_clock::now();
    std::cout << "Progress: " << FileChunker::progress << "% ";
    std::cout << "Speed: " << total/double(speed_avg.size()) << " MB/s ";
    std::cout << "Data processed this round: " << megabytes_processed_this_round << " MB" << std::endl;
    std::cout << "Data total: " <<  float(total_megabytes_processed) << "/" << float(file_size) / 1024 / 1024 << " MB" << std::endl;
}

int FileChunker::amount_of_chunks() const {
    return int(remaining_bytes / mt_chunk_size);
}
