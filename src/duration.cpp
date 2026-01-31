#include "duration.h"

Duration::Duration()
	: duration_(0), kind_(Kind::LINEAR)
{}

Duration::Duration(Uint64 duration, Kind kind)
	: duration_(duration), kind_(kind)
{}