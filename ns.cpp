#ifndef POINTSTRUCTURES_CPP
#define POINTSTRUCTURES_CPP
#include "StatusEnums.cpp"

namespace PointStructures {

	struct time_stamp {
		int s;
		int ns;
	};

	struct data_point {
		time_stamp timestamp;
		double value;
		int quality;
		QualityEnum::Quality quality_status;
	};
}

#endif