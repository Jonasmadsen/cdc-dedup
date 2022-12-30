#include <iostream>
#include "Deduper.h"

int main(){
    std::string input = "../Google_tests/test_data/test.wmv";
    std::string res = "../res.csv";
    Deduper d(input,  res, 512,6,16,1024*1024);
    d.silent = false;
    d.dedupe();
}
