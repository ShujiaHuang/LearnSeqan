#include <iostream>
#include <seqan/find.h>

using namespace seqan;

void basic() {

    CharString haystack = "Simon, send more money!";
    CharString needle   = "mo";
    Finder<CharString> finder(haystack);
    Pattern<CharString, Horspool> pattern(needle);
    while (find(finder, pattern)) {
        std::cout << "[" << beginPosition(finder) << ","
                  << endPosition(finder) << ")\t" << infix(finder) << "\n";
    }
    return;
}

int main() {

    basic();
    return 0;
}
