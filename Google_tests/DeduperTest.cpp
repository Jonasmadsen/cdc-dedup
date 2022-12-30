#include <filesystem>
#include "gtest/gtest.h"
#include "../FileChunker_lib/Deduper.h"

std::string root = "../../Google_tests/";
std::string temp_folder = root + "temp/";
std::string test_data_folder = root + "test_data/";
std::string res1 = temp_folder + "res1.";
std::string res2 = temp_folder + "res2.";

bool compare_files(const std::string &file1, const std::string &file2) {
    std::ifstream f1(file1);
    std::ifstream f2(file2);
    std::string line1, line2;

    while (std::getline(f1, line1) && std::getline(f2, line2))
        if (line1 != line2){
            //std::cout << "Line1: " << line1 << std::endl << "Line2: " << line2 << std::endl;
            return false;
        }

    //There might be empty lines we need to finish off.
    while (std::getline(f1, line1))
        if (!line1.empty())
            return false;

    while (std::getline(f2, line2))
        if (!line2.empty())
            return false;

    return true;
}

// Function to remove a file
void remove_file(const std::string& path) {
    bool silent = true;
    // Remove the file
    if (std::remove(path.c_str()) == 0) {
        if (!silent)
            std::cout << "File removed successfully" << std::endl;
    } else {
        if (!silent)
            std::cerr << "Error removing file" << std::endl;
    }
}


//Deduping the same file twice should produce the same file
TEST(DeduperSuite, one){
    auto test_name = ::testing::UnitTest::GetInstance()->current_test_info()->name();
    std::string res_1 = res1 + test_name;
    std::string res_2 = res2 + test_name;
    remove_file(res_1);
    remove_file(res_2);
    std::string input = test_data_folder + "test.wmv";
    Deduper d(input,res_1);
    d.dedupe();
    d.write_to_file();
    Deduper dd(input,  res_2);
    dd.dedupe();
    dd.write_to_file();
    EXPECT_EQ(compare_files(res_1, res_2), true);
    EXPECT_EQ(d.get_found_splits(), 19550);
}

//Deduping The same file with different amount of threads should produce the same result
TEST(DeduperSuite, two){
    auto test_name = ::testing::UnitTest::GetInstance()->current_test_info()->name();
    std::string res_1 = res1 + test_name;
    std::string res_2 = res2 + test_name;
    remove_file(res_1);
    remove_file(res_2);
    std::string input = test_data_folder + "test.wmv";
    Deduper d(input,  res_1, 300,1,3,1024*1024);
    d.dedupe();
    d.write_to_file();
    Deduper dd(input,  res_2, 300,2,3,1024*1024);
    dd.dedupe();
    dd.write_to_file();
    EXPECT_EQ(compare_files(res_1, res_2), true);
    EXPECT_EQ(d.get_found_splits(), 43467);
}

TEST(DeduperSuite, three){
    auto test_name = ::testing::UnitTest::GetInstance()->current_test_info()->name();
    std::string res_1 = res1 + test_name;
    std::string res_2 = res2 + test_name;
    remove_file(res_1);
    remove_file(res_2);
    std::string input = test_data_folder + "test.jpg";
    Deduper d(input,  res_1, 128,2,4,512);
    d.dedupe();
    d.write_to_file();
    Deduper dd(input,  res_2, 128,2,4,1024);
    dd.dedupe();
    dd.write_to_file();
    EXPECT_EQ(compare_files(res_1, res_2), true);
    EXPECT_EQ(d.get_found_splits(), 4656);
}

TEST(DeduperSuite, four){
    auto test_name = ::testing::UnitTest::GetInstance()->current_test_info()->name();
    std::string res_1 = res1 + test_name;
    std::string res_2 = res2 + test_name;
    remove_file(res_1);
    remove_file(res_2);
    std::string input = test_data_folder + "test.jpg";
    Deduper d(input,  res_1, 128,1,4,1024*615);
    d.dedupe();
    d.write_to_file();
    Deduper dd(input,  res_2, 128,4,4,1024*1024);
    dd.dedupe();
    dd.write_to_file();
    EXPECT_EQ(compare_files(res_1, res_2), true);
    EXPECT_EQ(d.get_found_splits(), 4656);
}

TEST(DeduperSuite, five){
    auto test_name = ::testing::UnitTest::GetInstance()->current_test_info()->name();
    std::string res_1 = res1 + test_name;
    std::string res_2 = res2 + test_name;
    remove_file(res_1);
    remove_file(res_2);
    std::string input = test_data_folder + "test.wmv";
    Deduper d(input,  res_1, 201,2,5,1023*1024);
    d.dedupe();
    d.write_to_file();
    Deduper dd(input,  res_2, 201,4,5,1024*1024);
    dd.dedupe();
    dd.write_to_file();
    EXPECT_EQ(compare_files(res_1, res_2), true);
    EXPECT_EQ(d.get_found_splits(), 65251);
}

TEST(DeduperSuite, six){
    auto test_name = ::testing::UnitTest::GetInstance()->current_test_info()->name();
    std::string res_1 = res1 + test_name;
    std::string res_2 = res2 + test_name;
    remove_file(res_1);
    remove_file(res_2);
    std::string input = test_data_folder + "test.jpg";
    Deduper d(input,  res_1, 256,2,10,1024);
    d.dedupe();
    d.write_to_file();
    Deduper dd(input,  res_2, 256,4,10,1024*1024);
    dd.dedupe();
    dd.write_to_file();
    EXPECT_EQ(compare_files(res_1, res_2), true);
    EXPECT_EQ(d.get_found_splits(), 2104);
}

TEST(DeduperSuite, seven){
    auto test_name = ::testing::UnitTest::GetInstance()->current_test_info()->name();
    std::string res_1 = res1 + test_name;
    std::string res_2 = res2 + test_name;
    remove_file(res_1);
    remove_file(res_2);
    std::string input = test_data_folder + "test.jpg";
    Deduper d(input,  res_1, 256,1,12,511);
    d.dedupe();
    d.write_to_file();
    Deduper dd(input,  res_2, 256,4,12,1025);
    dd.dedupe();
    dd.write_to_file();
    EXPECT_EQ(compare_files(res_1, res_2), true);
    EXPECT_EQ(d.get_found_splits(), 2200);
}

TEST(DeduperSuite, eight){
    auto test_name = ::testing::UnitTest::GetInstance()->current_test_info()->name();
    std::string res_1 = res1 + test_name;
    std::string res_2 = res2 + test_name;
    remove_file(res_1);
    remove_file(res_2);
    std::string input = test_data_folder + "test.jpg";
    Deduper d(input,  res_1, 249,2,3,700);
    d.dedupe();
    d.write_to_file();
    Deduper dd(input,  res_2, 249,4,3,5000);
    dd.dedupe();
    dd.write_to_file();
    EXPECT_EQ(compare_files(res_1, res_2), true);
    EXPECT_EQ(d.get_found_splits(), 3008);
}

TEST(DeduperSuite, nine){
    auto test_name = ::testing::UnitTest::GetInstance()->current_test_info()->name();
    std::string res_1 = res1 + test_name;
    std::string res_2 = res2 + test_name;
    remove_file(res_1);
    remove_file(res_2);
    std::string input = test_data_folder + "test.bin";
    Deduper d(input,  res_1, 250,8,3,125);
    d.dedupe();
    d.write_to_file();
    Deduper dd(input,  res_2, 250,9,3,125);
    dd.dedupe();
    dd.write_to_file();
    EXPECT_EQ(compare_files(res_1, res_2), true);
    EXPECT_EQ(d.get_found_splits(), 5);
}

TEST(DeduperSuite, ten){
    auto test_name = ::testing::UnitTest::GetInstance()->current_test_info()->name();
    std::string res_1 = res1 + test_name;
    std::string res_2 = res2 + test_name;
    remove_file(res_1);
    remove_file(res_2);
    std::string input = test_data_folder + "test.bin";
    Deduper d(input,  res_1, 16,1,3,1024);
    d.dedupe();
    d.write_to_file();
    Deduper dd(input,  res_2, 16,3,3,152);
    dd.dedupe();
    dd.write_to_file();
    EXPECT_EQ(compare_files(res_1, res_2), true);
    EXPECT_EQ(d.get_found_splits(), 65);
}

TEST(DeduperSuite, eleven){
    auto test_name = ::testing::UnitTest::GetInstance()->current_test_info()->name();
    std::string res_1 = res1 + test_name;
    std::string res_2 = res2 + test_name;
    remove_file(res_1);
    remove_file(res_2);
    std::string input = test_data_folder + "test.wmv";
    Deduper d(input,  res_1, 512,6,10,1024*1024*2);
    d.dedupe();
    d.write_to_file();
    Deduper dd(input,  res_2, 512,3,10,1024*1024*3);
    dd.dedupe();
    dd.write_to_file();
    EXPECT_EQ(compare_files(res_1, res_2), true);
    EXPECT_EQ(d.get_found_splits(), 6845);
}

TEST(DeduperSuite, twelve){
    auto test_name = ::testing::UnitTest::GetInstance()->current_test_info()->name();
    std::string res_1 = res1 + test_name;
    std::string res_2 = res2 + test_name;
    remove_file(res_1);
    remove_file(res_2);
    std::string input = test_data_folder + "test.jpg";
    Deduper d(input,  res_1, 64,2,2,512);
    d.dedupe();
    d.write_to_file();
    Deduper dd(input,  res_2, 64,4,2,512);
    dd.dedupe();
    dd.write_to_file();
    EXPECT_EQ(compare_files(res_1, res_2), true);
    EXPECT_EQ(d.get_found_splits(), 9938);
}

TEST(DeduperSuite, thirteen){
    auto test_name = ::testing::UnitTest::GetInstance()->current_test_info()->name();
    std::string res_1 = res1 + test_name;
    std::string res_2 = res2 + test_name;
    remove_file(res_1);
    remove_file(res_2);
    std::string input = test_data_folder + "test.jpg";
    Deduper d(input,  res_1, 64,1,2,512);
    d.dedupe();
    d.write_to_file();
    Deduper dd(input,  res_2, 64,4,2,1024);
    dd.dedupe();
    dd.write_to_file();
    EXPECT_EQ(compare_files(res_1, res_2), true);
    EXPECT_EQ(d.get_found_splits(), 9938);
}

TEST(DeduperSuite, fourteen){
    auto test_name = ::testing::UnitTest::GetInstance()->current_test_info()->name();
    std::string res_1 = res1 + test_name;
    std::string res_2 = res2 + test_name;
    remove_file(res_1);
    remove_file(res_2);
    std::string input = test_data_folder + "test.bin";
    Deduper d(input,  res_1, 64,1,10,1024);
    d.dedupe();
    d.write_to_file();
    Deduper dd(input,  res_2, 25,4,6,1024);
    dd.dedupe();
    dd.write_to_file();
    EXPECT_EQ(compare_files(res_1, res_2), false);
    EXPECT_EQ(d.get_found_splits(), 23);
}

TEST(DeduperSuite, fifteen){
    auto test_name = ::testing::UnitTest::GetInstance()->current_test_info()->name();
    std::string res_1 = res1 + test_name;
    std::string res_2 = res2 + test_name;
    remove_file(res_1);
    remove_file(res_2);
    std::string input = test_data_folder + "test.jpg";
    Deduper d(input,  res_1, 128,1,10,1024);
    d.dedupe();
    d.write_to_file();
    Deduper dd(input,  res_2, 64,4,10,1024);
    dd.dedupe();
    dd.write_to_file();
    EXPECT_EQ(compare_files(res_1, res_2), false);
    EXPECT_EQ(d.get_found_splits(), 5087);
}

TEST(DeduperSuite, sixteen){
    auto test_name = ::testing::UnitTest::GetInstance()->current_test_info()->name();
    std::string res_1 = res1 + test_name;
    std::string res_2 = res2 + test_name;
    remove_file(res_1);
    remove_file(res_2);
    std::string input = test_data_folder + "test.jpg";
    Deduper d(input,  res_1);
    d.mt_chunk_size = 105;
    d.window_size = 15;
    d.max_threads = 8;
    d.difficulty = 2;
    d.dedupe();
    d.write_to_file();
    Deduper dd(input,  res_2);
    dd.mt_chunk_size = 102;
    dd.window_size = 16;
    dd.max_threads = 8;
    dd.difficulty = 2;
    dd.dedupe();
    dd.write_to_file();
    EXPECT_EQ(compare_files(res_1, res_2), false);
    EXPECT_EQ(d.get_found_splits(), 494204);
}