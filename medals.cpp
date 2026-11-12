#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <unordered_map>

using std::size_t;

static void print_error(size_t line_number) {
	std::cerr << "ERROR " << line_number << std::endl;
}

static bool is_medal(const std::string &fragment) {
	if (fragment.size() == 1 &&
		(fragment.at(0) == '1' || fragment.at(0) == '2' || fragment.at(0) == '3')) {
		return true;
	}
	return false;
}

static bool is_year(const std::string &fragment) {
	if (fragment.size() != 4 && fragment.size() != 5) {
		return false;
	}

	if (fragment.at(0) < '1' || '9' < fragment.at(0)) {
		return false;
	}

	for (size_t i = 1; i < fragment.size(); ++i) {
		if (fragment.at(i) < '0' || fragment.at(i) > '9') {
			return false;
		}
	}

	// Edge cases:
	// excluding value ranges (1000 - 1799)
	if (fragment.size() == 4 && fragment.at(0) == '1' && fragment.at(1) < '8') {
		return false;
	}

	// and values between (60000 - 99999)
	if (fragment.size() == 5 && fragment.at(0) >= '6') {
		return false;
	}

	return true;
}

static bool is_game(const std::string &fragment) {
	for (char c : fragment) {
		if (not ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))) { 
			return false;
		}
	}

	return true;
}

static void end_parsing_game(bool &game_start_flag, bool &game_flag) {
	if (game_start_flag) {
		game_start_flag = false;
		game_flag = true;
	}
}

static bool read_input(size_t line_number, std::stringstream &ss,
					   std::string &year, std::string &game, std::string &medal,
					   bool &year_flag, bool &game_flag, bool &medal_flag) {
	// Data extracation is processed via stringstream word by word
	// To succesfully parse game:
	// game_start_flag and game_stream is used:
	// game_start_flag = true means that last read fragment
	// was game-type word
	// game_stream allows efficient adding new game-type words to the final
	// string

	bool game_start_flag = false;
	std::stringstream game_stream;

	// To extract single words from input we use >> operator
	// If input contains other whitespaces than given in the task
	// white_spaces_test would detect it earlier

	std::string fragment;
	while (ss >> fragment) {
		//std::cerr << fragment << std::endl;
		if (is_medal(fragment)) {

			// If there is already collected medal - the input is invalid
			if (medal_flag) {
				print_error(line_number);
				return false;
			}

			medal_flag = true;
			medal = fragment;

			end_parsing_game(game_start_flag, game_flag);
		}
		else if (is_year(fragment)) {

			// If there is already collected year - the input is invalid
			if (year_flag) {
				print_error(line_number);
				return false;
			}

			end_parsing_game(game_start_flag, game_flag);

			year_flag = true;
			year = fragment;
		}
		else if (is_game(fragment)) {

			// If there is already collected game - the input is invalid
			if (game_flag) {
				print_error(line_number);
				return false;
			}

			// Game start flag is set
			game_start_flag = true;
			game_stream << " ";
			game_stream << fragment;
		}
		else {
			print_error(line_number);
			return false;
		}
	}

	end_parsing_game(game_start_flag, game_flag);
	game = game_stream.str();

	return true;
}

static void update_data(size_t line_number, std::stringstream &ss,
						size_t &all_medals,
						std::unordered_map<std::string, size_t> &medals) {
	std::string year, game, medal;
	bool year_flag = false, medal_flag = false, game_flag = false;

	if (not read_input(line_number, ss, year, game, medal, year_flag, game_flag,
					medal_flag)) {
		return;
	}

	// If any components from the input are missing the input is invalid
	if (not year_flag || not medal_flag || not game_flag) {
		print_error(line_number);
		return;
	}

	static std::string separator = "|";

	// Otherwise, main data is updated
	++all_medals;
	++medals[{game + separator + year + "|" + medal}];

	// And also more filters for queries
	++medals[{game + "|" + year}];
	++medals[{game + "|" + medal}];
	++medals[{year + "|" + medal}];

	++medals[game];
	++medals[year];
	++medals[medal];
}

static void process_query(int line_number, std::stringstream &ss,
						  size_t &all_medals,
						  std::unordered_map<std::string, size_t> &medals) {
	std::string year, game, medal;
	bool year_flag = false, medal_flag = false, game_flag = false;

	std::string question_mark;
	ss >> question_mark;

	if (question_mark != "?") {
		print_error(line_number);
		return;
	}

	if (not read_input(line_number, ss, year, game, medal, year_flag, game_flag,
					medal_flag)) {
		return;
	}

	if (not game_flag && not year_flag && not medal_flag) {
		std::cout << all_medals << '\n';
	}
	else {
		std::stringstream map_key_stream;
		std::string space = "|";

		if (game_flag) {
			map_key_stream << game;
		}

		if (year_flag) {
			if (game_flag)
				map_key_stream << space;
			map_key_stream << year;
		}

		if (medal_flag) {
			if (year_flag || game_flag)
				map_key_stream << space;
			map_key_stream << medal;
		}

		std::string map_key = map_key_stream.str();
		if(medals.find(map_key) != medals.end()){
			std::cout << medals.at(map_key) << '\n';
		}
		else {
			std::cout << 0 << '\n';
		}
	}
}

static bool white_spaces_test(const std::string &input) {
	static const std::regex patterns(R"([\r\v\f])");

	if (std::regex_search(input, patterns)) {
		return false;
	}

	return true;
}

static void access_medals_database() {
	// Data is stored and counted in unorederd_map in which:
	// key = "game | year | medal" or any subset of {game, year, medal}
	// ex. medals["year|medal"] stores number of particular type of medals
	// collected in given year

	size_t all_medals = 0;
	std::unordered_map<std::string, size_t> medals;

	size_t line_number = 0;
	std::string input;

	// Valid line ends only with '\n'
	// which is standard setting for getline()
	while (getline(std::cin, input)) {

		++line_number;

		// ERROR - empty input
		if (input.empty()) {
			print_error(line_number);
			continue;
		}

		// ERROR - input contains whitespaces other than ' ' or '\t'
		if (not white_spaces_test(input)) {
			print_error(line_number);
			continue;
		}

		std::stringstream ss = std::stringstream(input);
		std::stringstream question_mark = std::stringstream(input);

		std::string qMark;
		question_mark >> qMark;
		if (qMark == "?") {
			process_query(line_number, ss, all_medals, medals);
		}
		else {
			update_data(line_number, ss, all_medals, medals);
		}
	}
}

int main() {
	access_medals_database();
	return 0;
}