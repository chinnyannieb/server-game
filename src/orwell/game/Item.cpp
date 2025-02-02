#include "orwell/game/Item.hpp"

#include "orwell/game/Team.hpp"
#include "orwell/game/item/Flag.hpp"
#include "orwell/support/GlobalLogger.hpp"
#include "orwell/game/Ruleset.hpp"

#include <sstream>

std::map<std::string, std::shared_ptr<orwell::game::Item> > orwell::game::Item::s_itemsByRfid = std::map<std::string, std::shared_ptr<orwell::game::Item> >();
std::map<int32_t, std::shared_ptr<orwell::game::Item> > orwell::game::Item::s_itemsByColor = std::map<int32_t, std::shared_ptr<orwell::game::Item> >();

namespace orwell
{
namespace game
{

Item::Item(
		std::string const & iName,
		std::string const & iRfid,
		boost::posix_time::milliseconds const & iTimeToCapture)
	: m_name(iName)
	, m_rfid(iRfid)
	, m_color(-1)
	, m_timeToCapture(iTimeToCapture)
{
	ORWELL_LOG_DEBUG("new Item (" << iName <<") by Rfid ("<< iRfid << ")");
}

Item::Item(
		std::string const & iName,
		int32_t const iColorCode,
		boost::posix_time::milliseconds const & iTimeToCapture)
	: m_name(iName)
	, m_color(iColorCode)
	, m_timeToCapture(iTimeToCapture)
{
	ORWELL_LOG_DEBUG("new Item (" << iName <<") by color ("<< iColorCode << ")");
}

Item::~Item()
{
}

void Item::InitializeStaticMaps()
{
	Item::s_itemsByRfid = std::map<std::string, std::shared_ptr<Item> >();
	Item::s_itemsByColor = std::map<int32_t, std::shared_ptr<Item> >();
}

std::string const & Item::getName() const
{
	return m_name;
}

std::string const & Item::getRfid() const
{
	return m_rfid;
}

int32_t Item::getColor() const
{
	return m_color;
}

std::shared_ptr<Item> Item::GetItemByRfid(
		std::string const & iRfid)
{
	std::shared_ptr< Item > aFound;
	std::map<std::string, std::shared_ptr<Item> >::iterator it = s_itemsByRfid.find(iRfid);
	if (it != s_itemsByRfid.end())
	{
		aFound = it->second;
	}
	else
	{
		ORWELL_LOG_ERROR("Tried to retrieve Item by RFID : " << iRfid << ". Not found");
	}
	return aFound;
}

std::shared_ptr<Item> Item::GetItemByColor(
		int32_t const iColorCode)
{
	std::shared_ptr< Item > aFound;
	std::map<std::int32_t, std::shared_ptr<Item> >::iterator it = s_itemsByColor.find(iColorCode);
	if (it != s_itemsByColor.end())
	{
		aFound = it->second;
	}
	else
	{
		ORWELL_LOG_ERROR("Tried to retrieve Item by Color code : " << iColorCode << ". Not found");
	}
	return aFound;
}

std::shared_ptr<Item> Item::CreateItem(
		std::string const & iType,
		std::string const & iName,
		std::string const & iRfid,
		int32_t const iColorCode,
		Ruleset const & iRuleset)
{
	if ("flag" == iType)
	{
		if (iRfid.empty() and -1 != iColorCode)
		{
			if (s_itemsByColor.find(iColorCode) != s_itemsByColor.end())
			{
				ORWELL_LOG_ERROR("Tried to add new Item by Color code : " << iColorCode << ". Already exists.");
			}
			else
			{
				std::shared_ptr<item::Flag> aNewFlag = std::make_shared<item::Flag>(
						iName,
						iColorCode,
						iRuleset.m_timeToCapture,
						iRuleset.m_pointsOnCapture);
				s_itemsByColor[iColorCode] = aNewFlag;
				return aNewFlag;
			}
		}
		else if (not iRfid.empty() and -1 == iColorCode)
		{
			if (s_itemsByRfid.find(iRfid) != s_itemsByRfid.end())
			{
				ORWELL_LOG_ERROR("Tried to add new Item by Rfid : " << iRfid << ". Already exists.");
			}
			else
			{
				std::shared_ptr<item::Flag> aNewFlag = std::make_shared<item::Flag>(
						iName,
						iRfid,
						iRuleset.m_timeToCapture,
						iRuleset.m_pointsOnCapture);
				s_itemsByRfid[iRfid] = aNewFlag;
				return aNewFlag;
			}
		}
	}

	return nullptr;
}

std::string Item::toLogString() const
{
	std::stringstream aLogString;
	aLogString << *this;
	return aLogString.str();
}

void Item::capture(Team & ioTeam)
{
	if (ioTeam.getName() != m_owningTeam)
	{
		innerCapture(ioTeam);
		m_owningTeam = ioTeam.getName();
	}
}

} // game
} // orwell

std::ostream & operator<<(
		std::ostream & ioOstream,
		orwell::game::Item const & aItem)
{
	ioOstream << "Item : " << aItem.getName();
	if (not aItem.getRfid().empty())
	{
		ioOstream << " - rfid : " << aItem.getRfid();
	}
	if (aItem.getColor() >= 0)
	{
		ioOstream << " - color : " << aItem.getColor();
	}
	return ioOstream;
}

