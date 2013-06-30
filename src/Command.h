/**
 * Small Test Shell
 * (c) Josh Farr
 */
#pragma once

#include <string>
#include <vector>

class Command
{
	public:
		typedef enum Connector 
		{ 
			NONE, 
			PIPE, 
			REDIRECT_INPUT, 
			REDIRECT_OUTPUT, 
			BACKGROUND 
		} Connector;

		typedef enum Status 
		{
			STATUS_NONE, 
			STATUS_EXECED 
		} Status;

		Command();

		void name(const std::string & str);

		std::string name();

		void arg(const std::string & str);

		std::string args();

		std::vector<std::string> & arg_list();

		void clear();

		void connector(Connector conn);
		Connector connector();

		int * pipe();

		void pipe(int * p);

		Status status();

		void status(Status s);

		int pid();

		void pid(int p);

		void piper(Command * p);

		Command * piper();

	private:
		std::string _name;
		std::vector<std::string> _argv;
		Connector _connector;
		int _pipe[2];
		Status _status;
		int _pid;
		Command * _piper;
};