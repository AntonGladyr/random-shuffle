#ifndef LIST_SHUFFLER_HPP
#define LIST_SHUFFLER_HPP

#include <vector>

class NumbersShuffler {
    public:
        std::vector<unsigned int> naiveShuffle(unsigned int length);
        std::vector<unsigned int> fisherYatesShuffle(unsigned int length);
        NumbersShuffler();
};

#endif // LIST_SHUFFLER_HPP
