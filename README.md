# minishell

A simple Unix shell implementation made as a 42 project.

## ✨ Features

- Execution of commands with arguments
- Pipes and redirections
- Built-in commands (cd, echo, export, etc.)
- Signal handling
- Environment variable management

## 👥 Authors

<table align="center">
  <tr>
    <td align="center">
      <a href="https://github.com/nachat-ayoub" style="text-decoration: none; color: inherit;">
        <img src="https://images.weserv.nl/?url=avatars.githubusercontent.com/nachat-ayoub?v=4&h=300&w=300&fit=cover&mask=circle&maxage=7d" width="80" height="80" alt="nachat-ayoub"/>
      </a>
      <br/>
      <sub><b><a href="https://profile.intra.42.fr/users/anachat" target="_blank">anachat</a></b></sub>
    </td>
    <td align="center">
      <a href="https://github.com/berila7" style="text-decoration: none; color: inherit;">
        <img src="https://images.weserv.nl/?url=avatars.githubusercontent.com/berila7?v=4&h=300&w=300&fit=cover&mask=circle&maxage=7d" width="80" height="80" alt="berila7"/>
      </a>
      <br/>
      <sub><b><a href="https://profile.intra.42.fr/users/mberila" target="_blank">mberila</a></b></sub>
    </td>
  </tr>
</table>


## ⚙️ How to use

```bash
make
./minishell
```

Compatible with bash behavior and subject to 42 school constraints.

## 🔐 Tokenize

### Tokens struct

```c
struct s_token
{
	char			*value;
	t_token_type	type;
	t_token			*next;
};
```

### Token types struct

```c
typedef enum e_token_type
{
	TOKEN_WORD,			//	(commands, arguments, filenames)
	TOKEN_PIPE,			//	|
	TOKEN_REDIR_IN,		//	<
	TOKEN_REDIR_OUT,	//	>
	TOKEN_REDIR_APPEND,	//	>>
	TOKEN_HEREDOC		//	<<
}	t_token_type;
```

- Let's take this example:

```bash
cat < Makefile
```

First, we process the input line, identifying specific tokens. These tokens can be special characters like `<`, `|`, `>`, `>>`, `<<`, or strings such as '`cat`'.

Then, for each token identified, we create a data structure that contains the token's value (e.g., (`cat`) or (`>`)) and its type (e.g., '`TOKEN_WORD`' or '`TOKEN_REDIR_IN`').

Finally, we add each of these structures to a linked list. In this list, each element (or 'node') points to the next node, creating a sequence that represents the parsed input. For example:

`cat` --> `>` --> `Makefile`"

<img src="https://i.imgur.com/dhhOQEe.png">

### Commands struct

```c
typedef struct s_cmd t_cmd;
struct s_cmd
{
    char        *args;
    char        *input_file;
    char        *output_file;
    int         append_mode;
    char        *heredoc_delim;
    t_cmd   *next;
};
```

### Understanding the Command Structure

- `args`: An array of strings representing the command and its arguments (e.g.,`["ls", "-la", NULL]`)
- `input_file`: The file to redirect input from (`<`)
- `output_file`: The file to redirect output to (`>` or `>>`)
- `append_mode`: Flag indicating if output should be appended (1) or overwritten (0)
- `heredoc_delim`: The delimiter string for heredoc (`<<`)
- `next`: Pointer to the next command in a pipeline

<h2>Example 1: Command with Input Redirection</h2>

```bash
cat < input.txt
```

```c
t_cmd cmd3 = {
    .args = {"cat", NULL},
    .input_file = "input.txt",
    .output_file = NULL,
    .append_mode = 0,
    .heredoc_delim = NULL,
    .next = NULL
};
```

<h2>Example 2: Simple Command</h2>

```bash
ls -la
```

```c
t_cmd cmd1 = {
    .args = {"ls", "-la", NULL},
    .input_file = NULL,
    .output_file = NULL,
    .append_mode = 0,
    .heredoc_delim = NULL,
    .next = NULL
};
```

<h4>Explanation:</h4>

- `args`: An array containing the command (`"ls"`) and its argument (`"-la"`), terminated by `NULL`
- All redirection fields are `NULL` because there are no redirections
- `next` is `NULL` because this is a standalone command (no pipe "`|`")

<h2>Example 3: Complex Pipeline with Redirections</h2>


```bash
grep "error" < logs.txt | sort | uniq > unique_errors.txt
```

```c
t_cmd cmd8_3 = {
    .args = {"uniq", NULL},
    .input_file = NULL,
    .output_file = "unique_errors.txt",
    .append_mode = 0,
    .heredoc_delim = NULL,
    .next = NULL
};

t_cmd cmd8_2 = {
    .args = {"sort", NULL},
    .input_file = NULL,
    .output_file = NULL,
    .append_mode = 0,
    .heredoc_delim = NULL,
    .next = &cmd8_3
};

t_cmd cmd8_1 = {
    .args = {"grep", "error", NULL},
    .input_file = "logs.txt",
    .output_file = NULL,
    .append_mode = 0,
    .heredoc_delim = NULL,
    .next = &cmd8_2
};
```