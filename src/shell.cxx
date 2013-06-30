/**
Small Test Shell
Version 0.0.1
(c) Josh Farr

Features
- execute single commands, passing any arguments (the execvp system call searches the PATH env var transparently)
- execute multiple commands in sequence using the command separator ';'
- pipe i/o of arbitrary command chains using the '|' separator (using a recursive exec method)
- built-in 'quit' / 'exit' command to terminate shell

Todo
- i/o redirection (processing of '>' and '<' operators already supported)
- job management ('fg' and 'bg' built-in commands, auto-background with '&' operator)
- error handling for 'command not found'
- command history

Notes
- there may still be problems with argv passing. sometimes passed args aren't recognized by
the child process. possibly an allocation problem.
- the prompt doesn't always get redrawn after a child process finishes. it might be useful 
to rely on terminfo/termcap/curses system methods/libraries to interact with the screen more 
intelligently.

Installation & Usage
g++ shell.cxx -o shell
./shell
**/
#include <unistd.h>
#include <sys/wait.h>

#include <string>
#include <iostream>
#include <vector>

#include "Chain.h"

int main(int argc, char *argv[])
{
	const std::string prompt = "small-sh # ";

	while (1)
	{
		// display the prompt
		std::cout << prompt;

		std::string cmd = "";
		Chain cli;
		char in = '\0';
		// don't eat newlines
		std::cin.ignore(0, '\n');
		// process input a character at a time
		while ((in = std::cin.get()) != EOF)
		{
			// if CR reached then process the line and start the main loop over
			if (in == '\n')
			{
				cli.process(cmd);
				cli.exec();
				break;
			}
			cmd += in;
		}
	}
	return 0;
}

