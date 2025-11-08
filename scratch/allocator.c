#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>

#define PAGE_SIZE 4096 // 4kB
#define ALIGN_TO(x, align) (((x) + (align) - 1) & ~((align) - 1))

typedef struct {
    void* base;
    size_t size;
    size_t num_pages;
    char* bitmap;
} Allocator;

Allocator* create(size_t size) {
    Allocator* alloc = malloc(sizeof(Allocator));
    if (!alloc) return NULL;

    alloc->size = ALIGN_TO(size, PAGE_SIZE);
    alloc->num_pages = size / PAGE_SIZE;

    // reserve virtual address space
    alloc->base = mmap(NULL, alloc->size,
                       PROT_NONE,
                       MAP_PRIVATE | MAP_ANONYMOUS,
                       -1, 0);

    if (alloc->base == MAP_FAILED) {
        free(alloc);
        return NULL;
    }

    // create allocation bitmap
    alloc->bitmap = calloc(alloc->num_pages, 1);
    if (!alloc->bitmap) {
        munmap(alloc->base, alloc->size);
        free(alloc);
        return NULL;
    }

    return alloc;
}

void destroy(Allocator* alloc) {
    if (!alloc) return;

    munmap(alloc->base, alloc->size);
    free(alloc->bitmap);
    free(alloc);
}


void* allocate_page(Allocator* alloc, size_t page_index) {
    if (page_index >= alloc->num_pages || alloc->bitmap[page_index]) {
        return NULL;
    }

    void* page_addr = (char*)alloc->base + (page_index * PAGE_SIZE);

    if (mprotect(page_addr, PAGE_SIZE, PROT_READ | PROT_WRITE) != 0) {
        perror("failed making page available");
        return NULL;
    }

    alloc->bitmap[page_index] = 1;
    return page_addr;
}

void free_page(Allocator* alloc, size_t page_index) {
    if (page_index >= alloc->num_pages || alloc->bitmap[page_index]) {
        return;
    }

    void* page_addr = (char*)alloc->base + (page_index + PAGE_SIZE);

    mprotect(page_addr, PAGE_SIZE, PROT_NONE);
    alloc->bitmap[page_index] = 0;

}




// #ifdef TEST_ALLOCATOR

// int main() {

//     size_t kB = (size_t)1024;
//     size_t mB = (size_t)kB * 1024;
//     size_t gB = (size_t)mB * 1024;

//     Allocator alloc = create(gB);

//     if (alloc) {

//         void* p1 = allocate_page(alloc, 0);
//         void* p2 = allocate_page(alloc, 1);

//         printf("p1: %p | p2: %p \n", &p1, &p2);

//         destroy(alloc);
//     }

//     return 0;
// }


// #endif
