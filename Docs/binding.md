# Binding

## Parameters

Binding is a convenient way to use the [RhIO API](api.md) with a
simplified workflow.

What you'll have to do is first instantiate a ``RhIO::Bind``,
that will be related with a node, and then declare your parameters
using the ``bindNew`` method:

```c++
float t = 0.0;
RhIO::Bind binder("path");

binder.bindNew("t", t)
    ->comment("Elapsed time");
```

The type of the parameter will be automatically resolved using the
compiler overload (since the second parameter is actually a reference
to a variable with a known type).
The metadata are exactly the same as the [API "new" methods](api.md).

You'll then be able to get data from RhIO (pull) or send data to RhIO (push):

```c++
while (true) {
    binder.pull();
    t += 0.02;
    usleep(20000);
    binder.push();
}
```

## PushOnly and PullOnly

In a lot of situations, you'll only want a parameter to be pulled (imported from
RhIO world) or pushed (exported to RhIO). You can use the third parameter to do it:

```c++
// The t parameter will be exported from RhIO, but never imported
binder.bindNew("t", t, RhIO::Bind::PushOnly);

// The t parameter will be imported from RhIO, but never exported
binder.bindNew("amplitude", amplitude, RhIO::Bind::PullOnly);
```

## Commands

Commands can be also bound using RhIO binding, but only works with class methods.
To do it, use the ``bindFunc`` method:

```c++
// In your class (for instance in your constructor)
binder.bindFunc("command", "command description", 
            &MyObject::myCommand, *this)
```
