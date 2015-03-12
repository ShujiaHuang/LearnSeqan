#include <iostream>
#include <seqan/sequence.h>
#include <seqan/seq_io.h>
#include <seqan/bam_io.h>

using namespace seqan;

void bamIO(int argc, char const **argv) {

    // Open input file, BamFileIn can read SAM and BAM files.
    BamFileIn bamFileIn(argv[1]);

    // Open output file, BamFileOut accepts also an ostream and a format tag.
    BamFileOut bamFileOut(std::cout, Sam());
    
    // Copy header
    BamHeader header;
    readHeader(header, bamFileIn);
    writeHeader(bamFileOut, header);
    
    // copy records
    BamAlignmentRecord record;
    while (!atEnd(bamFileIn)) {
        readRecord(record, bamFileIn);
        writeRecord(bamFileOut, record);
    }

    return;
}

int main(int argc, char const **argv) {

    if (argc < 2) {

        std::cerr << "USAGE: basic_seq_io_example FILENAME\n";
        return 1;
    }

    bamIO(argc, argv);
    return 0;
}
