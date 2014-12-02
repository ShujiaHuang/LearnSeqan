#include <iostream>
#include <seqan/sequence.h>
#include <seqan/index.h>

using namespace seqan;

void basic() {

    String<Dna> text = "ACGTTTGACAGCT";
    Index<String<Dna>, IndexEsa<> > index(text);

    StringSet<String<Dna> > stringSet;
    appendValue(stringSet, "ACGTCATCAT");
    appendValue(stringSet, "ACTTTG");
    appendValue(stringSet, "CACCCCCCTATTT");

    Index<StringSet<String<Dna> >, IndexEsa<> > indexSet(stringSet);

    return;
}

void testFinder() {

    String<Dna5> genome = "ACGTACGTACGTN";//"TTATTAAGCGTATAGCCCTATAAATATAA";
    Index<String<Dna5>, IndexEsa<> > esaIndex(genome);
    Finder<Index<String<Dna5>, IndexEsa<> > > esaFinder(esaIndex);

    while (find(esaFinder, "ACGT")) {
    //while(find(esaFinder, "TATAA")) {
        std::cout << position(esaFinder) << "\n";
    }

    clear(esaFinder);
    while (find(esaFinder, "ACGT")) {
        std::cout << position(esaFinder) << "\n";
    }
}

int main() {

    basic();
    testFinder();
    return 0;
}
