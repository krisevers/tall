#include <cstddef>
#include <sys/mman.h>

#define PAGE_SIZE 4096  //4kb
#define ALIGN_TO(x, align) (((x) + (align) - 1) & ~((align) - 1))

class Allocator {
    void* base;
    std::size_t size;
    std::size_t num_pages;
    char* bitmap;

    public:
        Allocator(std::size_t _size) {
            size = ALIGN_TO(_size, PAGE_SIZE);
            num_pages = _size / PAGE_SIZE;

            base = mmap(NULL, size,
                PROT_NONE,
                MAP_PRIVATE | MAP_ANONYMOUS,
                -1, 0);

            bitmap = static_cast<char*>(calloc(num_pages, 1));
        }

        ~Allocator() {
            munmap(base, size);
            free(bitmap);
        }

        void* allocate_page(std::size_t page_index) {
            if (page_index >= num_pages || bitmap[page_index]) {
                return NULL;
            }

            void* page_addr = (char*)base + (page_index * PAGE_SIZE);

            if (mprotect(page_addr, PAGE_SIZE, PROT_READ | PROT_WRITE) != 0) {
                perror("failed making page available");
                return NULL;
            }

            bitmap[page_index] = 1;
            return page_addr;
        }

        void free_page(std::size_t page_index) {
            if (page_index >= num_pages || !bitmap[page_index]) {
                return;
            }

            void* page_addr = (char*)base + (page_index + PAGE_SIZE);

            mprotect(page_addr, PAGE_SIZE, PROT_NONE);
            bitmap[page_index] = 0;
        }
};
