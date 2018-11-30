#include "LocationKind.h"

#include "SourcetrailException.h"

namespace sourcetrail
{
	LocationKind intToLocationKind(int i)
	{
		switch (i)
		{
		case LOCATION_TOKEN:
			return LOCATION_TOKEN;
		case LOCATION_SCOPE:
			return LOCATION_SCOPE;
		case LOCATION_QUALIFIER:
			return LOCATION_QUALIFIER;
		case LOCATION_LOCAL_SYMBOL:
			return LOCATION_LOCAL_SYMBOL;
		case LOCATION_SIGNATURE:
			return LOCATION_SIGNATURE;
		case LOCATION_COMMENT:
			return LOCATION_COMMENT;
		case LOCATION_ERROR:
			return LOCATION_ERROR;
		case LOCATION_FULLTEXT_SEARCH:
			return LOCATION_FULLTEXT_SEARCH;
		case LOCATION_SCREEN_SEARCH:
			return LOCATION_SCREEN_SEARCH;
		}
		throw SourcetrailException("Unable to convert integer \"" + std::to_string(i) + "\" to location kind.");
	}

	int locationKindToInt(LocationKind locationKind)
	{
		return locationKind;
	}
}
