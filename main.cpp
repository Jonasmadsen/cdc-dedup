#include <iostream>
#include "Deduper.h"

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cout << "Usage: " << argv[0] << " <input_file> <output_file>" << std::endl;
        return 1;
    }

    std::string input = argv[1];
    std::string res = argv[2];
    Deduper d(input, res, 512, 12, 16, 1024*1024);
    d.silent = false;
    d.dedupe();
    d.write_to_file();
    std::cout << "Finished";
    return 0;
}
