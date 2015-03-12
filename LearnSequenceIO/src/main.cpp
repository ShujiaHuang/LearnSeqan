#include <iostream>
#include <seqan/sequence.h>
#include <seqan/seq_io.h>

using namespace seqan;

void basic(int argc, char const **argv) {

    CharString id;
    Dna5String seq;
    
    SequenceStream seqStream(argv[1]);
    readRecord(id, seq, seqStream);

    std::cout << "\n>" << id << "\n\n" << seq << "\n";

    return;
}

int main(int argc, char const **argv) {

    if (argc < 2) {

        std::cerr << "USAGE: basic_seq_io_example FILENAME\n";
        return 1;
    }

    basic(argc, argv);
    return 0;
}
