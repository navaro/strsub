# strsub
This is a string substitution library written in C.

Substrings between an open and a closing delimiter token are replaced with a new string, for example:
```
The name of the module is [name].
```

In this case "[" and "]" is the opening and the closing delimiter tokens. An installed handler can replace [name] with a replacement string. One or more handlers can be installed for a pair of delimiter tokens.

# Example

To run the example open a codespace and compile the project. There is a Makefile so simply type ``` make ``` when the codespace is started.

Run an example using by trying something like this:


## Example 1