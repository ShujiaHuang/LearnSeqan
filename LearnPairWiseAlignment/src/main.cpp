#include <iostream>
#include <seqan/align.h>

using namespace seqan;

void globalAlign() {

    typedef String<char> TSequence;
    typedef Align<TSequence, ArrayGaps> TAlign;

    TSequence seq1 = "CDFGHC";//"CDFGHC";//"AAATGACGGATTG"; //"AAATGACGGATTG";
    TSequence seq2 = "CDEFGAHC";//"CDEFGAHC";//"TGGGA"; //"AGTCGGATCTACTG";

    TAlign align;
    resize(rows(align), 2);
    assignSource(row(align, 0), seq1);
    assignSource(row(align, 1), seq2);

    int score = globalAlignment(align, Score<int, Simple>(4, -2, -2, -4));
    std::cout << "Score: " << score << "\n";
    std::cout << align << "\n";


    // Band Alignment
    std::cout << "Band Alignment\n";
    score = globalAlignment(align, Score<int, Simple>(4, -2, -2, -4), -2, 2);
    std::cout << "Score: " << score << "\n";
    std::cout << align << "\n";

    return;
}
///////////////////////////////////////////////////////////////////////////////

void overlapAlign() {

    typedef String<char> TSequence;
    typedef StringSet<TSequence> TStringSet;
    typedef StringSet<TSequence, Dependent<> > TDepStringSet;
    typedef Graph<Alignment<TDepStringSet> > TAlignGraph;

    TSequence seq1 = "AAATGACGGATTG"; //"AAATGACGGATTG"; //"blablubalu";
    TSequence seq2 = "TGGGA"; //"AGTCGGATCTACTG"; //"abba";

    TStringSet sequences;
    appendValue(sequences, seq1);
    appendValue(sequences, seq2);

    TAlignGraph alignG(sequences);
    // All parameters of AlignConfig are set to true in Overlap alignment
    int score = globalAlignment(alignG, 
                                Score<int, Simple>(1, -1, -1), 
                                AlignConfig<true, true, true, true>());

    std::cout << "Score : " << score << "\n";
    std::cout << alignG << "\n";
    
    return;
}
///////////////////////////////////////////////////////////////////////////////

void specializedAlignment() {

    typedef String<char> TSequence;
    typedef Align<TSequence, ArrayGaps> TAlign;

    TSequence seq1 = "PNCFDAKQRTASRPL"; //"AAATGACGGATTG"; //"GARFIELDTHECAT";
    TSequence seq2 = "CFDKQKNNRTATRDTA"; //"TGGGA"; //"GARFIELDTHEBIGCAT";

    TAlign align;
    resize(rows(align), 2);
    assignSource(row(align, 0), seq1);
    assignSource(row(align, 1), seq2);

    int score = globalAlignment(align, 
                                Score<int, Simple>(1, -1, -1), 
                                Hirschberg());
    std::cout << "Hirschberg Score: " << score << "\n";
    std::cout << align << "\n";

    score = globalAlignment(align, MyersHirschberg());
    std::cout << "MyersHirschberg Score: " << score << "\n";
    std::cout << align << "\n";
    return;
}

///////////////////////////////////////////////////////////////////////////////

void rnaAlign() {

    typedef String<Rna> TSequence;
    typedef Align<TSequence, ArrayGaps> TAlign;
    typedef Row<TAlign>::Type TRow;
    typedef Iterator<TRow>::Type TRowIterator;

    TSequence seq1 = "AAGUGACUUAUUG";
    TSequence seq2 = "AGUCGGAUCUACUG";

    TAlign align;
    resize(rows(align), 2);
    assignSource(row(align, 0), seq1);
    assignSource(row(align, 1), seq2);

    int score = globalAlignment(align, MyersHirschberg());
    std::cout << "RNA MyersHirschberg Score: " << score << "\n";
    std::cout << align << "\n";

    unsigned aliLength = _max(length(row(align, 0)), length(row(align, 1)));
    for (unsigned i(0); i < length(rows(align)); ++i) {

        TRowIterator it = iter(row(align, i), 0);
        TRowIterator itEnd = iter(row(align, i), aliLength);
        unsigned pos = 0;
        std::cout << "Row: " << i << " contain gaps at positions: \n";
        while (it != itEnd) {
            if (isGap(it)) std::cout << pos << "\n";
            ++it;
            ++pos;
        }
    }

    std::cout << "\n\n";
   
    return;
}

///////////////////////////////////////////////////////////////////////////////

void localAlign() {

    Align<String<AminoAcid> > ali;
    resize(rows(ali), 2);
    assignSource(row(ali, 0), "PNCFDAKQRTASRPL");
    assignSource(row(ali, 1), "CFDKQKNNRTATRDTA");

    Score<int> sc(3, -2, -1, -5);
    unsigned count(0);
    LocalAlignmentEnumerator<Score<int>, Unbanded> enumerator(sc);
    while (nextLocalAlignment(ali, enumerator) && count < 3) {

        std::cout << "Score: " << getScore(enumerator) << "\n";
        std::cout << ali;
        ++count;
    }
    return;
}

///////////////////////////////////////////////////////////////////////////////

void completeExample() {
// General cigar as mapping information
// http://trac.seqan.de/wiki/Tutorial/PairwiseSequenceAlignment/Assignment/Assignment5ApproximatePatternMatching
    typedef String<char> TSequence;
    typedef StringSet<TSequence> TSequenceSet;
    typedef Gaps<TSequence, ArrayGaps> TGaps;
    typedef Iterator<TGaps>::Type TGapsIterator;
    typedef Iterator<String<int> >::Type TIterator;

    TSequence text    = "MISSISSIPPIANDMISSOURI";
    TSequence pattern = "SISSI";

    String<int> location;
    for (unsigned i(0); i < length(text) - length(pattern); ++i) {

        // Compute the MyersBitVector in current window of text.
        TSequence tmp = infix(text, i, i + length(pattern));
        if (globalAlignmentScore(tmp, pattern, MyersBitVector()) >= -2) {

            appendValue(location, i);
        }
    }
    


    TGaps gapsText;
    TGaps gapsPattern;
    assignSource(gapsPattern, pattern);
    std::cout << "Text: " << text << "\tPattern: " << pattern << "\n";
    for (TIterator it = begin(location); it != end(location); ++it) {

        // Clear previously computed gaps.
        clearGaps(gapsText);
        clearGaps(gapsPattern);
        // Only recompute the area within the current window over the text.
        TSequence textInfix = infix(text, *it, *it + length(pattern));
        assignSource(gapsText, textInfix);
        // Use semi-global alignment since we do not want to track leading/trailing gaps in the pattern.
        // Restirct search space using a band allowing at most 2 errors in vertical/horizontal direction.
        int score = globalAlignment(gapsText, 
                                    gapsPattern, 
                                    Score<int>(0, -1, -1), 
                                    AlignConfig<true, false, false, true>(),
                                    -2, 
                                    2);



        TGapsIterator itGapsPattern = begin(gapsPattern);
        TGapsIterator itGapsEnd     = end(gapsPattern);

        // Remove trailing gaps in pattern
        int count(0);
        while (isGap(--itGapsEnd)) ++count;
        setClippedEndPosition(gapsPattern, length(gapsPattern) - count);
        // Remove leading gaps in pattern
        if (isGap(itGapsPattern)) {
            setClippedBeginPosition(gapsPattern, countGaps(itGapsPattern));
            setClippedBeginPosition(gapsText,    countGaps(itGapsPattern));
        }


        // Reinitilaize the iterators.
        TGapsIterator itGapsText = begin(gapsText);
        itGapsPattern = begin(gapsPattern);
        itGapsEnd     = end(gapsPattern);
        // Use a stringstream to construct the cigar string.
        std::stringstream cigar;
        int numChar = 0;
        while (itGapsPattern != itGapsEnd) {

            // Count insertions.
            if (isGap(itGapsText)) {
                int numGaps = countGaps(itGapsText);
                cigar << numGaps << "I";
                itGapsText    += numGaps;
                itGapsPattern += numGaps;
                continue;
            }
            // Count deletions.
            if (isGap(itGapsPattern)) {
                int numGaps = countGaps(itGapsPattern);
                cigar << numGaps << "D";
                itGapsText += numGaps;
                itGapsPattern += numGaps;
                continue;
            }
            // Count matches.
            while (*itGapsText == *itGapsPattern && itGapsPattern != itGapsEnd) {
                ++numChar;
                ++itGapsText;
                ++itGapsPattern;
            } 
            if (numChar != 0) {
                cigar << numChar << "M";
                numChar = 0;
                continue;
            }
            // Count Mismatches
            while (*itGapsText != *itGapsPattern && itGapsPattern != itGapsEnd) {
                ++numChar;
                ++itGapsText;
                ++itGapsPattern;
            }
            if (numChar != 0) cigar << numChar << "S";
            numChar = 0;
            
        }

        std::cout << "Hit at position " << *it   << "\tTotal edits: " 
                  << abs(score) << ", " << score << "\tCigar: " << cigar.str() << "\n";

    }

    return;
}

int main() {

    //globalAlign();
    //overlapAlign();
    //specializedAlignment();
    //rnaAlign();
    //localAlign();
    completeExample();
    return 0;
}
