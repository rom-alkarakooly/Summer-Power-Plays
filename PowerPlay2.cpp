#include <iostream>
#include <string>
#include <random>
#include <vector>
#include <fstream>
#include <limits>

using namespace std;

struct Object {
    string name;
    int strength;
    int health;
};

int randomStatValue(double mean, double stddeviation, int minValue = 1) {
    random_device seed;
    default_random_engine gen(seed());
    normal_distribution<> d(mean, stddeviation);

    int value;
    do {
        value = round(d(gen));
    } while (value < minValue);

    return value;
}

bool isAlive(const Object& obj) {
    return obj.health > 0;
}

bool allMonstersDead(const vector<Object>& monsters) {
    for (int i = 0; i < monsters.size(); i++) {
        if (isAlive(monsters[i])) {
            return false;
        }
    }
    return true;
}

int calculateDamage(int strength) {
    random_device seed;
    default_random_engine gen(seed());
    normal_distribution<> d(strength, 2.0);
    int damage;
    do {
        damage = round(d(gen));
    } while (damage < 0);
    return damage;
}

vector<Object> loadMonstersFromFile(const string& filename) {
    ifstream file(filename);
    if (!file) {
        cerr << "Error opening file: " << filename << '\n';
        exit(1);
    }

    int numMonsters;
    file >> numMonsters;

    vector<Object> monsters;
    for (int i = 0; i < numMonsters; ++i) {
        Object monster;
        file >> ws; // consume any whitespace
        getline(file, monster.name);
        file >> monster.strength >> monster.health;
        monsters.push_back(monster);
    }

    return monsters;
}

int main() {
    Object player;
    player.name = "Bobby";
    player.health = randomStatValue(30, 5);
    player.strength = randomStatValue(5, 1);

    vector<Object> monsters = loadMonstersFromFile("monsters.txt");

    while (isAlive(player) && any_of(monsters.begin(), monsters.end(), isAlive)) {
        cout << "Player Name: " << player.name << '\n';
        cout << "Player Health: " << player.health << '\n';

        for (int i = 0; i < monsters.size(); i++) {
            cout << "Monster Name: " << monsters[i].name << '\n';
            if (isAlive(monsters[i])) {
                cout << "Monster Health: " << monsters[i].health << '\n';
            } else {
                cout << "Monster Health: DEAD" << '\n';
            }
        }

        char action;
        while (true) {
            cout << "Do you want to attack (a) or heal (h)? ";
            cin >> action;
            if (action == 'a' || action == 'h') {
                break;
            } else {
                cout << "Invalid action. Please choose 'a' to attack or 'h' to heal." << '\n';
            }
        }

        switch (action) {
            case 'a': {
                cout << "You chose to attack!" << '\n';
                int monstersIndex;
                while (true) {
                    cout << "Which monster do you want to attack? (1 to " << monsters.size() << "): ";
                    cin >> monstersIndex;
                    if (cin.fail() || monstersIndex < 1 || monstersIndex > monsters.size() || !isAlive(monsters[monstersIndex - 1])) {
                        cout << "Invalid monster choice or the monster is already dead." << '\n';
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n'); //this line just says im going to ignore the characters that were just written to avoid the error the user made
                    } else {
                        break;
                    }
                }
                int damage = calculateDamage(player.strength);
                monsters[monstersIndex - 1].health -= damage;
                cout << "You attacked " << monsters[monstersIndex - 1].name << " for " << damage << '\n';
                if (!isAlive(monsters[monstersIndex - 1])) {
                    cout << monsters[monstersIndex - 1].name << " is dead!" << '\n';
                }
                break;
            }
            case 'h': {
                cout << "You chose to heal!" << '\n';
                int healingAmount = 2 * player.strength;
                player.health += healingAmount;
                cout << "You healed " << healingAmount << "HP! " << '\n';
                break;
            }
        }

        if (!allMonstersDead(monsters)) {
            random_device seed;
            default_random_engine gen(seed());
            bernoulli_distribution d(0.75);

            for (int i = 0; i < monsters.size(); i++) {
                if (isAlive(monsters[i]) && d(gen)) {
                    int damage = calculateDamage(monsters[i].strength);
                    player.health -= damage;
                    cout << monsters[i].name << " has attacked you for " << damage << '\n';
                }
            }
        }
    }

    if (!isAlive(player) && allMonstersDead(monsters)) {
        cout << "GAME OVER: BOTH SIDES ARE DEAD!\n";
    } else if (!isAlive(player)) {
        cout << "GAME OVER: MONSTERS WIN!\n";
    } else if (allMonstersDead(monsters)) {
        cout << "GAME OVER: PLAYER WINS\n";
    }

    return 0;
}
