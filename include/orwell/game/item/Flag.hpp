/*  */

#include "orwell/game/Item.hpp"

#pragma once

#include <string>
#include <memory>

namespace orwell
{
namespace game
{
namespace item
{

class Flag : public Item
{
public :
	Flag(
			std::string const & iName,
			std::string const & iRfid,
			boost::posix_time::milliseconds const & iTimeToCapture,
			uint32_t const iPointsOnCapture);

	Flag(
			std::string const & iName,
			int32_t const iColorCode,
			boost::posix_time::milliseconds const & iTimeToCapture,
			uint32_t const iPointsOnCapture);

	virtual ~Flag();


private :
	uint32_t m_pointsOnCapture;

	void innerCapture(Team & ioTeam) override;
};

} // item
} // game
} // orwell

