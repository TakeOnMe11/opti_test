#pragma once
#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include "ns.cpp"
#include "single_include/nlohmann/json.hpp"
#include "StatusEnums.cpp"

using json = nlohmann::json;

class Database{
private:
	std::string path = "datasets/";
	double sum, average = 0;
public:
	int savePoint(const PointStructures::data_point& point, const std::string& file_name);

	int calculateAvg(const std::string& file_name,
		const PointStructures::time_stamp& start_time,
		const PointStructures::time_stamp& end_time,
		PointStructures::data_point& res_point);
};

#endif // !DATABASE_H