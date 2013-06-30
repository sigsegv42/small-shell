# Small Shell

This is a small command line shell application.  It's just a simple proof of concept and not meant to actually replace a real shell (e.g. bash, zsh, etc).


## Features
* execute single commands, passing any arguments (the execvp system call searches the PATH env var transparently)
* execute multiple commands in sequence using the command separator ';'
* pipe i/o of arbitrary command chains using the '|' separator (using a recursive exec method)
* built-in 'quit' / 'exit' command to terminate shell


## Todo
* I/O redirection (processing of '>' and '<' operators already supported)
* job management ('fg' and 'bg' built-in commands, auto-background with '&' operator)
* error handling for 'command not found'
* command history
