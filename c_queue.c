#include "godboltdbj.h"

// There are better ways to do this but this
// stands as pretty much as a 1 to 1 translation of the C++
// C isn't particular good at making generalised code.

#define DECLARE_QUEUE(TYPE, INT_TYPE)                                          \
    typedef struct queue_##TYPE##_##INT_TYPE {                                 \
        TYPE *buffer;                                                          \
        INT_TYPE front;                                                        \
        INT_TYPE back;                                                         \
        INT_TYPE capacity;                                                     \
        INT_TYPE size;                                                         \
    } queue_##TYPE##_##INT_TYPE;                                               \
                                                                               \
    void queue_##TYPE##_##INT_TYPE##_init(                                     \
        queue_##TYPE##_##INT_TYPE *queue_in) {                                 \
        queue_in->buffer = NULL;                                               \
        queue_in->front = 0;                                                   \
        queue_in->back = 0;                                                    \
        queue_in->capacity = 0;                                                \
        queue_in->size = 0;                                                    \
    }                                                                          \
    void queue_##TYPE##_##INT_TYPE##_deinit(                                   \
        queue_##TYPE##_##INT_TYPE *queue_in) {                                 \
        free(queue_in->buffer);                                                \
    }                                                                          \
    void queue_##TYPE##_##INT_TYPE##_should_reallocate(                        \
        queue_##TYPE##_##INT_TYPE *q) {                                        \
        if (q->capacity == q->size) {                                          \
            if (q->capacity == 0)                                              \
                q->capacity = 2;                                               \
            else                                                               \
                q->capacity *= 2;                                              \
                                                                               \
            TYPE *buffer_new = (TYPE *) malloc(sizeof(TYPE) * q->capacity);    \
            if (buffer_new == NULL)                                            \
                abort();                                                       \
                                                                               \
            memcpy(buffer_new, q->buffer, sizeof(TYPE) * q->size);             \
                                                                               \
            free(q->buffer);                                                   \
            q->buffer = buffer_new;                                            \
                                                                               \
            q->front = 0;                                                      \
            q->back = q->size;                                                 \
        }                                                                      \
    }                                                                          \
    void queue_##TYPE##_##INT_TYPE##_push_back(queue_##TYPE##_##INT_TYPE *q,   \
                                               const TYPE *data) {             \
        queue_##TYPE##_##INT_TYPE##_should_reallocate(q);                      \
                                                                               \
        q->buffer[q->back] = *data;                                            \
        q->back = (q->back + 1) % q->capacity;                                 \
        ++(q->size);                                                           \
    }                                                                          \
    void queue_##TYPE##_##INT_TYPE##_push_back_literal(                        \
        queue_##TYPE##_##INT_TYPE *q, TYPE data) {                             \
        queue_##TYPE##_##INT_TYPE##_should_reallocate(q);                      \
                                                                               \
        q->buffer[q->back] = data;                                             \
        q->back = (q->back + 1) % q->capacity;                                 \
        ++(q->size);                                                           \
    }                                                                          \
    void queue_##TYPE##_##INT_TYPE##_pop(queue_##TYPE##_##INT_TYPE *q) {       \
        assert(q->size != 0);                                                  \
                                                                               \
        q->front = (q->front + 1) % q->capacity;                               \
        --(q->size);                                                           \
    }                                                                          \
    TYPE *queue_##TYPE##_##INT_TYPE##_emplace_back(                            \
        queue_##TYPE##_##INT_TYPE *q) {                                        \
        queue_##TYPE##_##INT_TYPE##_should_reallocate(q);                      \
                                                                               \
        TYPE *data = &q->buffer[q->back];                                      \
                                                                               \
        q->back = (q->back + 1) % q->capacity;                                 \
        ++(q->size);                                                           \
        return data;                                                           \
    }                                                                          \
    TYPE *queue_##TYPE##_##INT_TYPE##_iter(queue_##TYPE##_##INT_TYPE *q,       \
                                           INT_TYPE i) {                       \
        assert(i >= 0 && i < q->size);                                         \
                                                                               \
        INT_TYPE index_rolling = (q->front + i) % q->capacity;                 \
        return &q->buffer[index_rolling];                                      \
    }                                                                          \
    TYPE *queue_##TYPE##_##INT_TYPE##_front(queue_##TYPE##_##INT_TYPE *q) {    \
        assert(q->size != 0);                                                  \
                                                                               \
        return &q->buffer[q->front];                                           \
    }                                                                          \
    TYPE *queue_##TYPE##_##INT_TYPE##_back(queue_##TYPE##_##INT_TYPE *q) {     \
        assert(q->size != 0);                                                  \
        INT_TYPE last = (q->back + (q->size - 1)) % q->capacity;               \
        return &q->buffer[last];                                               \
    }                                                                          \
    void queue_##TYPE##_##INT_TYPE##_clear(queue_##TYPE##_##INT_TYPE *q) {     \
        q->front = 0;                                                          \
        q->back = 0;                                                           \
        q->size = 0;                                                           \
    }

/*
*********************************************************************
TESTNG aka POC
**********************************************************************
*/

typedef struct CustomType {
    char a;
    int *b;
    float c;
} CustomType;

DECLARE_QUEUE(int, int)
DECLARE_QUEUE(CustomType, int)

int
example_c(void) {

    queue_int_int q;
    queue_int_int_init(&q);

    queue_int_int_push_back_literal(&q, 10);
    queue_int_int_push_back_literal(&q, 20);
    queue_int_int_push_back_literal(&q, 30);
    int a = 40;
    queue_int_int_push_back(&q, &a);

    queue_int_int_pop(&q);

    for (int i = 0; i < q.size; i++) {
        int *value = queue_int_int_iter(&q, i);
        printf("%d\n", *value);
    }

    while (q.size > 0) {
        int *front = queue_int_int_front(&q);
        printf("%d\n", *front);
        queue_int_int_pop(&q);
    }

    queue_int_int_clear(&q);

    queue_int_int_deinit(&q);

    queue_CustomType_int my_q;
    queue_CustomType_int_init(&my_q);

    CustomType my_type;
    queue_CustomType_int_push_back(&my_q, &my_type);

    queue_CustomType_int_pop(&my_q);

    queue_CustomType_int_deinit(&my_q);

    return 1;
}

int
main(void) {
    return example_c();
}
