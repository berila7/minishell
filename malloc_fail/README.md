# Testing Malloc Failure Handling

This tool allows you to test how your C programs handle malloc failures without modifying your source code. It works by intercepting malloc calls and forcing specific calls to return NULL.

## Files

- `malloc_override.c` - The library that intercepts malloc calls
- `malloc_fail` - A shell script to easily run tests with malloc failure injection

## Getting Started

1. Make the script executable:

```bash
chmod +x malloc_fail
```

2. Use the script directly - it will automatically compile the library if needed:

```bash
./malloc_fail -i 2 ./your_program arg1 arg2
```

That's it! The script handles everything else.

## Usage

```
Usage: ./malloc_fail [-d] [-i INDEX] program [arguments...]

Options:
  -i INDEX   Fail the malloc call at the specified index (starting from 0)
  -d         Enable debug output (shows all malloc calls)
  -h         Display this help message
```

### Examples

Test the third malloc call (index 2):
```bash
./malloc_fail -i 2 ./your_program
```

Show all malloc calls and fail the fifth one (index 4):
```bash
./malloc_fail -d -i 4 ./your_program arg1 arg2
```

## How It Works

1. The script compiles a shared library (`libmalloc_override.so`) if it doesn't exist.
2. It sets environment variables to control which malloc call should fail.
3. It uses `LD_PRELOAD` to inject the library into your program.
4. The library intercepts all malloc calls and makes the specified one fail.

## Testing Strategies

### Automated Testing

Create a script to test all possible malloc failure points:

```bash
#!/bin/bash
# test_all_mallocs.sh
PROGRAM="./your_program"
MAX_INDEX=20  # Test up to 20 malloc calls

for i in $(seq 0 $MAX_INDEX); do
  echo "Testing malloc failure at index $i"
  ./malloc_fail -i $i $PROGRAM > /dev/null 2>&1
  EXIT_CODE=$?
  if [ $EXIT_CODE -eq 0 ]; then
    echo "  ✓ Program handled the failure correctly"
  else
    echo "  ✗ Program failed with exit code $EXIT_CODE"
  fi
done
```

### Finding All Malloc Calls

To discover how many malloc calls your program makes:

```bash
./malloc_fail -d ./your_program | grep "malloc_override" | wc -l
```

## Notes

- The tool works with programs that use malloc directly or indirectly (through other standard library functions).
- Some system libraries might use malloc internally, so the first few malloc calls might not be from your code.
- The library does not intercept calloc, realloc, or aligned_alloc calls by default.