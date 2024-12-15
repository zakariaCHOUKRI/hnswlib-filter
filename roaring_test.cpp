#include "roaring.hh"
#include "roaring.c"

int main() {
    // Create a new Roaring bitmap
    roaring_bitmap_t *r = roaring_bitmap_create();

    // Add elements to the bitmap
    roaring_bitmap_add(r, 1);
    roaring_bitmap_add(r, 2);
    roaring_bitmap_add(r, 3);

    // Check if elements are in the bitmap
    if (roaring_bitmap_contains(r, 2)) {
        printf("Bitmap contains 2\n");
    }

    // Free the bitmap
    roaring_bitmap_free(r);

    return 0;
}