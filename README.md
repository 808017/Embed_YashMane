
# Embed_YashMane

**Candidate:** Yash Mane

## Assignment: Embedded Developer Fresher Assessment

### Project: Ring Buffer (Circular Buffer)

A fixed-capacity, 8-byte circular buffer implemented in standard C (C99). It supports writing and reading bytes in FIFO order, checking buffer status, and handling full and empty conditions.

## Features

- Initialize the buffer to an empty state.
- Write a byte into the buffer.
- Read a byte from the buffer.
- Prevent overwriting unread data when the buffer is full.
- Prevent reading invalid data when the buffer is empty.
- Query the current number of stored bytes.
- Check whether the buffer is full or empty.
- Use bitwise AND for efficient index wrap-around when the buffer size is a power of two.

## Requirements

- GCC compiler
- C99-compatible environment
- No external libraries

## Compile

Run this command in the project directory:

```bash
gcc -Wall -std=c99 ringbufQ.c -o ringbuf.exe
```

## Run

On Windows with MinGW / MSYS2 UCRT64:

```bash
./ringbuf.exe
```

## Expected Behavior

The program demonstrates:

1. Writing 8 bytes into the buffer.
2. Rejecting a write when the buffer is full.
3. Reading the first 3 bytes in FIFO order.
4. Writing 3 new bytes into the freed slots.
5. Reading all remaining bytes.
6. Rejecting a read when the buffer is empty.

## Project Files

- `ringbufQ.c` - Ring buffer implementation and test demonstration.
- `README.md` - Project description and build instructions.

## Author

Yash Mane
