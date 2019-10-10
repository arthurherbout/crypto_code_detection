//
//  CPPCorrection.cpp
//  CPPCorrection
//
//  Created by P1kachu on 15/09/14.
//  Copyright (c) 2014 P1kachu. All rights reserved.
//
//
//  Reviewed by:
//      - Lantare
//      - Nobody else.
//
//  Function prototypes will not be added in a different header file to make it easier,
//  but you should use headers in practice.

#include <iostream>
#include <cstdlib>
#define NUMBEROFELEMENTS(x)  (int)(sizeof (x) / sizeof (x[0])) //used to check how many elements are in an array for example

using namespace std;


//Function prototypes
void TriForce();
string Hello(string name);
int Addition(int a, int b);
int Subtraction(int a, int b);
int Multiplication(int a, int b);
int Division(int a, int b);
int rdm();
int rdmBounds(int min, int max);

string Reverse(string str);
bool logicGates(bool x, bool y);

int main()
{


    // Part One:


    // FIXME: Declare variables
    // Declare two variables: an integer named "age", and a string named "name" with corresponding values (your name and age)
    string name = "Stan";
    int age = 19;


    // FIXME: Print
    // Print the following sentence in the console "You are NAME and you are AGE years old !". Don't forget to add a newline at the end
    cout << "You are " << name << " and you are " << age << " years old !\n";

    // FiXME: Concatenation
    // Create a new string variable called "hello" which value is "Hello ". Add "name" at the end of "hello" (Concatenation) then print it
    string hello = "Hello ";
    hello += name;
    cout << hello << endl; //endl adds a newline

    // FIXME: Array
    // create a new string array called "shoppingList", with three elements of your choice. Create an int variable containing the number of
    // elements in "shoppingList" (using a function of the array/using the array)
    string shoppingList[3] = {"milk", "a Chevy Camaro", "a life"};
    int nbOfElements = NUMBEROFELEMENTS(shoppingList);

    // FIXME: For-loop - Integer
    // Create a simple for-loop for an integer "i" going from 1 to 10 that print the value of "i"
    for (int i = 1; i <= 10; i++) // when having only one line in a for-loop/if -statement/etc, {brackets} are optional
        cout << i << endl;

    // FIXME: For-loop - shoppingList
    // Create a for loop that iterate through "shoppingList" and prints each element.
    for (int j = 0; j < nbOfElements; j++)
        cout << shoppingList[j] << endl;

    // FIXME: Foreach-loop
    // Do the same with a foreach-loop.
    // Without using vectors, it's a pain in the ass apparently... So let's stay with the for-loop

    // FIXME: If-statement
    // Modify the first for-loop (with i from 1 to 10) such that it prints "(value of i) is even" when "i" is divisible
    // by 2 (You may want to learn more about "modulo" (%)). Else, print "(value of i) is odd".
    for (int i = 1; i <= 10; i++)
    {
        if (i%2 == 0)
            cout << i << " is even" << endl;
        else
            cout << i << " is odd" << endl;
    }
    // FIXME: Sum Up
    // Create a string variable called "element" with the value of your choice. Then create a for-loop that checks if "shoppingList" contains
    // "element". If yes, print "You have to buy (value of element) !", and stop the loop (search how to stop a loop).
    // If not, print "Nope, you don't need (value of "element")".
    string element = "a life";
    for (int i = 0; i < nbOfElements; i++)
    {
        if (element == shoppingList[i])
        {
            cout <<"You have to buy " << shoppingList[i] << " ! \n";
            break;
        }
        else
            cout << "Nope, you don't need " << shoppingList[i] << " ! \n";
    }




    // Part Two:


    // FIXME: Functions - Ascii
    // Create a function that returns nothing and which doesn't takes any parameter. It should just be named "TriForce"
    // and print the TriForce symbol (one triangle over two other ones, can be found on internet) with "TRIFORCE"
    // Don't forget to call the function !
    TriForce();


    // FIXME: Functions - One parameter
    // Create a function that takes a string as parameter and returns "Hello (value of string) !"
    cout << Hello("Stan");

    // FIXME: Functions - Multiple parameters
    // Create a function that takes two integers as parameters and returns the addition of these two.
    // You can do the same with multiplication, subtraction and division.
    cout << Addition(5, 12) << endl;;
    cout << Subtraction(5, 12) << endl;;
    cout << Multiplication(5, 12) << endl;;
    cout << Division(5, 12) << endl;; // returns 0 because "a" and "b" are Integers (no point)
    cout << Division(10, 2) << endl;;

    // FIXME: User entry
    // Create a string variable that takes what the user enter in the console as value. Then print "You entered (value of string)"
    string userInput;
    cin >> userInput;
    cout << "You entered " << userInput << endl;

    // FIXME: While loop
    // Create a while loop that takes a number and divides it by 2 until it is less than 3
    int number = 56;
    while (number > 3)
    {
        number /= 2; // <=> number = number / 2;
        cout << number << endl;
    }

    // FIXME: do-While loop
    // Do the same with a do-while loop
    int number2 = 86;
    do
    {
        number2 /= 2;
        cout << number2 << endl;
    } while (number2 > 3);

    // FIXME: Random generator
    // Create a function that returns a random number
    srand(time(NULL)); // Time used to seed the random
    cout << rdm();

    // FIXME: Random generator with bounds
    // Create another function that returns a random number between two bounds given as parameters.
    cout << rdmBounds(2, 26) << endl;
    cout << rdmBounds(2, 26) << endl;

    // FIXME: Multidimensional array
    // Create a two dimensional int array of 3 columns and 3 rows. Use 2 for-loops to add a random number
    // between 1 and 9 in each of the 9 rooms.
    // You may use one of the two previously created function.
    // Then print them such that they appear like this (with [x1,x9] being the 9 random integers):
    // {x1,x2,x3,}
    // {x4,x5,x6,}
    // {x7,x8,x9,}
    int multiArray[3][3];

    for (int i = 0; i < NUMBEROFELEMENTS(multiArray); i++)
        for (int j = 0; j < NUMBEROFELEMENTS(multiArray[i]); j++)
            multiArray[i][j] = rdmBounds(0, 10);


    for (int i = 0; i < NUMBEROFELEMENTS(multiArray); i++)
    {
        cout << "{";
        for (int j = 0; j < NUMBEROFELEMENTS(multiArray[i]); j++)
            cout << multiArray[i][j] << ", ";
        cout << "}\n";
    }

    //FIXME: Switch
    //Create a Switch that takes an integer "a" and return a sentence regarding the value of a
    //(Create 3 statements for 3 specific values and a default one)
    int a;
    cin >> a;
    switch(a){
    case 1:
        cout << "Num: 1\n";
        break;
    case 2:
        cout << "Num: 2\n";
        break;
    case 3:
        cout << "Num: 3\n";
        break;
    default:
        cout << "Number: " << a << endl;
        break;
    }

    //FIXME: logic Gates
    //Create 7 functions for each logic gates (And, Or, No, Nand, Nor).
    //Each function takes two booleans as parameters and returns the result of the logic gate.
    //(or You can do it with a switch and only one function)
    cout << "Logic gate: " << logicGates(true, false) << endl;

    //FIXME - Reverse
    //Create a function that reverse a string
    cout << Reverse("Hello Friend.") << endl;

    return 0;

}

void TriForce()
{
    cout << "   /\\  \n";
    cout << "  /__\\ \n";
    cout << " /\\  /\\  \n";
    cout << "/__\\/__\\ \n";
    cout << "TRIFORCE\n";
}

string Hello(string name)
{
    return "Hello " + name + " !\n";
}

int Addition(int a, int b)
{
    return a + b;
}

int Subtraction(int a, int b)
{
    return a - b;
}


int Multiplication(int a, int b)
{
    return a *b;
}


int Division(int a, int b)
{
    return a / b;
}


int rdm()
{
    return rand();
}


int rdmBounds(int min, int max)
{
    return min + (rand() % (max - min));
}

string Reverse(string str){
    string temp="";
    for(int i = str.length()-1; i >= 0; i--)
        temp += str[i];
    return temp;
}

bool logicGates(bool x, bool y){
    //And, Or, No, Nand, Nor,
    int i;
    cout << "Choose num: \n"
         << "0 - AND\n" << "1 - OR\n" << "2 - NO\n" << "3 - NAND\n"
         << "4 - NOR\n";
    cin >> i;

    switch(i){
    case 0://and
        if(x && y)
            return true;
        else
            return false;
        break;
    case 1://or
        if (x || y)
            return true;
        else
            return false;
        break;
    case 2://no
        if (x)
            return false;
        else
            return true;
        break;
    case 3://nand
        if(x && y)
            return false;
        else
            return true;
        break;
    case 4://nor
        if (x || y)
            return false;
        else
            return true;
        break;
    default:
        cout << "Wrong Value\n";
        break;
    }
}
