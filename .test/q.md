Can you help me creating the C program `pipex`?
- the program should be executed as follows:
```bash
./pipex file1 cmd1 cmd2 file2
```
- the program takes 4 arguments:
  - `file1` and `file2` are **file names**
  - `cmd1` and `cmd2` are **shell commands** with their parameters
- it must behave exactly like the following shell command:
```bash
$> < file1 cmd1 | cmd2 > file2
```

## Examples

```bash
$> ./pipex infile "ls -l" "wc -l" outfile
```
- Its behavior should be equivalent to:
```bash
$> < infile ls -l | wc -l > outfile
```

<BR>

```bash
$> ./pipex infile "grep a1" "wc -w" outfile
```
- its behavior should be equivalent to:
```bash
$> < infile grep a1 | wc -w > outfile
```

## Details

- program name: `pipex`
- program files: `Makefile`, `*.h`, `*.c`
- Makefile rules: `NAME`, `all`, `clean`, `fclean`, `re`
- arguments: `file`, `cmd1`, `cmd2`, `file2`
- `libft` authorized: Yes
- allowed functions: `open`, `close`, `read`, `write`, `malloc`, `free`, `perror`, `strerror`, `access`, `dup`, `dup2`, `execve`, `exit`, `fork`, `pipe`, `unlink`, `wait`, `waitpid` + any function from `libft`

## Requirements

- no memory leaks
- errors should be handled the same way as the shell command
```bash
< file1 cmd1 | cmd2 > file2
```

