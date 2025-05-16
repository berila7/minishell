#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <string.h>
#include <unistd.h>
#include <execinfo.h> // Include for backtrace functions

#define RESET       "\033[0m"
#define RED         "\033[31m"
#define GREEN       "\033[32m"
#define CYAN        "\033[36m"

// Tracks which malloc call we're on
static int malloc_call_index = 0;
// Which call to fail (set to -1 to disable failure injection)
static int fail_index = -1;
// Whether to print debug info
static int debug_output = 0;

// Function pointer to the real malloc
static void* (*real_malloc)(size_t) = NULL;

// Function to print the backtrace
void print_backtrace() {
    void* buffer[128];
    int nptrs = backtrace(buffer, 128);
    char** symbols = backtrace_symbols(buffer, nptrs);
    if (symbols) {
        fprintf(stderr, RED "[malloc_override] Backtrace of the failed malloc:\n" RESET);
        for (int i = 0; i < nptrs; i++) {
            fprintf(stderr, CYAN "  %s\n" RESET, symbols[i]);
        }
        free(symbols);
    } else {
        fprintf(stderr, RED "[malloc_override] Failed to retrieve backtrace symbols.\n" RESET);
    }
}

// Our replacement malloc function
void* malloc(size_t size) {
    // If this is the first call, get the real malloc
    if (!real_malloc) {
        real_malloc = dlsym(RTLD_NEXT, "malloc");
        if (!real_malloc) {
            fprintf(stderr, "Error: could not find real malloc\n");
            exit(1);
        }
    }

    // Increment the call counter
    int current_index = malloc_call_index++;
    
    // Check if this is the call we want to fail
    if (current_index == fail_index) {
        if (debug_output) {
            fprintf(stderr, "[malloc_override] Injecting malloc failure at call index %d (size: %zu)\n", 
                    current_index, size);
        }
        print_backtrace(); // Print the backtrace for the failed malloc
        return NULL; // Simulate allocation failure
    }
    
    // Otherwise, call the real malloc
    void* ptr = real_malloc(size);
    if (debug_output) {
        fprintf(stderr, "[malloc_override] malloc(%zu) = %p (index: %d)\n", 
                size, ptr, current_index);
    }
    return ptr;
}

// Setup function that runs before main
__attribute__((constructor))
void setup_malloc_hook() {
    // Get the malloc failure index from environment variable
    char* fail_index_str = getenv("MALLOC_FAIL_INDEX");
    if (fail_index_str) {
        fail_index = atoi(fail_index_str);
        if (fail_index >= 0) {
            fprintf(stderr, RED "[malloc_override] Will fail malloc call at index %d\n========================================================================\n" RESET, fail_index);
        }
    }
    
    // Check if debug output is enabled
    char* debug_str = getenv("MALLOC_DEBUG");
    if (debug_str && (strcmp(debug_str, "1") == 0 || strcmp(debug_str, "yes") == 0 || 
                      strcmp(debug_str, "true") == 0 || strcmp(debug_str, "on") == 0)) {
        debug_output = 1;
        fprintf(stderr, "[malloc_override] Debug output enabled\n");
    }
}