#pragma once
using namespace std;

void CreateTrain(const string train, const vector<string>& towns, map<string, vector<string>>& train_town, map<string, vector<string>>& town_train);
void GetTrainsForTown(const string& town, map<string, vector<string>>& town_train);
void GetTownsForTrain(const string& train, map<string, vector<string>>& train_town, map<string, vector<string>>& town_train);
void ShowTrains(const map<string, vector<string>>& train_town);
