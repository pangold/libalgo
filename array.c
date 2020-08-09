#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "array.h"

#ifndef NULL
#define NULL (void *)0
#endif
#define ARRAY_INIT_LEN 10
#define ARRAY_SIZE(n)   (array->sizeof_type * (n))         /* size of bits */
#define ARRAY_LSIZE(n)  (array->size - ARRAY_SIZE(n))      /* bits left */
#define ARRAY_INDEX(n)  (array->data + ARRAY_SIZE(n))
#define ARRAY_LINDEX(n) (array->data + ARRAY_LSIZE(n))
#define ARRAY_LENGTH    (array->size / array->sizeof_type)

struct array {
    char  *data;
    size_t sizeof_type;
    size_t size;
    size_t capacity;
};

inline static array_t *__array_alloc(size_t sizeof_type)
{
    array_t *array;
    array = (array_t *)malloc(sizeof(array_t));
    if (!array) return NULL;
    array->sizeof_type = sizeof_type;
    array->size = 0;
    array->capacity = sizeof_type * ARRAY_INIT_LEN;
    array->data = (char *)malloc(array->capacity);
    return array;
}

inline static void __array_free(array_t *array)
{
    if (array->data) free(array->data);
    array->data = NULL;
    free(array);
}

inline static void __array_clear(array_t *array)
{
    array->size = 0;
}

inline static void __array_push_at(array_t *array, void *data, size_t pos)
{
    if (array->size + array->sizeof_type * pos >= array->capacity) {
        array->capacity *= 2;
        array->data = (char *)realloc(array->data, array->capacity);
    }
    memmove(ARRAY_INDEX(pos + 1), ARRAY_INDEX(pos), ARRAY_LSIZE(pos));
    memcpy(ARRAY_INDEX(pos), data, array->sizeof_type);
    array->size += array->sizeof_type;
}

inline static void __array_push_front(array_t *array, void *data)
{
    __array_push_at(array, data, 0);
}

inline static void __array_push_back(array_t *array, void *data)
{
    if (array->size == array->capacity) {
        array->capacity *= 2;
        array->data = (char *)realloc(array->data, array->capacity);
    }
    memcpy(array->data + array->size, data, array->sizeof_type);
    array->size += array->sizeof_type;
}

inline static void __array_pop_at(array_t *array, void *data, size_t pos)
{
    if (array->size == 0) return;
    if (data) memcpy(data, ARRAY_INDEX(pos), array->sizeof_type);
    memmove(ARRAY_INDEX(pos), ARRAY_INDEX(pos + 1), ARRAY_LSIZE(pos + 1));
    array->size -= array->sizeof_type;
}

inline static void __array_pop_front(array_t *array, void *data)
{
    __array_pop_at(array, data, 0);
}

inline static void __array_pop_back(array_t *array, void *data)
{
    if (array->size == 0) return;
    if (data) memcpy(data, array->data + array->size - array->sizeof_type, array->sizeof_type);
    array->size -= array->sizeof_type;
}

inline void __array_swap(array_t *array, size_t m, size_t n)
{
    char *temp = (char *)malloc(array->sizeof_type);
    memcpy(temp, ARRAY_INDEX(m), array->sizeof_type);
    memcpy(ARRAY_INDEX(m), ARRAY_INDEX(n), array->sizeof_type);
    memcpy(ARRAY_INDEX(n), temp, array->sizeof_type);
    free(temp);
}

static void __array_reverse(array_t *array)
{
    size_t len = ARRAY_LENGTH;
    size_t half = len / 2;
    for (int i = 0; i < half; i++) {
        __array_swap(array, i, len - i - 1);
    }
}

static void __array_bubble_sort(array_t *array, int(*compare)(void *, void *))
{
    size_t i, j;
    size_t len = ARRAY_LENGTH;
    for (i = 0; i < len; i++) {
        for (j = 0; j < len - 1 - i; j++) {
            if (compare(ARRAY_INDEX(j), ARRAY_INDEX(j + 1)) == 0) {
                __array_swap(array, j, j + 1);
            }
        }
    }
}

static void __array_select_sort(array_t *array, int(*compare)(void *, void *))
{
    size_t i, j;
    size_t len = ARRAY_LENGTH;
    for (i = 0; i < len - 1; i++) {
        int index = i;
        for (j = i + 1; j < len; j++) {
            if (compare(ARRAY_INDEX(j), ARRAY_INDEX(index)) != 0) {
                index = j;
            }
        }
        if (index != i) {
            __array_swap(array, index, i);
        }
    }
}

static void __array_insert_sort(array_t *array, int(*compare)(void *, void *))
{
    size_t i, j;
    size_t len = ARRAY_LENGTH;
    char *data = (char *)malloc(array->sizeof_type);
    for (i = 1; i < len; i++) {
        memcpy(data, ARRAY_INDEX(i), array->sizeof_type);
        for (j = i - 1; j != (0 - 1) && compare(data, ARRAY_INDEX(j)) != 0; j--) {
            memcpy(ARRAY_INDEX(j + 1), ARRAY_INDEX(j), array->sizeof_type);
        }
        memcpy(ARRAY_INDEX(j + 1), data, array->sizeof_type);
    }
    free(data);
}

static void __array_shell_sort(array_t *array, int(*compare)(void *, void *))
{
    long i, j, gap = 1;
    size_t len = ARRAY_LENGTH;
    char *data = (char *)malloc(array->sizeof_type);
    while (gap < len / 3) gap = gap * 3 + 1;
    for (gap; gap > 0; gap = gap / 3) {
        for (i = gap; i < len; i++) {
            memcpy(data, ARRAY_INDEX(i), array->sizeof_type);
            for (j = i - gap; j >= 0 && compare(data, ARRAY_INDEX(j)) != 0; j -= gap) {
                memcpy(ARRAY_INDEX(j + gap), ARRAY_INDEX(j), array->sizeof_type);
            }
            memcpy(ARRAY_INDEX(j + gap), data, array->sizeof_type);
        }
    }
    free(data);
}

static void __array_quick_sort_partition(array_t *array, int(*compare)(void *, void *), int start, int end) 
{
    int left = start;
    int right = end - 1;
    if (start >= end) return;
    while (left < right) {
        while (left < right && compare(ARRAY_INDEX(left), ARRAY_INDEX(end)) != 0) 
            left++;
        while (left < right && compare(ARRAY_INDEX(right), ARRAY_INDEX(end)) == 0) 
            right--;
        if (left != right) 
            __array_swap(array, left, right);
    }
    if (compare(ARRAY_INDEX(left), ARRAY_INDEX(end)) == 0) {
        __array_swap(array, left, end);
    } else if (left < right) {
        left++;
    }
    if (start < left) {
        __array_quick_sort_partition(array, compare, start, left);
    }
    if (left + 1 < end) {
        __array_quick_sort_partition(array, compare, left + 1, end);
    }
}

static void __array_quick_sort(array_t *array, int(*compare)(void *, void *)) 
{
    __array_quick_sort_partition(array, compare, 0, ARRAY_LENGTH - 1);
}

static void __array_merge(array_t *array, char *reg, int(*compare)(void *, void *), size_t start, size_t end)
{
    if (start >= end) return;
    size_t len = end - start, mid = (len >> 1) + start;
    size_t start1 = start, end1 = mid;
    size_t start2 = mid + 1, end2 = end;
    size_t k = start;
    if (start1 < end1) __array_merge(array, reg, compare, start1, end1);
    if (start2 < end2) __array_merge(array, reg, compare, start2, end2);
#define REG_INDEX(n) (reg + array->sizeof_type * n)
    while (start1 <= end1 && start2 <= end2) {
        char *from = compare(ARRAY_INDEX(start1), ARRAY_INDEX(start2)) != 0
            ? ARRAY_INDEX(start1++) 
            : ARRAY_INDEX(start2++);
        memcpy(REG_INDEX(k++), from, array->sizeof_type);
    }
    if (start1 <= end1) {
        memcpy(REG_INDEX(k), ARRAY_INDEX(start1), ARRAY_SIZE(end1 - start1 + 1));
        k += (end1 - start1 + 1);
    }
    if (start2 <= end2) {
        memcpy(REG_INDEX(k), ARRAY_INDEX(start2), ARRAY_SIZE(end2 - start2 + 1));
        k += (end2 - start2 + 1);
    }
    memcpy(ARRAY_INDEX(start), REG_INDEX(start), ARRAY_SIZE(len + 1));
}

static void __array_merge_sort(array_t *array, int(*compare)(void *, void *))
{
    char *reg = (char *)malloc(array->size);
    __array_merge(array, reg, compare, 0, ARRAY_LENGTH - 1);
    free(reg);
}

array_t *array_alloc(size_t sizeof_type)
{
    return __array_alloc(sizeof_type);
}

void array_free(array_t *array)
{
    __array_free(array);
}

void array_clear(array_t *array)
{
    __array_clear(array);
}

int array_size(array_t *array)
{
    return ARRAY_LENGTH;
}

void array_reverse(array_t *array)
{
    __array_reverse(array);
}

void array_sort(array_t *array, int(*compare)(void *, void *))
{
#if defined(USE_BUBBLE_SORT)
    __array_bubble_sort(array, compare);
#elif defined(USE_SELECT_SORT)
    __array_select_sort(array, compare);
#elif defined(USE_INSERT_SORT)
    __array_insert_sort(array, compare);
#elif defined(USE_QUICK_SORT)
    __array_quick_sort(array, compare);
#elif defined(USE_SHELL_SORT)
    __array_shell_sort(array, compare);
#elif defined(USE_MERGE_SORT)
    __array_merge_sort(array, compare);
#else
    __array_merge_sort(array, compare);
#endif
}

void array_push_back(array_t *array, void *data)
{
    __array_push_back(array, data);
}

void array_push_front(array_t *array, void *data)
{
    __array_push_front(array, data);
}

void array_push_at(array_t *array, void *data, int n)
{
    __array_push_at(array, data, n);
}

void array_pop_back(array_t *array)
{
    __array_pop_back(array, NULL);
}

void array_pop_front(array_t *array)
{
    __array_pop_front(array, NULL);
}

void array_pop_at(array_t *array, int n)
{
    __array_pop_at(array, NULL, n);
}

void *array_data(array_t *array)
{
    return array->data;
}

void *array_at(array_t *array, int n)
{
    return ARRAY_INDEX(n);
}

void array_foreach(array_t *array, int(*cb)(void *, void *), void *arg)
{
    for (int i = 0; i < array->size; i += array->sizeof_type) {
        cb(array->data + i, arg);
    }
}

#if defined(ENABLE_ARRAY_TEST)
int array_int_compare_less(void *i1, void *i2)
{
    return *(int *)i1 < *(int *)i2;
}
int array_int_compare_more(void *i1, void *i2)
{
    return *(int *)i1 > *(int *)i2;
}
void array_int_print(array_t *array)
{
    int *data = (int *)array_data(array);
    size_t size = array_size(array);
    for (size_t i = 0; i < size; i++) {
        printf("%d ", data[i]);
    }
    printf("\n");
}
void array_int_test()
{
    array_t *array = array_alloc(sizeof(int));
    int int_array[] = { 2,5,6,4,3,1,8,9,7,0 };
    array_push_back(array, &int_array[0]);
    array_push_back(array, &int_array[1]);
    array_push_back(array, &int_array[2]);
    array_push_back(array, &int_array[3]);
    array_push_back(array, &int_array[4]);
    array_push_back(array, &int_array[5]);
    array_push_back(array, &int_array[6]);
    array_push_back(array, &int_array[7]);
    array_push_back(array, &int_array[8]);
    array_push_back(array, &int_array[9]);
    array_push_front(array, &int_array[9]);
    array_int_print(array);
    array_pop_front(array);
    array_int_print(array);
    array_push_at(array, &int_array[6], 3);
    array_int_print(array);
    array_pop_at(array, 3);
    array_int_print(array);
    assert(array_size(array) == 10);
    array_pop_back(array);
    array_reverse(array);
    array_int_print(array);
    array_push_back(array, &int_array[9]);
    array_push_front(array, &int_array[9]);
    array_sort(array, array_int_compare_less);
    array_int_print(array);
    array_free(array);
}
#endif
