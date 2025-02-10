#ifndef NUMBERS_SHUFFLER_HPP
#define NUMBERS_SHUFFLER_HPP

#include <vector>

class NumbersShuffler {
    public:
        std::vector<unsigned int> biasedNaiveShuffle(unsigned int length);
        std::vector<unsigned int> naiveShuffle(unsigned int length);
        std::vector<unsigned int> biasedFisherYatesShuffle(unsigned int length);
        std::vector<unsigned int> fisherYatesShuffle(unsigned int length);
        std::vector<unsigned int> biasedDurstenfeldShuffle(unsigned int length);
        std::vector<unsigned int> durstenfeldShuffle(unsigned int length);

        NumbersShuffler();
};

#endif // NUMBERS_SHUFFLER_HPP
