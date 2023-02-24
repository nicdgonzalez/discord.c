#include "hashtable.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef uint64_t HashCode;

static HashCode hash_function(struct hashtable *ht, TableKey key) {
    uint64_t hash = 1;

    while (*key != '\0') {
        hash *= 163;
        hash += (uint64_t) *(uint8_t *) key++;
    }

    return (hash %= ht->capacity);
}

static bool is_prime(uint64_t value) {
    uint32_t i = 2;

    while ((value > 2) && (i < (value / 2)) && ((value % i) != 0)) {
        i += 1;
    }

    return (i >= (value / 2));
}

static uint64_t get_next_prime(uint64_t value) {
    value += 1 * (value % 2 == 0);

    while (!is_prime(value)) {
        value += 2;
    }

    return value;
}

static void init(struct hashtable *ht, uint64_t max_capacity) {
    ht->capacity = get_next_prime(max_capacity);
    ht->count = 0;
    ht->table = (struct bucket *) calloc(ht->capacity, sizeof(struct bucket));

    for (int32_t index = ht->capacity; index--; ) {
        ht->table[index].previous = &(ht->table[index]);
        ht->table[index].next = (struct bucket *) NULL;
        ht->table[index].key = (const char *) NULL;
        ht->table[index].value = NULL;
    }

    return;
}

void hashtable_new(struct hashtable *ht) {
    init(ht, 7);
    return;
}

void hashtable_free(struct hashtable *ht) {
    free(ht->table);
    return;
}

void hashtable_insert(struct hashtable *ht, TableKey key, TableValue value) {
    HashCode index = hash_function(ht, key);
    struct bucket *bucket = &ht->table[index];

    if (bucket->key == NULL) {
        bucket->previous = bucket;
        bucket->next = (struct bucket *) NULL;
        bucket->key = key;
        bucket->value = value;
    }

    while ((bucket->key != key) && (bucket->next != (struct bucket *) NULL)) {
        bucket = bucket->next;

        if (bucket->key == key) {
            bucket->value = value;
        }
    }

    if ((bucket->key != key) && (bucket->next == NULL)) {
        bucket->next->previous = bucket;
        bucket->next->next = (struct bucket *) NULL;
        bucket->next->key = key;
        bucket->next->value = value;
    }

    if (ht->count < (ht->capacity * 0.75)) {
        return;
    }

    struct hashtable new_ht;
    init(&new_ht, (ht->capacity * 2));

    for (uint64_t index = ht->capacity; index--; ) {
        *bucket = ht->table[index];

        while (
                (bucket->key != (TableValue) NULL)
                | (bucket->next != (struct bucket *) NULL)
        ) {
            if (bucket->key != (TableKey) NULL) {
                hashtable_insert(&new_ht, bucket->key, bucket->value);
            }

            if (bucket->next == (struct bucket *) NULL) {
                break;
            }

            *bucket = *bucket->next;
        }
    }

    struct hashtable temp = *ht;
    *ht = new_ht;
    new_ht = temp;

    return;
}

TableValue hashtable_get(
    struct hashtable *ht,
    TableKey key,
    TableValue _default
) {
    HashCode index = hash_function(ht, key);
    struct bucket *bucket = &ht->table[index];

    while (bucket != NULL) {
        if (bucket->key == key) {
            return bucket->value;
        }

        *bucket = *bucket->next;
    }

    return _default;
}

void hashtable_delete(struct hashtable *ht, TableKey key) {
    HashCode index = hash_function(ht, key);
    struct bucket *bucket = &ht->table[index];

    while (1) {
        if ((bucket->key != key) && (bucket->next == (struct bucket *) NULL)) {
            break;
        }

        if (bucket->key == key) {
            bucket->key = (TableKey) NULL;
            bucket->value = (TableValue) NULL;

        if (bucket->next != (struct bucket *) NULL) {
            if (bucket->previous == bucket) {
                *bucket->next->previous = *bucket->next;
            }

            *bucket->next->previous = *bucket->previous;
            *bucket = *bucket->next;
        }

        break;
        }

        *bucket = *bucket->next;
    }

    return;
}

TableValue hashtable_pop(
    struct hashtable *ht,
    TableKey key,
    TableValue _default
) {
    TableValue value = hashtable_get(ht, key, _default);
    hashtable_delete(ht, key);

    return value;
}
