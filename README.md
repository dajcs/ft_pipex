# pipex

This is the pipex project @ 42 school.

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

## The Goal

The data should flow from `file1`, get processed by shell command `cmd1`, the result of this will be processed by `cmd2` and the final result is written to `file2`.

```yaml
+---------+      +--------+      +--------+      +---------+
|  file1  | ===> |  cmd1  | ===> |  cmd2  | ===> |  file2  |
+---------+      +--------+      +--------+      +---------+
 (Input)        (Process 1)     (Process 2)      (Output)
```

## Execution

### Step1: Preparation by Manager
- open `file1` for reading
- open `file2` for writing
- create a `pipe` -- this is a one-way data channel managed by the OS. It has a "read end" and a "write end"

```yaml
    +-----------------------------------------------------+
    |            Main pipex Process (Parent)              |
    |                                                     |
    |  IN: fd_in (from file1)     OUT: fd_out (to file2)  |
    |                                                     |
    |  create pipe: [ pipe_write | pipe_read ]            |
    |                                                     |
    +-----------------------------------------------------+
```

### Step2: Hiring the First Worker (cmd1)

The manager uses `fork()` to create a child process. The child will handle `cmd1`.

- **manager (parent) process**: creates *Child 1*
- **Child 1 process**: rewires its plumbing (`dup2`)
  - its standard input (stdin), which is normally the keyboard, is connected to `file1`
  - its standard output (stdout), which is normally the screen (terminal) is connected to the **pipe_write**
- **Child 1** transforms itself into `cmd1` using `execve`. It runs, reads from `file1`, and writes its output into the pipe.

```yaml
(Data flows from file1)
+---------+      +--------------------+
|  file1  |----->| (stdin)            |
+---------+      |      Child 1       |
                 |      (cmd1)        |
                 | (stdout)           |-----> [ pipe_write | ... ]
                 +--------------------+
```

### Step 3: Hiring the Second Worker (cmd2)

The manager `fork()`s again to create a second child for `cmd2`.

- **manager (parent) process**: creates *Child 2*
- **Child 2 process**: rewires its plumbing (`dup2`)
  - its standard input (stdin), will be connected to **pipe_read**
  - its standard output (stdout), will be to `file2`.
- **Child 2** transforms itself into `cmd2` using `execve`. It runs, reading **pipe_read** (which is `cmd1` output), and writes its final output to `file2`.

```yaml
                                          (Data flows into file2)
                         +--------------------+      +---------+
[ ... | pipe_read ]----->| (stdin)            |----->|  file2  |
                         |      Child 2       |      +---------+
                         |      (cmd2)        |
                         | (stdout)           |
                         +--------------------+
```

### Step4: Final Picture & Cleanup

The parent process closes its own copies of the pipe ends and then `wait()`s for both children to finish their jobs.

This creates the complete, uninterrupted flow:

```yaml
+-------+     +-----------+      [ pipe_write | pipe_read ]     +-----------+     +-------+
| file1 | --> |  Child 1  | --> [============PIPE=========] --> |  Child 2  | --> | file2 |
+-------+     |  (cmd1)   |                                     |  (cmd2)   |     +-------+
              +-----------+                                     +-----------+
```

## Bonus 1 - Multiple Pipes ( < file1 cmd1 | cmd2 | ... | cmdN > file2)

The logic simply extends. The parent creates a chain of workers. Each worker in the middle reads from the previous pipe and writes to a *new* pipe.

- **Child1(cmd1)**: Reads from `file1`, writes to *pipe1*.
- **Child2(cmd2)**: Reads from *pipe1*, writes to *pipe2*.
- **Child3(cmd3)**: Reads from *pipe2*, writes to *pipe3*.
- ...
- **ChildN(cmdN)**: Reads from *pipe(N-1)*, writes to `file2`.

```yaml
+-------+   +------+   +=======+   +------+   +=======+   +------+   +-------+
| file1 |-->| cmd1 |-->| Pipe1 |-->| cmd2 |-->| Pipe2 |-->| cmdN |-->| file2 |
+-------+   +------+   +=======+   +------+   +=======+   +------+   +-------+
```

## Bonus 2 - Here Document ( << LIMITER | cmd1 >> file )

This changes how the input is gathered.

- **there is no `file`**: Instead of reading from a file, pipex reads from the terminal
- **Temporary File**: it reads lines of input until it sees the LIMITER. All these lines are stored in a temporary, hidden file (e.g., .`heredoc_tmp`).
- **The Flow Begins**: this temporary file now acts as the *infile* for the first command. The rest of the process is the same as the basic case.
- **Append >>**: The output file is opened in "append" mode, so new content is added to the end instead of overwriting it.

```yaml
+----------+     +----------------+     +-------+     +======+     +------+     +------+
| Terminal | ==> | Write to temp  | ==> | .tmp  | ==> | cmd1 | ==> | Pipe | ==> | cmd2 |
+----------+     | file until     |     | file  |     +------+     +======+     +------+
(user types)     | LIMITER is hit |     +-------+                                  |
                 +----------------+                                                v
                                                                               +--------+
                                                                               | file   |
                                                                               |(append)|
                                                                               +--------+
```

In essence, the `pipex` program is a master plumber, using `fork`, `pipe`, `dupexecve` as its tools to construct a data pipeline according to the blueprint provided by the command-line arguments.
