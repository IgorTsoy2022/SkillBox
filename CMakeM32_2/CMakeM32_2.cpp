#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include "nlohmann_json/include/nlohmann/json.hpp"

struct Movie {
	std::unordered_map<std::string, std::string> names;
	std::vector<std::string> countries;
	int release_year = 0;
	int running_time = 0;
	std::string director;
	std::string screenwriter;
	std::string producer;
	std::unordered_map<std::string, std::string> starring;
	std::string cinematography;
	std::string editor;
	std::string composer;
	std::vector<std::string> production_companies;
};


template<class UnaryFunction>
void recursive_iterate(const nlohmann::json& j, UnaryFunction f)
{
	for (auto it = j.begin(); it != j.end(); ++it)
	{
		if (it->is_structured()) {
			recursive_iterate(*it, f);
		}
		else {
			f(it);
		}
	}
}

void print(const std::vector<Movie>& movies) {
	for (const auto& movie : movies) {
		if (movie.names.count("English") < 1) {
			continue;
		}
		std::cout << "\"" << movie.names.at("English") << "\"" << std::endl;
		for (const auto& [key, name] : movie.names) {
			if (key == "English") {
				continue;
			}
			std::cout << "    " << key << ": \"" << name << "\"" << std::endl;
		}

		std::cout << "    Countries: ";
		int count = 0;
		for (const auto& name : movie.countries) {
			if (count++ > 0) {
				std::cout << ", ";
			}
			std::cout << name;
		}
		std::cout << std::endl;

		std::cout << "    Release year: " << movie.release_year << std::endl;
		std::cout << "    Running time: " << movie.running_time << " min." << std::endl;
		std::cout << "    Director: " << movie.director << std::endl;
		std::cout << "    Screenwriter: " << movie.screenwriter << std::endl;
		std::cout << "    Producer: " << movie.producer << std::endl;

		std::cout << "    Starring:" << std::endl;
		for (const auto& [key, name] : movie.starring) {
			std::cout << "              " << key << " as " << name << std::endl;
		}

		std::cout << "    Cinematography: " << movie.cinematography << std::endl;
		std::cout << "    Editor: " << movie.editor << std::endl;
		std::cout << "    Composer: " << movie.composer << std::endl;

		std::cout << "    Production companies:" << std::endl;
		count = 0;
		for (const auto& name : movie.production_companies) {
			if (count++ > 0) {
				std::cout << ", ";
			}
			std::cout << name;
		}
		std::cout << std::endl;
	}
}

int main() {
	std::vector<Movie> movies(8);

	std::cout << "Creating a movie catalog." << std::endl;

	movies[0].names["English"] = "Red sun";
	movies[0].names["French"] = "Soleil rouge";
	movies[0].names["Italian"] = "Sole rosso";
	movies[0].names["Spanish"] = "Sol rojo";

	movies[0].countries = { "France", "Italy", "Spain" };

	movies[0].release_year = 1971;
	movies[0].running_time = 112;
	movies[0].director = "Terence Young";
	movies[0].screenwriter = "Laird Koenig";
	movies[0].producer = "";

	movies[0].starring["Charles Bronson"] = "Link Stuart";
	movies[0].starring["Ursula Andress"] = "Cristina";
	movies[0].starring["Toshiro Mifune"] = "Kuroda Jubei";
	movies[0].starring["Alain Delon"] = "Gauche";
	movies[0].starring["Capucine"] = "Pepita";

	movies[0].cinematography = "Henri Alekan";
	movies[0].editor = "Johnny Dwyre";
	movies[0].composer = "Maurice Jarre";

	movies[0].production_companies = {
		"Les Films Corona", "Oceania Produzioni Internazionali",
		"Cinematografiche Producciones Balcazar S.A." };


	movies[1].names["English"] = "Butch Cassidy and the Sundance Kid";
	movies[1].countries = { "USA" };
	movies[1].release_year = 1969;
	movies[1].running_time = 110;
	movies[1].director = "George Roy Hill";
	movies[1].screenwriter = "William Goldman";
	movies[1].producer = "John Foreman";

	movies[1].starring["Paul Newman"] = "Butch Cassidy";
	movies[1].starring["Robert Redford"] = "Sundance Kid";
	movies[1].starring["Katharine Ross"] = "Etta Place";
	movies[1].starring["Strother Martin"] = "Percy Garris";
	movies[1].starring["Jeff Corey"] = "Sheriff Bledsoe";
	movies[1].starring["Henry Jones"] = "Bike Salesman";
	movies[1].starring["Ted Cassidy"] = "Harvey Logan";

	movies[1].cinematography = "Conrad Hall";
	movies[1].editor = "John C. Howard";
	movies[1].composer = "Burt Bacharach";

	movies[1].production_companies = { "20th Century-Fox" };


	movies[2].names["English"] = "How to Steal a Million";
	movies[2].countries = { "USA" };
	movies[2].release_year = 1966;
	movies[2].running_time = 123;
	movies[2].director = "William Wyler";
	movies[2].screenwriter = "Harry Kurnitz";
	movies[2].producer = "Fred Kohlmar";

	movies[2].starring["Audrey Hepburn"] = "Nicole Bonnet";
	movies[2].starring["Peter O'Toole"] = "Simon Dermott";
	movies[2].starring["Hugh Griffith"] = "Charles Bonnet";
	movies[2].starring["Eli Wallach"] = "Davis Leland";
	movies[2].starring["Charles Boyer"] = "DeSolnay";

	movies[2].cinematography = "Charles Lang";
	movies[2].editor = "Robert Swink";
	movies[2].composer = "John Williams";

	movies[2].production_companies = { "World Wide Productions" };


	movies[3].names["English"] = "Monsters, Inc.";
	movies[3].countries = { "USA" };
	movies[3].release_year = 2001;
	movies[3].running_time = 92;
	movies[3].director = "Pete Docter";
	movies[3].screenwriter = "Andrew Stanton";
	movies[3].producer = "Darla K. Anderson";

	movies[3].starring["John Goodman"] = "James P. 'Sulley' Sullivan";
	movies[3].starring["Billy Crystal"] = "Mike Wazowski";
	movies[3].starring["Mary Gibbs"] = "Boo";
	movies[3].starring["Steve Buscemi"] = "Randall Boggs";
	movies[3].starring["James Coburn"] = "Henry J. Waternoose";
	movies[3].starring["Jennifer Tilly"] = "Celia Mae";
	movies[3].starring["Bob Peterson"] = "Roz";
	movies[3].starring["John Ratzenberger"] = "Yeti";

	movies[3].cinematography = "";
	movies[3].editor = "Robert Grahamjones";
	movies[3].composer = "Randy Newman";

	movies[3].production_companies = { "Pixar Animation Studios", "Walt Disney Pictures" };


	movies[4].names["English"] = "No Country for Old Men";
	movies[4].countries = { "USA" };
	movies[4].release_year = 2007;
	movies[4].running_time = 122;
	movies[4].director = "Joel Coen & Ethan Coen";
	movies[4].screenwriter = "Joel Coen & Ethan Coen";
	movies[4].producer = "Scott Rudin";

	movies[4].starring["Tommy Lee Jones"] = "Ed Tom Bell";
	movies[4].starring["Javier Bardem"] = "Anton Chigurh";
	movies[4].starring["Josh Brolin"] = "Llewelyn Moss";
	movies[4].starring["Woody Harrelson"] = "Carson Wells";
	movies[4].starring["Kelly Macdonald"] = "Carla Jean Moss";

	movies[4].cinematography = "Roger Deakins";
	movies[4].editor = "Roderick Jaynes";
	movies[4].composer = "Carter Burwell";

	movies[4].production_companies = { "Miramax Films", "Paramount Pictures" };


	movies[5].names["English"] = "Million Dollar Baby";
	movies[5].countries = { "USA" };
	movies[5].release_year = 2004;
	movies[5].running_time = 132;
	movies[5].director = "Clint Eastwood";
	movies[5].screenwriter = "Paul Haggis";
	movies[5].producer = "Clint Eastwood";

	movies[5].starring["Clint Eastwood"] = "Frankie Dunn";
	movies[5].starring["Hilary Swank"] = "Mary Margaret 'Maggie' Fitzgerald";
	movies[5].starring["Morgan Freeman"] = "Eddie 'Scrap-Iron' Dupris";

	movies[5].cinematography = "Tom Stern";
	movies[5].editor = "Joel Cox";
	movies[5].composer = "Clint Eastwood";

	movies[5].production_companies = { "Warner Bros. Pictures", "Lakeshore International" };


	movies[6].names["English"] = "Seven";
	movies[6].countries = { "USA" };
	movies[6].release_year = 1995;
	movies[6].running_time = 127;
	movies[6].director = "David Fincher";
	movies[6].screenwriter = "Andrew Kevin Walker";
	movies[6].producer = "Arnold Kopelson";

	movies[6].starring["Brad Pitt"] = "David Mills";
	movies[6].starring["Morgan Freeman"] = "William Somerset";
	movies[6].starring["Gwyneth Paltrow"] = "Tracy Mills";
	movies[6].starring["Kevin Spacey"] = "John Doe";
	movies[6].starring["R. Lee Ermey"] = "police captain";

	movies[6].cinematography = "Darius Khondji";
	movies[6].editor = "Richard Francis-Bruce";
	movies[6].composer = "Howard Shore";

	movies[6].production_companies = { "Arnold Kopelson Productions", "New Line Cinema" };


	movies[7].names["English"] = "The Shawshank Redemption";
	movies[7].countries = { "USA" };
	movies[7].release_year = 1994;
	movies[7].running_time = 142;
	movies[7].director = "Frank Darabont";
	movies[7].screenwriter = "Frank Darabont";
	movies[7].producer = "Niki Marvin";

	movies[7].starring["Tim Robbins"] = "Andy Dufresne";
	movies[7].starring["Morgan Freeman"] = "Ellis Boyd 'Red' Redding";
	movies[7].starring["Bob Gunton"] = "Samuel Norton";
	movies[7].starring["William Sadler"] = "Heywood";
	movies[7].starring["Clancy Brown"] = "Byron Hadley";

	movies[7].cinematography = "Roger Deakins";
	movies[7].editor = "Richard Francis-Bruce";
	movies[7].composer = "Thomas Newman";

	movies[7].production_companies = { "Castle Rock Entertainment", "Columbia Pictures" };

	print(movies);

	std::cout << "Creating a JSON file of the movie catalog." << std::endl;

	nlohmann::json arr = nlohmann::json::array();
//	nlohmann::json catalogue;

	for (const auto& movie : movies) {
		if (movie.names.count("English") < 1) {
			continue;
		}
		nlohmann::json dict;
		for (const auto& [key, name] : movie.names) {
			if (key == "English") {
				continue;
			}
			dict["names"][key] = name;
		}
		dict["countries"] = movie.countries;
		dict["release_year"] = movie.release_year;
		dict["running_time"] = movie.running_time;
		dict["director"] = movie.director;
		dict["screenwriter"] = movie.screenwriter;
		dict["producer"] = movie.producer;
		dict["starring"] = movie.starring;
		dict["cinematography"] = movie.cinematography;
		dict["editor"] = movie.editor;
		dict["composer"] = movie.composer;
		dict["production_companies"] = movie.production_companies;

//		catalogue[movie.names.at("English")] = dict;
        arr.push_back(std::make_pair(movie.names.at("English"), dict));
	}



	recursive_iterate(arr, [](nlohmann::json::const_iterator it) {
		std::cout << *it << std::endl;
		});



	movies.clear();


//	return 0;

	std::string path = "movies.json";
	
	std::ofstream file_write(path);
	if (file_write.is_open()) {
		file_write << arr;
		std::cout << "The JSON file was successfully written to disk." << std::endl;
		file_write.close();
	}
	else {
		std::cout << "Something wrong! The JSON file wasn't written to disk." << std::endl;
		file_write.close();
		return 0;
	}
	
	std::cout << "\n\n\nReading the JSON file of the movie catalog." << std::endl;



	std::ifstream file_read(path);
	if (file_read.is_open()) {

		auto movie_catalogue = nlohmann::json();
		file_read >> movie_catalogue;

		for (const auto& item : movie_catalogue.items()) {
			Movie mov;
			if (item.key().size() < 1) {
				continue;
			}
			std::cout << item.key() << "!!!\n";
			std::cout << item.value() << "\n";

			for (const auto& obj : item.value()) {
				std::cout << obj << "\n";
			}


//			mov.names["English"] = item.value();
			if (item.value().count("names") > 0) {
				
				for (const auto& i : item.value().at("names")) {
					std::cout << "item.value().at(\"names\") = " << i << "\n";
				}
				mov.names = item.value().at("names");
			}
//			mov.names = item.value()["names"];
/*
			mov.countries = item.value()["countries"];
			mov.release_year = item.value()["release_year"];
			mov.running_time = item.value()["running_time"];
			mov.director = item.value()["director"];
			mov.screenwriter = item.value()["screenwriter"];
			mov.producer = item.value()["producer"];
			mov.starring = item.value()["starring"];
			mov.cinematography = item.value()["cinematography"];
			mov.editor = item.value()["editor"];
			mov.composer = item.value()["composer"];
			mov.production_companies = item.value()["production_companies"];
			
*/
			movies.push_back(mov);
		}
		file_read.close();


		std::cout << "\n\n\nPRINT\n";
		print(movies);
	}
	else {
		std::cout << "File " << path << " was not found." << std::endl;
	}

	return 0;
}
