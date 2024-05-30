#include <iostream>
#include <string>
#include <random>
#include <algorithm>

using namespace std;

//Step 1

struct Object{
	string name;
	int strength;
    int health;
};


int randomStatValue(double mean, double stddeviation, int minValue = 1){
    random_device seed; //This is our seed
    default_random_engine gen(seed()); //Engine
    normal_distribution<> d(mean, stddeviation); //normal distribution

    int value;
    do{
        value = round(d(gen)); //make it so it is a rounded value so it is not a decimal when the value for health and strength are genearted
    } while (value < minValue); //the value generated should be above 1
    
    return value;

}

int randomMonsterSpawner(int minValue, int maxValue){
    random_device seed;
    default_random_engine gen(seed());
    uniform_int_distribution<> d(minValue, maxValue);

    return d(gen);
}

bool isAlive(const Object& obj){
    return obj.health > 0;
}

int main() {

    Object player;
    player.name = "Bobby"; // Fun name :)
    player.health = randomStatValue(30,5); //mean = 30, standard deviation = 5
    player.strength = randomStatValue(5,1); //mean = 5, standard deviation = 1

    return 0;

    //Step 2

    vector<Object> monsters; //vector of monsters created however it is empty

    int numMonsters = randomMonsterSpawner(1,3); //number of monsters between 1 and 3
    for (int i = 0; i < numMonsters; ++i){
        Object monster; //created an object through the previously created struct

        monster.name = "Monster " + to_string(i + 1);
        monster.health = randomStatValue(30,5);
        monster.strength = randomStatValue(5,1);
        monsters.push_back(monster);
    }


    while(isAlive(player) && any_of(monsters.begin(), monsters.end(), isAlive)) { //We use the any_of operator to check the beginning and the end of the monsters vector to see if they are alive 

        //Display Player Stats
        cout << "Player Name: " << player.name << '\n';
        cout << "Player Health: " << player.health << '\n';

        //Display Monster Stats
        for (int i = 0; i < monsters.size(); ++i) { //this for loop repeats until all the elements inside monsters is printed
            cout << "Monster Name: " << monsters[i].name << '\n';
            cout << "Monster Health: " << monsters[i].health << '\n';
        }

        cout << "Do you want to attack (a) or heal (h)? ";
        char action;
        cin >> action;

        switch (action) {
            case 'a': {
                cout << "You chose to attack!" << '\n';
                break;
            }
            case 'h': {
                cout << "You chose to heal!" << '\n';
                int healingAmount = 2 * player.health;
                player.health += healingAmount;
                cout << "You healed " << healingAmount << "HP! " << '\n';  
                break;
            }
            default:{
                cout << "Please choose and valid aciton." << '\n';
                break;
            }
        }
    }


    if(!isAlive(player)) {
        cout << "Player is  dead! " << '\n';
    }
    else{
        cout << "Monsters are dead! Players Wins!" << '\n';
    }

    return 0;
}





