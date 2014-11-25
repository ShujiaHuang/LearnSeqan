#include <iostream>
#include <seqan/basic.h>
#include <seqan/sequence.h>

using namespace seqan;

void Basic() {

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

    return;
}

////////////////////////////////////////////////////////////////////////////////

template <typename Tbet>
void ShowAllLettersOfMyAlphabet(Tbet const &) {

    typedef typename Size<Tbet>::Type TSize;
    TSize alphsize = ValueSize<Tbet>::VALUE;
    for (TSize i = 0; i < alphsize; ++i) {
        std::cout << i << ":" << Tbet(i) << " ";
    }
    std::cout << "\n";
    return;
}
void ShowLetter() {

    ShowAllLettersOfMyAlphabet(AminoAcid());
    ShowAllLettersOfMyAlphabet(Dna());
    ShowAllLettersOfMyAlphabet(Dna5());

    return;
}

////////////////////////////////////////////////////////////////////////////////
int main() {

    Basic();
    ShowLetter();
    return 0;
}
