#include "hnswlib.h"
#include "roaring.hh"
#include "roaring.c"
#include <unordered_map>
#include <iostream>
#include <chrono>

using namespace std;

const int NUM_ATTRIBUTES = 1000000;

// Mapping from labels to Roaring bitmaps representing attributes
unordered_map<hnswlib::labeltype, roaring_bitmap_t*> label_to_attributes;

// Filter class using Roaring bitmaps
class RoaringFilter: public hnswlib::BaseFilterFunctor {

    roaring_bitmap_t* query_attributes;

public:
    RoaringFilter(roaring_bitmap_t* query_attributes): query_attributes(query_attributes) {}

    bool operator()(hnswlib::labeltype label_id) {
        roaring_bitmap_t* point_attributes = label_to_attributes[label_id];
        // Check if the point attributes match the query attributes
        // To do so, we use the roaring_bitmap_and function
        // And the roaring_bitmap_equals function to check if the intersection
        // Is equal to the query attributes
        roaring_bitmap_t* intersection = roaring_bitmap_and(point_attributes, query_attributes);
        bool result = roaring_bitmap_equals(intersection, query_attributes);
        roaring_bitmap_free(intersection);
        return result;
    }
};

int main() {
    int dim = 16;               // Dimension of the elements
    int max_elements = 10000;   // Maximum number of elements, should be known beforehand
    int M = 16;                 // Tightly connected with internal dimensionality of the data
                                // strongly affects the memory consumption
    int ef_construction = 200;  // Controls index search speed/build speed tradeoff

    // Initing index
    hnswlib::L2Space space(dim);
    hnswlib::HierarchicalNSW<float>* alg_hnsw = new hnswlib::HierarchicalNSW<float>(&space, max_elements, M, ef_construction);

    // Generate random data and attributes
    mt19937 rng;
    rng.seed(47);
    uniform_real_distribution<> distrib_real;
    float* data = new float[dim * max_elements];

    auto start_time = chrono::high_resolution_clock::now();
    cout << "Generating random data and attributes..." << endl;

    for (int i=0; i<dim * max_elements; i++) {
        data[i] = distrib_real(rng);
    }

    for (int i=0; i<max_elements; i++) {
        roaring_bitmap_t* attributes = roaring_bitmap_create();
        for (int j=0; j<NUM_ATTRIBUTES; j++) {
            if (rng() % 2) {
                roaring_bitmap_add(attributes, j);
            }
        }
        label_to_attributes[i] = attributes;
    }

    auto end_time = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end_time - start_time;
    cout << "Data generation completed in " << duration.count() << " seconds." << endl;

    // Add data to index
    cout << "Adding data to index..." << endl;
    start_time = chrono::high_resolution_clock::now();

    for (int i=0; i<max_elements; i++) {
        alg_hnsw->addPoint(data + i * dim, i);
    }

    end_time = chrono::high_resolution_clock::now();
    duration = end_time - start_time;
    cout << "Data added to index in " << duration.count() << " seconds." << endl;

    // Create a query roaring bitmap
    roaring_bitmap_t* query_attributes = roaring_bitmap_create();
    roaring_bitmap_add(query_attributes, 0);
    roaring_bitmap_add(query_attributes, NUM_ATTRIBUTES/4);
    roaring_bitmap_add(query_attributes, NUM_ATTRIBUTES/2);
    roaring_bitmap_add(query_attributes, (NUM_ATTRIBUTES*3)/4);

    // Create filter using the query bitmap
    RoaringFilter filter(query_attributes);

    // Perform a search with the filter
    cout << "Performing search with filter..." << endl;
    start_time = chrono::high_resolution_clock::now();

    int k = 10;
    for (int i=0; i<max_elements; i++) {
        vector<pair<float, hnswlib::labeltype>> result = alg_hnsw->searchKnnCloserFirst(data + i * dim, k, &filter);
    }

    end_time = chrono::high_resolution_clock::now();
    duration = end_time - start_time;
    cout << "Search completed in " << duration.count() << " seconds." << endl;


    for (auto& pair : label_to_attributes) {
        roaring_bitmap_free(pair.second);
    }
    roaring_bitmap_free(query_attributes);
    delete[] data;
    delete alg_hnsw;

    return 0;
}