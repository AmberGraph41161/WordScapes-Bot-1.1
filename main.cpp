#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <limits>
#include <algorithm>
#include <vector>
#include <chrono>
#include <conio.h>

int factorial(int wordlength)
{
    int fact = 1;
    for(int x = 1; x <= wordlength; ++x)
    {
        fact *= x;
    }
    return fact;
}

void checkfiles()
{
	std::fstream engmix;
	std::fstream a_z;
	//check for missing files
	bool leave = false;
	engmix.open("resources\\engmix.txt", std::fstream::in);
	if(engmix.fail())
	{
		std::cerr << "failed to open \"resources\\engmix.txt\"" << std::endl;
		leave = true;
	}
	//97 = 'a'
	//122 = 'z'
	char letter = 97;
	while(true)
    {
        if(letter > 122)
        {
            break;
        }
        std::string tempstring(1, letter);
        std::string filelocation = "resources\\output\\" + tempstring + ".txt";
        a_z.open(filelocation.c_str(), std::fstream::in);
        if(a_z.fail())
        {
            leave = true;
            std::cerr << "failed to open \"" << filelocation << "\"" << std::endl;
        }
        letter++;
        a_z.close();
    }
	if(leave)
	{
		std::cerr << "\nABORTING PROGRAM\n" << std::endl;
		exit(EXIT_FAILURE);
	}
}

bool isword(std::string word, int minlength)
{
	checkfiles();
	std::fstream a_z;

	//open correct letter file by taking first letter of "word"
	char letter = word[0];
    std::string tempstring(1, letter);
    std::string filelocation = "resources\\output\\" + tempstring + ".txt";
    a_z.open(filelocation.c_str(), std::fstream::in);

    //check if is word
	std::string temp;
	while(!a_z.eof())
	{
		std::getline(a_z, temp);
		if(temp == word)
		{
		    if(temp.length() >= minlength)
            {
                a_z.close();
                return true;
            }
		}
	}
	a_z.close();
	return false;
}


int main()
{
	//checkfiles();
	//getting rid of checkfiles() for speed purposes

	//vars and objects
	int limiter = 0;
	int iteration = 1;
	std::string letters = "";
	std::vector<std::string> usedwords(0);

	//actually important stuff
	while(true)
    {
        std::cout << "Please Enter Letters" << std::endl;
        std::cout << "(no spaces)" << std::endl;
        std::cout << ">";
        std::getline(std::cin, letters);
        if(letters == "\\EXIT")
        {
            break;
        }
        std::cout << "\n" << "Length Limiter" << std::endl;
        std::cout << "(minimum word length)" << std::endl;
        std::cout << ">";
        std::cin >> std::setw(letters.length()) >> limiter;
        std::cout << "\n";
        while(limiter > letters.length() || std::cin.fail())
        {
            if(std::cin.fail())
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << " | Bad input" << std::endl;
            } else
            {
                std::cout << " | Limiter cannot be greater than amount of letters provided" << std::endl;
            }
            std::cout << " | please try again..." << std::endl;

            std::cout << "\n" << "Length Limiter" << std::endl;
            std::cout << "(minimum word length)" << std::endl;
            std::cout << ">";
            std::cin >> std::setw(letters.length()) >> limiter;
            std::cout << "\n";
        }

        //calculate max amount of iterations it might take
        std::cout << "max possible iterations: " << factorial(letters.length()) << std::endl;


        //permutation stuff
            //timer
            std::chrono::time_point<std::chrono::system_clock> START, END;
            START = std::chrono::high_resolution_clock::now();
        std::sort(letters.begin(), letters.end());
        do
        {
            for(int x = 0; x < letters.length(); x++)
            {
                std::string temp = letters.substr(0, x + 1);
                //starting from smaller words to larger is a bit more efficient perhaps?
                //std::string temp = letters.substr(0, letters.length() - x);
                if(isword(temp, limiter))
                {
                    bool alreadyused = false;
                    for(int x = 0; x < usedwords.size(); x++)
                    {
                        if(usedwords[x] == temp)
                        {
                            alreadyused = true;
                            break;
                        }
                    }
                    if(!alreadyused)
                    {
                        usedwords.push_back(temp);
                        std::cout << iteration << " iteration done __ " << temp << std::endl;
                        iteration++;
                    }
                }
            }
        }while(std::next_permutation(letters.begin(), letters.end()));
            //timer
            END = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> duration = END - START;
            std::cout << "operation completed in " << duration.count() << " seconds" << std::endl;

        //end
        std::cout << "___________________________________" << std::endl;
        std::cout << "\n" << "you have reached the end" << "\n" << std::endl;
        std::cout << "please press any key to start over" << std::endl;
        std::cout << "___________________________________" << std::endl;
        getch();
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        //reset vars before starting over
        limiter = 0;
        iteration = 1;
        letters = "";
        usedwords.clear();

        std::cout << "\n" << std::endl;
    }

    std::cout << "exiting program..." << std::endl;
    exit(EXIT_SUCCESS);
}
