#include <iostream>
#include <string>

#include <seqan/store.h>
#include <seqan/realign.h>

using namespace seqan;

int main(int argc, char** argv) {
    // Build paths to reference and to alignment.
    //std::string refPath = SEQAN_PATH_TO_ROOT();
    //refPath.append("/Users/LiuSiyang/iLibrary/BioLib/seqan/demos/tutorial/realign/ref.fa");
    //std::string samPath = SEQAN_PATH_TO_ROOT();
    //samPath.append("/Users/LiuSiyang/iLibrary/BioLib/seqan/demos/tutorial/realign/reads.sam");

    // Declare fragment store.
    FragmentStore<> store;

    // Load contigs and read alignment.
    std::string refPath = "../data/ref.fa";
    std::string samPath = "../data/reads.sam";
    loadContigs(store, refPath.c_str());
    BamFileIn bamFileIn(samPath.c_str());
    readRecords(store, bamFileIn);

    // Layout alignment and print.
    AlignedReadLayout layout;
    layoutAlignment(layout, store);
    printAlignment(std::cout, layout, store, /*contigID=*/ 0, /*posBegin=*/ 1060,
                   /*posEnd=*/ 1140, /*lineBegin=*/ 0, /*lineEnd=*/ 100);

    // Perform the realignment.
    reAlignment(store, /*contigID=*/ 0, /*method=*/ 1, /*bandwidth=*/ 20,
                /*includeReference=*/ true);
    layoutAlignment(layout, store);
    printAlignment(std::cout, layout, store, /*contigID=*/ 0, /*posBegin=*/ 1060,
                   /*posEnd=*/ 1140, /*lineBegin=*/ 0, /*lineEnd=*/ 100);

    return 0;
}
