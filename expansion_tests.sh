#!/bin/bash
# Reference tests - run this in bash first to see correct behavior
# Then run the same commands in your shell and compare


echo $USER ✅
echo "My user is $USER" ✅
echo 'My user is $USER'✅
echo "$USER is my username"✅

echo \"hello\"✅
echo \'world\'✅
echo \$USER✅
echo "\$USER"✅
echo '\$USER'✅

echo "\"$USER\""✅
echo "'$USER'"✅
echo '"$USER"'✅
echo "This is a \"quoted\" word"✅
echo 'This is a \'quoted\' word''✅

echo $123✅
echo $1USER✅
echo ${USER}123 ❌
echo "$1USER"✅
echo "$$"✅
echo "The value is $?"✅

echo "hello\nworld"❌
echo hello\\world✅
echo "hello\\world"✅
echo "hello\tworld"❌
echo "hello\aworld"❌

echo $✅
echo \✅
echo "\\"✅
echo \\\\✅
echo ""✅
echo ''✅
echo "\""✅
echo "'"✅
echo '"'✅
echo '\''✅

echo "\"$USER\" is using '$SHELL'"✅
echo "Path is $PATH"✅
echo 'Path is $PATH'✅
echo "$USER'$USER'$USER"✅
echo "$USER\"hello\"$USER"✅
echo "The date is `date`"❌

echo "Hello World" > "test file.txt"✅
echo 'Hello World' > 'test file2.txt'✅
echo "Quote \"test\"" > quote_file.txt✅
cat "test file.txt"✅

echo "Today is $(date)"❌
echo 'Today is $(date)'✅
echo "User: $USER, Host: $(hostname)"❌

echo "!@#$%^&*()_+"
echo "\!\@\#\$\%\^\&\*\(\)\_\+"❌
echo "Semi-colon: ; Pipe: | Ampersand: &"✅
echo "Dollar \$100 and percent \%20"✅

export EMPTY=""✅
echo "$EMPTY"✅
echo "$EMPTY$USER"✅
echo "$USER$EMPTY"✅

echo "===== BASIC VARIABLE EXPANSION ====="
echo "1. Simple variable: $USER"✅
echo "2. Variable with text: hello $USER"✅
echo "3. Adjacent variables: $USER$HOME"✅
echo "4. Non-existent variable: $NONEXISTENT"✅
echo "5. Special exit code: $?"✅

echo -e "\n===== ESCAPED VARIABLES ====="
echo "6. Escaped variable: \$USER"✅
echo "7. Double escaped: \\$USER"✅
echo "8. Triple escaped: \\\$USER"✅
echo "9. Escaped non-var: \$"✅
echo "10. Escaped+char: \$+"✅

echo -e "\n===== QUOTE HANDLING ====="
echo "11. Single quotes: '$USER'"✅
echo "12. Double quotes: \"$USER\""✅
echo "13. Mixed quotes 1: \"'$USER'\""✅
echo "14. Mixed quotes 2: '\"$USER\"'"✅

echo -e "\n===== BACKSLASH BEHAVIOR ====="
echo "15. Escaped space: hello\ world"❌
echo "16. Escaped quote 1: hello \"world\""✅
echo "17. Escaped quote 2: hello \'world\'"❌
echo "18. Escaped newline: hello\world"❌

echo -e "\n===== COMPLEX COMBINATIONS ====="✅
echo "19. Escape in double quotes: \"\$USER\""✅
echo "20. Double escape in quotes: \"\\$USER\""✅
echo "21. Backslash + single quotes: '\$USER'"✅
echo "22. Nested quotes with escape: \"'\$USER'\""✅
echo "23. Escaped double quotes: \"\\\"$USER\\\"\""✅
echo "24. Special chars: \"\$USER:\$HOME\""✅

# Edge cases to test manually
echo "Edge case 1: $"              # Lone dollar sign✅
echo "Edge case 2: \ "             # Backslash + space❌
echo "Edge case 3: \\"             # Double backslash✅
echo "Edge case 4: $?"             # Exit code✅
echo "Edge case 5: \"\\$USER\""    # Nested escapes✅
echo "Edge case 6: '\\$USER'"      # Backslash in quotes✅
echo "Edge case 7: \\\"$USER\\\""  # Escaped quotes outside✅