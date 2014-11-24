#include <iostream>
#include <seqan/basic.h>
#include <seqan/sequence.h>
#include <seqan/file.h>

//using namespace seqan;

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

int main() {

    BasicTest();

    return 0;
}
