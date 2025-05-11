#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <random>
#include <vector>
#include <map>

using namespace std;

struct Client {
    string ticket;
    int duration;
};

// Вывод талонов в одну строку
ostream& operator<<(ostream& os, vector<Client> v) {
    for (auto elem : v) {
        cout << elem.ticket << " ";
    }
    return os;
}

string Input() {
    return "<<< ";
}

string Output() {
    return ">>> ";
}

string TotalDuration(vector<Client> clients) {
    int summ {};
    for (const auto& client : clients) {
        summ += client.duration;
    }
    string res = " (" + to_string(summ) + " мин.): ";
    return res;
}

// Генерация талона
string GenerateTicket() {
    random_device rd;
    minstd_rand gen(rd());
    uniform_int_distribution<> lim(1, 1000);

    ostringstream oss;
    oss << "T" << setw(3) << setfill('0') << lim(gen);
    return oss.str();
}

// Запись в общую очередь по талону
void Enqueue(vector<Client>& queue, Client& client) {
    client.ticket = GenerateTicket();
    queue.push_back(client);

    cout << Output() << client.ticket << endl;
}

// Распределение посетителей по окнам
void Distribute(map<int, vector<Client>>& windows, vector<Client> clients, int total) {
    // Сортировка посетителей в порядке убывания пребывания
    vector<Client> sort_clients = clients;
    sort(sort_clients.begin(), sort_clients.end(), [](const Client& a, const Client& b) {
        return a.duration > b.duration;
    });

    // Вектор для хранения текущей суммы времени для каждого окна
    vector<int> window_times(total, 0);
    // Временная структура для хранения распределения
    map<int, vector<Client>> temp_windows;
    for (int i = 1; i <= total; ++i) {
        temp_windows[i] = {};
    }

    // Распределяем клиентов
    for (const auto& client : sort_clients) {
        // Находим окно с минимальным текущим временем
        int min_window = 1;
        for (int i = 2; i <= total; ++i) {
            if (window_times[i - 1] < window_times[min_window - 1]) {
                min_window = i;
            }
        }
        // Добавляем клиента в это окно
        temp_windows[min_window].push_back(client);
        window_times[min_window - 1] += client.duration;
    }

    // Переносим распределение в основную структуру
    windows = temp_windows;
    
    for (const auto& elem : windows) {
        cout << Output() << "Окно " << elem.first << TotalDuration(elem.second) << elem.second << endl;
    }
}

int main() {
    map<int, vector<Client>> queue;       // Окна регистратуры
    vector<Client> clients;               // Общая очередь из посетителей
    int total_windows {};

    cout << Output() << "Введите количество окон" << endl;
    cout << Input();
    cin >> total_windows;
    if (total_windows <= 0) {
        cout << "Некорретное количество окон. Попробуйте снова" << endl;
        abort();
    }
    for (int i = 1; i <= total_windows; ++i) {
        queue[i] = {};
    }

    string operation;
    while (true) {
        cout << Input();
        cin >> operation;
        if (operation == "ENQUEUE") {
            Client client;
            cin >> client.duration;
            Enqueue(clients, client);
        }
        else if (operation == "DISTRIBUTE") {
            Distribute(queue, clients, total_windows);
            break;
        }
        else {
            cout << Output() << "Неизвестная команда" << endl;
        }
    }

    return 0;
}
