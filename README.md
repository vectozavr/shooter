# Shooter 3Dzavr SFML version

<h4>About:</h4>

Source code of simple shooter on [3Dzavr SFML game engine](https://github.com/vectozavr/3dzavr/tree/sfml_version)

![Project demonstration](img/gamePlay2.png)

[![MIT License](https://img.shields.io/badge/license-MIT-blue.svg?style=flat)](http://choosealicense.com/licenses/mit/)
![downloads](https://img.shields.io/github/downloads/vectozavr/shooter/total)
[![Community Discord](https://img.shields.io/discord/788056426324426782.svg?label=discord&logo=discord)](https://discord.gg/NzSf5tyS)
![Commits](https://img.shields.io/github/last-commit/vectozavr/shooter)
[![TODOs](https://badgen.net/https/api.tickgit.com/badgen/github.com/vectozavr/shooter)](https://www.tickgit.com/browse?repo=github.com/vectozavr/shooter)
![Repo size](https://img.shields.io/github/repo-size/vectozavr/shooter)

<h4>Installation:</h4>
1) [Download release](https://github.com/vectozavr/shooter/releases/tag/0.1.0), [unzip it](https://www.7-zip.org) and run shooter.exe file


2) Write <b>ip</b> and <b>port</b> of server in <b>connect.txt</b> file.


3) Write <b>port</b> of the server in <b>server.txt</b> file (only for computer where the server will be running).


3) Enjoy gaming!

[<h5>Click here if you want to run the server</h3>](https://github.com/vectozavr/shooter_server)

<h4>Control:</h4>

<b>Mouse</b>, <b>Space</b>, <b>A</b>, <b>S</b>, <b>W</b>, <b>D</b> – player control.

<b>SHIFT</b> – slow motion (this ability is not infinite: its bar is next to hp)

<b>E & Q </b> or keys <b> <- -> </b> – change weapon

<b>R</b> – recharge

<b>O</b> – turn OpenGL on/off

<b>Tab</b> – turn debug mode on/off

<h4>Playing with a source code:</h4>

1) [Download and install OpenAL library](https://openal.org/downloads/) for SFML sound support (in current version you can't setup this engine without OpenAL)

2) Clone this repository

3) Open project
   
   Using [CLion](https://www.jetbrains.com/clion/) with [MinGW (32-bit)](https://sourceforge.net/projects/mingw-w64/files/Toolchains%20targetting%20Win32/Personal%20Builds/mingw-builds/7.3.0/threads-posix/dwarf/i686-7.3.0-release-posix-dwarf-rt_v5-rev0.7z/download) compiler: open <b>CMakeList.txt</b> as a project

   Using [Visual Studio](https://visualstudio.microsoft.com/ru/): open <b>shooter.sln</b> as a project

4) Built project and run the game

Structure:
![Project demonstration](img/structure.png)

Online:
![Project demonstration](img/gamePlay4.png)

GamePlay:
![Project demonstration](img/gamePlay3.png)
![Project demonstration](img/gamePlay5.png)
![Project demonstration](img/gamePlay6.png)
![Project demonstration](img/gamePlay7.png)
![Project demonstration](img/opengl.png)
