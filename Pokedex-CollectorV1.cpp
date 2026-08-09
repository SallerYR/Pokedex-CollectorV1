/* ============================================================================
  POKEDEX COLLECTOR - v1.0
  Assessment 03 - Individual Program Design
  16264 - Saller Yero Rezende  |  PRG1006 - Programming 02
 ----------------------------------------------------------------------------
  A simple console game: search the tall grass, throw a Poke Ball, and catch
  all 5 Pokemon to complete your Pokedex. Throwing a Poke Ball always works.

  Object-Oriented design with cooperating classes:
      Pokemon  - BASE class for one Pokemon (name, rarity, rate, caught flag)
      Pidgey / Rattata / Pikachu / Eevee / Snorlax - CHILD classes of Pokemon
      Player   - the trainer (name, decides actions, throws the Poke Ball)
      Pokedex  - keeps track of which Pokemon have been caught
      Game     - the controller that ties everything together

  POLYMORPHISM: Pokemon has a "virtual" method getCry(). Each child class
  overrides it with its own sound (e.g. Pikachu -> "Pika pika!"). Because the
  Pokemon are stored as base-class pointers (Pokemon*), calling wild->getCry()
  automatically runs the RIGHT version for the actual Pokemon at run time.
 ============================================================================
 */
 #include <iostream>   // for std::cout / std::cin (printing and reading input)
#include <string>     // for std::string and std::stoi
#include <vector>     // for std::vector (a simple, resizable list)
#include <cstdlib>    // for rand() and srand() (random numbers)
#include <ctime>      // the "time" library: used to seed random and to pause
#include <stdexcept>  // for the exception types used by try / catch (stoi)
using namespace std;
/* ----------------------------------------------------------------------------
  HELPER: wait()
  A small pause that uses the <ctime> library. It reads the current time and
  loops until the requested number of seconds has passed. This is used to add
  a little suspense while searching and while throwing the Poke Ball.
 ----------------------------------------------------------------------------
 */
 void wait(int seconds) {
    time_t start = time(0);                 // remember the time we started
    while (time(0) - start < seconds) {     // keep looping until time is up
        // do nothing - just wait
    }
}
/* ----------------------------------------------------------------------------
  HELPER: getIntInput()
  Safely reads a whole number from the player. If the player types letters
  or other non-number text, cin would normally "fail" and get stuck in an
  infinite loop. This function catches that: it clears the error and throws
  away the bad input, then returns -1 to mean "invalid choice".
 ----------------------------------------------------------------------------
 */
 int getIntInput() {
    int value;
    cin >> value;
    if (cin.fail()) {              // true if the input was not a number
        cin.clear();               // clear the error flag so cin works again
        cin.ignore(10000, '\n');   // discard the leftover bad text
        return -1;                 // -1 signals "not a valid choice"
    }
    return value;
}
/* ============================================================================
  BASE CLASS: Pokemon
  Holds the data shared by every Pokemon. All data is private (encapsulation)
  and can only be read or changed through the public methods below.
   ============================================================================
*/
class Pokemon {
private:
    string name;          // the Pokemon's name, e.g. "Pikachu"
    string rarity;        // how rare it is, e.g. "Common"
    double encounterRate; // chance of appearing when searching (as a percent)
    string asciiArt;      // this Pokemon's own little ASCII picture
    bool isCaught;        // true once it has been caught
public:
    // Constructor: sets up a new Pokemon with its starting values.
    // "art" is the ASCII picture shown when this Pokemon is encountered.
    Pokemon(string n, string r, double rate, string art) {
        name = n;
        rarity = r;
        encounterRate = rate;
        asciiArt = art;
        isCaught = false;     // every Pokemon starts "not caught"
    }
    // A "virtual" destructor is required for a base class used through
    // pointers, so that "delete" cleans up the child object correctly.
    virtual ~Pokemon() {}
    // "Getters" - let other classes READ the private data safely
    string getName() { return name; }
    string getRarity() { return rarity; }
    double getEncounterRate() { return encounterRate; }
    string getAsciiArt() { return asciiArt; }
    bool getIsCaught() { return isCaught; }
    // "Setter" - the only way to CHANGE the caught status from outside
    void setCaught(bool status) { isCaught = status; }
    // POLYMORPHISM: this is "virtual", so a child class can REPLACE it with
    // its own sound. If a child does not override it, this default is used.
    virtual string getCry() { return "..."; }
};
/* 
  ============================================================================
  CHILD CLASSES: one per Pokemon.
  Each passes its own data to the Pokemon (base) constructor, and OVERRIDES
  getCry() with its own sound. This is the polymorphism in action.
  ============================================================================
 */
 class Pidgey : public Pokemon {
public:
    Pidgey() : Pokemon("Pidgey", "Common", 35,
        "        ,~.\n"
        "       (o o)\n"
        "      /  V  \\\n"
        "     /(  _  )\\\n"
        "       ^^ ^^  \n") {
    }
    string getCry() override { return "Pidge pidge!"; }
};
class Rattata : public Pokemon {
public:
    Rattata() : Pokemon("Rattata", "Common", 30,
        "       (\\_/)\n"
        "       ( o.o)\n"
        "       (> < )\n") {
    }
    string getCry() override { return "Ratta! Squeak! Squeee!"; }
};
class Pikachu : public Pokemon {
public:
    Pikachu() : Pokemon("Pikachu", "Uncommon", 20,
        "       /\\_/\\\n"
        "      ( o.o )\n"
        "       > ^ <\n") {
    }
    string getCry() override { return "Pika pika!"; }
};
class Eevee : public Pokemon {
public:
    Eevee() : Pokemon("Eevee", "Rare", 10,
        "      /\\   /\\\n"
        "     ( * . * )\n"
        "      >  v  <\n") {
    }
    string getCry() override { return "Vee vee!"; }
};
class Snorlax : public Pokemon {
public:
    Snorlax() : Pokemon("Snorlax", "Legendary", 5,
        "          _____\n"
        "      .-''     ''-.\n"
        "     /   -     -   \\\n"
        "    |      ---      |\n"
        "    |    \\___/      |\n"
        "   /|               |\\\\\n"
        "  / |               | \\\\\n"
        "    |_____/ \\_______|\n"
        "       /_/   \\_\\\\\n") {
    }
    string getCry() override { return "Snoooor... zzz..."; }
};
/* ============================================================================
  CLASS: Player
  Represents the trainer. Chooses what to do at an encounter and throws the
  Poke Ball (which always catches the Pokemon in this simple version).
 ============================================================================
 */
 class Player {
private:
    string name;   // the trainer's name
public:
    // Constructor: give the player a default name
    Player() { name = "Trainer"; }
    string getName() { return name; }
    void setName(string n) { name = n; }
    // Ask the player to choose an action at an encounter: 1 = throw, 2 = run.
    // This uses a TRY / CATCH system so a bad input (letters, or a number that
    // is not 1 or 2) is rejected and asked again - it never counts as "run".
    int decideAction() {
        while (true) {   // keep asking until the player gives a valid choice
            cout << "  1. Throw Poke Ball" << endl;
            cout << "  2. Run Away" << endl;
            cout << "  Enter your choice (1-2): ";

            string input;
            cin >> input;            // read one word (skips spaces/newlines)

            try {
                // stoi() converts text to an int. If "input" is not a number
                // (e.g. "abc") it THROWS invalid_argument, jumping to catch.
                int choice = stoi(input);

                // If it IS a number but not 1 or 2, we throw our own error.
                if (choice != 1 && choice != 2) {
                    throw out_of_range("choice must be 1 or 2");
                }

                return choice;       // valid: give back 1 or 2
            }
            catch (const exception& e) {
                // Runs for BOTH bad cases above. Show a message and loop again.
                cout << "  Invalid choice, please enter 1 or 2." << endl << endl;
            }
        }
    }
    // Throw the Poke Ball at a Pokemon. Because there is no catch-chance,
    // this simply marks the Pokemon as caught. Takes a pointer so it works
    // with any child type of Pokemon.
    void throwPokeball(Pokemon* p) {
        p->setCaught(true);
    }
};
/* ============================================================================
  CLASS: Pokedex
  Stores the Pokemon that have been caught and reports on progress.
  It holds Pokemon* POINTERS so polymorphism keeps working.
 ============================================================================
 */
 class Pokedex {
private:
    vector<Pokemon*> caughtPokemon;  // the list of caught Pokemon (pointers)
    int totalPokemonAvailable;       // how many exist in total (5)
public:
    // Constructor: remember how many Pokemon there are to catch in total
    Pokedex(int total) { totalPokemonAvailable = total; }
    // Add a caught Pokemon to the Pokedex
    bool addPokemon(Pokemon* p) { // takes a pointer so it works with any child type of Pokemon
        caughtPokemon.push_back(p);
        return true;
    }
    // How many Pokemon have been caught so far
    int getCaughtCount() { return caughtPokemon.size(); }
    // The Pokedex is complete when caught count equals the total available
    bool isComplete() { return getCaughtCount() == totalPokemonAvailable; }
    // Print ALL Pokemon: [X] for caught, [ ] for not yet caught.
    // The full list is passed in so the Pokedex can show every Pokemon,
    // not only the ones already caught, plus the progress line.
    void displayPokedex(vector<Pokemon*>& allPokemon) { // using * for polymorphism
        for (int i = 0; i < (int)allPokemon.size(); i++) {
            if (allPokemon[i]->getIsCaught()) {
                cout << "  [X] " << allPokemon[i]->getName()
                    << " - Caught" << endl;
            }
            else {
                cout << "  [ ] " << allPokemon[i]->getName()
                    << " - Not yet caught" << endl;
            }
        }
        cout << endl;
        cout << "  Progress: " << getCaughtCount()
            << " / " << totalPokemonAvailable << " Pokemon collected" << endl;
    }
};
/* ============================================================================
//  CLASS: Game
//  The central controller. It owns the Player, the Pokedex and the list of
//  available Pokemon, and runs the main game loop.
 ============================================================================
 */
 class Game {
private:
    Player player;                     // the one player
    Pokedex pokedex;                   // the player's Pokedex
    vector<Pokemon*> availablePokemon; // all the Pokemon that can be found
    bool isRunning;                    // true while the game is running
public:
    // Constructor: build the Pokedex (5 total) and fill the Pokemon list.
    // We store base-class pointers (Pokemon*) that actually point to the
    // child objects (Pidgey, Rattata, ...). "new" creates each child.
    Game() : pokedex(5) {
        isRunning = true;
        availablePokemon.push_back(new Pidgey());
        availablePokemon.push_back(new Rattata());
        availablePokemon.push_back(new Pikachu());
        availablePokemon.push_back(new Eevee());
        availablePokemon.push_back(new Snorlax());
    }
    // Destructor: free the memory for every Pokemon we created with "new".
    ~Game() {
        for (int i = 0; i < (int)availablePokemon.size(); i++) {
            delete availablePokemon[i];
        }
    }
    // ---- The main game loop -------------------------------------------------
    void run() {
        displayWelcomeScreen();
        // Keep showing the menu until the player wins or chooses to exit
        while (isRunning && !pokedex.isComplete()) {
            displayMainMenu();
            int choice = getIntInput();   // safely read the menu choice
            if (choice == 1)      searchForPokemon();
            else if (choice == 2) viewPokedex();
            else if (choice == 3) displayInstructions();
            else if (choice == 4) exitGame();
            else cout << "  Invalid choice, please try again." << endl;
        }
        // If the loop ended because the Pokedex is full, show the win screen
        if (pokedex.isComplete()) checkVictory();
    }
    // ---- Screen: Welcome ----------------------------------------------------
    void displayWelcomeScreen() {
        cout << "=================================================" << endl;
        cout << "          POKEDEX COLLECTOR - v1.0" << endl;
        cout << "=================================================" << endl;
        cout << endl;
        // Ask the player for their name and store it in the Player object.
        // getline reads the whole line, so names with spaces are allowed.
        cout << "  What is your name, Trainer? ";
        string trainerName;
        getline(cin, trainerName);
        if (!trainerName.empty()) {   // only change it if they typed something
            player.setName(trainerName);
        }
        cout << endl;
        cout << "  Welcome, " << player.getName() << "!" << endl << endl;
        cout << "  Somewhere out there, five Pokemon are" << endl;
        cout << "  waiting to be found:" << endl << endl;
        cout << "    Pidgey  Rattata  Pikachu  Eevee  Snorlax" << endl << endl;
        cout << "  Your mission: search the tall grass," << endl;
        cout << "  throw Poke Balls, and complete your Pokedex!" << endl << endl;
        cout << "  Press ENTER to begin...";
        cin.get();   // wait for the player to press ENTER
    }
    // ---- Screen: Main Menu --------------------------------------------------
    void displayMainMenu() {
        cout << endl;
        cout << "=================================================" << endl;
        cout << "                  MAIN MENU" << endl;
        cout << "=================================================" << endl;
        cout << "  1. Search for Pokemon" << endl;
        cout << "  2. View Pokedex" << endl;
        cout << "  3. Instructions" << endl;
        cout << "  4. Exit" << endl;
        cout << "=================================================" << endl;
        cout << "  Enter your choice (1-4): ";
    }
    // ---- Screen: Instructions -----------------------------------------------
    void displayInstructions() {
        cout << endl;
        cout << "=================================================" << endl;
        cout << "                INSTRUCTIONS" << endl;
        cout << "=================================================" << endl;
        cout << "  - Choose 'Search for Pokemon' to explore." << endl;
        cout << "  - A wild Pokemon may appear at random." << endl;
        cout << "  - You may Throw a Poke Ball or Run Away." << endl;
        cout << "  - Throwing a Poke Ball always catches the" << endl;
        cout << "    Pokemon and adds it to your Pokedex." << endl;
        cout << "  - Rarer Pokemon just show up less often." << endl;
        cout << "  - Collect all 5 Pokemon to win the game." << endl;
        cout << "  - Gotta Catch em All!" << endl << endl;
        cout << "  Press ENTER to return to the Main Menu...";
        cin.ignore(); // clear the leftover newline from the menu choice
        cin.get();    // wait for ENTER
    }
    // ---- Action: Search for a Pokemon ---------------------------------------
    void searchForPokemon() {
        cout << endl;
        cout << "=================================================" << endl;
        cout << "                 SEARCHING..." << endl;
        cout << "=================================================" << endl;
        cout << "  You head out into the tall grass..." << endl;
        wait(2);   // pause for suspense (uses the time library)
        cout << "  Rustle... rustle..." << endl;
        wait(1);
        // Pick a random Pokemon based on the encounter rates
        int index = generateRandomPokemon();
        resolveEncounter(availablePokemon[index]);
    }
    // ---- Pick a random Pokemon using weighted odds --------------------------
    //  Pidgey 35% | Rattata 30% | Pikachu 20% | Eevee 10% | Snorlax 5%
    int generateRandomPokemon() {
        int roll = (rand() % 100) + 1;   // a random number from 1 to 100
        if (roll <= 35)      return 0;   // Pidgey
        else if (roll <= 65) return 1;   // Rattata
        else if (roll <= 85) return 2;   // Pikachu
        else if (roll <= 95) return 3;   // Eevee
        else                 return 4;   // Snorlax
    }
    // ---- Handle the encounter (throw or run) --------------------------------
    //  "wild" is a Pokemon* (base pointer). wild->getCry() runs the correct
    //  child version - that is the polymorphism doing its job.
    void resolveEncounter(Pokemon* wild) {
        cout << endl;
        cout << "=================================================" << endl;
        cout << "            A WILD POKEMON APPEARS!" << endl;
        cout << "=================================================" << endl;
        cout << wild->getAsciiArt();               // this Pokemon's own picture
        cout << endl;
        cout << "  It's a wild " << wild->getName() << "!" << endl;
        cout << "  " << wild->getName() << " cries: \""
           << wild->getCry() << "\"" << endl;    // polymorphic call
        cout << "  Rarity: " << wild->getRarity() << endl << endl;
        
        // Ask the player what to do (1 = throw, 2 = run). decideAction() now
        // loops with try/catch, so it only ever returns a valid 1 or 2.
        
        int action = player.decideAction();
        if (action == 1) {
            // Throw the Poke Ball - always catches in this version
            cout << endl << "  You threw a Poke Ball..." << endl;
            wait(1);
            // Show "Shaking..." three times, once per second, for suspense.
            // Each wait(1) uses the time library to pause for 1 second.
            for (int i = 0; i < 3; i++) {
                cout << "  Shaking..." << endl;
                wait(1);
            }
            player.throwPokeball(wild);   // mark the Pokemon as caught
            pokedex.addPokemon(wild);     // add it to the Pokedex
            cout << endl;
            cout << "=================================================" << endl;
            cout << "                   GOTCHA!" << endl;
            cout << "=================================================" << endl;
            cout << "  " << wild->getName() << " was caught!" << endl;
            cout << "  " << wild->getName()
                << " has been added to your Pokedex." << endl;
        }
        else {
            // action == 2: Run away - come back and find it another time
            cout << endl << "  You ran away safely!" << endl;
        }
    }
    // ---- Action: View the Pokedex -------------------------------------------
    void viewPokedex() {
        cout << endl;
        cout << "=================================================" << endl;
        cout << "                   POKEDEX" << endl;
        cout << "=================================================" << endl;
        pokedex.displayPokedex(availablePokemon);  // pass the full list of 5
        cout << endl << "  Press ENTER to return to the Main Menu...";
        cin.ignore(); // clear the leftover newline
        cin.get();    // wait for ENTER
    }
    // ---- Action: Exit the game ----------------------------------------------
    void exitGame() {
        isRunning = false;
        cout << endl;
        cout << "=================================================" << endl;
        cout << "               THANKS FOR PLAYING" << endl;
        cout << "=================================================" << endl;
        cout << "  Pokedex saved." << endl;
        cout << "  Progress: " << pokedex.getCaughtCount()
            << " / 5 Pokemon collected." << endl << endl;
        cout << "  See you next time, " << player.getName() << "!" << endl;
    }
    // ---- Win check: show the Victory screen ---------------------------------
    bool checkVictory() {
        cout << endl;
        cout << "=================================================" << endl;
        cout << "          CONGRATULATIONS, TRAINER!" << endl;
        cout << "=================================================" << endl;
        cout << "  You have completed the Pokedex!" << endl;
        cout << "  All 5 Pokemon have been caught." << endl << endl;
        cout << "  Thank you for playing Pokedex Collector." << endl;
        return true;
    }
};
/*  ============================================================================
    main() - the program's starting point
     ============================================================================
 */
 int main() {
    // Seed the random number generator with the current time (from <ctime>)
    // so the encounters are different each time the game is played.
    srand(time(0));
    Game game;   // create the game
    game.run();  // start it
    return 0;    // tell the operating system everything finished OK
}