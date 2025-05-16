#!/bin/bash

# Usage:
# ./find_malloc_fail.sh [--max MAX_INDEX] [--msg "Break Message"] [--makeargs "other make args"]
#
# Defaults:
#   --max:   100
#   --msg:   "My Allocation Error"
#   --makeargs: (empty)
#
# Example:
# ./find_malloc_fail.sh --max 50 --msg "My Allocation Error" --makeargs "DEBUG=1"

# Default values
MAX=10
MSG="My Allocation Error"
MAKEARGS=""

# Parse arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        --max)
            MAX="$2"
            shift 2
            ;;
        --msg)
            MSG="$2"
            shift 2
            ;;
        --makeargs)
            MAKEARGS="$2"
            shift 2
            ;;
        *)
            echo "Unknown argument: $1"
            exit 1
            ;;
    esac
done

found=0

for ((i=0; i<=MAX; i++)); do
    echo "Testing N=$i"
    # Run the build and capture output (stdout + stderr)
    output=$( (make re N=$i $MAKEARGS && ./minishell) 2>&1 )
    last_line=$(echo "$output" | tail -n 1)
    echo "=========> Last Line: '$last_line'"
    if [ "$last_line" == "$MSG" ]; then
        echo "Found fail index: $i (matched message: '$MSG')"
        found=1
        break
    fi
done

if [ "$found" -eq 0 ]; then
    echo "Did not find a fail index (no match for message: '$MSG' in the last line up to N=$MAX)"
fi