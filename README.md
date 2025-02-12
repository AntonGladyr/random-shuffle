# random-shuffle

This application generates a shuffled list or produces statistics based on multiple
shuffling iterations. It supports various shuffling algorithms and accepts command-line
parameters to control its behavior.

## Command-Line Parameters

The application accepts the following parameters:

```bash
./shuffler --mode <generate|stats> --n <permutation length> --algorithm <algorithm name or index> [--iterations <iterations>]
```

### Options

* `--mode` Specifies the operation mode:

* * `generate`: Generate and print a single shuffled list.
* * `stats`: Run the selected shuffling algorithm multiple times to produce statistical
    frequency data.

* `--n` A positive integer that sets the permutation length (the number of elements in the 
shuffled list).

* `--algorithm` Selects the shuffling algorithm. You may provide:

* * The name of the algorithm (e.g., durstenfeldShuffle), or
* * A 1-based index corresponding to the available algorithms list.

* `--iterations` (optional) A positive integer representing the number of iterations to run
when in stats mode. Defaults to 1 if not specified.

## Available Algorithms

The following shuffling algorithms are supported:

1. `biasedNaiveShuffle`
2. `naiveShuffle`
3. `biasedFisherYatesShuffle`
4. `fisherYatesShuffle`
5. `biasedDurstenfeldShuffle`
6. `durstenfeldShuffle`
7. `randomShuffle`
8. `mergeShuffle`
9. `parallelMergeShuffle`

## Example Usage

### Generate a Shuffled List

Generate a shuffled list of 100 elements using algorithm 6 (durstenfeldShuffle):

```bash
./shuffler --mode generate --n 100 --algorithm 6
```

Or, using the algorithm name:

```bash
./shuffler --mode generate --n 100 --algorithm durstenfeldShuffle
```

### Produce Statistics

Run 100 iterations of shuffling a list of 1000 elements using the fisherYatesShuffle algorithm to produce statistics:

```bash
./shuffler --mode stats --n 1000 --algorithm fisherYatesShuffle --iterations 100
```

## Using Docker

You can run the application in a Docker container by passing the command-line arguments
through Docker commands or Docker Compose.

### Docker Run

For example, to produce statistics with a permutation length of 131072 using
durstenfeldShuffle and 10 iterations, run:

```bash
docker run --rm -it your-image-name --mode stats --n 131072 --algorithm durstenfeldShuffle --iterations 10
```

### Docker Compose

In `docker-compose.yml` file, modify the `command` arguments and the run:

```bash
docker compose up
```

## TODO

* Produce charts
