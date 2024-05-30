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

bool allMonstersDead(const vector<Object>& monsters) {
    for(int i = 0; i < monsters.size(); i++){
        if(isAlive(monsters[i])){
            return false;
        } 
    }
    return true;
}

int main() {

    Object player;
    player.name = "Bobby"; // Fun name :)
    player.health = randomStatValue(30,5); //mean = 30, standard deviation = 5
    player.strength = randomStatValue(5,1); //mean = 5, standard deviation = 1

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
        // I orginally had this code to display then replaced it so we can have the monster name always show and either the the monster health show or monster is dead
    
        /*
        for (int i = 0; i < monsters.size(); ++i) { 
            cout << "Monster Name: " << monsters[i].name << '\n';
            cout << "Monster Health: " << monsters[i].health << '\n';
        }
        */

        for (int i = 0; i < monsters.size(); i++){
            cout << "Monster Name: " << monsters[i].name << '\n';
            if (isAlive(monsters[i])){
                cout << "Monster Health " << monsters[i].health << '\n';
            } 
            else {
                cout << "Monster Health: DEAD XD" << '\n';
            }
        }

        
        char action;
        while (true) {
            // Ask the user for action: attack or heal
            cout << "Do you want to attack (a) or heal (h)? ";
            cin >> action;

            // Check if the input is valid
            if (action == 'a' || action == 'h') {
                break;
            } else {
                cout << "Invalid action. Please choose 'a' to attack or 'h' to heal." << '\n';
            }
        }
        

        switch (action) {
            case 'a': {
                cout << "You chose to attack!" << '\n';
                cout << "Which monster do you want to attack? (1 to " << monsters.size() << "): ";
                int monstersIndex;
                cin >> monstersIndex;
                if (monstersIndex >= 1 && monstersIndex <= monsters.size() && isAlive(monsters[monstersIndex - 1])) {
                    monsters[monstersIndex -1].health -= player.strength;
                    cout << "You attacked " << monsters[monstersIndex - 1].name << " for " << player.strength << '\n';
                    if(!isAlive(monsters[monstersIndex - 1])) {
                        cout << monsters[monstersIndex - 1].name << " is dead!" << '\n';
                    }
                }
                else {
                    cout << "Invalid monster choice or the monster is already dead." << '\n';
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


            //Monster Turn To Attack
            if(!allMonstersDead(monsters)){
                random_device seed;
                default_random_engine gen(seed());
                bernoulli_distribution d(0.75); 

                for (int i = 0; i < monsters.size(); i++) {
                if(isAlive(monsters[i]) && d(gen)){
                    player.health -= monsters[i].strength;
                    cout << monsters[i].name << " has attacked you for " << monsters[i].strength << '\n';
                }
            }
        }
    }


    //Determine if player wins or loses
    //Changed this code to make it so that there can be either player wins or monsters win
    
    /*
    if(!isAlive(player)) {
        cout << "Player is  dead! " << '\n';
    }
    else{
        cout << "Monsters are dead! Players Wins!" << '\n';
    }
    */
    
   if(!isAlive(player) && allMonstersDead(monsters)){
    cout << "GAME OVER: BOTH SIDES ARE DEAD!\n";
   }
   else if(!isAlive(player)){
    cout << "GAME OVER: MONSTERS WIN!\n";
   }
   else if(allMonstersDead(monsters)){
    cout << "GAME OVER: PLAYER WINS";
   }

    return 0;
}





