# strsub

This is a string substitution library written in C.

Substrings between open and closing delimiter tokens are replaced with new strings, for example:
```
[name] was in [location].
```

In this case "[" and "]" is the opening and the closing delimiter tokens. One or more handlers can be installed to do string replacement. If the parser finds the delimiter tokens, the handlers are called to provide a replacement for the strings, "name" and "location" in the example above. The final output string could appear as follows:

```
Helly R. was in Dover.
```

If no handler can replace the string, it is left unchanged.

# Example 1



The examples compile and runs perfectly in a GitHub codespace. There is a Makefile so simply type ``` make ``` in the terminal when the codespace is started for the repository. In this example, we are parsing a string from a source to a destination by replacing text from a config file. The replacement is the "value" of the "key-value" pair in a config file. The example installs a handler using a key-value lookup from the config file to replace the strings. Run an example by trying the following in the terminal:

```
./build/strsub  "[name] was in [location]."
```

It should give the following output: 

```
Mark S. was in Amsterdam.
```

If no config file is specified "default.cfg" is used, expected in the current directory. Or you can use a specific configuration file using the --config option:

```
./build/strsub  "[name] was in [location]." --config=./test/test.cfg
```

This should give the following output:

```
Helly R. was in Dover.
```


# Example 2

In this example, we substitute the string between the '#' delimiters. We are directly outputting each character, and only replacing the delimited ASCII key code inside the delimiters with the character, before writing it to the console. By using this method, we can also execute other commands such as changing the font through delimiter-coded commands.

```
./build/strsub  "[name] was in [location] on #219# [date] #219#"
```

This should output:

```
 Helly R. was in Dover on � 2023-01-11 11:59:38 �	
```


# Escape Characters

There is also configurable escape character ("\" for the example):
```
./build/strsub "\[name] will be replaced by [name]."
```
Which should give the following output:
```
[name] will be replaced by Mark S.
```
