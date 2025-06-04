/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _test.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberila <mberila@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 16:50:08 by berila            #+#    #+#             */
/*   Updated: 2025/06/04 11:05:37 by mberila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <dirent.h>
# include <signal.h>
# include <fcntl.h>
# include <termios.h>
# include <limits.h>
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <sys/wait.h>
# include <sys/stat.h> 
# include <string.h>
# include <errno.h>

// static int	word_count(char *str)
// {
// 	int		count;
// 	int		flag_pnt;
// 	size_t	i;

// 	count = 0;
// 	flag_pnt = 0;
// 	i = 0;
// 	while (str[i])
// 	{
// 		if ((str[i] != ' ' || str[i] != '\t') && flag_pnt == 0)
// 		{
// 			flag_pnt = 1;
// 			count++;
// 		}
// 		else if (str[i] == ' ' || str[i] == '\t')
// 			flag_pnt = 0;
// 		i++;
// 	}
// 	return (count);
// }

// static char	*fill_word(char *str, int start, int end)
// {
// 	char	*word;
// 	int		i;

// 	i = 0;
// 	word = malloc((end - start + 1) * sizeof(char));
// 	while (start < end)
// 	{
// 		word[i] = str[start];
// 		i++;
// 		start++;
// 	}
// 	return (word);
// }

// static char	**init_spl(char *s, char **result)
// {
// 	size_t	i;
// 	int		j;
// 	int		s_word;

// 	i = 0;
// 	j = 0;
// 	s_word = -1;
// 	while (i <= strlen(s))
// 	{
// 		if ((s[i] != ' ' && s[i] != '\t') && s_word < 0)
// 			s_word = i;
// 		else if (((s[i] == ' ' || s[i] == '\t') || i == strlen(s)) && s_word >= 0)
// 		{
// 			result[j] = fill_word(s, s_word, i);
// 			s_word = -1;
// 			j++;
// 		}
// 		i++;
// 	}
// 	return (result);
// }

// char	**gc_split(char *s)
// {
// 	char	**res;

// 	if (!s)
// 		return (NULL);
// 	res = malloc((word_count(s) + 1) * sizeof(char *));
// 	return (init_spl(s, res));
// }

// char	*add_quotes_to_str(char *str)
// {
// 	char	*quoted_str;
// 	int		len;

// 	len = strlen(str);
// 	if (!str || !*str)
// 		return (NULL);
// 	quoted_str = malloc(len + 3);
// 	int	i = 0;
// 	int	j = 0;
// 	quoted_str[0] = '\"';
// 	j += 1;
// 	while (str[i])
// 		quoted_str[j++] = str[i++];
// 	quoted_str[len + 1] = '\"';
// 	quoted_str[len + 2] = '\0';
// 	return (quoted_str);
// }

// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <stdbool.h>

// // Structure to hold a single token (quoted or unquoted)
// typedef struct {
//     char *text;
//     bool is_quoted;  // true if this was inside quotes, false otherwise
// } Token;

// // Structure to hold all parsed tokens
// typedef struct {
//     Token *tokens;
//     int count;
//     bool is_valid;
// } ParseResult;

// // Function to parse string with dynamic quote handling
// ParseResult parse_dynamic_quoted_string(char *input) {
//     ParseResult result = {NULL, 0, false};
    
//     if (input == NULL || strlen(input) == 0) {
//         return result;
//     }
    
//     int len = strlen(input);
//     int capacity = 10;  // Initial capacity for tokens array
//     result.tokens = malloc(capacity * sizeof(Token));
//     if (result.tokens == NULL) {
//         return result;
//     }
    
//     int pos = 0;
//     result.count = 0;
    
//     while (pos < len) {
//         // Resize array if needed
//         if (result.count >= capacity) {
//             capacity *= 2;
//             Token *temp = realloc(result.tokens, capacity * sizeof(Token));
//             if (temp == NULL) {
//                 // Cleanup on failure
//                 for (int i = 0; i < result.count; i++) {
//                     free(result.tokens[i].text);
//                 }
//                 free(result.tokens);
//                 result.tokens = NULL;
//                 result.count = 0;
//                 return result;
//             }
//             result.tokens = temp;
//         }
        
//         if (input[pos] == '"') {
//             // Found start of quoted string
//             pos++; // Skip opening quote
//             int start = pos;
            
//             // Find closing quote
//             while (pos < len && input[pos] != '"') {
//                 pos++;
//             }
            
//             if (pos >= len) {
//                 // No closing quote found - treat as error or include remaining text
//                 // For robustness, we'll include the text without quotes
//                 int text_len = len - start;
//                 result.tokens[result.count].text = malloc(text_len + 1);
//                 if (result.tokens[result.count].text != NULL) {
//                     strncpy(result.tokens[result.count].text, input + start, text_len);
//                     result.tokens[result.count].text[text_len] = '\0';
//                     result.tokens[result.count].is_quoted = true;
//                     result.count++;
//                 }
//                 break;
//             } else {
//                 // Found closing quote
//                 int text_len = pos - start;
//                 result.tokens[result.count].text = malloc(text_len + 1);
//                 if (result.tokens[result.count].text != NULL) {
//                     strncpy(result.tokens[result.count].text, input + start, text_len);
//                     result.tokens[result.count].text[text_len] = '\0';
//                     result.tokens[result.count].is_quoted = true;
//                     result.count++;
//                 }
//                 pos++; // Skip closing quote
//             }
//         } else {
//             // Found unquoted text
//             int start = pos;
            
//             // Find next quote or end of string
//             while (pos < len && input[pos] != '"') {
//                 pos++;
//             }
            
//             int text_len = pos - start;
//             if (text_len > 0) {
//                 result.tokens[result.count].text = malloc(text_len + 1);
//                 if (result.tokens[result.count].text != NULL) {
//                     strncpy(result.tokens[result.count].text, input + start, text_len);
//                     result.tokens[result.count].text[text_len] = '\0';
//                     result.tokens[result.count].is_quoted = false;
//                     result.count++;
//                 }
//             }
//         }
//     }
    
//     result.is_valid = true;
//     return result;
// }

// // Function to free the parse result
// void free_parse_result(ParseResult *result) {
//     if (result != NULL && result->tokens != NULL) {
//         for (int i = 0; i < result->count; i++) {
//             free(result->tokens[i].text);
//         }
//         free(result->tokens);
//         result->tokens = NULL;
//         result->count = 0;
//         result->is_valid = false;
//     }
// }

// // Helper function to print parse result
// void print_parse_result(const ParseResult *result, const char *input) {
//     printf("Input: %s\n", input);
//     if (result->is_valid && result->count > 0) {
//         printf("Parsed %d token(s):\n", result->count);
//         for (int i = 0; i < result->count; i++) {
//             printf("  [%d] %s: \"%s\"\n", 
//                    i + 1,
//                    result->tokens[i].is_quoted ? "QUOTED" : "UNQUOTED",
//                    result->tokens[i].text);
//         }
//     } else {
//         printf("No tokens found or parsing failed.\n");
//     }
//     printf("\n");
// }

// // Enhanced function for specific format checking
// bool check_specific_format(const ParseResult *result, const char *expected_quoted, const char *expected_unquoted) {
//     if (!result->is_valid || result->count != 2) {
//         return false;
//     }
    
//     // Check if first token is quoted and matches expected
//     if (!result->tokens[0].is_quoted || strcmp(result->tokens[0].text, expected_quoted) != 0) {
//         return false;
//     }
    
//     // Check if second token is unquoted and matches expected
//     if (result->tokens[1].is_quoted || strcmp(result->tokens[1].text, expected_unquoted) != 0) {
//         return false;
//     }
    
//     return true;
// }

// // Function to get only quoted parts
// char** get_quoted_parts(const ParseResult *result, int *quoted_count) {
//     if (!result->is_valid || result->count == 0) {
//         *quoted_count = 0;
//         return NULL;
//     }
    
//     // Count quoted parts
//     int count = 0;
//     for (int i = 0; i < result->count; i++) {
//         if (result->tokens[i].is_quoted) {
//             count++;
//         }
//     }
    
//     if (count == 0) {
//         *quoted_count = 0;
//         return NULL;
//     }
    
//     // Allocate array for quoted parts
//     char **quoted_parts = malloc(count * sizeof(char*));
//     if (quoted_parts == NULL) {
//         *quoted_count = 0;
//         return NULL;
//     }
    
//     // Copy quoted parts
//     int quoted_index = 0;
//     for (int i = 0; i < result->count; i++) {
//         if (result->tokens[i].is_quoted) {
//             int len = strlen(result->tokens[i].text);
//             quoted_parts[quoted_index] = malloc(len + 1);
//             if (quoted_parts[quoted_index] != NULL) {
//                 strcpy(quoted_parts[quoted_index], result->tokens[i].text);
//                 quoted_index++;
//             }
//         }
//     }
    
//     *quoted_count = count;
//     return quoted_parts;
// }

// // Function to free quoted parts array
// void free_quoted_parts(char **quoted_parts, int count) {
//     if (quoted_parts != NULL) {
//         for (int i = 0; i < count; i++) {
//             free(quoted_parts[i]);
//         }
//         free(quoted_parts);
//     }
// }

// int main() {
//     // Test cases for various formats
//     // const char *test_strings[] = {
//     //     "\"hello\"world",           // Original format
//     //     "hello\"world\"",           // Quote at end
//     //     "hello\"world\"test",       // Quote in middle
//     //     "\"hello\"\"world\"",       // Multiple quoted parts
//     //     "start\"middle\"end",       // Quote in middle with text on both sides
//     //     "\"one\"two\"three\"four",  // Multiple quotes
//     //     "\"nested quotes are tricky\"", // Single quoted part
//     //     "no quotes here",           // No quotes
//     //     "\"unclosed quote",         // Unclosed quote
//     //     "multiple\"quotes\"in\"string\"", // Many quotes
//     //     "",                         // Empty string
//     //     "\"\"empty\"\"",           // Empty quoted parts
//     // };
    
//     // int num_tests = sizeof(test_strings) / sizeof(test_strings[0]);
// 	char	*str = "\"hello world\"";
    
//     printf("Dynamic String Parser Test\n");
//     printf("==========================\n\n");
    
//     // for (int i = 0; i < num_tests; i++) {
//         ParseResult result = parse_dynamic_quoted_string(str);
//         print_parse_result(&result, str);
        
//         // Example: Get only quoted parts
//         int quoted_count;
//         char **quoted_parts = get_quoted_parts(&result, &quoted_count);
//         if (quoted_count > 0) {
//             printf("Quoted parts only:\n");
//             for (int j = 0; j < quoted_count; j++) {
//                 printf("  \"%s\"\n", quoted_parts[j]);
//             }
//             printf("\n");
//         }
        
//         free_quoted_parts(quoted_parts, quoted_count);
//         free_parse_result(&result);
//         printf("----------------------------------------\n");
//     // }
    
//     return 0;
// }

// // int main(void)
// // {
// // 	// char *test_str = "Hello	world!	This is    		 a test    	   string.";
// // 	// char **result = gc_split(test_str);
	
// // 	// if (result)
// // 	// {
// // 	// 	for (int i = 0; result[i]; i++)
// // 	// 	{
// // 	// 		printf("Word %d: '%s'\n", i, result[i]);
// // 	// 		free(result[i]); // Free each word
// // 	// 	}
// // 	// 	free(result); // Free the array itself
// // 	// }
	
// // 	char	*str = "hello world";
// // 	char	*quoted = add_quotes_to_str(str);
// // 	printf("%s\n", quoted);
// // 	return 0;
// // }

// char *process_dynamic_quoted(t_gcnode **gc, char *input, t_data *data)
// {
//     if (!input || strlen(input) == 0)
//         return gc_strdup(gc, "");
    
//     int len = strlen(input);
//     int pos = 0;
//     char *result = gc_strdup(gc, "");
    
//     if (!result)
//         return NULL;
    
//     while (pos < len)
//     {
//         if (input[pos] == '"')
//         {
//             // Found start of quoted string
//             int quote_start = pos;
//             pos++; // Skip opening quote
            
//             // Find closing quote
//             while (pos < len && input[pos] != '"')
//                 pos++;
            
//             if (pos >= len)
//             {
//                 // No closing quote found - treat remaining as unquoted
//                 char *remaining = gc_malloc(gc, len - quote_start + 1);
//                 if (!remaining)
//                     return result;
                
//                 strcpy(remaining, input + quote_start);
//                 char *expanded = expand_variables(gc, remaining, data);
//                 if (!expanded)
//                     return result;
                
//                 char *new_result = gc_strjoin(gc, result, expanded);
//                 return new_result ? new_result : result;
//             }
            
//             // Extract quoted part including quotes
//             int quoted_len = pos - quote_start + 1; // +1 to include closing quote
//             char *quoted_part = gc_malloc(gc, quoted_len + 1);
//             if (!quoted_part)
//                 return result;
            
//             strncpy(quoted_part, input + quote_start, quoted_len);
//             quoted_part[quoted_len] = '\0';
            
//             // Process with remove_quotes
//             char *unquoted = remove_quotes(gc, quoted_part);
//             if (!unquoted)
//                 return result;
            
//             // Join to result
//             char *new_result = gc_strjoin(gc, result, unquoted);
//             if (!new_result)
//                 return result;
//             result = new_result;
            
//             pos++; // Skip closing quote
//         }
//         else
//         {
//             // Found unquoted text
//             int start = pos;
            
//             // Find next quote or end of string
//             while (pos < len && input[pos] != '"')
//                 pos++;
            
//             // Extract unquoted part
//             int unquoted_len = pos - start;
//             if (unquoted_len > 0)
//             {
//                 char *unquoted_part = gc_malloc(gc, unquoted_len + 1);
//                 if (!unquoted_part)
//                     return result;
                
//                 strncpy(unquoted_part, input + start, unquoted_len);
//                 unquoted_part[unquoted_len] = '\0';
                
//                 // Process with expand_variables
//                 char *expanded = expand_variables(gc, unquoted_part, data);
//                 if (!expanded)
//                     return result;
                
//                 // Join to result
//                 char *new_result = gc_strjoin(gc, result, expanded);
//                 if (!new_result)
//                     return result;
//                 result = new_result;
//             }
//         }
//     }
    
//     return result;
// }

char	*add_quotes_to_str(char *str)
{
	char	*quoted_str;
	int		len;
    int     test;
    int     test1;
    int     test1;

	if (!str || !*str)
		return (NULL);
	len = strlen(str);
	quoted_str = malloc(len + 3);
	quoted_str[0] = '\"';
	strncpy(quoted_str + 1, str, len);
	quoted_str[len + 1] = '\"';
	quoted_str[len + 2] = '\0';
	return (quoted_str);
}

int main(void)
{
    char    *str = "hello world";
    char    *quoted_str;

    quoted_str = add_quotes_to_str(str);
    printf("The result is: [%s]\n", quoted_str);
}