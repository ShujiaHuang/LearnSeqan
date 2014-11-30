#include <iostream>
#include <seqan/sequence.h>
#include <seqan/file.h>

using namespace seqan;

///////////////////////////////////////////////////////////////////////////////
void basic() {

    Dna5String genome = "ANTGGTTNCAACNGTAANTGCTGANNNACATGTNCGCGTGTA";

    Iterator<Dna5String>::Type it = begin(genome);    
    
    std::cout << genome << "\n";
    for (; !atEnd(it, genome); goNext(it)) {
        if (getValue(it) == 'N') value(it) = 'A';
    }

    std::cout << genome << "\n";
}

///////////////////////////////////////////////////////////////////////////////

void testRoot() {

    Dna5String genome = "ANTGGTTNCAACNGTAANTGCTGANNNACATGTNCGCGTGTA";

    Iterator<Dna5String, Rooted>::Type it = begin(genome);

    std::cout << genome << "\n";
    for (; !atEnd(it); goNext(it)) {
        if (getValue(it) == 'N') value(it) = 'A';
    }

    std::cout << genome << "\n";
}

int main() {

    basic();
    testRoot();
    return 0;
}




