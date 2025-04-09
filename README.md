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

if the the character is '<' the next argument is the input file
<h2>example:</h2>

```c
t_command cmd3 = {
    .args = {"cat", NULL},
    .input_file = "input.txt",
    .output_file = NULL,
    .append_mode = 0,
    .heredoc_delim = NULL,
    .next = NULL
};
```