/*
    Project name: Role Playing Game
    Author: Onni Bucht

    Code is available at https://github.com/onni82/role-playing-game
*/

#include <iostream> // For cout and cin
#include <string> // For string variables
#include <cctype> // For using tolower() for example
#include <vector> // For using vector arrays
#include <stdlib.h> // For using system("cls")
#include <conio.h> // For using getch() on Windows
#include <algorithm> // For using sort()

using namespace std;

// This struct can create objects that are in your inventory
// Property definitions: name is the name of the item, effect is the effect of the item and amount is the amount that you have of that specific item
struct Item {
    string name;
    int amount;

    // Defines a funktion that compares the object's name with the object your searching for in your inventory
    static bool compareByName(const Item& p, const string& targetName) {
        return p.name < targetName;
    }
};

// This function is used to compare items in one's inventory while sorting it
bool compareItems(const Item& a, const Item& b) {
    return a.name < b.name;
}

// This struct can create an entity, such as your player or a monster that you encounter
struct Entity {
    string name; // Name of the object
    int health; // Number of hit points (HP) the object has
    int level;
    int currentExp; // Current experience points (EXP)

};

// Binary search function for a struct object
// Returns the index of the object being searched for if it exists, otherwise -1 is returned
template<typename T>
int binarySearch(const vector<T>& arr, const string& targetName, bool (*compareFunc)(const T&, const string&)) {
    int left = 0;
    int right = arr.size() - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (compareFunc(arr[mid], targetName)) {
            left = mid + 1; // The object can be in the right half
        } else if (compareFunc(arr[mid], targetName)) {
            right = mid - 1; // The object can be in the left half
        } else {
            return mid; // The object could be found
        }
    }

    return -1; // The object couldn't be found
}
/*Pseudo code for above part:
left = 0
right = array size -1

WHILE left <= right
    mid = left + (right - left) / 2

    IF array[mid] < key
        left = mid + 1
    ELSE IF array[mid] > key
        right = mid -1
    ELSE return mid
    END IF
END WHILE*/

// Function to trim string variables to not contain newlines
string trim(string input) {
    string output;

    for (char ch : input) { // Requires C++11 to compile
        if (ch == '\n' || ch == '\r')
            continue;
        output += ch;
    }

    return output;
}

// Function that prompts the user to press a button before clearing the screen
void ClearScreen() {
    cout << "Hit a key to proceed.\n";
    getch(); // Function that reads keystrokes on Windows
    //getchar(); // Similar to the above command, however, a press of the return key is required
    system("cls"); // Clears the terminal on Windows
    //system("clear"); // macOS equivalent of the above command
}

// Gives the player/entity experience
// First argument is the entity that you want to give experience while the second argument is the amount of experience you want to give
void RewardExp(Entity &player, int amountOfExp) {
    player.currentExp += amountOfExp;
    cout << "Player " << player.name << " gained " << amountOfExp << " experience.\n";
}

// Function that raises the level of the entity specified in the argument
// All entities start at level 0, but each level is rendered as level+1 in the game to make it look like starting at level 1
void LevelUp(Entity &player) {
    static const int requiredExp[] = { // Sets the amount of experience needed to level up (e.g. 50 from 0 to 1, 100 from 1 to 2)
        50, 100, 150, 200, 250, 300, 350, 400, 450, 500
    };

    while(player.currentExp >= requiredExp[player.level]) {
        ++player.level;
        cout << player.name << " leveled up to level " << player.level+1 << ".\n";
    }
}

// Function that adds an item to the inventory
// Arguments in call: the array/inventory in where to add the item, name of the item and amount of the particular item you want to add
void AddItem(vector<Item> &itemList, string nameOfItem, int amountOfItem) {
    int result = binarySearch(itemList, nameOfItem, &Item::compareByName); // Searches for the item in the inventory

    if (result != -1) { // If the item is found in the inventory
        itemList[result].amount += amountOfItem;
        if (amountOfItem > 1) {
            cout << "Picked up " << amountOfItem << " " << nameOfItem << "s. Now a total of " << itemList[result].amount << ".\n";
        } else {
            cout << "Picked up " << amountOfItem << " " << nameOfItem << ". Now a total of " << itemList[result].amount << ".\n";
        }
    } else { // If the item couldn't be found in the inventory
        Item item; // Creates a new item
        item.name = nameOfItem;
        item.amount = amountOfItem;
        itemList.push_back(item); // Adds the item to the inventory
        cout << "Picked up " << amountOfItem << " " << nameOfItem;

        if (amountOfItem > 1) { // Adds a lower-case S at the end if plural
             cout << "s";
        }

        cout << ".\n";
    }
}

// Function to be able to use items that are in one's inventory
// The arguments are: the array/inventory you want to update after using the item, the name of the item and the entity that the item should affect
void UseItem(vector<Item> &itemList, string nameOfItem, Entity &player) {
    int result = binarySearch(itemList, nameOfItem, &Item::compareByName); // Searches for the item in one's inventory

    if (result != -1) { // If the item is found in the inventory
        if (nameOfItem == "Potion") {
            player.health += 15;
        } else if (nameOfItem == "Greater Potion") {
            player.health += 25;
        }

        itemList[result].amount -= 1;

        cout << "Used " << nameOfItem << " on " << player.name << ".\n";

        if (itemList[result].amount == 0) { // Checks if the item's amount reaches zero
            itemList.erase(itemList.begin() + result); // Removes the item from the inventory
        }
    }
}

// Function that tells what effect an object (which can be in the inventory) gives when used
string GetItemDescription(string nameOfItem) {
    string itemDescription;
    if (nameOfItem == "Potion") {
        itemDescription = "Heals 15 HP per item";
    } else if (nameOfItem == "Greater Potion") {
        itemDescription = "Heals 25 HP per item";
    } else {
        itemDescription = "No description";
    }
    return itemDescription;
}

// Function that starts a battle with an entity/monster
void BattleEntity (Entity &player, Entity &enemy, vector<Item> &itemList) {
    cout << "Initiating battle with level " << enemy.level+1 << " " << enemy.name << " who has " << enemy.health << " HP.\n";
    cout << "Level " << player.level+1 << " player " << player.name << " has " << player.health << " HP.\n";

    ClearScreen();

    char optionInBattle; // Variable for menu selection in battle

    while(true) {
        if (player.health <= 0) { // Checks if the player has died in battle
            cout << "Player " << player.name << " died. Running away from battle.\n";
            break;
        }
        if (enemy.health <= 0) { // Checks if the monster has died in battle
            cout << enemy.name << " died.\n";
            RewardExp(player, (enemy.level+1) * 15); // Gives the player experience (= monster level * 15)
            LevelUp(player);
            break;
        }

        cout << "What do you want to do? (A)ttack, (I)tem or (R)un?: ";
        cin >> optionInBattle;

        if (optionInBattle == 'r' || optionInBattle == 'R') { // Runs from battle
            cout << "Player " << player.name << " ran away from battle.\n";
            break;
        }

        switch(optionInBattle) { // Checks option during battle
            case 'a': case 'A': // When you attack during battle
            {
                cout << "Player " << player.name << " attacks " << enemy.name << ".\n" << enemy.name << " lost 10 HP.\n";
                enemy.health -= (player.level+1)*10;
                cout << "Player " << player.name << ": " << player.health << " HP.\n";
                cout << enemy.name << ": " << enemy.health << "HP.\n";
                ClearScreen();
                break;
            }
            case 'i': case 'I': // When you use an item in battle
            {
                if (itemList.size() == 0) { // Automatically attacks if there are no items in the inventory
                    cout << "You have no items in your inventory.\n";
                    cout << "Player " << player.name << " attacks " << enemy.name << ".\n" << enemy.name << " lost 10 HP.\n";
                    enemy.health -= (player.level+1)*10;
                    cout << "Player " << player.name << ": " << player.health << " HP.\n";
                    cout << enemy.name << ": " << enemy.health << "HP.\n";
                    ClearScreen();
                    break;
                }

                cout << "Your inventory:\n";
                for (unsigned i = 0; i < itemList.size(); ++i) { // Prints each item in one's inventory
                    cout << "[" << i << "] " << itemList[i].amount << " " << itemList[i].name;
                    if (itemList[i].amount > 1){
                        cout << "s";
                    }
                    cout << ". " << GetItemDescription(itemList[i].name) << ".\n";
                }
                cout << "Your pick: ";

                int itemToUse;

                if (cin >> itemToUse) { // Uses an item on the player
                    UseItem(itemList, itemList[itemToUse].name, player);

                    cout << "Player " << player.name << ": " << player.health << " HP.\n";
                    cout << enemy.name << ": " << enemy.health << "HP.\n";
                }

                ClearScreen();

                break;
            }
            default:
            break;
        }
        if (player.health <= 0) { // Checks if the player has died in battle
            cout << "Player " << player.name << " died. Running away from battle.\n";
            break;
        }
        if (enemy.health <= 0) { // Checks if the monster has died in battle
            cout << enemy.name << " died.\n";
            RewardExp(player, (enemy.level+1) * 15); // Gives the player experience (= monster level * 15)
            LevelUp(player);
            break;
        }
        cout << enemy.name << " attacks " << player.name << ".\n" << player.name << " lost 10 HP.\n";
        player.health -= (enemy.level+1)*10;
        cout << "Player " << player.name << ": " << player.health << " HP.\n";
        cout << enemy.name << ": " << enemy.health << "HP.\n";
        ClearScreen();
    }
}

int main(int argc, char* argv[]) {
    Entity user; // Creates the player as an object of the entity struct

    if (argc > 1) { // Checks for any executable arguments. The player name will be based on the first argument if it exists
        user.name = trim(argv[1]);
    } else { // No executable arguments found. Will ask for player name
        cout << "What's your name? ";
        string input;
        getline(cin, input);
        user.name = trim(input);
    }
/*  Pseudo code for code above:
    IF program arguments > 1 THEN
    player name = argument[1]
    ELSE player name = input from user
    END IF*/

    user.health = 20; // Sets the player's hit points
    user.level = 0; // Sets the player's level
    user.currentExp = 0; // Sets the amount of experience the player starts with

    cout << "Hello " << user.name << ". Let's start your adventure. You start with two potions.\n";
    ClearScreen();

    vector<Item> inventory = { // Requires C++11 to compile
        {"Potion", 2}
    };

    // Creates the first boss, which is an orc
    Entity orc;
    orc.name = "Zug Zug the Orc";
    orc.health = 25;
    orc.level = 0;
    orc.currentExp = 0;

    sort(inventory.begin(), inventory.end(), compareItems); // Sorts the inventory
    BattleEntity(user, orc, inventory); // Starts a battle with the first boss
    ClearScreen();

    cout << "You enter a big forest. There's a big bush. You checked the bush and found something.\n";
    ClearScreen();

    AddItem(inventory, "Potion", 5); // Adds five potions to inventory
    sort(inventory.begin(), inventory.end(), compareItems); // Sorts the inventory
    cout << "As you walk further into the forest, you get ambushed by the Elven King and his army.\n";
    ClearScreen();

    // Creates the second boss, which is the King of the Elves
    Entity elf;
    elf.name = "Elven King";
    elf.health = 25;
    elf.level = 0;
    elf.currentExp = 0;

    sort(inventory.begin(), inventory.end(), compareItems); // Sorts the inventory
    BattleEntity(user, elf, inventory); // Starts a battle with the King of the Elves
    ClearScreen();

    cout << "After facing the Elven King you noticed he dropped 10 greater potions.\n";
    AddItem(inventory, "Greater Potion", 10); // Adds 10 greater potions to inventory
    sort(inventory.begin(), inventory.end(), compareItems); // Sorts the inventory
    ClearScreen();

    cout << "You feel like resting by a tree, but you notice it seems alive, like it can talk.\n";
    ClearScreen();

    // Creates the ent entity for the upcoming battle
    Entity ent;
    ent.name = "Ent";
    ent.health = 30;
    ent.level = 1;
    ent.currentExp = 0;

    sort(inventory.begin(), inventory.end(), compareItems); // Sorts the inventory
    BattleEntity(user, ent, inventory); // Starts a battle with the ent
    ClearScreen();

    cout << "Congratulations! You finished the game.\n";
    ClearScreen();

    return 0;
}
