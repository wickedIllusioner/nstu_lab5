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

// Проверка на дупликаты
bool hasDuplicates(const vector<string>& towns) {
    for (size_t i = 0; i < towns.size(); ++i) {
        for (size_t j = i + 1; j < towns.size(); ++j) {
            if (towns[i] == towns[j]) {
                return true;
            }
        }
    }
    return false;
}



// Создание нового маршрута
void CreateTrain(const string train, const vector<string>& towns, map<string, vector<string>>& train_town, map<string, vector<string>>& town_train) {
    // Проверка на существование маршрута
    if (train_town.find(train) != train_town.end()) {
        cout << "Поезд " << train << " уже существует" << endl;
	return ;
    }

    // Проверка на количество остановок
    if (towns.size() == 1) {
        cout << "Маршрут должен содержать более одной остановки" << endl;
	return ;
    }

    // Проверка на дубликаты
    if (hasDuplicates(towns)) {
        cout << "Маршрут содержит повторяющиеся остановки" << endl;
	return ; 
    }

    train_town[train] = towns;
    for (const string& town : towns) {
        town_train[town].push_back(train);
    }
    if (train_town[train].size() == 1) {
        cout << "Остановка не может быть одна" << endl;
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
void GetTownsForTrain(const string& train, map<string, vector<string>>& train_town, map<string, vector<string>>& town_train) {
    if (train_town.find(train) == train_town.end()) {
        cout << "Такого поезда не существует" << endl;
        return ;
    }

    cout << "Train " << train << ": ";
    for (auto town : train_town[train]) {
        cout << town;
	vector<string> other_trains;
	for (const string& t : town_train[town]) {
	    if (t != train)
	        other_trains.push_back(t);
	}
	
	if (!other_trains.empty()) {
	    cout << "( ";
	    for (auto elem : other_trains) 
                cout << elem << " ";
	    cout << ")";
	}
	cout << " ";
    }

    cout << endl;


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
