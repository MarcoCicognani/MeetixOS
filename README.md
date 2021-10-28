# MeetiX OS

**MeetiX OS** is an operating system for the intel x86 platform. The kernel of the system is mostly derived from the **
Ghost Kernel** of **Max Schlüssel**, in fact, most of the features (at kernel level) are the same at the moment. I
called my derivation of the Ghost kernel "**EvangelionNG**"; this bizarre name is a composite word, where Evangelion (**
not the anime :-)**) means **Good News**
and the appendix **NG** means **New Generation**, the resultant means of the name is ***New Generation of Good News***.
The reasons of this name date back to the birth of the project, when taken by the enthusiasm, I decided to call "**
MeetiX**" my own OS because it had to become something mythical for the eyes of those who used it (I was about 14 years
old at the time), the kernel, instaed, must have been something superb too and bring new things to the already crowded
and consolidated world of kernels. The kernel is written in **C++**/**C**/**Assembly** like most of the userspace
applications, commands, servers and libraries. I started this project four years ago as a joke, but now is a real
research project for me and my IT culture. The sources are released under the **GPL3 licence**.
****

# Kernel Features

- **Multitasking, Multithreadding** and **Multiprocessor (SMP)**
- **IPC** with **s_pipe, Signal, Messages and Shared Memory**
- **VM86** support for bios calls
- **Micro Kernel** Architecture
- **Memory Paging**
- **ELF32 support**
- **COM1** logging

****

# Userspace Features

- **Non Unix-like** architecture but basic **POSIX1** support
- **Graphical** and **interactive** system

****

# Programmer libraries

- **Cairo**, **libPNG**, **libZ**, **Freetype** and **Pixman** libs port
- **High Level C++ libraries** to simplify the system interaction
- **Low Level C library** to interact with the **kernel (EvaApi)**
- **GNU libstdC++v3** port
- Ghost OS **libC** port

****

# Userspace Applications, Commands and Drivers

On MeetiX OS the userspace executables are divided by

**Applications**: applications are all the graphical programs avaible on the system, they are contained into the **
/app/** directory.

**Commands**: commands are all the command line programs avaible on the system, they are contained into the **/cmd/**
directory.

**Servers**: the servers, on the micro kernel architecture, are the userspace kernel modules and the drivers, the serves
are container into the **/sys/eva/server/** directory.

The system doesn't have many applications or drivers yet, but something to try there is:

### Applications

- **calculator**: basic graphical calculator.
- **CandyNote**: an incomplete text editor.
- **CandyShell**: the default system terminal, includes a basic shell.
- **Terminal**: a nearly complete terminal that use **mx** as shell.
- **exec**: basic interface to start commands from the gui.
- **Gurf**: basic game inspired by Simon game.
- **MXLogin**: the gui login client.
- **OsmosUI**: the graphical desktop.
- **UItweak**: client to change desktop background.
- **welcome**: graphical Welcome dialog.

### Commands

- **cp**: copy command, to copy files.
- **cpudet**: get all the processor specifications.
- **echo**: repeat what is provided by m_arguments.
- **ename**: corresponding of uname of unix's world.
- **find**: semplified find utility.
- **fspech**: get the file specifications.
- **gzip**: compress or decompress files.
- **idle**: is not a command properly, this program is started by the kernel at boot on each cpu core to maintain the cpu usage at 0%.
- **js**: the **DukTape** javascript interpreter.
- **s_kill**: s_kill a m_pthread or a m_process_creation_identifier.
- **ls**: list content of a directory.
- **mx**: a basic shell.
- **ntf**: send to the desktop a m_message_buffer popup.
- **pcitool**: list the pci devices.
- **power**: send to the **Spawner** the shutdown or the reboot command.
- **ps**: list the running processes.
- **rd**: read a file.
- **stress**: test the system capabilities.
- **ts**: list the running threads.
- **wr**: write on a file.

### Servers

- **ps2**:     manage the reading of the keyboard and the mouse.
- **spawner**: is the most important server, manage the launch of the new processes, the shutdown and reboot and many
  other stuffs.
- **time**:    provides the date, and the time to the system
- **vbe**:     vesa video driver.
- **zipNET**:  the window server.

# Screenshot v0.7.1

<img src="http://www.meetixos.org/wp-content/uploads/2017/11/Screenshot_20171023_171358-768x577.png">
