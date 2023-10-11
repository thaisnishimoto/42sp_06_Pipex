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

The program `pipex` should:
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

2. Compile and run
```sh
make bonus
./pipex infile cmd1 cmd2 outfile
```

4. Compile bonus and run for multiple commands or here_doc usage
```sh
make bonus
./pipex infile cmd1 cmd2 [cmd3 ...] outfile
./pipex here_doc LIMITER cmd cmd1 [cmd3 ...] outfile
```

<h2>
Error handling
</h2>


<h2>
Resources
</h2>

I created my own unit test.

Here are a few other third party testers that I found usefull:

MiniLibX is a library based on Xlib meant for beginners to learn the basics of graphics development. It allows to run applications on windows, move windows around the screen and click on items. <br>
It was used to develop this project. So, to run the game, you might need to install [MiniLibX](https://github.com/42Paris/minilibx-linux) on your machine:
```sh
sudo apt-get install gcc make xorg libxext-dev libbsd-dev
```
