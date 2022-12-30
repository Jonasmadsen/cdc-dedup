#include "gtest/gtest.h"
#include "../FileChunker_lib/FileChunker.h"

TEST(FileChunkerSuite, one){
    std::string root = "../../Google_tests/";
    std::string temp_folder = root + "temp/";
    std::string test_data_folder = root + "test_data/";
    std::string input = test_data_folder + "test.wmv";
    FileChunker f(input, 1024, 16);
    int count = 0;
    while (f.has_more()){
        f.get_next_mt_task(true);
        count++;
    }
    EXPECT_EQ(count,25631);
}

TEST(FileChunkerSuite, two){
    std::string root = "../../Google_tests/";
    std::string temp_folder = root + "temp/";
    std::string test_data_folder = root + "test_data/";
    std::string input = test_data_folder + "test.jpg";
    FileChunker f(input, 1024, 16);
    int count = 0;
    while (f.has_more()){
        f.get_next_mt_task(true);
        count++;
    }
    EXPECT_EQ(count,1451);
}

TEST(FileChunkerSuite, three){
    std::string root = "../../Google_tests/";
    std::string temp_folder = root + "temp/";
    std::string test_data_folder = root + "test_data/";
    std::string input = test_data_folder + "test.jpg";
    FileChunker f(input, 1024, 16);
    int count = 0;
    while (f.has_more()){
        f.get_next_mt_task(true);
        count++;
    }
    FileChunker ff(input, 1024, 16);
    int count1 = 0;
    while (ff.has_more()){
        ff.get_next_mt_task(true);
        count1++;
    }
    EXPECT_EQ(count,count1);
}

TEST(FileChunkerSuite, four){
    std::string root = "../../Google_tests/";
    std::string temp_folder = root + "temp/";
    std::string test_data_folder = root + "test_data/";
    std::string input = test_data_folder + "test.jpg";
    FileChunker f(input, 1024, 16);
    int count = 0;
    while (f.has_more()){
        f.get_next_mt_task(true);
        count++;
    }
    FileChunker ff(input, 1024, 140);
    int count1 = 0;
    while (ff.has_more()){
        ff.get_next_mt_task(true);
        count1++;
    }
    EXPECT_EQ(count,count1);
}

TEST(FileChunkerSuite, five){
    std::string root = "../../Google_tests/";
    std::string temp_folder = root + "temp/";
    std::string test_data_folder = root + "test_data/";
    std::string input = test_data_folder + "test.jpg";
    FileChunker f(input, 512, 1024);
    int count = 0;
    while (f.has_more()){
        f.get_next_mt_task(true);
        count++;
    }
    FileChunker ff(input, 1024, 140);
    int count1 = 0;
    while (ff.has_more()){
        ff.get_next_mt_task(true);
        count1++;
    }
    EXPECT_NE(count,count1);
}

TEST(FileChunkerSuite, six){
    std::string root = "../../Google_tests/";
    std::string temp_folder = root + "temp/";
    std::string test_data_folder = root + "test_data/";
    std::string input = test_data_folder + "test.bin";
    FileChunker f(input, 255, 16);
    int count = 0;
    while (f.has_more()){
        f.get_next_mt_task(true);
        count++;
    }
    EXPECT_EQ(count,10);
}

TEST(FileChunkerSuite, seven){
    std::string root = "../../Google_tests/";
    std::string temp_folder = root + "temp/";
    std::string test_data_folder = root + "test_data/";
    std::string input = test_data_folder + "test.bin";
    FileChunker f(input, 159, 16);
    int count = 0;
    while (f.has_more()){
        f.get_next_mt_task(true);
        count++;
    }
    EXPECT_EQ(count,16);
}

TEST(FileChunkerSuite, eight){
    std::string root = "../../Google_tests/";
    std::string temp_folder = root + "temp/";
    std::string test_data_folder = root + "test_data/";
    std::string input = test_data_folder + "test.jpg";
    FileChunker f(input, 159, 16);
    long long last_offset = -1;
    while (f.has_more()){
        mt_task task = f.get_next_mt_task(true);
        EXPECT_GT(task.offset,last_offset);
        EXPECT_GE(task.offset,0);
        last_offset = task.offset;
    }
}