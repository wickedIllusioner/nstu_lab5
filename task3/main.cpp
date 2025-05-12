#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include "header.h"

using namespace std;

// Вид запросов
enum class QueryType {
    CREATE_TRAIN,
    TRAINS_FOR_TOWN,
    TOWNS_FOR_TRAIN,
    TRAINS
};

// Структура для запросов
struct Query {
    QueryType type;
    string train;
    string town;
    vector<string> towns;
};

// Обработка ввода
istream& operator>>(istream& is, Query& q) {
    string operation;
    is >> operation;

    if (operation == "CREATE_TRAIN") {
        q.type = QueryType::CREATE_TRAIN;
        is >> q.train;
        int town_count;
        is >> town_count;
        q.towns.resize(town_count);

        for (string& stop: q.towns) {
            is >> stop;
        }
    }

    else if (operation == "TRAINS_FOR_TOWN") {
        q.type = QueryType::TRAINS_FOR_TOWN;
        is >> q.town;
    }

    else if (operation == "TOWNS_FOR_TRAIN") {
        q.type = QueryType::TOWNS_FOR_TRAIN;
        is >> q.train;
    }

    else if (operation == "TRAINS") {
        q.type = QueryType::TRAINS;
    }

    else if (operation == "EXIT") {
        abort();
    }

    else {
        cout << "Неизвестная команда" << endl;
    }

    return is;
}

int main() {
    Query q;
    map<string, vector<string>> trains_to_towns, towns_to_trains;

    while (true) {
        cin >> q;
        switch (q.type) {
            case QueryType::CREATE_TRAIN:
                CreateTrain(q.train, q.towns, trains_to_towns, towns_to_trains);
                break;
            case QueryType::TRAINS_FOR_TOWN:
                GetTrainsForTown(q.town, towns_to_trains);
                break;
            case QueryType::TOWNS_FOR_TRAIN:
                GetTownsForTrain(q.train, trains_to_towns, towns_to_trains);
                break;
            case QueryType::TRAINS:
                ShowTrains(trains_to_towns);
                break;
        }
    }
    return 0;
}
