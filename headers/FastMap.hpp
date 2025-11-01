#ifndef FAST_MAP_HPP
#define FAST_MAP_HPP

#include <functional>

template <typename KeyType, typename ValueType>
class FastMap {
    struct Slot {
        KeyType slot_key;
        ValueType slot_value;
        bool is_active;
        Slot() : is_active(false) {}
    };

    Slot* hash_table;
    unsigned int table_capacity;
    unsigned int entries_count;

    unsigned int compute_hash(const KeyType& key) const {
        return std::hash<KeyType>{}(key) % table_capacity;
    }

    void insert_pair(const KeyType& key, const ValueType& value) {
        unsigned int base_index = compute_hash(key);
        unsigned int current_index = base_index;
        unsigned int probe_count = 1;

        while (hash_table[current_index].is_active) {
            if (hash_table[current_index].slot_key == key) {
                hash_table[current_index].slot_value = value;
                return;
            }
            current_index = (base_index + probe_count * probe_count) % table_capacity;
            probe_count++;
        }

        hash_table[current_index].slot_key = key;
        hash_table[current_index].slot_value = value;
        hash_table[current_index].is_active = true;
        entries_count++;
    }

    void rebuild_table() {
        unsigned int old_capacity = table_capacity;
        Slot* old_table = hash_table;

        table_capacity = (table_capacity == 0) ? 32 : table_capacity * 2;
        hash_table = new Slot[table_capacity];
        entries_count = 0;

        for (unsigned int i = 0; i < old_capacity; i++) {
            if (old_table[i].is_active) {
                insert_pair(old_table[i].slot_key, old_table[i].slot_value);
            }
        }
        delete[] old_table;
    }

public:
    FastMap() : hash_table(nullptr), table_capacity(0), entries_count(0) {}

    ~FastMap() {
        delete[] hash_table;
    }

    void store(const KeyType& key, const ValueType& value) {
        if (table_capacity == 0 || (double)entries_count / table_capacity > 0.75) {
            rebuild_table();
        }
        insert_pair(key, value);
    }

    ValueType* retrieve(const KeyType& key) {
        if (table_capacity == 0) return nullptr;

        unsigned int base_index = compute_hash(key);
        unsigned int current_index = base_index;
        unsigned int probe_count = 1;

        while (hash_table[current_index].is_active) {
            if (hash_table[current_index].slot_key == key) {
                return &hash_table[current_index].slot_value;
            }
            current_index = (base_index + probe_count * probe_count) % table_capacity;
            probe_count++;
        }
        return nullptr;
    }

    const ValueType* retrieve(const KeyType& key) const {
        return const_cast<FastMap*>(this)->retrieve(key);
    }

    unsigned int entry_count() const { return entries_count; }
    bool has_entries() const { return entries_count > 0; }
};

#endif // FAST_MAP_HPP
