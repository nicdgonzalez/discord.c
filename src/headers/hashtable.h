#ifndef NDG_HASHTABLE_H_
#define NDG_HASHTABLE_H_

/// @brief Represents an entry in the Hash Table.
/// @param previous A reference to the preceding node in the linked list.
/// @param next A reference to the following node in the linked list.
/// @param key The name to use when looking up an entry.
/// @param value The data to store in the Hash Table.
struct bucket {
    struct bucket *previous;
    struct bucket *next;
    const char * key;
    void * value;
};

/// @brief Represents a Hash Table.
/// @param capacity Represents the number of entries the Hash Table can store.
/// @param count Counter for current number of entries in the Hash Table.
/// @param table Stores entries in the Hash Table as doubly-linked-lists.
struct hashtable {
    unsigned long long int capacity;
    unsigned long long int count;
    struct bucket *table;
};

/// @brief Constructor for the HashTable structure.
/// @param ht A reference to a uninitialized HashTable structure.
void hashtable_init(struct hashtable *);

/// @brief Destructor for the HashTable structure.
/// @param ht A reference to a constructed HashTable structure.
void hashtable_free(struct hashtable *);

/// @brief Creates/updates entries in the Hash Table.
/// @param ht A reference to a constructed HashTable structure.
/// @param key The name to use when looking up an entry.
/// @param value The data to store in the Hash Table.
void hashtable_insert(struct hashtable *, const char *, void *);

/// @brief Retrieves data from the Hash Table.
/// @param ht A reference to a constructed HashTable structure.
/// @param key The name to use when looking up an entry.
/// @param default_ Default data to return if key was not found.
/// @return The data stored at const char * if found else default data.
void * hashtable_get(struct hashtable *, const char *, void *);

/// @brief Removes an entry from the Hash Table.
/// @param ht A reference to a constructed HashTable structure.
/// @param key The name to use when looking up an entry.
void hashtable_delete(struct hashtable *, const char *);

/// @brief Retrieves data while also removing it from the Hash Table.
/// @param ht A reference to a constructed HashTable structure.
/// @param key The name to use when looking up an entry.
/// @param default_ Default data to return if key was not found.
/// @return The data stored at const char * if found else default data.
void * hashtable_pop(struct hashtable *, const char *, void *);

#endif  // !NDG_HASHTABLE_H_
