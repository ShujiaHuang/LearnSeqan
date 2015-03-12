#include <iostream>
#include <seqan/sequence.h>  // CharString, ...
#include <seqan/stream.h>    // to stream a CharString into cout

using namespace seqan;

int main(int, char const **) {
    std::cout << "Hello World!" << std::endl;
    CharString mySeqAnString = "Hello SeqAn!";
    std::cout << mySeqAnString << std::endl;
    return 1;
}
