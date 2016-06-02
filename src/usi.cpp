
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <functional>
using std::string;

#include "usi.h"

namespace Usi {

	typedef std::function<void(std::vector<string>)> UsiCommandProc;

	namespace {
		bool usi_exit = false;
	}

	void command_usi(std::vector<string> params)
	{
	}

	void command_isready(std::vector<string> params)
	{
	}

	void command_setoption(std::vector<string> params)
	{
	}

	void command_usinewgame(std::vector<string> params)
	{
	}

	void command_usiposition(std::vector<string> params)
	{
	}

	void command_go(std::vector<string> params)
	{
	}

	void command_stop(std::vector<string> params)
	{
	}

	void command_ponderhit(std::vector<string> params)
	{
	}

	void command_quit(std::vector<string> params)
	{
	}

	void command_gameover(std::vector<string> params)
	{
	}

	void usi_run(std::istream& sin, std::ostream& oout)
	{
		std::map<string, UsiCommandProc> command_map;
		command_map.insert(std::make_pair("usi",		&command_usi));
		command_map.insert(std::make_pair("isready",	&command_isready));
		command_map.insert(std::make_pair("setoption",	&command_setoption));
		command_map.insert(std::make_pair("usinewgame",	&command_usinewgame));
		command_map.insert(std::make_pair("position",	&command_usiposition));
		command_map.insert(std::make_pair("go",			&command_go));
		command_map.insert(std::make_pair("stop",		&command_stop));
		command_map.insert(std::make_pair("ponderhit",	&command_ponderhit));
		command_map.insert(std::make_pair("quit",		&command_quit));
		command_map.insert(std::make_pair("gameover",	&command_gameover));

		while ( !usi_exit ) {
			string s;
			std::getline(sin, s);

			std::istringstream iss(s);
			for ( string param; std::getline(iss, param, ' '); ) {
			}
		}
	}
}
