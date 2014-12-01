#include <iostream>
#include <time.h>

#include <seqan/basic.h>
#include <seqan/sequence.h>
#include <seqan/file.h>

using namespace seqan;

///////////////////////////////////////////////////////////////////////////////
void BasicTest() {

    seqan::String<seqan::Dna5String> dnaSeq = "TAtaN";
    seqan::append(dnaSeq, "CGCG");
    seqan::appendValue(dnaSeq, "cct");
    std::cout << dnaSeq[0] << "\n" << seqan::length(dnaSeq) << std::endl;

    seqan::String<seqan::Dna5String> readList;
    seqan::resize(readList, 2);
    readList[0] = "GGTTTCGACG";
    readList[1] = "AAGATGTCGC";
    seqan::appendValue(readList, "TATGCATGAT");

    std::cout << seqan::length(readList) << ":" << readList[1] << std::endl;
    std::cout << seqan::length(readList[0]) << std::endl;
}

///////////////////////////////////////////////////////////////////////////////
Dna GetRevCompl(Dna const &nucleotide) {

    if (nucleotide == 'A') return (Dna)'T';
    if (nucleotide == 'T') return (Dna)'A';
    if (nucleotide == 'C') return (Dna)'G';
    if (nucleotide == 'N') return (Dna)'N';

    return (Dna)'C';
}
void TestRevCompl() {

    Dna5String genome = "TATATACGCGCGAGTCGTNN";
    Dna5String revComplGenome;

    resize(revComplGenome, length(genome));
    for (size_t i(0); i < length(genome); ++i) {

        revComplGenome[length(genome) - 1 - i] = GetRevCompl(genome[i]);
    }
    std::cout << genome << std::endl;
    std::cout << revComplGenome << std::endl; // Not right, cause by all the 'N' convert to 'A' by the 'Dna' data type.

    std::cout << "Before Rever: " << genome << "\n";
    reverseComplement(genome);
    std::cout << "After Rever : " << genome << std::endl; // Always right
}

///////////////////////////////////////////////////////////////////////////////
// Function to print simple alignment between two sequences with the same length
template <typename TText1, typename TText2>
void printAlign(TText1 const & genomeFragment, TText2 const & read) {
        std::cout <<  "Alignment " << std::endl;
        std::cout << "  genome : " << genomeFragment << std::endl;
        std::cout << "  read   : " << read << std::endl;
}
void Test_BS() {
    // Build reads and genomes
    DnaString chr1 = "TATAATATTGCTATCGCGATATCGCTAGCTAGCTACGGATTATGCGCTCTGCGATATATCGCGCTAGATGTGCAGCTCGATCGAATGCACGTGTGTGCGATCGATTAGCGTCGATCATCGATCTATATTAGCGCGCGGTATCGGACGATCATATTAGCGGTCTAGCATTTAG";
    // Build List containing all reads
    typedef String<DnaString> TDnaList;
    TDnaList readList;
    resize(readList, 4);
    readList[0] = "TTGCTATCGCGATATCGCTAGCTAGCTACGGATTATGCGCTCTGCGATATATCGCGCT";
    readList[1] = "TCGATTAGCGTCGATCATCGATCTATATTAGCGCGCGGTATCGGACGATCATATTAGCGGTCTAGCATT";
    readList[2] = "AGCCTGCGTACGTTGCAGTGCGTGCGTAGACTGTTGCAAGCCGGGGGTTCATGTGCGCTGAAGCACACATGCACA";
    readList[3] = "CGTGCACTGCTGACGTCGTGGTTGTCACATCGTCGTGCGTGCGTACTGCTGCTGACA";
    // Append a second chromosome sequence fragment to chr1
    DnaString chr2 = "AGCCTGCGTACGTTGCAGTGCGTGCGTAGACTGTTGCAAGCCGGGGGTTCATGTGCGCTGAAGCACACATGCACACGTCTCTGTGTTCCGACGTGTGTCACGTGCACTGCTGACGTCGTGGTTGTCACATCGTCGTGCGTGCGTACTGCTGCTGACACATGCTGCTG";
    append(chr1, chr2);
    // Print readlist
    std::cout << " \n Read list: " << std::endl;
    for(unsigned i = 0; i < length(readList); ++i)
        std::cout << readList[i] << std::endl;
    // Assume we have mapped the 4 reads to chr1 (and chr2) and now have the mapping start positions (no gaps).
    // Store the start position in a String alignPosList: 7, 100, 172, 272
    String<unsigned> alignPosList;
    resize(alignPosList, 4);
    alignPosList[0] = 7;
    alignPosList[1] = 100;
    alignPosList[2] = 172;
    alignPosList[3] = 272;
    // Optional
    // Bisulfite conversion
    // Assume chr1 is beeing bisulfate treated: Copy chr1 to a new genome bsChr1 and exchange every 'C' with a 'T'
    DnaString bsChr1;;
    assign(bsChr1, chr1);
    for(unsigned i = 0; i < length(bsChr1); ++i)
        if(bsChr1[i] == 'C')
            bsChr1[i] = 'T';
    // Print alignments of the reads with chr1 (or bsChr1) sequence using the function printAlign
    // and the positions in alignPosList.
    // To do that, you have to create a copy of the fragment in chr1 (bsChr1) that is aligned to the read.
    std::cout << " \n Print alignment: " << std::endl;
    for(unsigned i = 0; i < length(readList); ++i)
    {
        // Begin position beginPosition of a given alignment between the read and the genome
        unsigned beginPosition = alignPosList[i];
        // Genome fragment
        DnaString genomeFragment;
        // We have to create a copy of the corresponding fragment of the genome, where the read aligns to
        for (unsigned j = 0; j < length(readList[i]); ++j)
            appendValue(genomeFragment, chr1[beginPosition+j]);
        // Call of our function to print the simple alignment
        printAlign(genomeFragment, readList[i]);
    }
}


///////////////////////////////////////////////////////////////////////////////
void Test_Conversions() {

    String<char> source = "acgtgcat";
    String<Dna> target;
    //assign(target, source);
    std::cout << "Before Move: " << source << "\n";
    move(target, source);
    std::cout << target << "\n" << length(source) << "\n";
}

///////////////////////////////////////////////////////////////////////////////

void SeqInDepth() {

    String<Dna> dnaSeq;
    // Sets the capacity of dnaSeq to 5.
    resize(dnaSeq, 4, Exact());
    // Only "TATA" is assigned to dnaSeq, since dnaSeq is limited to 4.
    assign(dnaSeq, "TATAGGGG", Limit());
    std::cout << dnaSeq << "\n";
    // Use the default expansion strategy.
    append(dnaSeq, "GCGCGC");
    std::cout << dnaSeq << "\n";
 
    //////////////////////////////
    std::cout << "\n** New Example **\n";
    unsigned num = 1000000;
    
    time_t start;
    String<Dna> str;
    clear(str);
    start = time(NULL);
    for (size_t i(0); i < num; ++i) {
        appendValue(str, 'A', Exact());
    }
    std::cout << "Strategy Exact() took: " << time(NULL) - start << "s \n\n";
    clear(str);

    start = time(NULL);
    for (unsigned i(0); i < num; ++i) {
        appendValue(str, 'A', Generous());
        //appendValue(str, 'A');
    }
    std::cout << "Strategy Generous() took: " << time(NULL) - start << "s\n\n";
   
    return;
}

///////////////////////////////////////////////////////////////////////////////
int main() {

    //BasicTest();
    //TestRevCompl();
    //Test_BS();
    //Test_Conversions();
    SeqInDepth();
    return 0;
}
