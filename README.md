# Rhoban Input/Output Library

![RhIO](Docs/imgs/rhio.gif)

RhIO is a lightweight library that can be linked against your application
in order to interract with your program on-the-fly, through its integrated server.

Main feature are parameters that can be exposed in order to be changed, monitored
or persisted to configuration files. 

We also provide a shell client that will provide a bash-like interface to walk
your nodes and parameters, and even trigger methods that are in your code.

## Documentation

* [Example Video](https://youtu.be/MOizgXYENLc)
* [Getting started](/Docs/getting_started.md)
    * [Download](/Docs/getting_started.md#download)
    * [Building](/Docs/getting_started.md#building)
* [The API (server-side)](/Docs/api.md)
    * [Parameters](/Docs/api.md#parameters)
    * [Persistence](/Docs/api.md#persistence)
    * [Commands](/Docs/api.md#commands)
    * [Streams](/Docs/api.md#streams)
    * [Frames](/Docs/api.md#frames)
* [Using binding (server-side)](/Docs/binding.md)
* [Shell commands and features (client-side)](/Docs/shell.md)
* [Using a joypad with the shell](/Docs/joypad.md)
* [How does it works?](/Docs/how_does_it_works.md)

## License

RhIO is under MIT License, please read the LICENSE file for further details.
Do not hesitate to fork this repository and customize it!

## Overall Architecture

![RhIO](Docs/imgs/architecture.png)

