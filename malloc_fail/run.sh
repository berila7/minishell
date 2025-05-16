#!/bin/bash

#
# ╔╦╗╔═╗╦  ╦  ╔═╗╔═╗  ╔═╗╔═╗╦╦    
# ║║║╠═╣║  ║  ║ ║║    ╠╣ ╠═╣║║    
# ╩ ╩╩ ╩╩═╝╩═╝╚═╝╚═╝  ╚  ╩ ╩╩╩═╝  
#
# A stylish script to test malloc failure handling with backtrace functionality
# Version 1.1.0
#

# Terminal colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
CYAN='\033[0;36m'
BOLD='\033[1m'
RESET='\033[0m'

set -e  # Exit on error

# Default values
FAIL_INDEX=-1
DEBUG_OUTPUT=0
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
LIB_PATH="$SCRIPT_DIR/libmalloc_override.so"

# Function to display usage with colorful output
show_usage() {
    echo -e "${BOLD}╔════════════════════════════════════════════════════════════════════╗${RESET}"
    echo -e "${BOLD}║${RESET}                     ${CYAN}MALLOC FAILURE TESTER${RESET}                         ${BOLD}║${RESET}"
    echo -e "${BOLD}╚════════════════════════════════════════════════════════════════════╝${RESET}"
    echo
    echo -e "${BOLD}Usage:${RESET} $0 ${GREEN}[-d]${RESET} ${GREEN}[-i INDEX]${RESET} ${YELLOW}program [arguments...]${RESET}"
    echo
    echo -e "${BOLD}Options:${RESET}"
    echo -e "  ${GREEN}-i INDEX${RESET}   Fail the malloc call at the specified index (starting from 0)"
    echo -e "  ${GREEN}-d${RESET}         Enable debug output (shows all malloc calls and backtrace on failure)"
    echo -e "  ${GREEN}-h${RESET}         Display this help message"
    echo
    echo -e "${BOLD}Example:${RESET}"
    echo -e "  $0 ${GREEN}-i 3${RESET} ${YELLOW}./my_program arg1 arg2${RESET}"
    echo -e "  ${CYAN}This will run my_program and make the 4th malloc call (index 3) return NULL with a backtrace.${RESET}"
    echo
}

# Function to display a spinner during compilation
show_spinner() {
    local pid=$1
    local delay=0.1
    local spinstr='|/-\'
    while [ "$(ps a | awk '{print $1}' | grep $pid)" ]; do
        local temp=${spinstr#?}
        printf " [%c]  " "$spinstr"
        local spinstr=$temp${spinstr%"$temp"}
        sleep $delay
        printf "\b\b\b\b\b\b"
    done
    printf "    \b\b\b\b"
}

# Function to check if library exists and build it if needed
ensure_library() {
    if [ ! -f "$LIB_PATH" ]; then
        echo -e "${YELLOW}⚙️ Library $LIB_PATH not found. Building it now...${RESET}"
        if ! command -v gcc >/dev/null 2>&1; then
            echo -e "${RED}❌ Error: gcc compiler not found. Please install gcc to use this tool.${RESET}"
            exit 1
        fi
        
        if [ ! -f "$SCRIPT_DIR/malloc_override.c" ]; then
            echo -e "${RED}❌ Error: Source file malloc_override.c not found in $SCRIPT_DIR${RESET}"
            exit 1
        fi
        
        echo -ne "${CYAN}🔧 Compiling library... ${RESET}"
        gcc -shared -fPIC "$SCRIPT_DIR/malloc_override.c" -o "$LIB_PATH" -ldl -rdynamic > /dev/null 2>&1 &
        show_spinner $!
        
        if [ $? -ne 0 ]; then
            echo -e "${RED}❌ Error: Failed to compile the library${RESET}"
            exit 1
        fi
        echo -e "${GREEN}✅ Library built successfully!${RESET}"
    fi
}

# Function to print a horizontal line
print_line() {
    echo -e "${BOLD}────────────────────────────────────────────────────────────────────${RESET}"
}

# Parse command-line options
while getopts "i:dh" opt; do
    case $opt in
        i)
            FAIL_INDEX=$OPTARG
            if ! [[ "$FAIL_INDEX" =~ ^[0-9]+$ ]]; then
                echo -e "${RED}❌ Error: Fail index must be a non-negative integer${RESET}"
                exit 1
            fi
            ;;
        d)
            DEBUG_OUTPUT=1
            ;;
        h)
            show_usage
            exit 0
            ;;
        \?)
            echo -e "${RED}❌ Invalid option: -$OPTARG${RESET}" >&2
            show_usage
            exit 1
            ;;
    esac
done

# Skip past the options
shift $((OPTIND-1))

# Check if a program was specified
if [ $# -eq 0 ]; then
    echo -e "${RED}❌ Error: No program specified${RESET}"
    show_usage
    exit 1
fi

# Ensure the library exists
ensure_library

# Export environment variables for the library
export MALLOC_FAIL_INDEX=$FAIL_INDEX
if [ $DEBUG_OUTPUT -eq 1 ]; then
    export MALLOC_DEBUG=1
fi

# Create the LD_PRELOAD value
if [ -n "$LD_PRELOAD" ]; then
    export LD_PRELOAD="$LIB_PATH:$LD_PRELOAD"
else
    export LD_PRELOAD="$LIB_PATH"
fi

# Display test configuration
print_line
echo -e "${BOLD}🚀 Running with these settings:${RESET}"
echo -e "${BOLD}Program:${RESET}        ${YELLOW}$1${RESET}"
if [ $FAIL_INDEX -ge 0 ]; then
    echo -e "${BOLD}Fail Index:${RESET}     ${RED}$FAIL_INDEX${RESET} (will make malloc call #$FAIL_INDEX return NULL)"
else
    echo -e "${BOLD}Fail Index:${RESET}     ${GREEN}None${RESET} (normal execution)"
fi
echo -e "${BOLD}Debug Output:${RESET}    $([ $DEBUG_OUTPUT -eq 1 ] && echo "${CYAN}Enabled${RESET}" || echo "Disabled")"
print_line
echo

# Run the program with the remaining arguments
echo -e "${GREEN}▶️  Starting program...${RESET}"
echo
exec "$@"