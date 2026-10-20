#include "../h/Allocator.hpp"

Allocator::Elem *Allocator::first = nullptr;

void *Allocator::allocate(size_t size) {

    if (size <= 0) return nullptr;
    size *= MEM_BLOCK_SIZE;

    if (first == nullptr) {
        Elem *tmp = (Elem *) HEAP_START_ADDR;
        tmp->size = size;
        tmp->next = nullptr;
        first = tmp;
        return &tmp->space;
    }

    if ((unsigned long) first - (unsigned long) HEAP_START_ADDR >= size + sizeof(Elem *) + sizeof(size_t)) {
        Elem *tmp = (Elem *) HEAP_START_ADDR;
        tmp->size = size;
        tmp->next = (Elem *) first;
        first = tmp;
        return &tmp->space;
    }

    Elem *pom = nullptr;
    for (pom = (Elem *) first; pom->next != nullptr; pom = pom->next) {
        if ((unsigned long) pom->next - ((unsigned long) &pom->space + pom->size) >=
            (unsigned long) size + sizeof(Elem *) + sizeof(size_t)) {
            Elem *tmp = (Elem *) ((unsigned long) &pom->space + (unsigned long) pom->size);
            tmp->size = size;
            tmp->next = pom->next;
            pom->next = tmp;
            return &tmp->space;
        }
    }

    if (((unsigned long) &pom->space + pom->size + size) <= (unsigned long) HEAP_END_ADDR) {
        Elem *tmp = (Elem *) ((unsigned long) &pom->space + (unsigned long) pom->size);
        tmp->size = size;
        tmp->next = nullptr;
        pom->next = tmp;
        return &tmp->space;
    }

    return nullptr;
}

int Allocator::deallocate(void *mem) {

    if (mem == nullptr) return 0;

    Elem *previous = nullptr, *current = (Elem *) first;
    while (current != nullptr) {

        if (&current->space == mem) {
            current = current->next;
            if (previous != nullptr) previous->next = current;
            else first = current;
            return 0;
        } else {
            previous = current;
            current = current->next;
        }
    }

    return -1;
}

size_t Allocator::getFreeSpace() {
    size_t size = 0;
    if (first == nullptr) {
        size = (uint64)HEAP_END_ADDR - (uint64)HEAP_START_ADDR;
    } else {
        if (first > HEAP_START_ADDR) {
            size += (uint64) first - (uint64) HEAP_START_ADDR;
        }
        Elem *temp = first;
        while (temp != nullptr) {
            if (temp->next != nullptr) {
                size += (uint64) (temp->next) - (uint64) (temp) - temp->size;
            } else {
                size += (uint64) HEAP_END_ADDR - (uint64) (temp) - temp->size;
            }
            temp = temp->next;
        }
    }
    return size;
}


size_t Allocator::getLargestFree() {
    size_t largest = 0;
    if (first == nullptr) {
        largest = (uint64)HEAP_END_ADDR - (uint64)HEAP_START_ADDR;
    } else {
        if (first > HEAP_START_ADDR) {
            largest = (uint64)first - (uint64)HEAP_START_ADDR;
        }
        Elem *temp = first;
        while (temp != nullptr) {
            if (temp->next != nullptr) {
                size_t largest_candidate = (uint64) (temp->next) - (uint64) (temp) - temp->size;
                if (largest_candidate > largest) {
                    largest = largest_candidate;
                }
            } else {
                size_t largest_candidate = (uint64) HEAP_END_ADDR - (uint64) (temp) - temp->size;
                if (largest_candidate > largest) {
                    largest = largest_candidate;
                }
            }
            temp = temp->next;
        }
    }
    return largest;
}
