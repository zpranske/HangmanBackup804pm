#include <iostream>
#include <fstream>
#include <map>
#include <set>
#include <string>
#include <cstdlib>
#include <ctime>
#include <vector>

using namespace std;

const int LIVES = 7;

void loadDict(map<int,set<string>>& dict, ifstream& in){
    string temp;
    while(in >> temp){
        dict[temp.length()].insert(temp);
    }
}

template <typename T>
void printDict(map<T,set<string>>& dict, ostream& out) {
    for (auto i = dict.begin(); i != dict.end(); i++) {
        for ( auto j = i->second.begin(); j != i->second.end(); j++ ){
            out << *j << endl;
        }
    }
}

void printDict(map<int,set<string>>& dict, int n, ostream& out) {
    for (map<int,set<string>>::iterator i = dict.begin(); i != dict.end(); i++) {
        if (i->first == n) {
            for ( set<string>::iterator itr = i->second.begin(); itr != i->second.end(); ++itr ){
                out << *itr << endl;
            }
        }
    }
}

void getDictInfo(map<int,set<string>>& dict, int& min, int& max) {
    map<int,set<string>>::iterator i;
    i = dict.begin();
    min = i->first;
    i = --dict.end();
    max = i->first;
}

void loadSubdict(map<int,set<string>>& src, set<string>& dest, int n) {
    for (auto i = src.begin(); i != src.end(); i++) {
        if (i->first == n) {
            for ( auto j = i->second.begin(); j != i->second.end(); j++ ){
                dest.insert(*j);
            }
        }
    }
}

char getGuess(vector<char>& guesses, int& numGuesses) {
    char c = '\0';
    cout << "Guess a letter: ";
    while (!(islower(c))) {
        cin >> c;
        if (isupper(c))
            c = tolower(c);
        if (!islower(c))
            cout << "Please enter a letter, not just some random character you moron\n";
    }
    guesses.push_back(c);
    numGuesses++;
    return c;
}

void print(set<string> s, ostream& out) {
    for (auto i = s.begin(); i != s.end(); i++) {
        out << *i << " ";
    }
    out << endl;
}

template <typename T>
void print(vector<T> s, ostream& out) {
    for (auto i = s.begin(); i != s.end(); i++) {
        out << *i << " ";
    }
    out << endl;
}

void printSpaces(string word, vector<int> guessLocations, ostream& out) {
    out << endl;
    for (int i = 0; i < word.length(); i++) {
        bool isIn = false;
        for (auto j = guessLocations.begin(); j != guessLocations.end(); ++j) {
            if (i == *j)  isIn = true;
        }
        isIn? out << word[i] << " " : out << "_ ";
    }
    out << endl;
}

bool updateDict(set<string>& subdict, vector<char>& guesses, char curGuess) {
    int length = subdict.begin()->length(); //Delete if included in class
    string lastword;

    for (auto i = subdict.begin(); i != subdict.end(); i++) {
        string word = *i;
        cout << "word = " << word << " ";
        vector<int> guessLocations;
        for (int k = 0; k < word.length(); k++) {
            if (word[k] == curGuess) guessLocations.push_back(k);
        }
        for (auto a = guessLocations.begin(); a != guessLocations.end(); a++) {
            cout << *a << " ";
        }
        //If there are any words associated with empty vectors, we can make those
        //our new list. Otherwise we have to revert to normal hangman as all words
        //Will have been removed from subdict. Use last word encountered for normal hangman
        bool evil = guessLocations.empty();
        evil? cout << "Evil " : cout << "Not evil";
        if (!evil) {
            subdict.erase(i);
            lastword = *i;
        }
        cout << endl;
    }
    if(subdict.empty()) {
        subdict.insert(lastword);
        cout << "No more evil words: play hangman with " << lastword << endl;
    }
    else {
        cout << "New dict with words not containing " << curGuess << ":\n";
        for (auto i = subdict.begin(); i!=subdict.end(); i++) {
            string word = *i;
            cout << word << " ";
        }
        cout << endl << endl;
    }
}

bool isLastWord(set<string>& subdict) {
    return subdict.size() == 1;
}

void playRegularHangman(set<string>& subdict, vector<char>& guesses, vector<int> guessLocations) {
    auto i = subdict.begin();
    string word = *i;
    for (auto j = guesses.begin(); j != guesses.end(); ++j) {
        for (int k = 0; k < word.length(); k++) {
            if (word[k] == *j) {
                guessLocations.push_back(k);
            }
        }
    }
        //print(guessLocations, cout);
    printSpaces(word, guessLocations, cout);
}

bool done(set<string>& subdict, vector<int>& locations) {
    cout << "In bool done\n";
    auto i = subdict.begin();
    string word = *i;
    cout << "gl size = " << locations.size() << endl;
    cout << "word length = " << word.length() << endl;
    if (locations.size() == word.length()) return true;
    return false;
}

int main() {
    ifstream in;
    in.open("dict.txt");

    map<int,set<string>> dict;
    loadDict(dict, in);
    printDict(dict, 21, cout);

    int minSize = 0, maxSize = 0;
    getDictInfo(dict, minSize, maxSize);
    cout << minSize << " " << maxSize << endl;

    int wordLength = 0;
    bool valid = false;
    cout << "Pick a word length: \n";
    while (!valid) {
        cin >> wordLength;
        if (wordLength >= minSize && wordLength <= maxSize) valid = true;
        else {
            cout << "No words of that length exist. Please choose a length between ";
            cout << minSize << " and " << maxSize << ":\n";
        }
    }

    set<string> subdict1;
    loadSubdict(dict, subdict1, wordLength);
    print(subdict1, cout);
    cout << endl << endl;

    vector<char> guesses;
    int lives = LIVES;
    int numGuesses = 0;
    char currentGuess;
    vector<int> guessLocations;

    while (lives > 0) {
        //updateScreen();
        while (!isLastWord(subdict1)) {
            currentGuess = getGuess(guesses, numGuesses);
            updateDict(subdict1, guesses,currentGuess);
            lives--;
        }
        cout << "Number of guesses so far: " << numGuesses << endl;
        cout << "Letters guessed: ";
        for (auto i = guesses.begin(); i != guesses.end(); i++) {
            cout << *i << " ";
        }

        while (!done(subdict1, guessLocations)) {
            cout << "Looping here\n";
            currentGuess = getGuess(guesses, numGuesses);
            guesses.push_back(currentGuess);
            print(guesses, cout);
            playRegularHangman(subdict1, guesses, guessLocations);
            lives--;
        }

        if (guesses.size() == 0) {
            //print win message
            //exit to menu or whatever
        }
    }
    return 0;
}