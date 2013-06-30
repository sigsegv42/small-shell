/**
 * Small Test Shell
 * (c) Josh Farr
 */

#include "Chain.h"

#include <cassert>
#include <iostream>

void Chain::process(const std::string cmds)
{
	// break cmds into a list of command's
	std::string piece;
	bool in_arg = false;
	Command cmd;
	for (unsigned int i = 0; i < cmds.size(); i++)
	{
		// skip extra whitespace
		if (cmds[i] == ' ' && piece.size() == 0)
			continue;
		if (cmds[i] == ' ')
		{
			// got a command name
			if (!in_arg)
			{
				cmd.name(piece);
				in_arg = true;
			}
			else // got a command argument
			{
				cmd.arg(piece);
			}
			piece = "";
		}
		else if (cmds[i] == '|')
		{
			cmd.connector(Command::PIPE);
			_commands.push_back(cmd);
			cmd.clear();
			in_arg = false;
		}
		else if (cmds[i] == '>')
		{
			cmd.connector(Command::REDIRECT_OUTPUT);
			_commands.push_back(cmd);
			cmd.clear();
			in_arg = false;
		}
		else if (cmds[i] == '<')
		{
			cmd.connector(Command::REDIRECT_INPUT);
			_commands.push_back(cmd);
			cmd.clear();
			in_arg = false;
		}
		else if (cmds[i] == '&')
		{
			cmd.connector(Command::BACKGROUND);
			_commands.push_back(cmd);
			cmd.clear();
			in_arg = false;
		}
		else if (cmds[i] == ';')
		{
			_commands.push_back(cmd);
			cmd.clear();
			in_arg = false;
		}
		else
		{
			piece += cmds[i];
		}
	}
	if (piece.size() > 0)
	{
		// got a command name
		if (!in_arg)
		{
			cmd.name(piece);
			in_arg = true;
		}
		else // got a command argument
		{
			cmd.arg(piece);
		}
		piece = "";
		_commands.push_back(cmd);
		cmd.clear();
		in_arg = false;
	}
}
void Chain::exec(void)
{
	std::vector<Command>::iterator it = _commands.begin();

	for (; it != _commands.end(); it++)
	{
		if (it->status() == Command::STATUS_NONE)
			exec(it);
	}
}
void Chain::exec(std::vector<Command>::iterator & cmd)
{
	// redirect the output of this command to the input of the next command
	if (cmd->connector() == Command::PIPE)
	{
		// create a pipe
		int * fd = cmd->pipe();
		if (pipe(fd) == -1)
		{
			std::cerr << "pipe failed" << std::endl;
		}
		// execute the 2nd command first
		// this is the recursive bit
		std::vector<Command>::iterator cmd2 = cmd + 1;
		assert(cmd2 != _commands.end());
		cmd2->piper(&(*cmd));
		exec(cmd2);
		// now execute original command
		int pid;
		pid = exec(*cmd);
		assert(pid != 0);
		cmd++;

		// close the file descriptors
		close(fd[0]);
		close(fd[1]);
		// wait for the process to finish
		int status;
		while (wait(&status) != pid);
	}
	else
	{
		// now execute command
		int pid;
		pid = exec(*cmd);
		assert(pid != 0);
	}
}
int Chain::exec(Command & cmd)
{
	///std::cout << "cmd = " << cmd.name() << " - " << cmd.args() << std::endl;
	// handle built-in commands
	if (cmd.name() == "quit" || cmd.name() == "exit")
	{
		exit(EXIT_SUCCESS);
	}

	// turn the argument list into a c-style array that exec can handle
	std::vector<std::string> & args = cmd.arg_list();
	char **argv = new char *[args.size() + 2];
	// the first argument is the name of the command
	argv[0] = new char [cmd.name().size() + 1];
	strncpy(argv[0], cmd.name().c_str(), cmd.name().size());
	// followed by the actual arguments
	for (unsigned int j = 1; j <= args.size(); j++)
	{
		char * arg = new char [args[j - 1].size() + 1];
		strncpy(arg, args[j - 1].c_str(), args[j - 1].size());
		argv[j] = arg;
	}
	// and terminated with a null entry
	argv[args.size() + 1] = 0;

	// fork a new process for the command to execute in
	pid_t pid;
	pid = fork();
	cmd.pid(pid);
	cmd.status(Command::STATUS_EXECED);
	if (pid == 0)
	{
		/**
			if we're piping to or from this command then we need a close/dup2/close call series here.
			if we're piping the output of this command to another command then it's:
			close(fd[0]); dup2(fd[1], 1); close(fd[1]);
			if we're piping the output of another command into this command then it's:
			close(fd[1]); dup2(fd[0], 0); close(fd[0]);
			but in this case the fd set comes from the previous command
		**/
		Command * prev;
		prev = cmd.piper();
		// if there's a piper, it's the command before this one
		if (prev != 0)
		{
			// we redirect the output of the piper to the input of this command
			int * fd = prev->pipe();
			close(fd[1]); 
			dup2(fd[0], 0); 
			close(fd[0]);
		}
		if (cmd.connector() == Command::PIPE)
		{
			// we redirect the output of this command to the next command
			int * fd = cmd.pipe();
			close(fd[0]); 
			dup2(fd[1], 1); 
			close(fd[1]);
		}
		// execute the command
		int eval;
		eval = execvp(cmd.name().c_str(), argv);
		return 0;
	}

	// cleanup our c-style array
	for (unsigned int j = 0; j < args.size(); j++)
	{
		delete argv[j];
	}
	delete [] argv;

	return pid;
}
