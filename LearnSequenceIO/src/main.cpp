#include <iostream>
#include <seqan/sequence.h>
#include <seqan/seq_io.h>
#include <seqan/bam_io.h>

using namespace seqan;

int fastaIO(int argc, char const **argv) {

    if (argc != 2)
    {
        std::cerr << "USAGE: build_fai FILE.fa\n" << argc;
        return 1;
    }

    FaiIndex faiIndex;
    if (!build(faiIndex, argv[1]))
    {
        std::cerr << "ERROR: Could not build FAI index for file " << argv[1] << ".\n";
        return 1;
    }

    CharString faiFilename = argv[1];
    append(faiFilename, ".fai");

    if (!save(faiIndex, toCString(faiFilename)))
    {
        std::cerr << "ERROR: Could not write the index to file!\n";
        return 1;
    }

    std::cout << "Index file " << faiFilename << " was successfully created.\n";
    return 0;

}

void baseIO(int argc, char const **argv) {

    if (argc < 3)
    {
        std::cerr << "USAGE: " << argv[0] << " INPUT.bam OUTPUT.sam" << "\n";
        return;
    }

    // Open input BAM file.
    BamFileIn bamFileIn;
    if (!open(bamFileIn, argv[1]))
    {
        std::cerr << "ERROR: could not open input file " << argv[1] << ".\n";
        return;
    }

    // Open output SAM file.
    BamFileOut samFileOut;
    if (!open(samFileOut, argv[2]))
    {
        std::cerr << "ERROR: could not open output file " << argv[2] << ".\n";
        return;
    }

    // Copy header.
    BamHeader header;
    try
    {
        readHeader(header, bamFileIn);
        writeHeader(samFileOut, header);
    }
    catch (IOError const & e)
    {
        std::cerr << "ERROR: could not copy header. " << e.what() << "\n";
    }

    // Copy all records.
    BamAlignmentRecord record;
    while (!atEnd(bamFileIn))
    {
        try
        {
            readHeader(header, bamFileIn);
            writeRecord(samFileOut, record);
        }
        catch (IOError const & e)
        {
            std::cerr << "ERROR: could not copy record. " << e.what() << "\n";
        }
    }
}

void bamIO(int argc, char const **argv) {

    if (argc < 2) {
        std::cerr << "USAGE: basic_seq_io_example FILENAME\n";
        return;
    }

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

    bamIO(argc, argv);
    //baseIO(argc, argv);
    //fastaIO(argc, argv);
    return 0;
}
