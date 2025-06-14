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
    std::unordered_map<std::string, std::string> starring;
    std::string cinematography;
    std::string editor;
    std::string composer;
    std::vector<std::string> production_companies;
};

int main() {
    Movie mov;

    mov.names["English"] = "Red sun";
    mov.names["French"] = "Soleil rouge";
    mov.names["Italian"] = "Sole rosso";
    mov.names["Spanish"] = "Sol rojo";

    mov.countries = { "France", "Italy", "Spain" };

    mov.release_year = 1971;
    mov.running_time = 112;

    mov.director = "Terence Young";
    mov.screenwriter = "Laird Koenig";

    mov.starring["Charles Bronson"] = "Link Stuart";
    mov.starring["Ursula Andress"] = "Cristina";
    mov.starring["Toshiro Mifune"] = "Kuroda Jubei";
    mov.starring["Alain Delon"] = "Gauche";
    mov.starring["Capucine"] = "Pepita";

    mov.cinematography = "Henri Alekan";
    mov.editor = "Johnny Dwyre";
    mov.composer = "Maurice Jarre";

    mov.production_companies = { 
        "Les Films Corona", "Oceania Produzioni Internazionali",
        "Cinematografiche Producciones Balcazar S.A." };

    nlohmann::json dict;

    dict["names"] = mov.names;
    dict["countries"] = mov.countries;
    dict["release_year"] = mov.release_year;
    dict["running_time"] = mov.running_time;
    dict["director"] = mov.director;
    dict["screenwriter"] = mov.screenwriter;
    dict["starring"] = mov.starring;
    dict["cinematography"] = mov.cinematography;
    dict["editor"] = mov.editor;
    dict["composer"] = mov.composer;
    dict["production_companies"] = mov.production_companies;

    std::ofstream file("movie.json");
    if (file.is_open()) {
        file << dict;
        file.close();
    }

    return 0;
}
