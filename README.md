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

There is also an escape character:
```
./build/strsub "\[name] will be replaced by [name]."
### EXAMPLE 1 ###

    [name] will be replaced by Mark S.
```

## Example 1:

In this example, we are parsing a string from a source to a destination by replacing text from the configuration file. The replacement is the "value" of the "key-value" pair in a configuration text file. We also have a specific handler to replace [date] with the current date and time. In this examples [date] and %date% is the same.

## Example 2:

In this example, we use the output from the previous example and additionally substitute the text between the '#' delimiters. Note the subtle difference, we are directly outputting the character corresponding to the ASCII key code to the console using the substitution function. By using this method, we can also execute other commands such as changing the font through delimiter-coded commands.


