
#include "common.h"
#include "usi.h"
#include "board.h"

namespace Usi {

	struct UsiCommand {
		string command;
		vector<string> params;
	};

	typedef function<void(UsiCommand)> UsiCommandProc;

	namespace {
		struct Stream {
			istream& is;
			ostream& os;
		};
		shared_ptr<Stream> stream_;
	}

	shared_ptr<UsiCommand> input_usi()
	{
		string s;
		std::getline(stream_->is, s);

		list<string> params;
		string param;
		istringstream iss(s);
		while ( std::getline(iss, param, ' ') ) {
			params.push_back(param);
		}

		if ( !params.empty() ) {
			auto command = params.front();
			params.pop_front();
			return shared_ptr<UsiCommand>(new UsiCommand { command, vector<string>(params.begin(), params.end()) });
		}
		else {
			return nullptr;
		}
	}

	void output_usi(const UsiCommand& command)
	{
		stream_->os << command.command;
		for ( auto param : command.params ) {
			stream_->os << " " << param;
		}
		stream_->os << std::endl;
	}

	///-----------------------------------------------------------------------------------------------------------------
	///	usi コマンドに対する処理
	///
	void command_usi(const UsiCommand& command)
	{
		// usiコマンドの返信として、以下のコマンドを返す。
		//   id name <エンジン名>
		//   id author <制作者名>
		//   usiok
		//
		// usiokの返信までに時間がかかってはダメ。
		//

		output_usi(UsiCommand { "id", { "name",   "Kioh Tempura" } });
		output_usi(UsiCommand { "id", { "author", "Tempura" } });
		output_usi(UsiCommand { "usiok" });
	}

	///-----------------------------------------------------------------------------------------------------------------
	///	isready コマンドに対する処理
	///
	void command_isready(const UsiCommand& command)
	{
		// 対局開始前のエンジン初期化処理を行い、対局準備が整ったら readyok を返す。

		output_usi(UsiCommand { "readyok" });
	}

	void command_setoption(const UsiCommand& command)
	{
	}

	void command_usinewgame(const UsiCommand& command)
	{
	}

	void command_usiposition(const UsiCommand& command)
	{
	}

	void command_go(const UsiCommand& command)
	{
	}

	void command_stop(const UsiCommand& command)
	{
	}

	void command_ponderhit(const UsiCommand& command)
	{
	}

	void command_quit(const UsiCommand& command)
	{
	}

	void command_gameover(const UsiCommand& command)
	{
	}

	void command_debug(const UsiCommand& command)
	{
		auto& os = stream_->os;

		if ( command.params.empty() ) {
			// 盤面表示
			os << Board::current() << std::endl;
			return;
		}

		auto sub_command = command.params[0];
		if ( sub_command == "board" ) {
			// 盤面表示
			os << Board::current() << std::endl;
		}
	}

	void usi_run(istream& is, ostream& os)
	{
		stream_ = shared_ptr<Stream>(new Stream { is, os });

		map<string, UsiCommandProc> command_map;
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
		command_map.insert(std::make_pair("d",			&command_debug));

		for ( ;; ) {
			auto command = input_usi();
			if ( command != nullptr ) {
				auto it = command_map.find(command->command);
				if ( it != command_map.end() ) {
					it->second(*command);
				}
			}
		}
	}
}
