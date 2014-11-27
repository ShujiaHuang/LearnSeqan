#include <iostream>
#include <seqan/basic.h>
#include <seqan/sequence.h>
#include <seqan/file.h>

using namespace seqan;

void basic() {

    StringSet<DnaString> stringSet;
    DnaString str0 = "TATA";
    DnaString str1 = "CGCG";
    appendValue(stringSet, str0);
    appendValue(stringSet, str1);
    appendValue(stringSet, "CTct");

    std::cout << stringSet[0] << "\n";

    // (1) Access by position
    std::cout << "Owner: " << std::endl;
    std::cout << "Position 0: " << value(stringSet, 0) << std::endl;

    // Get the corresponding ids
    unsigned id0 = positionToId(stringSet, 0);
    unsigned id1 = positionToId(stringSet, 1);
    // (2) Access by id
    std::cout << "Id 0:  " << valueById(stringSet, id0) << std::endl;

    // Lets create a string set of type dependent to represent strings, 
    // which are stored in the StringSet of type Owner
    StringSet<DnaString, Dependent<Tight> > depSet;
    // We assign the first two strings of the owner string set to the 
    // dependent StringSet, but in a reverse order
    assignValueById(depSet, stringSet, id1);
    assignValueById(depSet, stringSet, id0);

    std::cout << "\nDependent: \n";
    std::cout << "Pos 0: " << value(depSet, 0) << "\t" << depSet[0] << std::endl;
    std::cout << "Id  0: " << valueById(depSet, id0) << std::endl;


    // Iterator
    std::cout << "\nIterator for StringSet: \n";
    typedef Iterator<StringSet<DnaString> >::Type TStrSetIter;
    for (TStrSetIter it(begin(stringSet)); it != end(stringSet); ++it) {
        std::cout << value(it) << "\t" << *it << std::endl;
    }

    // Concat Test
    typedef Concatenator<StringSet<DnaString> >::Type TConcat;
    TConcat concatSet = concat(stringSet);
    
    Iterator<TConcat>::Type it    = begin(concatSet);
    Iterator<TConcat>::Type itEnd = end(concatSet);

    for (; it != itEnd; goNext(it)) {
        std::cout << getValue(it) << "-" << *it << ' ';
    }
    std::cout << std::endl;;
    
    return;
}

///////////////////////////////////////////////////////////////////////////////

template <typename TStringSet, typename TId>
bool isElement(TStringSet & stringSet1, TId & id) {

    for (unsigned i = 0; i < length(stringSet1); ++i) {
        // Get the id of the element at position i
        if (positionToId(stringSet1, i) == id)
              return true;
    }
    return false;
}

int test2() {
    // Build strings
    DnaString str0 = "TATA";
    DnaString str1 = "CGCG";
    DnaString str2 = "TTAAGGCC";
    DnaString str3 = "ATGC";
    DnaString str4 = "AGTGTCA";
    // Build owner string set and append strings
    StringSet<DnaString> stringSetOw;
    appendValue(stringSetOw, str0);
    appendValue(stringSetOw, str1);
    appendValue(stringSetOw, str2);
    appendValue(stringSetOw, str3);
    appendValue(stringSetOw, str4);
    // Get corresponding ids for positions
    unsigned id0 = positionToId(stringSetOw, 0);
    unsigned id1 = positionToId(stringSetOw, 1);
    unsigned id2 = positionToId(stringSetOw, 2);
    unsigned id3 = positionToId(stringSetOw, 3);
    // Build dependent string set and assigns strings by id
    StringSet<DnaString, Dependent<Generous> > stringSetDep;
    assignValueById(stringSetDep, stringSetOw, id0);
    assignValueById(stringSetDep, stringSetOw, id1);
    assignValueById(stringSetDep, stringSetOw, id3);
    // Call function to check if a string is contained and output result
    std::cout << "Does the string set contain the string with the id 'id3'? "<< 
                isElement(stringSetDep, id3) << std::endl;
    std::cout << "Does the string set contain the string with the id 'id2'? "<<  
                isElement(stringSetDep, id2) << std::endl;
}
///////////////////////////////////////////////////////////////////////////////


int main() {

//    basic();
    test2();
    return 0;
}
