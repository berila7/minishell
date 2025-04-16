#!/bin/bash
# Reference tests - run this in bash first to see correct behavior
# Then run the same commands in your shell and compare

echo "===== BASIC VARIABLE EXPANSION ====="
echo "1. Simple variable: $USER"
echo "2. Variable with text: hello $USER"
echo "3. Adjacent variables: $USER$HOME"
echo "4. Non-existent variable: $NONEXISTENT"
echo "5. Special exit code: $?"

echo -e "\n===== ESCAPED VARIABLES ====="
echo "6. Escaped variable: \$USER"
echo "7. Double escaped: \\$USER"
echo "8. Triple escaped: \\\$USER"
echo "9. Escaped non-var: \$"
echo "10. Escaped+char: \$+"

echo -e "\n===== QUOTE HANDLING ====="
echo "11. Single quotes: '$USER'"
echo "12. Double quotes: \"$USER\""
echo "13. Mixed quotes 1: \"'$USER'\""
echo "14. Mixed quotes 2: '\"$USER\"'"

echo -e "\n===== BACKSLASH BEHAVIOR ====="
echo "15. Escaped space: hello\ world"
echo "16. Escaped quote 1: hello \"world\""
echo "17. Escaped quote 2: hello \'world\'"
echo "18. Escaped newline: hello\
world"

echo -e "\n===== COMPLEX COMBINATIONS ====="
echo "19. Escape in double quotes: \"\$USER\""
echo "20. Double escape in quotes: \"\\$USER\""
echo "21. Backslash + single quotes: '\$USER'"
echo "22. Nested quotes with escape: \"'\$USER'\""
echo "23. Escaped double quotes: \"\\\"$USER\\\"\""
echo "24. Special chars: \"\$USER:\$HOME\""

# Edge cases to test manually
echo "Edge case 1: $"              # Lone dollar sign
echo "Edge case 2: \ "             # Backslash + space
echo "Edge case 3: \\"             # Double backslash
echo "Edge case 4: $?"             # Exit code
echo "Edge case 5: \"\\$USER\""    # Nested escapes
echo "Edge case 6: '\\$USER'"      # Backslash in single quotes
echo "Edge case 7: \\\"$USER\\\""  # Escaped quotes outside quotes