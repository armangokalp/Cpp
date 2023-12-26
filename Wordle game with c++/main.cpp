//Made with Visual Studio 2019
//Arman Gokalp        00029398

#include <iostream>
#include <fstream>
#include <vector>
#include <string>


struct cell
{
    char letter;
    char color;

}; //stores the letter and color for the slots of the 2D matrix


std::string input(int& letter, int& attempt)
{
    std::string input_name, str, s;
    std::ifstream input;

    do
    {
        std::cout << "Please enter the file name: ";    std::cin >> input_name;
        input.open(input_name.c_str());

        if (input.fail())
        {
            std::cout << "Couldn't find the file!\n";
        }

    } while (input.fail()); //input check

    //takes the values from the file
    while (std::getline(input, str))
        if (isdigit(str[0]))  attempt = stoi(str);
        else     s = str;       letter = s.length();

    input.close();

    return s; //returns the secret word
}


char color(std::string letter, char input, int i)
{
    char result = '-';
    for (int k = 0; k < letter.size(); k++)
    {
        if (letter.at(k) == input)
        {
            if (k == i)         result = 'G'; // Both the word and place is correct
            else                result = 'Y'; // Word exists wrong place
        }
        if (result == '-')      result = 'B'; // Word doesnt exist
    }

    return result;

}   //matches the letters with corresponding colors depending on their accuracy


bool is_all_lowercaseLetters(std::string input)
{
    for (int i = 0; i < input.length(); i++)
        if (isupper(input.at(i)) || !isalpha(input.at(i)))  return false;

    return true;
}

bool no_repeated_words(std::string input)
{
    int offset = 1;

    for (int i = 0; i < input.length() - 1; i++)
    {
        for (int k = offset; k < input.length(); k++)
        {
            if (input.at(i) == input.at(k))     return false;
        }
        offset += 1;
    }

    return true;
}

bool word_is_tried_before(std::string input, std::vector<std::vector<cell> > _cell)
{
    int match_count = 0;

    for (int i = 0; i < _cell.size(); i++)
    {
        for (int k = 0; k < _cell[i].size(); k++)
        {
            if (input.at(k) == _cell[i][k].letter)
                match_count++;

            if (match_count == _cell[i].size())
                return true;
        }

        match_count = 0;
    }

    return false;
}

bool green_letter_is_changed(std::string input, std::vector<std::vector<cell> > _cell, int turn)
{
    if (turn != 0)
        for (int i = 0; i < input.length(); i++)
            if (_cell[turn - 1][i].color == 'G' && _cell[turn - 1][i].letter != input[i])
                return true;

    return false;
}


bool inputFail(std::string input, int letter, std::vector<std::vector<cell> > _cell, int turn)
{
    if (input.length() != letter)
    {
        if (input.length() > letter)   std::cout << "The word is too long!" << std::endl;
        else                           std::cout << "The word is too short!" << std::endl;

        return true;
    }
    else if (!is_all_lowercaseLetters(input))
    {
        std::cout << "Your input has illegal letters!" << std::endl;

        return true;
    }
    else if (!no_repeated_words(input))
    {
        std::cout << "Your input has a duplicate letter!" << std::endl;

        return true;
    }
    else if (word_is_tried_before(input, _cell))
    {
        /*if (green_letter_is_changed(input, _cell, turn))
        {
            std::cout << "You did not use the green letter in your word!" << std::endl;

            return true;
        } //this if statement is for in case of multiple tries for the green letter error inputs
        */
        std::cout << "You've already tried this word!" << std::endl;

        return true;
    }
    else if (green_letter_is_changed(input, _cell, turn))
    {
        std::cout << "You did not use the green letter in your word!" << std::endl;

        return true;
    }

    else return false;

}   //all the input checks for user guesses are done in this function


int main()
{
    int letter = 1, attempt = 6;

    std::cout << "Welcome to SWordle!\n";
    std::string secretWord = input(letter, attempt);  //takes user input opens the txt file and writes the information into the variables
    std::cout << "The word that you will guess has " << letter << " letters and you have " << attempt << " attempts.\n";

    //Creates a 2D matrix to represent user's guesses
    std::vector<std::vector<cell> > _cell(attempt);
    for (int i = 0; i < attempt; i++)
        _cell[i].assign(letter, cell{ '-', '-' });


    //The game starts taking user guesses
    std::string guess;  const int temp = attempt;

    while (guess != secretWord && attempt > 0)
    {
        int turn = temp - attempt;

        do
        {
            std::cout << "What's your guess? ";   std::cin >> guess;

        } while (inputFail(guess, letter, _cell, turn));  //The game warns the user until an appropriative input is entered

        //Puts the user guess into the 2D matrix
        for (int i = 0; i < letter; i++)
        {
            _cell[turn][i].letter = guess.at(i);
            _cell[turn][i].color = color(secretWord, guess.at(i), i);
        }

        //Displays the 2D matrix
        for (int i = 0; i < temp; i++)
        {
            for (int k = 0; k < letter; k++)
            {
                std::cout << "||" << _cell[i][k].letter << "," << _cell[i][k].color;
            }
            std::cout << "||\n";
        }

        attempt -= 1;

    } //loops until the user either finds the secret word or exceeds their attempt limit

    //Game ends
    if (guess != secretWord) std::cout << "You lose!" << std::endl;
    else                     std::cout << "You win!"  << std::endl;
}