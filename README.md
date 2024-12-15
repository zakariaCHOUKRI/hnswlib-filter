# Project: Optimizing Multi-Attribute Filtering in Approximate Nearest Neighbor (ANN) Search

This project focuses on optimizing multi-attribute filtering in Approximate Nearest Neighbor (ANN) search using HNSW indexing. Each data point in the index can have at least 1000 labels (e.g., tags), and queries may specify one or two attributes as filters. The current filtering mechanism in HNSW gives the ability to evaluate custom functions for each visited point. Deciding if a point validates a query filter will require scanning of all point attributes and seeing if one or two of them validate the query tags (O(p) where p is the number of attributes per point). The project aims to improve the filtering process by:
1. Using bitsets to represent attributes of points for faster evaluation.
2. Further optimizing with Roaring Bitmaps, a compressed bitmap structure designed for high-performance set operations.

## Created Files

I only created 3 of the files that are in this repository, which are:
- ```example_Op.cpp```
- ```example_bitset.cpp```
- ```example_croaring.cpp```



Each one of these files does the filtering respectively, by using the O(p) approach that checks all attributes, the bitset approach, and its optimization with the Roaring Bitmaps library.

## Benchmarking and Modularity

The code also contains timing mechanisms to benchmark each approach. It is also highly modular as I used nearly the same code that the developers of hnswlib used in their ```example_filter.cpp``` file, with the only difference being the filter class and function and how the attributes are represented.

## How to Run the Code

For each approach, you can change the constant representing the number of attributes ```NUM_ATTRIBUTES``` before compiling, then compile using:

```sh
g++ -std=c++11 -o example_Op example_Op.cpp
g++ -std=c++11 -o example_bitset example_bitset.cpp
g++ -std=c++11 -o example_croaring example_croaring.cpp
```

Then run each executable.

## Output on My Machine

### 1 Thousand Attributes

**Bitset example run:**
```
Generating random data and attributes...
Data generation completed in 0.351814 seconds.
Adding data to index...
Data added to index in 6.9822 seconds.
Performing search with filter...
Search completed in 3.75686 seconds.
```

**Roaring bitmap example run:**
```
Generating random data and attributes...
Data generation completed in 0.421354 seconds.
Adding data to index...
Data added to index in 6.17978 seconds.
Performing search with filter...
Search completed in 5.21877 seconds.
```

**O(p) example run:**
```
Generating random data and attributes...
Data generation completed in 0.480259 seconds.
Adding data to index...
Data added to index in 6.8397 seconds.
Performing search with filter...
Search completed in 38.2249 seconds.
```

### 1 Million Attributes

**Bitset example run:**
```
Generating random data and attributes...
Data generation completed in 342.698 seconds.
Adding data to index...
Data added to index in 7.15187 seconds.
Performing search with filter...
Search completed in 310.597 seconds.
```

**Roaring bitmap example run:**
```
Generating random data and attributes...
Data generation completed in 355.498 seconds.
Adding data to index...
Data added to index in 6.203 seconds.
Performing search with filter...
Search completed in 8.6197 seconds.
```

**O(p) example run:**
```
Generating random data and attributes...
Data generation completed in 438.377 seconds.
Adding data to index...
Data added to index in 6.62276 seconds.
Performing search with filter...
(it took more than 90 minutes so I stopped it manually)
```