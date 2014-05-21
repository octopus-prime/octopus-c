#pragma once

#include "node.hpp"
#include "score.hpp"
#include <cstdint>
#include <memory>
#include <mutex>

namespace chess {

class tablebase_t
{
private:
	std::mutex _mutex;
	std::unique_ptr<char[]> _cache;
	std::size_t _pieces;

public:
	tablebase_t(const std::string& path, const std::size_t size);

	operator std::size_t() const;
	bool operator()(const node_t& node, const std::uint_fast8_t height, score_t& score) const;
};

inline
tablebase_t::operator std::size_t() const
{
	return _pieces;
}

}
