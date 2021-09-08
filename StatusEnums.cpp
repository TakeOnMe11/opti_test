#pragma once
#ifndef STATUSENUMS_CPP
#define STATUSENUMS_CPP



namespace QualityEnum {

	enum Quality {
		GOOD,
		UNDEFINED,
		BAD
	};
}

namespace OperationEnum {

	enum OperationStatus {
		SUCCESS,
		ERROR,
		TIME_LOWER_ZERO,
		START_GREATER_END,
		ARRAY_NULL
	};
}

#endif // !STATUSENUMS_CPP