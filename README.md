<p align="center">
  <img src="https://github.com/thaisnishimoto/42-project-badges/blob/main/badges/pipexm.png" alt="Pipex bonus"/>
</p>

<h1 align=center>
	<b>Pipex</b>
</h1>

<p align="center"><i>"This project will let you discover in detail a UNIX mechanism that you already know by using it in your program."</i></p>  
<h2>
 Status
</h2>

**Finished:**  2023, October 11th <br>
**Grade:** 125/100

<h2>
Content
</h2>

[Mandatory Part](https://github.com/thaisnishimoto/42sp_06_Pipex#about-the-project---mandatory-part)

[Bonus Part](https://github.com/thaisnishimoto/42sp_06_Pipex#about-the-project---bonus-part)

[Testers](https://github.com/thaisnishimoto/42sp_06_Pipex#resource---testers)

<h2>
About the project - mandatory part
</h2>

The program `pipex` should behave exactly as the shell command below:
```sh
< file1 cmd1 | cmd2 > file2
```

`< file1` - is used to redirect input. Instead of reading from stdin, the command will read from file1. <br>
`cmd1` - is the first shell command that will process the input. <br>
`|` - the pipe sends the output of cmd1 as input to cmd2. <br>
`cmd2` - is the second shell command that will process the input from cmd1. <br>
`> file2` - is used to redirect output. Instead of writing to stdout, the command will write to file2. <br>

<h4>
 Compile mandatory to pipe 2 commands
</h4>

```sh
git clone git@github.com:thaisnishimoto/42sp_06_Pipex.git
```
```sh
make
```
```sh
./pipex infile cmd1 cmd2 outfile
```

<h2>
About the project - bonus part
</h2>

The program `pipex` should also:
- Handle multiple pipes
```sh
< infile cmd1 | cmd2 | cmd3 ... | cmdn > outfile
```
- Support << and >>
```sh
cmd1 << LIMITER | cmd2 >> outfile
```

`cmd1` - is the first command that will process the input. <br>
`<< LIMITER` - redirects input from the following lines to be written in the terminal, until there is a line containing only the LIMITER <br>
`|` - the pipe sends the output of cmd1 as input to cmd2. <br>
`cmd2` - is the second shell command that will process the input from cmd1. <br>
`>> outfile` - is used to redirect output to outfile. If outfile doesn't exist, it is created. If it does exist, the output is added to the end of the file, instead of overwriting it. <br>

<h4>
 Compile bonus for multiple commands or here_doc usage
</h4>

```sh
make bonus
``` 
```sh
./pipex_bonus infile cmd1 cmd2 [cmd3 ...] outfile
```
```sh
./pipex_bonus here_doc LIMITER cmd cmd1 [cmd3 ...] outfile
```

<h2>
Resource - testers
</h2>

I created my own unit test, with some basic commands. It compares the oufile of `pipex` with the original pipe's oufile. <br>
Don't forget to also compare the exit code with `echo $?` <br>
To run them, download the directory [my_tests](https://github.com/thaisnishimoto/42sp_06_Pipex/tree/master/my_tests) in your project's directory.
```sh
cd my_tests
make
make bonus
```

Here are a few other repositories with third party testers that I found usefull: <br>

[pipex-tester](https://github.com/denisgodoy/pipex-tester): Various error handling cases, tests quoted arguments and checks for memory leak

[PIPEXaminator](https://github.com/mariadaan/PIPEXaminator): A few extra error cases, such as invalid command flag

[teste_pipex](https://github.com/D-Daria/pipex): Additional cases, such as command written with path

[pipex_tester_42](https://github.com/Yoo0lh/pipex_tester_42): A few tests for the bonus part
