#include <iostream>
#include <fstream>
#include <string>

#include "Database.h"
#include "StatusEnums.cpp"

int point_number, res;
double input_value;
Database *data_base = new Database();
PointStructures::data_point point, res_point;
PointStructures::time_stamp start_timestamp;
PointStructures::time_stamp end_timestamp;
PointStructures::time_stamp point_timestamp;
QualityEnum::Quality quality_status;
std::string file_name;

void printMenu() {
	std::cout << "1. Save points" << std::endl;
	std::cout << "2. Calculate average" << std::endl;
	std::cout << "3. Exit" << std::endl;
	std::cout << "> ";
}

template < typename T >
void inputValidate(T &input) {
	while (!(std::cin >> input) || (std::cin.peek() != '\n')) {
		std::cin.clear();
		while (std::cin.get() != '\n');
		std::cout << "Error! Wrong type! Try again!!!" << std::endl;
	}
}

void fileNameValidate(std::string &name) {
	while (!(std::cin >> file_name) || (std::cin.peek() != '\n') || (strstr(file_name.c_str(), "."))) {
		std::cin.clear();
		while (std::cin.get() != '\n');
		std::cout << "Error! Wrong type! String required\n";
	}
}

int getUserChoice() {
	int variant;

	while (!(std::cin >> variant) || variant < 1 || variant > 3) {
		std::cin.clear();
		while (std::cin.get() != '\n');
		std::cout << "Incorrect input. Try again: ";
	}
	return variant;
}

void savePoints() {
	std::cout << "Enter the file name without the extension: ";
	fileNameValidate(file_name);
	std::cout << "Enter the number of points in dataset: ";
	inputValidate<int>(point_number);
	std::cout << "===============\n";

	for (int i = 0; i < point_number; i++) {
		do {
			std::cout << "Enter the value: ";
			inputValidate<double>(point.value);

			std::cout << "Enter seconds of the point: ";
			inputValidate<int>(point_timestamp.s);
			std::cout << "Enter nanoseconds of the point: ";
			inputValidate<int>(point_timestamp.ns);
			
			std::cout << "Enter the quality (1-4: Bad; 5-10:Good; Other:Undefined): ";
			inputValidate<int>(point.quality);

			if (point.quality > 0 && point.quality < 5) {
				point.quality_status = QualityEnum::BAD;
			}
			else if (point.quality >= 5 && point.quality <= 10) {
				point.quality_status = QualityEnum::GOOD;
			}
			else {
				point.quality_status = QualityEnum::UNDEFINED;
			}

			point.timestamp = point_timestamp;
			res = data_base->savePoint(point, file_name);
			if (res == OperationEnum::SUCCESS) {
				std::cout << "Operation completed successfully" << std::endl;
			}
			else if (res == OperationEnum::TIME_LOWER_ZERO) {
				std::cout << "Time can't be lower than zero" << std::endl;
			}
			else {
				std::cout << "Error! Try again." << std::endl;
			}
		} while (res != OperationEnum::SUCCESS);
	}
}

void calculateAverage() {
	std::cout << "Enter the name of the file in which you want to calculate the average: ";
	fileNameValidate(file_name);
	std::cout << "Enter the seconds of the start of the interval: ";
	inputValidate<int>(start_timestamp.s);
	
	std::cout << "Enter the nanoseconds of the start of the interval: ";
	inputValidate<int>(start_timestamp.ns);
	
	std::cout << "Enter the seconds of the end of the interval: ";
	inputValidate<int>(end_timestamp.s);
	
	std::cout << "Enter the nanoseconds of the end of the interval: ";
	inputValidate<int>(end_timestamp.ns);
	
	res = data_base->calculateAvg(file_name, start_timestamp, end_timestamp, res_point);

	if (res == OperationEnum::SUCCESS) {
		json res_json;
		res_json["timestamp"]["s"] = res_point.timestamp.s;
		res_json["timestamp"]["ns"] = res_point.timestamp.ns;
		res_json["value"] = res_point.value;
		res_json["quality"] = res_point.quality;
		res_json["quality_status"] = res_point.quality_status;
		std::cout << "Result: " << std::endl;
		std::cout << std::setw(4) << res_json << std::endl;
	}
	else if (res == OperationEnum::START_GREATER_END) {
		std::cout << "Start time can't be greater end time. Repeat operation." << std::endl;
	}
	else if (res == OperationEnum::ARRAY_NULL) {
		std::cout << "No point fits the specified interval." << std::endl;
	}
	else if (res == OperationEnum::ERROR) {
		std::cout << "Error! Try to repeat the operation." << std::endl;
	}
}
 
int main(){
	int variant;
	do {
		printMenu();
		variant = getUserChoice();
		switch (variant) {
		case 1:
			savePoints();
			break;
		case 2:
			calculateAverage();
			break;
		}
	} while (variant != 3);
}
