#pragma once

#include <string>
#include <map>
#include <memory>
#include <ostream>

#include <boost/date_time/posix_time/posix_time_types.hpp>

namespace orwell
{
namespace game
{
class Team;
class Ruleset;

class Item
{
protected:
	Item(
			std::string const & iName,
			std::string const & iRfid,
			boost::posix_time::milliseconds const & iTimeToCapture);

	Item(
			std::string const & iName,
			int32_t const iColorCode,
			boost::posix_time::milliseconds const & iTimeToCapture);

	virtual ~Item();

public:
	std::string const & getName() const;
	std::string const & getRfid() const;
	int32_t getColor() const;

	static void InitializeStaticMaps();

	static std::shared_ptr<Item> GetItemByRfid(
			std::string const & iRfid);

	static std::shared_ptr<Item> GetItemByColor(
			int32_t const iColorCode);

	static std::shared_ptr<Item> CreateItem(
			std::string const & iType,
			std::string const & iName,
			std::string const & iRfid,
			int32_t const iColorCode,
			Ruleset const & iRuleset);

	virtual std::string toLogString() const;

	void capture(Team & ioTeam);

private:
	std::string m_name;
	std::string m_rfid;
	int32_t m_color;
	std::string m_owningTeam;
	boost::posix_time::milliseconds m_timeToCapture;

	static std::map<std::string, std::shared_ptr<Item> > s_itemsByRfid;
	static std::map<int32_t, std::shared_ptr<Item> > s_itemsByColor;

	virtual void innerCapture(Team & ioTeam) = 0;
};

} // game
} // orwell

std::ostream & operator<<(std::ostream& oOstream, const orwell::game::Item & aItem);

