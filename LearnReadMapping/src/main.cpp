#include <iostream>
#include <cstdio>

#include <seqan/basic.h>
#include <seqan/file.h>
#include <seqan/index.h>
#include <seqan/store.h>

using namespace seqan;

void miniMapping(int argc, char const **argv) {
    
    // Some type
    typedef FragmentStore<>::TReadSeqStore TReadSeqStore;
    typedef Value<TReadSeqStore>::Type TReadSeq;
    typedef FragmentStore<>::TContigStore TContigStore;
    typedef Value<TContigStore>::Type TContigStoreElement;
    typedef TContigStoreElement::TContigSeq TContigSeq;
    typedef Index<TReadSeqStore, 
                  IndexQGram<Shape<Dna, UngappedShape<11> >, OpenAddressing> 
                 > TIndex;
    typedef Pattern<TIndex, Swift<SwiftSemiGlobal> > TPattern;
    typedef Finder<TContigSeq, Swift<SwiftSemiGlobal> > TFinder;
    typedef FragmentStore<>::TAlignedReadStore TAlignedReadStore;
    typedef Value<TAlignedReadStore>::Type TAlignedRead;

    const double EPSILON = 0.08;

    if (argc < 3) {
        std::cerr << "Invalid number of arguments.\n"
                  << "Usage: " << argv[0] << " " 
                  << "GENOME.fasta READS.fasta OUT.sam\n";
        return;
    }

    // 1) Load contigs and reads.
    FragmentStore<> fragStore;
    if (!loadContigs(fragStore, argv[1])) return;
    if (!loadReads(fragStore, argv[2])) return;

    // 2) Build an index over all reads and s SWIFT pattern over this index
    TIndex index(fragStore.readSeqStore);
    TPattern pattern(index);
    
    // 3) Enumerate all epsilon matches.
    for (unsigned i(0); i < length(fragStore.contigStore); ++i) {
        TFinder finder(fragStore.contigStore[i].seq);
        while (find(finder, pattern, EPSILON)) {
            // Verify match
            Finder<TContigSeq> verifyFinder(fragStore.contigStore[i].seq);
            setPosition(verifyFinder, beginPosition(finder));
            Pattern<TReadSeq, HammingSimple> 
                verifyPattern(fragStore.readSeqStore[position(pattern).i1]);
            unsigned readLength = 
                length(fragStore.readSeqStore[position(pattern).i1]);
            int minScore = -static_cast<int>(EPSILON * readLength);
            while (find(verifyFinder, verifyPattern, minScore) &&
                   position(verifyFinder) < endPosition(infix(finder))) {

                TAlignedRead match(length(fragStore.alignedReadStore),
                                   position(pattern).i1, 
                                   i,
                                   beginPosition(verifyFinder),
                                   endPosition(verifyFinder));
                appendValue(fragStore.alignedReadStore, match);
            }

            // 4) Write out Sam file
            std::ofstream samFile(argv[3], std::ios_base::out);
            write(samFile, fragStore, Sam());
        }
    }
    
    return;
}

int main(int argc, char const **argv) {

    miniMapping(argc, argv);
    return 0;
}
