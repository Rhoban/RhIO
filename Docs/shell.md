# Shell

## Running the shell

As [explained before](getting_started.md), the shell can be built using the cmake
of the ``Shell/`` directory. After building the shell binary, you can also ``make install``
to have it in your binary directory and run it from anywhere. The usage is:

    rhio [server [one shot command]]

The first argument of the program is the ip/hostname of the RhIO server to connect
to. If you give it no argument, it will connect to localhost.

If you specify more than one argument, all the rest will be considered as a command,
that will be runned once connected, and then the shell will quit (the one shot 
command). Thus, if you type:

    rhio 127.0.0.1 ls

This will run the ``ls`` command and quit.

## Features

The shell features a pretty much complete line edit, supporting cursor, colors,
auto-completion, CTRL+A, CTRL+E, CTRL+L etc.

It also stores the last typed commands into your ``$HOME/.rhio_history`` file, just like
bash.

## Commands

### ``help``

The first thing you may want to know about is the ``help`` command, which will list you
all the possible commands to type.

The green commands are the remote ones defined by the user in its code. All the others
are natives commands that will be available in any situation.

### ``ls``

Just like the well-known `̀`ls`` command, it will list the nodes that are currently
available and their descriptions.

Note that you can give ``ls`` an argument (and, for instance type ``ls /``) to list 
another node path.

### ``cd``

The ``cd`` command will change your current working directory. This directory will be 
indicated in the command prompt:

    RhIO:/test#

The ̀``test/`` prefix indicate that we are in the test directory

### Reading & setting parameter

If you type the name of a parameter, it will be readed, you can also directly set a new
value to this parameter using ``=``:

    RhIO:/test# cd /test
    RhIO:/test# amplitude
    amplitude=12
    RhIO:/test# amplitude=15
    RhIO:/test# amplitude
    amplitude=15

One can also define values with an absolute name, or a relative one:
   
   RhIO:/test# ../test/amplitude=26
   RhIO:/test# /test/amplitude=42

