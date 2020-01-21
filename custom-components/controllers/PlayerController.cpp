//
// Created by Олег Бобров on 10.12.2019.
//

#include "PlayerController.hpp"
#include "../../static/Database.h"
#include "../../network/Network.hpp"
#include "../../static/Time.h"
#include "../../utility/random.hpp"
#include "../Game.hpp"

#include <queue>
#include <bitset>
#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <set>

#include <ctime>
#include <fstream>

using Random = effolkronium::random_static;

std::unordered_map<int, std::set<int> > timeTableVertex;
std::unordered_map<int, std::set<int> > timeTableLine;

std::unordered_map<int, std::set<int> > productFict;
std::unordered_map<int, std::set<int> > armorFict;

std::unordered_map<int, long long> fLine[50];
std::unordered_map<int, long long> fVertex[50];
std::vector<std::pair<int, std::pair<int, int> > > trainTimeTable[50];

int countType1 = 0;

void PlayerController::start() {
    Network::onMapResponse1.addListener<PlayerController, &PlayerController::onMapLayer1>(this);
    Network::onGamesResponse.addListener<PlayerController, &PlayerController::onGames>(this);
}

void PlayerController::onDestroy() {
    Network::onMapResponse1.removeListener<PlayerController, &PlayerController::onMapLayer1>(this);
    Network::onGamesResponse.removeListener<PlayerController, &PlayerController::onGames>(this);
}

void PlayerController::onMapLayer1(const nlohmann::json &json) {
    if (Game::current->trySetLayer1(json)) {
        Game::current->applyLayer1(json);
    } else {
        Network::send(Action::MAP, {{"layer", 1}});
    }
}

void PlayerController::onGames(const nlohmann::json &json) {
    if (isGameFinished(json)) {
        Game::current->setState(GameState::FINISHED);
    }
}

bool PlayerController::isGameFinished(const nlohmann::json &json) {
    if (json.contains("games")) {
        for (auto game : json["games"]) {
            if (game["name"] == PlayerConfig::login["game"] && game["state"] == GameState::FINISHED) {
                return true;
            }
        }
    }
    return false;
}

//TODO: make friend or comparator function
bool operator< (El left, El right) {
    if (left.value < right.value || (left.value == right.value && left.spent > right.spent)) return true;
    return false;
}


std::vector<int> operator+= (std::vector<int> &left, std::vector<int> &right) {
    for (int i = 0; i < right.size(); i++) left.push_back(right[i]);
    return left;
}


//TODO: point has post field
bool isStorage(Point* point) {

    if (point -> post == nullptr) return false;
    if (point -> post -> type == PostType::STORAGE) return true;
    return false;
}
//TODO: point has post field


bool isMarket(Point* point) {
    if (point -> post == nullptr) return false;
    if (point -> post -> type == PostType::MARKET) return true;
    return false;
}


bool PlayerController::isTown(int vertex) {

    if (vertex == Database::playerTown -> point -> idx) return true;
    return false;
}

//TODO: point has post field or I can store all markets
int marketProduct(Point* point, int cost) {

    auto *marketObj = static_cast<Market* >(point -> post);

    int pointIdx = marketObj -> point -> idx;

    int timeNow = cost + Database::tick;
    int timeLeft = 0;

    if (productFict[pointIdx].size()) {
        timeLeft = *(--(productFict[pointIdx].end()));
    }

    int sum = (timeNow - timeLeft) * (marketObj -> replenishment);
    sum = std::min(sum, marketObj -> product_capacity);

    return sum;

    //return marketObj->productFict;
}


int storageArmor(Point* point, int cost) {

    auto *storageObj = static_cast<Storage* >(point -> post);

    int pointIdx = storageObj -> point -> idx;

    int timeNow = cost + Database::tick;
    int timeLeft = 0;

    if (productFict[pointIdx].size()) {
        timeLeft = *(--(armorFict[pointIdx].end()));
    }

    int sum = (timeNow - timeLeft) * (storageObj -> replenishment);
    sum = std::min(sum, storageObj -> armor_capacity);

    return sum;


    //return storageObj->armorFict;
}


std::vector<int> showWay(std::vector<int> &finalWay) {
    return finalWay;
}


int getTrainVertex(Train* train) {
    if (train->position == 0) return train->line->points[0]->idx;
    else return train->line->points[1]->idx;
}


Point* getTrainPoint(Train* train) {
    if (train->position == 0) return train->line->points[0];
    else return train->line->points[1];
}


void clearLinesAndVertex(int marketCount) {

    for (int i = 0; i < marketCount; i++) {
        fVertex[i].clear();
        fLine[i].clear();
    }
}


void PlayerController::trainUpgrade() {

    for (auto trainNow : Database::playerTrains) {
        if (trainNow -> level == 1) tryTrainUpdate(trainNow);
    }

    for (auto trainNow : Database::playerTrains) {
        if (trainNow -> level == 2) tryTrainUpdate(trainNow);
    }

    for (auto trainNow : Database::playerTrains) {
        if (trainNow -> level == 3) tryTrainUpdate(trainNow);
    }
    return;
}


void checkOnBestWay(El &now, std::vector<int> &bestWay, int &bestCost) {

    if (now.value - now.spent > bestCost && now.way.size() > 2) {

        bestCost = now.value - now.spent;
        bestWay = now.way;

        std::cout << "Best cost=" << bestCost << " value=" << now.value << std::endl;
    }

    return;
}


void PlayerController::addTimeTable(std::vector<int> needWay, int idx) {

    int timeNow = 0;
    // if (Database::tick > 10) timeNow = 1;

    for (int i = 0; i < needWay.size(); i++) {

        if (i == 0) {

            if (isTown(needWay[i]) == false) {
                timeTableVertex[needWay[i]].insert(timeNow + Database::tick);
                trainTimeTable[idx].push_back({1, {needWay[i], timeNow + Database::tick}});
            }

        }
        else {

            for (auto line : Database::lines) {
                if (line.second -> points[0] -> idx == needWay[i - 1]
                 && line.second -> points[1] -> idx == needWay[i]) {

                            if (line.second -> idx == 612) {
                                // std::cout << "START 612!; time=" << timeNow + Database::tick << std::endl;
                            }

                            int len = line.second -> length;
                            for (int j = 1; j < len; j++) {

                                timeNow++;

                                timeTableLine[line.second -> idx * 1000 + j].insert(timeNow + Database::tick);
                                trainTimeTable[idx].push_back({2, {line.second -> idx * 1000 + j, timeNow + Database::tick}});

                                if (line.second -> idx == 612) {
                                    // std::cout << "INSERT 612! pos=" << j << " time=" << timeNow + Database::tick << std::endl;
                                }

                            }
                            timeNow++;

                            if (isTown(needWay[i]) == false){
                                timeTableVertex[needWay[i]].insert(timeNow + Database::tick);
                                trainTimeTable[idx].push_back({1, {needWay[i], timeNow + Database::tick}});
                            }

                            break;
                        }
                else if (line.second -> points[1] -> idx == needWay[i - 1]
                      && line.second -> points[0] -> idx == needWay[i]) {

                          if (line.second -> idx == 612) {
                                // std::cout << "START 612!; time=" << timeNow + Database::tick << std::endl;
                            }

                            int len = line.second -> length;
                            for (int j = len - 1; j >= 1; j--) {

                                timeNow++;

                                if (line.second -> idx == 612) {
                                    // std::cout << "INSERT 612! pos=" << j << " time=" << timeNow + Database::tick << std::endl;
                                }

                                timeTableLine[line.second -> idx * 1000 + j].insert(timeNow + Database::tick);
                                trainTimeTable[idx].push_back({2, {line.second -> idx * 1000 + j, timeNow + Database::tick}});


                            }
                            timeNow++;

                            if (isTown(needWay[i]) == false){
                                timeTableVertex[needWay[i]].insert(timeNow + Database::tick);
                                trainTimeTable[idx].push_back({1, {needWay[i], timeNow + Database::tick}});

                            }

                            break;
                        }
            }
        }
    }
}


bool PlayerController::isFreeTimeTable(El now) {

    std::set<int> ::iterator t;

    if (now.line == nullptr) {

        t = timeTableVertex[now.vertex].lower_bound(now.spent + Database::tick);
        if (t != timeTableVertex[now.vertex].end() && *t == now.spent + Database::tick) return false;

        t = timeTableVertex[now.vertex].lower_bound(now.spent + Database::tick + 1);
        if (t != timeTableVertex[now.vertex].end() && *t == now.spent + Database::tick + 1) return false;
    }
    else {

        t = timeTableLine[now.line -> idx * 1000 + now.position].lower_bound(now.spent + Database::tick);
        if (t != timeTableLine[now.line -> idx * 1000 + now.position].end() && *t == now.spent + Database::tick) return false;

        t = timeTableLine[now.line -> idx * 1000 + now.position].lower_bound(now.spent + Database::tick + 1);
        if (t != timeTableLine[now.line -> idx * 1000 + now.position].end() && *t == now.spent + Database::tick + 1) return false;
    }

    return true;
}

void logTrain(Train* train) {


    if (train -> position == 0) std::cout << "Train idx=" << train -> idx << " lvl=" << train -> level << " point=" << train -> line -> points[0] -> idx << " position=" << train -> position;
    else if (train -> position == train -> line -> length) std::cout << "Train idx=" << train -> idx << " lvl=" << train -> level << " point=" << train -> line -> points[1] -> idx << " position=" << train -> position;
    else std::cout << "Train idx=" << train -> idx << " lvl=" << train -> level << " line=" << train -> line -> idx << " position=" << train -> position;
    std::cout << " size=" << train -> needWay.size();
    std::cout << " tick=" << Database::tick;
    std::cout << std::endl;


}


void logTime(unsigned int time) {

    std::ofstream out;          // поток для записи
    out.open("test.txt", std::ios::out | std::ios::app); // окрываем файл для записи

    out << "Lost time= " << time << " mc" << std::endl;
    out.close();
}


void addFictProduct(std::vector<int> &way, int capacity){

    // int now = 0;
    int len = 0;

    for (int i = way.size() - 1; i >= 0; i--) {

        for (auto post : Database::posts) if (post.second -> type == PostType::MARKET && way[i] == post.second -> point -> idx) {

            // auto *marketObj = static_cast<Market* >(post.second);
            // int add = capacity - now;
            // marketObj -> productFict = std::max(0, marketObj -> productFict - std::min(marketObj -> product, add));
            //now += std::min(add, marketObj -> product);

            productFict[way[i]].insert(len + Database::tick);
        }

        if (i >= 1) {
            for (auto line : Database::lines) {
                if (line.second -> points[0] -> idx == way[i] && line.second -> points[1] -> idx == way[i - 1]) len += line.second -> length;
                if (line.second -> points[1] -> idx == way[i] && line.second -> points[0] -> idx == way[i - 1]) len += line.second -> length;
            }
        }
    }
}


void addFictArmor(std::vector<int> &way, int capacity){

    // int now = 0;
    int len = 0;

    for (int i = way.size() - 1; i >= 0; i--) {

        for (auto post : Database::posts) if (post.second -> type == PostType::STORAGE && way[i] == post.second -> point -> idx) {

            // auto *storageObj = static_cast<Storage* >(post.second);
            // int add = capacity - now;
            // storageObj -> armorFict = std::max(0, storageObj -> armorFict - std::min(storageObj -> armor, add));
            // now += std::min(add, storageObj -> armor);

            armorFict[way[i]].insert(len + Database::tick);
        }

        if (i >= 1) {
            for (auto line : Database::lines) {
                if (line.second -> points[0] -> idx == way[i] && line.second -> points[1] -> idx == way[i - 1]) len += line.second -> length;
                if (line.second -> points[1] -> idx == way[i] && line.second -> points[0] -> idx == way[i - 1]) len += line.second -> length;
            }
        }
    }
}

void tryTrainUpdate(Train*);


void clearTimeTable(int xx) {

    while(trainTimeTable[xx].size()) {

        std::pair<int, std::pair<int, int> > a = trainTimeTable[xx].back();
        trainTimeTable[xx].pop_back();

        if (a.first == 1) timeTableVertex[a.second.first].erase(a.second.second);
        else timeTableLine[a.second.first].erase(a.second.second);

    }
}


void PlayerController::update() {
    int _count = 0;
    for (auto train : Database::playerTrains) {
        if (_count < countType1) train -> move_type = 1;
        else train -> move_type = 2;

        _count += 1;
    }

    if (Database::isMapUpdated) {
        //TODO: most of gameLogic
        strategyIteration();
        std::cout << "TICK=" << Database::tick << std::endl;
    }

}

void PlayerController::strategyIteration() {

    unsigned int start_time =  clock(); // начальное время

    std::cout << "=====================================" << std::endl;
    std::cout << "Product=" << Database::playerTown->product <<  " Armor=" << Database::playerTown->armor  <<  " Population=" << Database::playerTown->population << " level=" << Database::playerTown->level << std::endl;


    //TODO: iterate over Database::playerTrains
    for (auto train_now : Database::playerTrains) {

        tryTrainUpdate(train_now);
        if (train_now -> cooldown != 0) {

            std::cout << "Train idx=" << train_now -> idx << " lvl=" << train_now -> level << " IN COOL DOWN! cooldown=" << train_now -> cooldown << std::endl;
            train_now -> needWay.clear();
            clearTimeTable(train_now -> idx);

            continue;
        }

        logTrain(train_now);

        if (train_now->move_type == 0) {
            train_now->move_type = 1;
            //TODO: move_type of trains + destiny;
        }
        int train_pos = train_now->position;
        if (train_pos == 0 || train_pos == train_now->line->length) {

            if (train_now->needWay.size() == 0) {

                if (train_now -> move_type == 1) train_now -> needWay = PlayerController::trainWayToProducts(train_now).first;
                else if (train_now -> move_type == 2) {

                    if (train_now->level <= 2 || Database::playerTown->level <= 2 || Database::playerTown -> armor <= 50) train_now->needWay = PlayerController::trainWayToStorage(train_now).first;
                    else train_now->needWay = PlayerController::trainWayToProducts(train_now).first;
                }
            }
        }
    }

    /// first for trains, which go to Town
    for (auto train_now : Database::playerTrains) {
        if (train_now -> needWay.size() != 0 && train_now -> needWay.back() == Database::playerTown -> point -> idx) trainIteration(train_now);
    }
    /// then for other
    for (auto train_now : Database::playerTrains) {
        if (train_now -> needWay.size() != 0 && train_now -> needWay.back() != Database::playerTown -> point -> idx) trainIteration(train_now);
    }

    Database::isMapUpdated = false;
    Network::send(Action::TURN);
    Network::send(Action::MAP, {{"layer", 1}});
    Network::send(Action::GAMES);

    /// WARNING!!
    //Database::tick += 1;

    unsigned int end_time = clock(); // конечное время
    unsigned int search_time = end_time - start_time; // искомое время
    logTime(search_time);
}


bool NextSet(std::vector<int> &a, int n, int m)
{
    int k = m;
    for (int i = k - 1; i >= 0; --i)
        if (a[i] < n - k + i + 1)
        {
            ++a[i];
            for (int j = i + 1; j < k; ++j)
                a[j] = a[j - 1] + 1;
            return true;
        }
    return false;
}


std::pair<std::vector<int>, int> PlayerController::trainWayToProducts(Train* train) {

    std::vector<int> finalWay;
    int bestCost = -1e9;

    std::priority_queue<El> q;
    El start;

    int fixNumber = 1e9;
    int marketCount = Database::markets.size();

    int summary1 = 0, summary2 = 0;

    std::vector<int> needMarkets;
    for (int i = 0; i < Database::markets.size(); i++) {
        needMarkets.push_back(Database::markets[i] -> point -> idx);
    }
    sort(needMarkets.begin(), needMarkets.end());

    int n = needMarkets.size();
    int m = 0;

    if (Random::get(0, 2) == 0) m = 1;
    else m = 2;


    std::vector<int> a;
    for (int i = 0; i < n; i++) a.push_back(i + 1);

    std::vector<int> b;
    for (int i = 0; i < m; i++) b.push_back(i);

    do{

        do {

            clearLinesAndVertex(marketCount);

            start.vertex = getTrainVertex(train);
            start.point = getTrainPoint(train);

            start.marketPos = 0;
            start.maxValue = train -> goods_capacity;

            q.push(start);

            int _count = 0;
            int kol2 = 0;
            int kol3 = 0;

            while(!q.empty()) {

                El to, now;

                now = q.top();
                q.pop();


                if (!isFreeTimeTable(now)) continue;
                if (isStorage(now.point) && now.value == 0) continue;

                if (fVertex[now.marketPos][now.vertex] >= now.value - now.spent + fixNumber) continue;
                fVertex[now.marketPos][now.vertex] = now.value - now.spent + fixNumber;

                kol2 += 1;
                _count += 1;

                now.way.push_back(now.vertex);

                if (isTown(now.vertex)) checkOnBestWay(now, finalWay, bestCost);

                if (now.marketPos != m && now.vertex == needMarkets[a[b[now.marketPos]] - 1]) {
                    now.marketPos += 1;
                    now.value += marketProduct(now.point, now.spent);
                    now.value = std::min(now.value, now.maxValue);
                }

                // std::cout << "Try do something; idx=" << now.vertex << " is Town=" << isTown(now.vertex) << " marketPos=" << now.marketPos << " product=" << now.value << " spent=" << now.spent << std::endl;
                for (auto line : now.point -> adjacent) {

                    if (line.second -> points[0] -> idx == now.vertex) {

                        to = now;
                        to.line = line.second;
                        for (int j = 1; j < line.second -> length; j++) {

                            to.spent += 1;
                            to.position = j;

                            if (!isFreeTimeTable(to)) goto point1;
                        }
                        to.spent += 1;
                        to.line = nullptr;

                        to.vertex = line.second -> points[1] -> idx;
                        to.point = line.second -> points[1];

                        q.push(to);
                    }
                    else if (line.second -> points[1] -> idx == now.vertex) {

                        to = now;
                        to.line = line.second;
                        for (int j = line.second -> length - 1; j >= 1; j--) {

                            to.spent += 1;
                            to.position = j;

                            if (!isFreeTimeTable(to)) goto point1;
                        }
                        to.spent += 1;
                        to.line = nullptr;

                        to.vertex = line.second -> points[0] -> idx;
                        to.point = line.second -> points[0];

                        q.push(to);
                    }

                    point1:;
                }
            }
      } while(next_permutation(b.begin(), b.end()));

    } while (NextSet(a, n, m));


    addTimeTable(finalWay, train -> idx);

    reverse(finalWay.begin(), finalWay.end());
    if (finalWay.size()) finalWay.pop_back();

    addFictProduct(finalWay, train -> goods_capacity);

    return {finalWay, summary1 + summary2};
}



std::pair<std::vector<int>, int> PlayerController::trainWayToStorage(Train* train) {

    std::vector<int> finalWay;
    int bestCost = -1e9;

    std::priority_queue<El> q;
    El start;

    int fixNumber = 1e9;
    int storagesCount = Database::storages.size();

    int summary1 = 0, summary2 = 0;

    std::vector<int> needStorages;
    for (int i = 0; i < Database::storages.size(); i++) {
        needStorages.push_back(Database::storages[i] -> point -> idx);
    }
    sort(needStorages.begin(), needStorages.end());

    int n = needStorages.size();
    int m = train -> level;
    if (m == 3) m -= 1;

    if (Random::get(0, 1) == 0) m -= 1;


    std::vector<int> a;
    for (int i = 0; i < n; i++) a.push_back(i + 1);

    std::vector<int> b;
    for (int i = 0; i < m; i++) b.push_back(i);

    /*
    std::cout << "n=" << n << " m=" << m << std::endl;
    for (auto i : a) std::cout << i << " ";
    std::cout << std::endl;
    for (auto i : b) std::cout << i << " ";
    std::cout << std::endl;
    */


    do{

        do {

            /*
            std::cout << "go" << std::endl;
            for (int i = 0; i < m; i++) std::cout << a[b[i]] - 1 << " ";
            std::cout << std::endl;

            for (int i = 0; i < m; i++) std::cout << needStorages[a[b[i]] - 1] << " ";
            std::cout << std::endl;
            */

            clearLinesAndVertex(storagesCount);

            start.vertex = getTrainVertex(train);
            start.point = getTrainPoint(train);

            start.marketPos = 0;
            start.maxValue = train -> goods_capacity;

            q.push(start);

            while(!q.empty()) {

                El to, now;

                now = q.top();
                q.pop();

                if (!isFreeTimeTable(now)) continue;
                if (isMarket(now.point) && now.value == 0) continue;

                if (fVertex[now.marketPos][now.vertex] >= now.value - now.spent + fixNumber) continue;
                fVertex[now.marketPos][now.vertex] = now.value - now.spent + fixNumber;


                now.way.push_back(now.vertex);
                if (isTown(now.vertex)) checkOnBestWay(now, finalWay, bestCost);

                /// if (now.marketPos != storagesCount && now.vertex == needStorages[a[b[now.marketPos]] - 1]) {
                if (now.marketPos != m && now.vertex == needStorages[a[b[now.marketPos]] - 1]) {
                    now.marketPos += 1;
                    now.value += storageArmor(now.point, now.spent);
                    now.value = std::min(now.value, now.maxValue);
                }

                for (auto line : now.point -> adjacent) {

                    if (line.second -> points[0] -> idx == now.vertex) {

                        to = now;
                        to.line = line.second;
                        for (int j = 1; j < line.second -> length; j++) {

                            to.spent += 1;
                            to.position = j;

                            if (!isFreeTimeTable(to)) goto point1;
                        }
                        to.spent += 1;
                        to.line = nullptr;

                        to.vertex = line.second -> points[1] -> idx;
                        to.point = line.second -> points[1];

                        q.push(to);
                    }
                    else if (line.second -> points[1] -> idx == now.vertex) {

                        to = now;
                        to.line = line.second;
                        for (int j = line.second -> length - 1; j >= 1; j--) {

                            to.spent += 1;
                            to.position = j;

                            if (!isFreeTimeTable(to)) goto point1;
                        }
                        to.spent += 1;
                        to.line = nullptr;

                        to.vertex = line.second -> points[0] -> idx;
                        to.point = line.second -> points[0];

                        q.push(to);
                    }

                    point1:;
                }
            }
        } while(next_permutation(b.begin(), b.end()));

    } while (NextSet(a, n, m));

    addTimeTable(finalWay, train -> idx);

    reverse(finalWay.begin(), finalWay.end());
    if (finalWay.size()) finalWay.pop_back();

    addFictArmor(finalWay, train -> goods_capacity);

    return {finalWay, summary1 + summary2};
}


void PlayerController::trainIteration(Train* train) {
    json message;

    message["train_idx"] = train->idx;

    if (train->position == 0 &&
        train->line->points[1]->idx == train->needWay.back()) {

        message["speed"] = 1;
        message["line_idx"] = train->line->idx;
        Network::send(Action::MOVE, message);

        train->needWay.pop_back();
        return;
    }


    if (train->position == train -> line -> length &&
        train->line->points[0]->idx == train->needWay.back()) {

        message["speed"] = -1;
        message["line_idx"] = train->line->idx;
        Network::send(Action::MOVE, message);

        train->needWay.pop_back();
        return;
    }

    if (train -> position != 0 && train -> position != train -> line -> length) return;

    //TODO: store way as array of lines
    for (auto line : Database::lines) {
        if (line.second->points[0]->idx == getTrainVertex(train) &&
            line.second->points[1]->idx == train->needWay.back()) {

            message["speed"] = 1;
            message["line_idx"] = line.second->idx;
            Network::send(Action::MOVE, message);

            train->needWay.pop_back();
            return;
        }

        if (line.second->points[1]->idx == getTrainVertex(train) &&
            line.second->points[0]->idx == train->needWay.back()) {

            message["speed"] = -1;
            message["line_idx"] = line.second->idx;
            Network::send(Action::MOVE, message);


            train->needWay.pop_back();
            return;
        }
    }
    train->needWay.clear();
}


void PlayerController::tryTrainUpdate(Train* mainTrain) {

    if (mainTrain -> position != 0
    && mainTrain -> position != mainTrain -> line -> length) return;

    if (mainTrain -> position == 0
    && Database::playerTown -> point -> idx != mainTrain -> line -> points[0] -> idx) return;

    if (mainTrain -> position == mainTrain -> line -> length
    && Database::playerTown -> point -> idx != mainTrain -> line -> points[1] -> idx) return;

    if (Database::playerTown->armor >= 30 && mainTrain->level == 1) {

        json message;

        std::vector<int> a{};
        std::vector<int> b{mainTrain->idx};

        message["posts"] = a;
        message["trains"] = b;

        mainTrain->level += 1;
        Database::playerTown->armor -= 30;

        std::cout << "UPGRADE TO LEVEL 2, MESSAGE=" << message << std::endl;
        Network::send(Action::UPGRADE, message);
    }

    if (Database::playerTown->armor >= 60 && mainTrain->level == 2) {

        json message;

        std::vector<int> a{};
        std::vector<int> b{mainTrain->idx};

        message["posts"] = a;
        message["trains"] = b;

        mainTrain->level += 1;
        Database::playerTown->armor -= 60;

        std::cout << "UPGRADE TO LEVEL 3, MESSAGE=" << message << std::endl;

        Network::send(Action::UPGRADE, message);
    }

    if (mainTrain->level == 3) {
        if (Database::playerTown->armor >= 75 && Database::playerTown->level == 1) {
            json message;

            std::vector<int> a{Database::playerTown->idx};
            std::vector<int> b{};

            message["posts"] = a;
            message["trains"] = b;

            std::cout << "UPGRADE TOWN TO LEVEL 2, MESSAGE=" << message << std::endl;

            Database::playerTown->level += 1;
            Database::playerTown->armor -= 75;

            Network::send(Action::UPGRADE, message);
        }
        if (Database::playerTown->armor >= 150 && Database::playerTown->level == 2) {
            json message;

            std::vector<int> a{Database::playerTown->idx};
            std::vector<int> b{};

            message["posts"] = a;
            message["trains"] = b;

            std::cout << "UPGRADE TOWN TO LEVEL 3, MESSAGE=" << message << std::endl;

            Database::playerTown->level += 1;
            Database::playerTown->armor -= 150;
            Network::send(Action::UPGRADE, message);
        }
    }

}