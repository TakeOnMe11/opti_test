#include "Database.h"

int Database::savePoint(const PointStructures::data_point& point, const std::string& file_name) {

	json j;

	if ((point.timestamp.s < 0) || (point.timestamp.ns < 0)) {
		return OperationEnum::TIME_LOWER_ZERO;
	}

	j["timestamp"]["s"] = point.timestamp.s;
	j["timestamp"]["ns"] = point.timestamp.ns;
	j["value"] = point.value;
	j["quality"] = point.quality;
	j["quality_status"] = point.quality_status;

	try {
		std::ofstream out(file_name + ".json", std::ios::app);
		if (out.is_open()) {
			out << j << std::endl;
		}
		out.close();
		return OperationEnum::SUCCESS;
	}
	catch (std::ios_base::failure& fail) {
		return OperationEnum::ERROR;
	}
}

int Database::calculateAvg(const std::string& file_name,
	const PointStructures::time_stamp& start_time,
	const PointStructures::time_stamp& end_time,
	PointStructures::data_point& res_point) {

	std::string line;
	int worst_quality;
	std::vector<PointStructures::data_point> point_arr;
	std::vector<PointStructures::data_point> good_point_arr;
	std::vector<int> worst_quality_arr;
	json j;
	sum = 0;

	if ((start_time.s < 0) || (start_time.ns < 0) || (end_time.s < 0) || (end_time.ns < 0)) {
		return OperationEnum::TIME_LOWER_ZERO;
	}
	if ((start_time.s > end_time.s) || ((start_time.s >= end_time.s) && (start_time.ns >= end_time.ns))) {
		return OperationEnum::START_GREATER_END;
	}

	try {
		std::ifstream in(file_name + ".json");
		if (in.is_open()) {
			while (getline(in, line)) {
				j = json::parse(line);
				PointStructures::data_point point{
					j["timestamp"]["s"].get<int>(),
					j["timestamp"]["ns"].get<int>(),
					j["value"].get<double>(),
					j["quality"].get<int>(),
					j["quality_status"].get<QualityEnum::Quality>(),
				};
				point_arr.push_back(point);
			}
		}
		in.close();
	}
	catch (std::ios_base::failure& fail) {
		return OperationEnum::ERROR;
	}

	try {
		for (PointStructures::data_point i : point_arr) {
			if (i.quality_status == QualityEnum::GOOD) {
				if (i.timestamp.s > start_time.s) {
					if (i.timestamp.s < end_time.s) {
						good_point_arr.push_back(i);
						worst_quality_arr.push_back(i.quality);
					}
					else if ((i.timestamp.s <= end_time.s) && (i.timestamp.ns <= end_time.ns)) {
						good_point_arr.push_back(i);
						worst_quality_arr.push_back(i.quality);
					}
				}
				else if ((i.timestamp.s >= start_time.s) && (i.timestamp.ns >= start_time.ns)) {
					if (i.timestamp.s < end_time.s) {
						good_point_arr.push_back(i);
						worst_quality_arr.push_back(i.quality);
					}
					else if ((i.timestamp.s <= end_time.s) && (i.timestamp.ns <= end_time.ns)) {
						good_point_arr.push_back(i);
						worst_quality_arr.push_back(i.quality);
					}
				}
			}
		}
		if (good_point_arr.empty()) {
			throw - 2;
		}
		if (worst_quality_arr.empty()) {
			throw - 1;
		}
		worst_quality = *std::min_element(worst_quality_arr.begin(), worst_quality_arr.end());

		for (PointStructures::data_point i : good_point_arr) {
			sum += i.value;
		}
		if (sum == 0) {
			throw - 1;
		}
		if (sizeof(sum) > sizeof(double)) {
			for (PointStructures::data_point i : good_point_arr) {
				sum += i.value / good_point_arr.size();
			}
		}
		else {
			average = sum / good_point_arr.size();
		}
	}
	catch (int e) {
		if (e == -2) {
			return OperationEnum::ARRAY_NULL;
		}
		return OperationEnum::ERROR;
	}

	res_point.timestamp = start_time;
	res_point.quality = worst_quality;
	res_point.quality_status = QualityEnum::GOOD;
	res_point.value = average;

	worst_quality_arr.clear();
	good_point_arr.clear();

	return OperationEnum::SUCCESS;
}
