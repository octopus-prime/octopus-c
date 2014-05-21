/*
 * iterative_deepening.hpp
 *
 *  Created on: 30.06.2013
 *      Author: mike_gresens
 */

#pragma once

#include "environment.hpp"
#include "principal_variation_search.hpp"
//#include "node_evaluator.hpp"
#include <chrono>
#include <thread>
#include <tbb/parallel_invoke.h>

namespace chess {

class iterative_deepening
{
private:
	typedef std::chrono::system_clock::time_point time_point;

public:
	typedef std::chrono::system_clock::duration duration;
	typedef std::array<std::string, 256> PV;
	typedef std::function<void (const std::uint_fast8_t iteration, const std::uint_fast8_t max_height, const score_t score, const duration duration, const std::size_t count, const PV::const_iterator pv_begin, const PV::const_iterator pv_end)> report_function;
/*
	static PV::const_iterator foo(node_t node, const line_t& line, PV::iterator pv, std::size_t length)
	{
		for (std::size_t i = 0; i < length; ++i)
		{
			const move_t move = line[i];
			if (!move)
				break;
			*pv++ = boost::lexical_cast<std::string>(std::make_pair(std::ref(node), std::ref(move)));
			node = node_t(node, move, active_moves_tag());
		}
		return pv;
	}
*/
private:
	environment_t& environment;
	principal_variation_search search;
	report_function report;
	volatile bool done;

protected:
	void iterative_search(const std::string& fen, const std::uint_fast8_t depth, const time_point& until)
	{
		tbb::task_group_context context;
		std::thread thread;
		done = false;

		auto do_wait = [&context, &thread, &until, this]()
		{
			thread = std::thread
			(
				[&context, &until, this]()
				{
					while (std::chrono::system_clock::now() < until && done == false)
						std::this_thread::sleep_for(std::chrono::seconds(1));
					context.cancel_group_execution();
				}
			);
		};

		auto do_search = [&context, this, &fen, depth]()
		{
			const node_t node(fen);

//			line_t line;
			score_t score;
			PV pv;
			const auto ts = std::chrono::system_clock::now();
			for (std::uint_fast8_t iteration = 1; iteration <= depth; ++iteration)
			{
				score = search(node, -30000, +30000, iteration, 0, move_t());//, line);

				const auto te = std::chrono::system_clock::now();

				if (context.is_group_execution_cancelled())
					break;

				report(iteration, environment.max_height(), score, te - ts, environment.node_count(), pv.begin(), search(node, iteration, pv.begin()));
//				report(iteration, environment.max_height(), score, te - ts, environment.node_count(), pv.begin(), foo(node, line, pv.begin(), iteration));

				if (-30000 + 100 > score || score > +30000 - 100)
					break;
			}

			done = true;
		};
/*
		auto do_search_aspiration = [&context, this, &done, &fen, depth]()
		{
			const node_t node(fen);

			score_t score = 0;//node_evaluator()(node);
			PV pv;
			const auto ts = std::chrono::system_clock::now();
			for (std::uint_fast8_t iteration = 1; iteration <= depth; ++iteration)
			{
				const score_t a = score - 25;
				const score_t b = score + 25;

				score = search(node, a, b, iteration, 0, move_t());

				if (context.is_group_execution_cancelled())
					break;

				const auto te = std::chrono::system_clock::now();

				report(iteration, environment.max_height(), score, te - ts, environment.node_count(), pv.begin(), search(node, iteration, pv.begin()));

				if (score <= a)
				{
					const score_t a = score - 100;
					const score_t b = score + 0;

					score = search(node, a, b, iteration, 0, move_t());

					if (context.is_group_execution_cancelled())
						break;

					const auto te = std::chrono::system_clock::now();

					report(iteration, environment.max_height(), score, te - ts, environment.node_count(), pv.begin(), search(node, iteration, pv.begin()));

					if (score <= a)
					{
						score = search(node, -30000, score, iteration, 0, move_t());

						if (context.is_group_execution_cancelled())
							break;

						const auto te = std::chrono::system_clock::now();

						report(iteration, environment.max_height(), score, te - ts, environment.node_count(), pv.begin(), search(node, iteration, pv.begin()));
					}
					else if (score >= b)
					{
						score = search(node, score, +30000, iteration, 0, move_t());

						if (context.is_group_execution_cancelled())
							break;

						const auto te = std::chrono::system_clock::now();

						report(iteration, environment.max_height(), score, te - ts, environment.node_count(), pv.begin(), search(node, iteration, pv.begin()));
					}
				}

				else if (score >= b)
				{
					const score_t a = score - 0;
					const score_t b = score + 100;

					score = search(node, a, b, iteration, 0, move_t());

					if (context.is_group_execution_cancelled())
						break;

					const auto te = std::chrono::system_clock::now();

					report(iteration, environment.max_height(), score, te - ts, environment.node_count(), pv.begin(), search(node, iteration, pv.begin()));

					if (score <= a)
					{
						score = search(node, -30000, score, iteration, 0, move_t());

						if (context.is_group_execution_cancelled())
							break;

						const auto te = std::chrono::system_clock::now();

						report(iteration, environment.max_height(), score, te - ts, environment.node_count(), pv.begin(), search(node, iteration, pv.begin()));
					}
					else if (score >= b)
					{
						score = search(node, score, +30000, iteration, 0, move_t());

						if (context.is_group_execution_cancelled())
							break;

						const auto te = std::chrono::system_clock::now();

						report(iteration, environment.max_height(), score, te - ts, environment.node_count(), pv.begin(), search(node, iteration, pv.begin()));
					}
				}

				if (-30000 + 100 > score || score > +30000 - 100)
					break;
			}

			done = true;
		};
*/
		try
		{
			tbb::parallel_invoke
			(
				do_wait,
				do_search,//_aspiration,
				context
			);
		}
		catch (...)
		{
			done = true;
			thread.join();
			throw;
		}

		thread.join();
	}

public:
	iterative_deepening(environment_t& environment, const report_function report)
	:
		environment(environment),
		search(environment),
		report(report),
		done()
	{
	}

	void operator()(const std::string& fen, const std::uint_fast8_t depth)
	{
		iterative_search
		(
			fen,
			depth,
			time_point::max()
		);
	}

	void operator()(const std::string& fen, const std::chrono::seconds seconds)
	{
		iterative_search
		(
			fen,
			std::numeric_limits<std::uint_fast8_t>::max(),
			std::chrono::system_clock::now() + seconds
		);
	}

	void stop()
	{
		done = true;
	}
};

}
