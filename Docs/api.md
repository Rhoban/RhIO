# RhIO API

RhIO will automatically run a server on startup when it is linked into your app,
you don't have to do it manually (it uses compiler constructors).

You'll first have to include the ``RhIO.hpp`` file:

```cpp
#include <RhIO.hpp>
```

Then, you can access the root node of RhIO using ``RhIO::Root`̀`. There is several 
examples in the `Examples/` directory of this repository.

## <a name="parameters"></a> Parameters

RhIO can deal with ints, floats/doubles, strings and bools parameters.

### Declaring parameters

You can define values using ``newInt``, ``newFloat``, ``newString`` or
``newBool``. Additional metadata can be passed using method chaining, just like this:

```cpp
RhIO::Root.newInt("paramInt")
    ->comment("My first RhIO int");
```
The available methods are:

* `comment(string)`: a comment text describing the parameter
* `minimum(value)` and `maximum(value)`: minimum and maximum boundaries for the value
  of this parameter
* `defaultValue(value)`: the default value of the parameter
* `persisted(bool)`: wether to persist or not this parameter (see [below](#persistence))

The parameter name can be a hierarchical path:

```cpp
RhIO::Root.newInt("/path/to/paramInt");
```

Note that these methods can't be called twice with the same parameter name, since it is
used to **declare** the parameter.

### Setting parameters

Parameters can be set using ``setInt``, ``setFloat``, ``setString`` or ``setBool``:

```cpp
RhIO::Root.setInt("/path/to/paramInt", 42);
```

The third parameter of these methods is the timestamp that will be used (will default to 
the current timestamp):

```cpp
RhIO::Root.setInt("/path/to/paramInt", 42, std::chrono::steady_clock::now());
```

### Getting parameters

Parameters can be get using ``getInt``, ``getFloat``, ``getString`` or ``getBool``:

```cpp
std::cout << "a=" << RhIO::Root.getInt("/path/to/paramInt") << std::endl;
```

### Nodes

For convenience, you may want to get access to nodes other than root. This can be done
using ``newChild``, for node creation and ``getChild`` method, to access the node:

```cpp
RhIO::Root.newChild("/hello/world");
auto node = RhIO::Root.getChild("/hello/world");
node.newInt("test"); // Will declare /hello/world/test
```

One can also test the existence of a node using ``nodeExist`` method.

## <a name="persistence"></a> Persistence

Any node can be persisted and/or loaded to filesystem using ``load`` and ``save`` methods.
Note that only the persisted parameters (i.e those with the ``persisted(true)`` metadata)
will be concerned by these operations.

If you want to save all your parameters to the `rhio/` directory (for instance when your
program quit):

```c++
RhIO::Root.save("rhio");
```

In the same way, you can load it (for instance, on the start-up of your program):

```c++
RhIO::Root.load("rhio");
```

## <a name="commands"></a> Commands

Commands are custom application-side methods that can be called with arguments passed by
the user.

Here is an example of "echo" command:

```c++
RhIO::Root.newCommand("echo", 
    "An example echo command", 
    [](const std::vector<std::string>& args) -> std::string
    {
        return "ECHO: " + args[0];
    });
```

## <a name="streams"></a> Streams

TO BE DOCUMENTED

