# strsub
This is a string substitution library written in C.

Substrings between an open and a closing delimiter token are replaced with a new string, for example:
```
The name of the module is [name].
```

In this case "[" and "]" is the opening and the closing delimiter tokens. A installed handler can replace [name] with a replacement string. One or more handlers can be installed for a pair of delimiter tokens. If no handler is found to replace the text, the string is left unchanged.

# Example

To run the example open a codespace and compile the project. There is a Makefile so simply type ``` make ``` when the codespace is started.

Run an example using by trying something like this:

```
./build/strsub  "[name] was in [location] on #219# [date] #219#"
### EXAMPLE 1 ###

    Mark S. was in Amsterdam on #219# 2023-01-11 11:58:43 #219#

### EXAMPLE 2 ###

    Mark S. was in Amsterdam on � 2023-01-11 11:58:43 �

```

Or you can use a specific configuration file using the --config option:
```
./build/strsub  "[name] was in [location] on #219# %date% #219#" --config=./test/test.cfg
### EXAMPLE 1 ###

    Helly R. was in Dover on #219# 2023-01-11 11:59:38 #219#

### EXAMPLE 2 ###

    Helly R. was in Dover on � 2023-01-11 11:59:38 �

```

> **Note**: In this example we also use the second handler (%%) to replace date, this is only for example purposes:

## Example 1:

Parse a string from a source to a destination replacing text from the configuration file and we also use the [date] string. In this examples [date] and %date% is the same.

## Example 2:

We use the output from example 2 but this time also replaces the text between the '#' delimiters. Here, pay attention to the subtle difference, we are directly outputting to the console but by using the substitution function we can substitute a character before the output is written to the console.

