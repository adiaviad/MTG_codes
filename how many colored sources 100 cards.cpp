// how many colored sources 100 cards.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include<algorithm>
#include<chrono>
#include <iostream>
#include <random>
#include <algorithm>
using namespace std;
using namespace std::chrono;
enum index_names
{
    spell = 0,
    relevant_land = 1,
    irrelevant_land = 2,
    number_of_cards = 3,
    lands=4
};

void reset_data(int** data, int s);
void init_data(int** data, int s);
void print_array(int* a, int n);
void refilll_libary(int* library,int size, int number_of_lands, int relevant_land_number,int irrelevant_land_number);
void draw(int *hand, int* library);
void draw(int *hand, int* library, int number_of_cards);
void put_lands_on_bottom(int* hand, int lands);
void add_result_to_data(int** data, int turn, int* hand);
void print_and_store_data(int** data, int turns, int land_count, double csv[40][28], int csv_row);
void print_csv(double csv[40][28], int max_col, int max_row, int min_row);
int number_of_sumulations = 5 * pow(10, 6);
int main()
{   

    
    int min_lands = 10;
    int number_of_lands = 41;
    int max_turns = 7;
    int** data =(int**) malloc((sizeof(int*)) * max_turns);
    init_data(data, max_turns);
    double csv[40][28];

    int hand[5] = { 0,0,0,0,0 };
    int library[100];
    int library_size = (sizeof library) / (sizeof(int));
    for (number_of_lands = 35; number_of_lands < 45; number_of_lands++)
    {


        for (size_t relevant_land_number = min_lands; relevant_land_number < number_of_lands; relevant_land_number++)
        {


            int irrelevant_land_number = number_of_lands - relevant_land_number;
            reset_data(data, max_turns);

            for (size_t i = 0; i < number_of_sumulations; i++)
            {
                //one simulation
                {
                    bool hand_kept = false;
                    //mulligan 
                    for (int hand_size = 7; hand_size >= 4 && !hand_kept; hand_size--)
                    {
                        memset(hand, 0, sizeof(int) * 5);//reset hand
                        refilll_libary(library, library_size, number_of_lands, relevant_land_number, irrelevant_land_number);
                        draw(hand, library, 7);
                        if (2 <= hand[lands] && hand[lands] <= 5)
                            hand_kept = true;

                        if (hand_kept)
                        {
                            //if hand_size==7 no cards need to be removed from hand due to mulligan
                            if (hand_size == 6)//1 card needs to be put at the botom 
                            {
                                if (hand[spell] > 3) {
                                    hand[spell]--;
                                }
                                else
                                {
                                    put_lands_on_bottom(hand, 1);
                                }
                            }
                            else if (hand_size == 5)
                            {
                                if (hand[spell] > 3) {
                                    hand[spell] -= 2;
                                }
                                else if (hand[spell] == 3)
                                {
                                    put_lands_on_bottom(hand, 1);
                                    hand[spell] -= 1;
                                }
                                else
                                {
                                    put_lands_on_bottom(hand, 2);
                                }
                            }
                            else if (hand_size == 4)
                            {
                                if (hand[spell] > 3)
                                {
                                    hand[spell] -= 3;
                                }
                                else
                                {
                                    int spells = hand[spell];
                                    put_lands_on_bottom(hand, 4 - spells);
                                    hand[spell] -= spells - 1;
                                }
                            }
                        }
                    }
                    bool relevant = true;
                    for (size_t turns = 1; turns <= max_turns && relevant; turns++)
                    {
                        if (hand[lands] < turns) {
                            relevant = false;
                        }
                        else
                        {
                            add_result_to_data(data, turns, hand);
                            draw(hand, library);
                        }
                    }
                }

            }

            print_and_store_data(data, max_turns, relevant_land_number, csv, relevant_land_number - min_lands);

            cout << relevant_land_number << " lands done";
        }
        print_csv(csv, 28, number_of_lands, min_lands);
    }
}


void print_array(int* a, int n)
{
    printf("\n");
    for (size_t i = 0; i < n; i++)
    {
        printf("%d, ", a[i]);
    }
    printf("\n");

}

void shuffleArray(int arr[], int size) {
    static int seed = time(0);
    srand(seed);  // Seed the random number generator with current time

    for (int i = size - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        std::swap(arr[i], arr[j]);
    }
    seed = rand();
}
void refilll_libary(int* library, int library_size, int number_of_lands, int relevant_land_number, int irrelevant_land_number)
{
    int cards_in_library = 0;
    for (size_t i = 0; i < relevant_land_number; i++)
    {
        library[cards_in_library] = relevant_land;
        cards_in_library++;
    }
    for (size_t i = 0; i < irrelevant_land_number; i++)
    {
        library[cards_in_library] = irrelevant_land;
        cards_in_library++;
    }
    for (size_t i = 0; i < (library_size - number_of_lands); i++)
    {
        library[cards_in_library] = spell;
        cards_in_library++;
    }
    shuffleArray(library, library_size);

}


void draw(int* hand, int* library)
{
    
    int type_index = library[hand[number_of_cards]];
    hand[type_index]++;
    hand[number_of_cards]++;
    //the index of the lands is 1 or 2. so this if asks if the index is of a land
    if (type_index>0)
    {
        hand[lands]++;
    }
}
void draw(int* hand, int* library, int number_of_cards)
{
    
    for (size_t i = 0; i < number_of_cards; i++)
    {
        draw(hand, library);
    }
}

void put_lands_on_bottom(int* hand, int lands_to_bottom)
{
    int bottomable_irrevelevant_lands = min(hand[irrelevant_land], lands_to_bottom);
    hand[irrelevant_land] -= bottomable_irrevelevant_lands;

    int bottomable_revelevant_lands = min(hand[relevant_land], lands_to_bottom-bottomable_irrevelevant_lands);
    hand[relevant_land] -= bottomable_revelevant_lands;

    hand[lands] -= lands_to_bottom;
}
void add_result_to_data(int** data, int turn, int* hand)
{
    data[turn-1][0]++;//relevant games;
    for (size_t i = 0; i < turn; i++)
    {
        //for each possible mana cost on that turn check whether it can be cast.
        //for example for turn 3, CCC, 1CC, 2C is chekced
        if (hand[relevant_land] >= turn-i) 
        {
            data[turn - 1][i + 1]++;
        }
    }
}
void print_and_store_data(int** data, int turns, int land_count, double csv[40][28], int csv_row)
{
    int csv_col = 0;
    std::cout << "number of lands: " << land_count << endl;
    for (size_t row = 1; row <= turns; row++)
    {
       // cout << "turn " << row <<endl;
        for (size_t i = 0; i < row; i++)
        {
            double num;
            int relevant_game = data[row - 1][0];
            if (relevant_game == 0) {
                num = 0;
            }
            else
            {
                num = data[row - 1][i + 1] / (double)number_of_sumulations;
            }
            //cout << num << "\t";
            csv[csv_row][csv_col++] = num;
        }
        //cout << endl << endl;
    }
}
void print_csv(double csv[40][28], int max_col, int max_row,int min_row)
{
    cout << "\ncsv" << endl;
    for (size_t row = 0; row < max_row; row++)
    {
        cout << row + min_row <<",";
        for (size_t col = 0; col < max_col; col++)
        {
            cout << csv[row][col] << ",";
        }
        cout << endl;
    }
}
//row is index starts at 1
void print_row(int** data, int row, int** csv, int csv_col, int csv_row)
{
    //print mana costs
    /*for (size_t i = 0; i < row; i++)
    {
        int num = row - i;
        if (num != 0) {
            cout << num;
        }
        for (size_t j = 0; j < num; j++)
        {
            cout << "C";
        }
        cout << "\t"; 
    }
    cout << endl;*/
    //print precents
    for (size_t i = 0; i < row; i++)
    {
        double num;
        int relevant_game = data[row - 1][0];
        if (relevant_game == 0) {
            num = 0;
        }
        else
        {
            num = data[row - 1][i + 1]/(double)relevant_game;
        }
        cout << num << "\t";
        
    }
    cout << endl << endl;

}
void reset_data(int** data, int s)
{
    for (size_t i = 0; i < s; i++)
    {
        memset(data[i], 0, sizeof(int) * (i + 2));

    }
}
void init_data(int** data, int s)
{
    int size;
    for (size_t i = 0; i < s; i++)
    {
        size = sizeof(int) * (i + 2);
        data[i] = (int*)malloc(size);

    }
    reset_data(data, s);

}
// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
