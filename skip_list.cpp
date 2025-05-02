#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Player {
public:
    string name;
    int score;

    Player(int score, string name) : score(score), name(name) {}
    Player() : name(""), score(-1) {}

    bool operator<(const Player& other) const {
        if(score == other.score){
            return name < other.name;
        }
        return score > other.score;
    }

    bool operator>(const Player& other) const {
        if(score == other.score){
            return name > other.name;
        }
        return score < other.score;
    }

    bool operator==(const Player& other) const {
        return name == other.name;
    }

    bool operator!=(const Player& other) const {
        return name != other.name;
    }

    friend ostream& operator<<(ostream& os, const Player& player) {
        os << player.name << " (" << player.score << ")";
        return os;
    }
};

template <typename T>
class Node {
public:
    T key;
    vector<Node*> nodeArray;

    Node(T key, int level) : key(key), nodeArray(level, nullptr) {}
};

template <typename T>
class SkipList {
private:
    int level, maxLevel;
    Node<T>* head;

    int randomLevel() {
        int randomLevel = 1;
        while (((float)rand() / RAND_MAX) < 0.5 && randomLevel < maxLevel)
            randomLevel++;
        return randomLevel;
    }

public:
    SkipList(int maxLevel) : maxLevel(maxLevel), level(1) {
        head = new Node<T>(T(), maxLevel);
    }

    ~SkipList() {
        delete head;
    }
    
    void insert(T element) {
        vector<Node<T>*> update(maxLevel, nullptr);
        Node<T>* current = head;

        for (int i = level - 1; i >= 0; i--) {
            while (current->nodeArray[i] != nullptr && current->nodeArray[i]->key < element) {
                current = current->nodeArray[i];
            }
            update[i] = current;
        }

        current = current->nodeArray[0];

        if (current == nullptr || current->key != element) {
            int randomLvl = randomLevel();

            if (randomLvl > level) {
                for (int i = level; i < randomLvl; i++) {
                    update[i] = head;
                }
                level = randomLvl;
            }

            Node<T>* newNode = new Node<T>(element, randomLvl);

            for (int i = 0; i < randomLvl; i++) {
                newNode->nodeArray[i] = update[i]->nodeArray[i];
                update[i]->nodeArray[i] = newNode;
            }

            cout << "Inserted: " << element << endl;
        }
    }

    void remove(T element) {
        vector<Node<T>*> update(maxLevel, nullptr);
        Node<T>* current = head;

        for (int i = level - 1; i >= 0; i--) {
            while (current->nodeArray[i] != nullptr && current->nodeArray[i]->key < element) {
                current = current->nodeArray[i];
            }
            update[i] = current;
        }

        current = current->nodeArray[0];

        if (current != nullptr && current->key == element) {
            for (int i = 0; i < level; i++) {
                if (update[i]->nodeArray[i] != current)
                    break;
                update[i]->nodeArray[i] = current->nodeArray[i];
            }
            delete current;

            while (level > 1 && head->nodeArray[level - 1] == nullptr) {
                level--;
            }
        }
    }

    bool find(T key) {
        Node<T>* current = head;

        for (int i = level - 1; i >= 0; i--) {
            while (current->nodeArray[i] != nullptr && current->nodeArray[i]->key < key) {
                current = current->nodeArray[i];
            }
        }

        
        current = current->nodeArray[0];
        return current != nullptr && current->key == key;
    }

    Node<T>* search(T key) {
        Node<T>* current = head;

        for (int i = level - 1; i >= 0; i--) {
            while (current->nodeArray[i] != nullptr && current->nodeArray[i]->key < key) {
                current = current->nodeArray[i];
            }
        }

        current = current->nodeArray[0];
        return current;
    }

    void displayAll() {
        cout << "Players in the game:\n";
        Node<T>* current = head->nodeArray[0];
        while (current != nullptr) {
            cout << current->key << endl;
            current = current->nodeArray[0];
        }
    }

    void displayTopN(int n) {
        cout << "Top " << n << " Players:\n";
        Node<T>* current = head->nodeArray[0];
        int count = 0;
        while (current != nullptr && count < n) {
            cout << current->key << endl;
            current = current->nodeArray[0];
            count++;
        }
    }
};

void displayGameCataloge() {
    cout << "\nWelcome to FCAI's game, choose an operation:\n";
    cout << "1 - Add a new player to the game\n";
    cout << "2 - Join an existing player to the game\n";
    cout << "3 - Delete a player from the game\n";
    cout << "4 - View a player's score\n";
    cout << "5 - Retrieve the top players\n";
    cout << "6 - Change a player's score\n";
    cout << "7 - Display all players\n";
    cout << "8 - Exit\n";
}

int main() {
    SkipList<Player> game(5), deleted(5);
    int choice;

    do {
        displayGameCataloge();
        cout << "\nEnter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                string name;
                cout << "Enter the name: ";
                cin >> name;
                game.insert(Player(0, name));
                cout << name << " added with score 0.\n";
                break;
            }
            case 2: {
                string name;
                cout << "Enter the name: ";
                cin >> name;
                Player playerToSearch(0, name);
                Node<Player>* deletedPlayerNode = deleted.search(playerToSearch);
                if (deletedPlayerNode != nullptr) {
                    Player deletedPlayer = deletedPlayerNode->key; // Retrieve the actual player object with their score.
                    deleted.remove(deletedPlayer); // Remove the player from the deleted list.
                    game.insert(deletedPlayer); // Reinsert the player into the active game.
                    cout << name << " rejoined the game with score: " << deletedPlayer.score << endl;
                } else {
                    cout << "Player not found in the deleted list.\n";
                }
                break;
            }
            case 3: {
                string name;
                cout << "Enter the name: ";
                cin >> name;
                if (game.find(Player(0, name))) {
                    game.remove(Player(0, name));
                    deleted.insert(Player(0, name));
                    cout << name << " removed from the game.\n";
                } else {
                    cout << "Player not found in game.\n";
                }
                break;
            }
            case 4: {
                string name;
                cout << "Enter the name: ";
                cin >> name;
                if (game.find(Player(0, name))) {
                    cout << game.search(Player(0, name))->key.score;
                } else {
                    cout << name << endl;
                    cout << "Player not found in game.\n";
                }
                break;
            }
            case 5: {
                int n;
                cout << "Enter the number of top players to retrieve: ";
                cin >> n;
                game.displayTopN(n);
                break;
            }
            case 6: {
                string name;
                int scoreChange;
                cout << "Enter the name: ";
                cin >> name;
                cout << "Enter score change (+/-): ";
                cin >> scoreChange;

                if (game.find(Player(0, name))) {
                    game.remove(Player(0, name));
                    game.insert(Player(scoreChange, name));
                    cout << name << "'s score updated to " << scoreChange << ".\n";
                } else {
                    cout << "Player not found.\n";
                }
                break;
            }
            case 7:
                game.displayAll();
                break;
            case 8:
                cout << "Exiting the game. Goodbye!\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 8);

    return 0;
}
