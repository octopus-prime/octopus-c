/*
 * universal_chess_interface.cpp
 *
 *  Created on: 12.07.2013
 *      Author: mike_gresens
 */

#include "environment.hpp"
#include "search/iterative_deepening.hpp"
#include "io.hpp"

class engine
{
private:
	std::string egtb_path;
	std::size_t egtb_size;
	std::size_t hash_size;
	std::string fen;
	std::shared_ptr<chess::environment_t> environment;

protected:
	void parse_option(const std::string& command);
	void parse_position(const std::string& command);

	static void print_info(const std::uint_fast8_t iteration, const std::uint_fast8_t max_height, const chess::score_t score, const chess::iterative_deepening::duration duration, const std::size_t count, const chess::iterative_deepening::PV::const_iterator pv_begin, const chess::iterative_deepening::PV::const_iterator pv_end);

public:
	engine();
	void parse(const std::string& command);
};

engine::engine()
:
	egtb_path("/home/mike_gresens/Downloads/egtb"),
	egtb_size(256),
	hash_size(1024),
	fen(),
	environment()
{
}

void
engine::parse(const std::string& command)
{
	std::size_t index;

	// ucinewgame
	index = command.find("ucinewgame");
	if (std::string::npos != index)
	{
		environment = std::make_shared<chess::environment_t>(hash_size << 20, egtb_path, egtb_size << 20);
		fen.clear();
		return;
	}

	// uci
	index = command.find("uci");
	if (std::string::npos != index)
	{
		std::cout << "id name Assassin" << std::endl;
		std::cout << "id author Mike Gresens" << std::endl;
		std::cout << "option name Hash type spin min 1024 max 8192 default " << hash_size << std::endl;
		std::cout << "option name NalimovPath type string default " << egtb_path << std::endl;
		std::cout << "option name NalimovCache type spin min 1024 max 8192 default " << egtb_size << std::endl;
		std::cout << "uciok" << std::endl;
		return;
	}

	// setoption
	index = command.find("setoption name");
	if (std::string::npos != index)
	{
		parse_option(command.substr(index + 15));
		return;
	}

	// isready
	index = command.find("isready");
	if (std::string::npos != index)
	{
		std::cout << "readyok" << std::endl;
		return;
	}

	// position
	index = command.find("position");
	if (std::string::npos != index)
	{
		parse_position(command.substr(index + 9));
		return;
	}

	index = command.find("go");
	if (std::string::npos != index)
	{
		chess::iterative_deepening search(*environment, print_info);
		std::thread foo
		(
			[&search]()
			{
				for (;;)
				{
					std::string command;
					std::getline(std::cin, command);

					if (command == "quit")
						exit(0);

					if (command == "stop")
						break;
				}
				search.stop();
			}
		);
		search(fen, std::chrono::seconds(900));
		foo.join();
		std::cout << "bestmove " << "e2e4" << std::endl;
		return;
	}
}

void
engine::parse_option(const std::string& command)
{
	std::size_t index;

	// Hash
	index = command.find("Hash value");
	if (std::string::npos != index)
	{
		hash_size = std::stoi(command.substr(index + 11));
		return;
	}

	// NalimovPath
	index = command.find("NalimovPath value");
	if (std::string::npos != index)
	{
		egtb_path = command.substr(index + 18);
		return;
	}

	// NalimovCache
	index = command.find("NalimovCache value");
	if (std::string::npos != index)
	{
		egtb_size = std::stoi(command.substr(index + 19));
		return;
	}
}

void
engine::parse_position(const std::string& command)
{
	std::size_t index;

	index = command.find("startpos");
	if (std::string::npos != index)
	{
		fen = chess::node_t::INITIAL_POSITION;
		return;
	}

	index = command.find("fen");
	if (std::string::npos != index)
	{
		fen = command.substr(index + 4);
		return;
	}
}

void
engine::print_info(const std::uint_fast8_t iteration, const std::uint_fast8_t max_height, const chess::score_t score, const chess::iterative_deepening::duration duration, const std::size_t count, const chess::iterative_deepening::PV::const_iterator pv_begin, const chess::iterative_deepening::PV::const_iterator pv_end)
{
	const auto cs = std::chrono::duration_cast<std::chrono::duration<long, std::centi>>(duration);
	const auto s = std::chrono::duration_cast<std::chrono::duration<double, std::ratio<1>>>(duration);

	std::cout << "info";
	std::cout << " depth " << int(iteration);
	std::cout << " score cp " << score;
	std::cout << " time " << cs.count();
	std::cout << " nodes " << count;
	std::cout << " nps " << int(count / s.count());
	std::cout << " pv ";
	std::copy(pv_begin, pv_end, std::ostream_iterator<std::string>(std::cout, " "));
	std::cout << std::endl;
};

int main()
{
	engine engine;

	for (;;)
	{
		std::string command;
		std::getline(std::cin, command);

		if (command == "quit")
			break;

		engine.parse(command);
	}

	return 0;
}
