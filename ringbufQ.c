
#include <stdio.h>
#include <stdint.h>

#define BUFFER_SIZE 8U
#define INDEX_MASK (BUFFER_SIZE - 1U)

#define STATUS_OK 0
#define STATUS_ERROR (-1)

typedef struct
{
    uint8_t data[BUFFER_SIZE];
    uint8_t head;
    uint8_t tail;
    uint8_t count;
} RingBuffer;

/* Initialize the buffer to an empty state. */
void ringbuf_init(RingBuffer *rb)
{
    uint8_t i;

    for (i = 0U; i < BUFFER_SIZE; i++)
    {
        rb->data[i] = 0U;
    }

    rb->head = 0U;
    rb->tail = 0U;
    rb->count = 0U;
}

/* Write one byte. Fail if the buffer is full. */
int ringbuf_write(RingBuffer *rb, uint8_t value)
{
    if (rb->count == BUFFER_SIZE)
    {
        return STATUS_ERROR;
    }

    rb->data[rb->head] = value;

    /*
     * Bitwise AND replaces modulo for index wrapping.
     * This is efficient on MCUs without a hardware divider.
     * It works because BUFFER_SIZE is a power of two.
     */
    rb->head = (uint8_t)((rb->head + 1U) & INDEX_MASK);
    rb->count++;

    return STATUS_OK;
}

/* Read one byte. Fail if the buffer is empty. */
int ringbuf_read(RingBuffer *rb, uint8_t *value)
{
    if (rb->count == 0U)
    {
        return STATUS_ERROR;
    }

    *value = rb->data[rb->tail];
    rb->tail = (uint8_t)((rb->tail + 1U) & INDEX_MASK);
    rb->count--;

    return STATUS_OK;
}

/* Return the number of stored bytes. */
uint8_t ringbuf_count(const RingBuffer *rb)
{
    return rb->count;
}

/* Return 1 if full, otherwise 0. */
int ringbuf_is_full(const RingBuffer *rb)
{
    return (rb->count == BUFFER_SIZE) ? 1 : 0;
}

/* Return 1 if empty, otherwise 0. */
int ringbuf_is_empty(const RingBuffer *rb)
{
    return (rb->count == 0U) ? 1 : 0;
}

int main(void)
{
    RingBuffer rb;
    uint8_t value;
    uint8_t i;
    const uint8_t initial_data[] = {
        0x41U, 0x42U, 0x43U, 0x44U,
        0x45U, 0x46U, 0x47U, 0x48U
    };
    const uint8_t new_data[] = {
        0x49U, 0x4AU, 0x4BU
    };

    ringbuf_init(&rb);

    printf("=== Ring Buffer Demonstration ===\n");

    /* Step 1: Fill the buffer with eight bytes. */
    for (i = 0U; i < BUFFER_SIZE; i++)
    {
        if (ringbuf_write(&rb, initial_data[i]) == STATUS_OK)
        {
            printf("[WRITE] 0x%02X -> OK (count=%u)",
                   (unsigned int)initial_data[i],
                   (unsigned int)ringbuf_count(&rb));

            if (ringbuf_is_full(&rb))
            {
                printf(" FULL");
            }

            printf("\n");
        }
    }

    /* Step 2: Attempt to write into a full buffer. */
    if (ringbuf_write(&rb, 0x99U) != STATUS_OK)
    {
        printf("[WRITE] 0x99 -> FAIL (buffer full)\n");
    }

    /* Step 3: Read the first three bytes. */
    for (i = 0U; i < 3U; i++)
    {
        if (ringbuf_read(&rb, &value) == STATUS_OK)
        {
            printf("[READ]    -> 0x%02X (count=%u)\n",
                   (unsigned int)value,
                   (unsigned int)ringbuf_count(&rb));
        }
    }

    /* Step 4: Reuse the three freed slots. */
    for (i = 0U; i < 3U; i++)
    {
        if (ringbuf_write(&rb, new_data[i]) == STATUS_OK)
        {
            printf("[WRITE] 0x%02X -> OK (count=%u)\n",
                   (unsigned int)new_data[i],
                   (unsigned int)ringbuf_count(&rb));
        }
    }

    printf("Buffer full: %s, count=%u\n",
           ringbuf_is_full(&rb) ? "YES" : "NO",
           (unsigned int)ringbuf_count(&rb));

    /* Step 5: Read all eight remaining bytes. */
    for (i = 0U; i < BUFFER_SIZE; i++)
    {
        if (ringbuf_read(&rb, &value) == STATUS_OK)
        {
            printf("[READ]    -> 0x%02X (count=%u)\n",
                   (unsigned int)value,
                   (unsigned int)ringbuf_count(&rb));
        }
    }

    printf("Buffer empty: %s, count=%u\n",
           ringbuf_is_empty(&rb) ? "YES" : "NO",
           (unsigned int)ringbuf_count(&rb));

    /* Step 6: Attempt to read from an empty buffer. */
    if (ringbuf_read(&rb, &value) != STATUS_OK)
    {
        printf("[READ] (empty) -> FAIL (buffer empty)\n");
    }

    return 0;
}
