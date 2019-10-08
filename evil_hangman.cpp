#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <cstdlib>
#include <random>
#include <ctime>


using namespace std;

void print_word(const string &word_so_far);
void print_hangman(const int guess_count);
void update_word
(const char guess, string &word_so_far, vector<string> &dictionary, int &guess_count);
void createDict(const string Filename, vector<string> &dict, int wordLen);


int main(int argc, char **argv) {
  srand((unsigned int)time(0));
  unsigned int incorrect_guesses = 0;
  unsigned int word_length = 0;
  bool won = false;

  cout << "Word length? ";
  cin >> word_length;

  vector<string> dictionary; 
  createDict("dictionary.txt", dictionary, word_length); 

  string word_so_far;
  word_so_far.resize(word_length, '_');


  int guesses = 0;
  while ( won == false ) {
    //Display Information to Player
    cout << "Guesses remaining: " << 7-guesses << endl;
    char guessed_chars[8];
    cout << "Chars guessed: ";
    for ( int i = 0; i < guesses; i++) { cout << guessed_chars[i]; }
    cout << endl;

    //Get the player's guess
    char guess_char;
    cout << "Guess letter: ";   
    cin >> guess_char;
    guessed_chars[guesses] = guess_char;

    guesses++;

    vector<string> words_to_prune; 

    // Remove all the words that contain the guessed character
    for (int i = 0; i < dictionary.size(); i++) {
      string word = dictionary[i];
      char *cstr_word = new char [ word.length()+1 ];
      strcpy(cstr_word, word.c_str());
      
      for ( int j = 0; j < word.length()+1; j++ ) {
        if ( guess_char == cstr_word[j] ) {
          //cout << "Remove: " << word << endl;
          words_to_prune.push_back(word);
          break; // We got a match, no need to keep checking.
        }
      }
    }
    // If the elements to remove list is smaller than the dict list:
    //    Remove the elements.
    //    else
    //    Finally choose a word
    if ( words_to_prune.size() < dictionary.size() ) {
      // Remove elements from dictionary that contain the guessed char
      vector<string> tmp_dict;
      set_difference(
          dictionary.begin(), dictionary.end(), 
          words_to_prune.begin(), words_to_prune.end(),
        inserter(tmp_dict, tmp_dict.begin()));
      dictionary = tmp_dict;

      // Print the hangman
      print_hangman(guesses);
      // Print the word so far
      print_word(word_so_far);
    } else {

      //cout << "Looks like we have to pick a word..." << endl;

      // Choose the word with the most unique characters.
      update_word(guess_char, word_so_far, dictionary, guesses); 

      // Print the hangman
      print_hangman(guesses);
      // Print the word so far
      print_word(word_so_far);
    }


    unsigned int correct_count = 0;
    for ( int i = 0; i < word_so_far.length(); i++ ) {
      if ( word_so_far[i] != '_' ) {
        correct_count++;
      }
    }

    if ( guesses >= 7 ) { 
      cout << "\033[1;31mYou lose. Better luck next time...\033[0m\n" << endl;
      break; 
    }
    else if ( correct_count == word_length && guesses < 7 ) {
      cout << "\033[1;32mYou won! Nothing beats a nice fair game of hangman...\033[0m\n" << endl;
      break;
    
    }

  }
  
  getchar();
  return 0;
}

void createDict(const string Filename, vector<string> &dict, int wordLen) {
  ifstream dictFile;
  dictFile.open("dictionary.txt", fstream::in);

  string newWord;
  getline(dictFile, newWord); 
  while ( !dictFile.eof() ) { 
    //cout << newWord.length()-1 << ": " << newWord << endl;
    if (newWord.length()-1 == wordLen) {
      //cout << newWord << endl;
      dict.push_back(newWord);
    }
    getline(dictFile, newWord);
  }


  return;
}

// Printing a space between each letter so that it is easier to read
void print_word(const string &word_so_far) {
  cout << "Word so far: ";

  for (int i = 0; i < word_so_far.size(); i++) {
    if ('_' == word_so_far[i]) {
      cout << "__ ";
    }
    else {
      cout << word_so_far[i] << ' ';
    }
  }

  cout << endl;

  return;
}

void print_hangman(const int guess_count) {
  char hangman[][7] = {
    {' ', ' ', '_', '_', '_', ' ', '\0'},
    {' ', '|', ' ', ' ', '|', ' ', '\0'},
    {' ', '|', ' ', ' ', ' ', ' ', '\0'},
    {' ', '|', ' ', ' ', ' ', ' ', '\0'},
    {' ', '|', ' ', ' ', ' ', ' ', '\0'},
    {' ', '|', ' ', ' ', ' ', ' ', '\0'},
    {'_', '|', '_', ' ', ' ', ' ', '\0'}
  };

  if (guess_count >= 1) {
    // Add head
    hangman[2][4] = 'O';
  }
  if (guess_count >= 2) {
    // Add neck
    hangman[3][4] = '|';
  }
  if (guess_count >= 3) {
    // Add left arm
    hangman[3][3] = '\\';
  }
  if (guess_count >= 4) {
      // Add right arm
      hangman[3][5] = '/';
  }
  if (guess_count >= 5) {
      // Add torso
      hangman[4][4] = '|';
  }
  if (guess_count >= 6) {
      // Add left leg
      hangman[5][3] = '/';
  }
  if (guess_count >= 7) {
      // Add right leg
      hangman[5][5] = '\\';
  }

  for (int y = 0; y < 7; y++) {
    cout << hangman[y] << endl;
  }

  return;
}

// Only called if we have to give them a letter, so time to choose a word
void update_word
(const char guess, string &word_so_far, vector<string> &dictionary, int &guess_count) {
  // Not choosing the first one or the last one, but a random one in the
  // middle.
  // random_number = random.randint(1, len(dictionary) - 1)
  //int random_number = rand() % (dictionary.size() - 2) + 1;
  //string random_word = dictionary[random_number];
  int *unique_counts = new int [ dictionary.size() ];
  for (int i = 0; i < dictionary.size(); i++) {
    string word = dictionary[i];
    char *cstr_checkMe = new char [ word.length()+1 ];
    strcpy(cstr_checkMe, word.c_str());

    vector<char> unique_chars;
    unique_chars.push_back(cstr_checkMe[0]);
    for ( int j = 0; j < word.length()+1; j++ ) {
      bool char_match = false;
      for ( int k = 0; k < word.length()+1; k++) {
        if ( cstr_checkMe[j] == unique_chars[k] ) {
          char_match = true;
        }
      }
      if ( !char_match ) { 
          unique_chars.push_back(cstr_checkMe[j]);
      }
    } 
    unique_counts[i] = unique_chars.size()-1;
    //cout << unique_chars.size()-1 << "-" << word << endl; 

  }
  int max = 0;
  int ind = 0;
  for ( int i = 0; i < dictionary.size(); i++ ) {
    if ( unique_counts[i] > max ) { max = unique_counts[i]; ind = i; } 
  }
  string tmp_word = dictionary[ind];
  dictionary.clear();
  dictionary.push_back(tmp_word);

  for (int i = 0; i < dictionary.size(); i++) {
    string word = dictionary[i];
    //cout << "  " << word << endl;
  }

  //cout << dictionary.size() << endl;


  bool guess_in_word = false;
  for (int i = 0; i < tmp_word.length(); i++) {
      if (guess == tmp_word[i]) {
          word_so_far[i] = guess;
          guess_in_word = true;
      }
  }
  if ( guess_in_word ) { guess_count--; }

  return;
}
