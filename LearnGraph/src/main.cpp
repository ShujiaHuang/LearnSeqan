#include <iostream>
#include <seqan/graph_types.h>
#include <seqan/graph_algorithms.h>
#include <seqan/basic/basic_math.h> // Specific in HMM

using namespace seqan;

void basic() {

    typedef unsigned int TCargo;
    typedef Graph<Undirected<TCargo> > TGraph;
    typedef VertexDescriptor<TGraph>::Type TVertexDescriptor;

    TGraph g;
    TVertexDescriptor vertBerlin   = addVertex(g);
    TVertexDescriptor vertHamburg  = addVertex(g);
    TVertexDescriptor vertHannover = addVertex(g);
    TVertexDescriptor vertMainz    = addVertex(g);
    TVertexDescriptor vertMuenchen = addVertex(g);

    addEdge(g, vertBerlin,   vertHamburg,  289);
    addEdge(g, vertBerlin,   vertHannover, 286);
    addEdge(g, vertBerlin,   vertMainz,    573);
    addEdge(g, vertBerlin,   vertMuenchen, 586);
    addEdge(g, vertHannover, vertMuenchen, 572);
    addEdge(g, vertHamburg,  vertMainz,    521);
    
    FILE *strmWrite = fopen("graph.dot", "w");
    write(strmWrite, g, DotDrawing());
    fclose(strmWrite);

    std::cout << g << "\n";

    typedef String<char> TCityName;
    typedef String<TCityName> TProperties;
    TProperties cityNames;
    resizeVertexMap(g, cityNames);
    assignProperty(cityNames, vertBerlin, "Berlin");
    assignProperty(cityNames, vertHamburg, "Hamburg");
    assignProperty(cityNames, vertHannover, "Hannover");
    assignProperty(cityNames, vertMainz, "Mainz");
    assignProperty(cityNames, vertMuenchen, "Muenchen");

    typedef Iterator<TGraph, VertexIterator>::Type TVertexIterator;
    TVertexIterator itV(g);
    for (; !atEnd(itV); goNext(itV)) {
        std::cout << value(itV) << ':' << getProperty(cityNames, value(itV)) << "\n";
    }

    std::cout << "\n";

    // Find the shortest-path
    typedef Size<TGraph>::Type TSize;
    InternalMap<TCargo> cargoMap;
    String<TVertexDescriptor> preMap;
    String<TSize> distMap;
    dijkstra(g, vertHannover, cargoMap, preMap, distMap);
    TVertexIterator itV2(g);
    while (!atEnd(itV2)) {
        std::cout << "Shortest path from " << property(cityNames, vertHannover) 
                  << " to " << property(cityNames, value(itV2)) << ": "
                  << property(distMap, value(itV2)) << "\n";
        goNext(itV2);
    }
    std::cout << "\n";

    return;
}

void HMM() {

    typedef LogProb<> TProbability;
    typedef Dna TAlphabet;
    typedef Size<TAlphabet>::Type TSize;
    typedef Graph<Hmm<TAlphabet, TProbability, Default> > THmm;
    typedef VertexDescriptor<THmm>::Type TVertexDescriptor;
    typedef EdgeDescriptor<THmm>::Type TEdgeDescriptor;

    Dna dnaA = 'A';
    Dna dnaC = 'C';
    Dna dnaG = 'G';
    Dna dnaT = 'T';

    THmm hmm;
    TVertexDescriptor begState = addVertex(hmm);
    assignBeginState(hmm, begState);
    TVertexDescriptor exonState = addVertex(hmm);
    emissionProbability(hmm, exonState, dnaA) = 0.25;
    emissionProbability(hmm, exonState, dnaC) = 0.25;
    emissionProbability(hmm, exonState, dnaG) = 0.25;
    emissionProbability(hmm, exonState, dnaT) = 0.25;
    
    TVertexDescriptor spliceState = addVertex(hmm);
    emissionProbability(hmm, spliceState, dnaA) = 0.05;
    emissionProbability(hmm, spliceState, dnaC) = 0.0;
    emissionProbability(hmm, spliceState, dnaG) = 0.95;
    emissionProbability(hmm, spliceState, dnaT) = 0.0;

    TVertexDescriptor intronState = addVertex(hmm);
    emissionProbability(hmm, intronState, dnaA) = 0.4;
    emissionProbability(hmm, intronState, dnaC) = 0.1;
    emissionProbability(hmm, intronState, dnaG) = 0.1;
    emissionProbability(hmm, intronState, dnaT) = 0.4;

    TVertexDescriptor endState = addVertex(hmm);
    assignEndState(hmm, endState);
    addEdge(hmm, begState, exonState, 1.0);
    addEdge(hmm, exonState, exonState, 0.9);
    addEdge(hmm, exonState, spliceState, 0.1);
    addEdge(hmm, spliceState, intronState, 0.1);
    addEdge(hmm, intronState, intronState, 0.9);
    addEdge(hmm, intronState, endState, 0.1);

    // Check the HMM
    std::cout << "\n" << hmm << "\n\n";

    
    // Test
    String<Dna> sequence = "CTTCATGTGAAAGCAGACGTAAGTCA";
    String<TVertexDescriptor> path;
    TProbability p = viterbiAlgorithm(hmm, sequence, path);
    std::cout << "Viterbi algorithm\n";
    std::cout << "Probability of best path: " << p << "\n";
    std::cout << "Sequence: \n";
    for (TSize i(0); i < length(sequence); ++i)
        std::cout << sequence[i] << ",";
    std::cout << "\n";
    std::cout << "State path: \n";
    for (TSize i(0); i < length(path); ++i) {
        std::cout << path[i];
        if (isSilent(hmm, path[i])) std::cout << " (Silent)";
        if (i < length(path) - 1) std::cout << ",";
    }
    std::cout << "\n\n";

    std::cout << "Forward algorithm\n";
    p = forwardAlgorithm(hmm, sequence);
    std::cout << "Probability that the HMM generated the sequence: " 
              << p << "\n\n";
    std::cout << "Backward algorithm\n";
    p = backwardAlgorithm(hmm, sequence);
    std::cout << "Probability taht the HMM generated the sequence: " << p << "\n";

    return;
}

int main() {

    basic();
    HMM();
    return 0;
}
