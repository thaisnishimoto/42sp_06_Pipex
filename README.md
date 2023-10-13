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
About the project - mandatory part
</h2>

The program `pipex` should behave as the shell command below:
```sh
< file1 cmd1 | cmd2 > file2
```

1. Clone the repository
```sh
git clone git@github.com:thaisnishimoto/42sp_06_Pipex.git
```

2. Compile and run
```sh
make
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
cmd << LIMITER | cmd1 >> outfile
```

1. Clone the repository
```sh
git clone git@github.com:thaisnishimoto/42sp_06_Pipex.git
```

2. Compile bonus and run for multiple commands or here_doc usage
```sh
make bonus
./pipex infile cmd1 cmd2 [cmd3 ...] outfile
./pipex here_doc LIMITER cmd cmd1 [cmd3 ...] outfile
```

<h2>
Error handling
</h2>

<h2>
Resources - testers
</h2>

I created my own unit test.

Here are a few other repositories with third party testers that I found usefull: <br>

[pipex-tester](https://github.com/denisgodoy/pipex-tester): Various error handling cases, tests quoted arguments and checks for memory leak

[PIPEXaminator](https://github.com/mariadaan/PIPEXaminator): A few extra error cases, such as invalid command flag

[teste_pipex](https://github.com/D-Daria/pipex): Additional cases, such as command written with path

[pipex_tester_42](https://github.com/Yoo0lh/pipex_tester_42): A few tests for the bonus part
