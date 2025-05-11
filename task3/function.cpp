#include <iostream>
#include <sstream>
#include <vector>
#include <map>
using namespace std;

ostream& operator<<(ostream& os, vector<string> v) {
    for (auto elem : v) {
        cout << elem << " ";
    }
    return os;
}

// Создание нового маршрута
void CreateTrain(const string train, const vector<string>& towns, map<string, vector<string>>& train_town, map<string, vector<string>>& town_train) {
    train_town[train] = towns;
    for (const string& town : towns) {
        town_train[town].push_back(train);
    }
}

// Вывод проходящих поездов мимо населенного пункта
void GetTrainsForTown(const string& town, map<string, vector<string>>& town_train) {
    if (town_train.find(town) == town_train.end()) {
        cout << "В указанный населенный пункт поезда отсутствуют" << endl;
        return ;
    }

    cout << "Town " <<  town << ": " << town_train[town] << endl;
}

// Вывод населенных пунктов, через которые проходит поезд
void GetTownsForTrain(const string& train, map<string, vector<string>>& train_town) {
    if (train_town.find(train) == train_town.end()) {
        cout << "Такого поезда не существует" << endl;
        return ;
    }

    cout << "Train " << train << ": " << train_town[train] << endl;
}

// Вывод всех маршрутов
void ShowTrains(const map<string, vector<string>>& train_town) {
    if (train_town.empty()) {
        cout << "Нет данных" << endl;
        return ;
    }
    for (const auto& train : train_town) {
        cout << train.first << ": " << train.second << endl;
    }
}
