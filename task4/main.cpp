#include <iostream>
#include <sstream>
#include <set>
#include <map>

using namespace std;

// Добавление в список друзей
void AddFriend(map<string, set<string>>& list, string person1, string person2) {
    if (person1 == person2) {
        cout << "Одинаковые имена!" << endl;
	return ;
    }
    list[person1].insert(person2);
    list[person2].insert(person1);
}

// Подсчет друзей у человека
int CountFriends(map<string, set<string>>& list, string person) {
    return list[person].size();
}

// Проверка на дружбу первого со вторым
bool FriendCheck(map<string, set<string>>& list, string person1, string person2) {
    if (person1 == person2) {
        cout << "Одинаковые имена!" << endl;
	return false;
    }
    if (list[person1].contains(person2) && list[person2].contains(person1)) {
        return true;
    }
    return false;
}

int main() {
    int query_count;
    string query;
    map<string, set<string>> friend_list;
    cout << "Введите количество запросов: ";
    cin >> query_count;

    for (int i = 0; i <= query_count; ++i) {
        getline(cin, query);
        istringstream cmd(query);
        string operation;
        cmd >> operation;

        if (operation == "FRIENDS") {
            string person1, person2;
            cmd >> person1 >> person2;
            AddFriend(friend_list, person1, person2);
        }
        else if (operation == "COUNT") {
            string person;
            cmd >> person;
            int total_friends = CountFriends(friend_list, person);
            cout << total_friends << endl;
        }

        else if (operation == "QUESTION") {
            string person1, person2;
            cmd >> person1 >> person2;
            if (FriendCheck(friend_list, person1, person2)) {
                cout << "YES" << endl;
            }
            else {
                cout << "NO" << endl;
            }
        }
    }

    return 0;
}
