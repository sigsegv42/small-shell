/**
 * Small Test Shell
 * (c) Josh Farr
 */
#pragma once

#include "Command.h"

class Chain
{
	public:
		void process(const std::string cmds);

		void exec();

		void exec(std::vector<Command>::iterator & cmd);

		int exec(Command & cmd);

	private:
		std::vector<Command> _commands;
};