#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <tuple>
#include <map>

using namespace std;
const int MAX_CELL_CAPACITY = 10;


// Структура для товара
struct CellUnit {
    string good;
    int amount;
};

ostream& operator<<(ostream& os, vector<CellUnit> v) {
    for (auto elem : v) {
        cout << elem.good << ", " << elem.amount << "; ";
    }
    return os;
}

// Вывод инструкции
void GetInstructions() {
    cout << "==================================" << endl;
    cout << "1. ADD <товар> <кол-во> <ячейка> - добавить товар;" << endl;
    cout << "2. REMOVE <товар> <кол-во> <ячейка> - удалить товар" << endl;
    cout << "3. INFO - получить информацию о складе" << endl;
    cout << "==================================" << endl;
    cout << "Ввод: ";

}

// Количество товаров в ячейке
int CellSpace(const vector<CellUnit> cell) {
    int summ {};
    for (auto elem : cell) {
        summ += elem.amount;
    }
    return summ;
}

// Проверка на наличие ячейки на складе
bool CheckCell(string cell, const map<string, vector<CellUnit>> db) {
    if (auto search = db.find(cell); search != db.end()) {
        return true;
    }
    return false;
}

// Удаление товара из ячейки
bool RemoveFromCell(map<string, vector<CellUnit>>& db, const string& good, int amount, const string& cell) {
    int total {};
    for (const auto& elem : db[cell]) {
        if (elem.good == good)
            total += elem.amount;
    }
    if (total < amount) return false;

    vector<CellUnit> newCell;
    int remains = amount;
    for (const auto& elem : db[cell]) {
        if (elem.good == good) {
            if (elem.amount > remains) {
                newCell.push_back({elem.good, elem.amount - remains});
                remains = 0;
            }
            else {
                remains -= elem.amount;
            }
        }
        else
            newCell.push_back(elem);
    }
    db[cell] = newCell;
    return true;
}

// Загруженность склада в процентах
tuple<double, double, double> WarehouseWorkload(const map<string, vector<CellUnit>>& db) {
    double areaTotal {}, areaA {}, areaB {};
    for (const auto& elem : db) {
        areaTotal += CellSpace(elem.second);
        if (elem.first.starts_with("А")) areaA += CellSpace(elem.second);
        else if (elem.first.starts_with("Б")) areaB += CellSpace(elem.second);
    }

    return make_tuple(areaTotal / 7200 * 100, areaA / 3600 * 100, areaB / 3600 * 100);
}


int main() {
    map<string, vector<CellUnit>> db;
    vector<string> zones {"А", "Б"};

    // Создание ячеек хранения
    for (int i = 0; i < 2; ++i) {
        for (int stell = 1; stell <= 4; ++stell) {
            for (int vert = 1; vert <= 18; ++vert) {
                for (int shelf = 1; shelf <= 5; ++shelf) {
                    string cell = zones[i] + to_string(stell) + to_string(vert) + to_string(shelf);
                    db[cell] = {};
                }
            }
        }
    }

    // Основной функционал
    string operation, cell;
    CellUnit unit;

    while (operation != "EXIT") {
        GetInstructions();
        cin >> operation;
        if (operation == "ADD") {
            cin >> unit.good >> unit.amount >> cell;
            if (!CheckCell(cell, db)) {
                cout << "Ячейки не существует. Повторите ввод снова" << endl;
                continue;
            }
            if (CellSpace(db[cell]) + unit.amount <= MAX_CELL_CAPACITY) {
                db[cell].push_back(unit);
                cout << "Успешно!" << endl;
            }
            else {
                cout << "Невозможно выполнить операцию: количество товаров в ячейке превышает допустимый максимум" << endl;
                continue;
            }
        }

        if (operation == "REMOVE") {
            cin >> unit.good >> unit.amount >> cell;
            if (!CheckCell(cell, db)) {
                cout << "Ячейки не существует. Повторите ввод снова" << endl;
                continue;
            }
            if (RemoveFromCell(db, unit.good, unit.amount, cell)) {
                cout << "Удаление завершено!" << endl;
            }
            else {
                cout << "Невозможно выполнить операцию: введенное число для удаления превышает количество товара в ячейке" << endl;
                continue;
            }
        }

        if (operation == "INFO") {
            auto wl = WarehouseWorkload(db);
            cout << "Общая загруженность склада: " << setprecision(2) << get<0>(wl) << "%" << endl;
            cout << "Загруженность зоны А: " << setprecision(2) << get<1>(wl) << "%" << endl;
            cout << "Загруженность зоны Б: " << setprecision(2) << get<2>(wl) << "%" << endl;
            for (const auto& item : db) {
                if (CellSpace(item.second) != 0)
                    cout << item.first << ": " << item.second << endl;
            }
            cout << "Пустые ячейки: ";
            for (const auto& item : db) {
                if (CellSpace(item.second) == 0)
                    cout << item.first << " ";
            }
            cout << endl;
        }

        else {
            cout << "Неизвестная операция. Повторите ввод снова" << endl;
        }
    }

    return 0;
}

