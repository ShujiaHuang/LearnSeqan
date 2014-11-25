#include <iostream>
#include <seqan/basic.h>
#include <seqan/sequence.h>

using namespace seqan;

int main() {

    unsigned alphSize = ValueSize<Dna>::VALUE;
    std::cout << "Aphabet size of Dna: " << alphSize << std::endl;

    unsigned bits = BitsPerValue<Dna>::VALUE;
    std::cout << "Number of bits needed to store a value of type Dna : " << bits << std::endl;
    bits = BitsPerValue<Dna5>::VALUE;
    std::cout << "Number of bits needed to store a value of type Dna5: " << bits << std::endl;

    Dna a = 'A';
    Dna c = 'C';
    Dna g = 'G';
    Dna t = 'T';

    std::cout << "A: " << (unsigned)ordValue(a) << std::endl;
    std::cout << "C: " << (unsigned)ordValue(c) << std::endl;
    std::cout << "G: " << (unsigned)ordValue(g) << std::endl;
    std::cout << "T: " << (unsigned)ordValue(t) << std::endl;
    
    return 0;
}
