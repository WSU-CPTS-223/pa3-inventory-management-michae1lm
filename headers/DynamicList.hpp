#ifndef DYNAMIC_LIST_HPP
#define DYNAMIC_LIST_HPP

#include <exception>
#include <stdexcept>
#include <utility>

template <class ItemType>
class DynamicList {
    ItemType* elements;
    unsigned int current_size;
    unsigned int max_capacity;

    void increase_capacity(unsigned int new_max) {
        ItemType* new_elements = new ItemType[new_max];
        for (unsigned int i = 0; i < current_size; i++) {
            new_elements[i] = elements[i];
        }
        delete[] elements;
        elements = new_elements;
        max_capacity = new_max;
    }

public:
    DynamicList() : elements(nullptr), current_size(0), max_capacity(0) {}
    ~DynamicList() { delete[] elements; }

    DynamicList(const DynamicList& copy_from)
        : elements(nullptr), current_size(0), max_capacity(0) {
        if (copy_from.max_capacity > 0) {
            elements = new ItemType[copy_from.max_capacity];
            for (unsigned int i = 0; i < copy_from.current_size; i++) {
                elements[i] = copy_from.elements[i];
            }
            current_size = copy_from.current_size;
            max_capacity = copy_from.max_capacity;
        }
    }

    DynamicList& operator=(const DynamicList& copy_from) {
        if (this != &copy_from) {
            delete[] elements;
            elements = nullptr;
            current_size = 0;
            max_capacity = 0;
            if (copy_from.max_capacity > 0) {
                elements = new ItemType[copy_from.max_capacity];
                for (unsigned int i = 0; i < copy_from.current_size; i++) {
                    elements[i] = copy_from.elements[i];
                }
                current_size = copy_from.current_size;
                max_capacity = copy_from.max_capacity;
            }
        }
        return *this;
    }

    DynamicList(DynamicList&& move_from) noexcept
        : elements(move_from.elements),
          current_size(move_from.current_size),
          max_capacity(move_from.max_capacity) {
        move_from.elements = nullptr;
        move_from.current_size = 0;
        move_from.max_capacity = 0;
    }

    DynamicList& operator=(DynamicList&& move_from) noexcept {
        if (this != &move_from) {
            delete[] elements;
            elements = move_from.elements;
            current_size = move_from.current_size;
            max_capacity = move_from.max_capacity;
            move_from.elements = nullptr;
            move_from.current_size = 0;
            move_from.max_capacity = 0;
        }
        return *this;
    }

    void append(const ItemType& new_item) {
        if (current_size >= max_capacity) {
            unsigned int new_cap = (max_capacity == 0) ? 8 : max_capacity * 2;
            increase_capacity(new_cap);
        }
        elements[current_size++] = new_item;
    }

    void append(ItemType&& new_item) {
        if (current_size >= max_capacity) {
            unsigned int new_cap = (max_capacity == 0) ? 8 : max_capacity * 2;
            increase_capacity(new_cap);
        }
        elements[current_size++] = std::move(new_item);
    }

    unsigned int count() const { return current_size; }
    bool is_empty() const { return current_size == 0; }

    ItemType& access(unsigned int position) {
        if (position >= current_size) {
            throw std::out_of_range("Position beyond list size");
        }
        return elements[position];
    }

    const ItemType& access(unsigned int position) const {
        if (position >= current_size) {
            throw std::out_of_range("Position beyond list size");
        }
        return elements[position];
    }

    ItemType& operator[](unsigned int position) { return elements[position]; }
    const ItemType& operator[](unsigned int position) const { return elements[position]; }

    void reset() { current_size = 0; }
};

#endif // DYNAMIC_LIST_HPP
