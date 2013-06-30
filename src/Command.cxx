/**
 * Small Test Shell
 * (c) Josh Farr
 */

#include "Command.h"

Command::Command() : _connector(NONE), _status(STATUS_NONE), _piper(0), _pid(-1)
{
}

void Command::name(const std::string & str)
{
	_name = str;
}

std::string Command::name()
{
	return _name;
}

void Command::arg(const std::string & str)
{
	_argv.push_back(str);
}

std::string Command::args()
{
	std::string str;
	for (unsigned int i = 0; i < _argv.size(); i++)
	{
		if (i > 0)
			str += " ";
		str += _argv[i];
	}
	return str;
}

std::vector<std::string> & Command::arg_list()
{
	return _argv;
}

void Command::clear()
{
	_name = "";
	_argv.clear();
	_connector = NONE;
	_status = STATUS_NONE;
	_piper = 0;
	_pid = -1;
}

void Command::connector(Connector conn)
{
	_connector = conn;
}

Command::Connector Command::connector()
{
	return _connector;
}

int * Command::pipe()
{
	return _pipe;
}

void Command::pipe(int * p)
{
	memcpy(_pipe, p, sizeof(int) * 2);
}

Command::Status Command::status()
{
	return _status;
}

void Command::status(Status s)
{
	_status = s;
}

int Command::pid()
{
	return _pid;
}

void Command::pid(int p)
{
	_pid = p;
}

void Command::piper(Command * p)
{
	_piper = p;
}

Command * Command::piper()
{
	return _piper;
}
