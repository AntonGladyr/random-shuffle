#ifndef NUMBERS_SHUFFLER_HPP
#define NUMBERS_SHUFFLER_HPP

#include <random>
#include <vector>

class NumbersShuffler {
    public:
        std::vector<unsigned int> biasedNaiveShuffle(unsigned int length);
        std::vector<unsigned int> naiveShuffle(unsigned int length);
        std::vector<unsigned int> biasedFisherYatesShuffle(unsigned int length);
        std::vector<unsigned int> fisherYatesShuffle(unsigned int length);
        std::vector<unsigned int> biasedDurstenfeldShuffle(unsigned int length);
        std::vector<unsigned int> durstenfeldShuffle(unsigned int length);
        std::vector<unsigned int> randomShuffle(unsigned int length);
        std::vector<unsigned int> mergeShuffle(unsigned int length);
        std::vector<unsigned int> parallelMergeShuffle(unsigned int length);

        NumbersShuffler();
    private:
        // Static flag to ensure std::rand() is seeded only once.
        static bool s_randSeeded;
    
        // Static mt19937 engine, seeded once using std::random_device.
        static std::mt19937 s_mtEngine;

        // A thread-local engine for safe concurrent use.
        static std::mt19937& getThreadLocalEngine() {
            thread_local std::mt19937 engine(std::random_device{}());
            return engine;
        }

        static void mergeShuffleRec(
            std::vector<unsigned int>& arr,
            std::vector<unsigned int>& temp,
            unsigned int start,
            unsigned int end
        );

        static void parallelMergeShuffleRec(
            std::vector<unsigned int>& arr,
            std::vector<unsigned int>& temp,
            unsigned int start,
            unsigned int end
        );
};

#endif // NUMBERS_SHUFFLER_HPP
