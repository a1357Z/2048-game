#include <iostream>
#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

/*
Notes:
    classes dependent on some class should be declared after the prerequiste classes (topoligical sorting)
    use -> when using this keyword in classes
    creating an object without any parameters, remove the brackets at the end --> Utility u;
    creating an object with parameters --> Game game_2048(4);
    when creating parametrized constructors make sure to create default constructors as well else you might get errors.

    testing the classes
    1. make the methods public
    2. do unit testing of the methods
    3. if u try testing the entire app in a go, then debuggin becomes very hard and time consuming
    4. try using the built in debugger with break points
*/

// tested utility
class Utility
{
public:
    Utility() {}
    int generateRandomNumber(int upperLimit)
    {
        ll t = rand();
        return t % (upperLimit + 1);
    }
};

class Board
{
private:
    vector<vector<int>> board;
    vector<pair<int, int>> availableCells;
    int size;
    Utility utility;

    void initialize()
    {
        // cout<<"running initialize"<<endl;
        int count = 2;
        while (count > 0 && checkGameState() != 0)
        {
            count--;
            pair<int, int> p = this->randomCellPicker();
            int row = p.first, col = p.second;
            board[row][col] = 2;
        }
    }

    bool valid(int idx)
    {
        return idx < size && idx > -1;
    }

public:
    void left()
    {
        for (int row = 0; row < board.size(); row++)
        {
            // use 2 ptrs first pointing to the available position and second pointing the pending number
            int avail = 0, pend = 0;

            // move all the zeros to the right
            for (; pend < size; pend++)
            {
                if (board[row][pend] != 0)
                {
                    int val = board[row][pend];
                    board[row][pend] = 0;
                    board[row][avail++] = val;
                }
            }

            avail = -1;
            pend = 0;
            // pending logic to add the numbers

            // add all the next contiguos equal numbers
            // if the last number matches with these numbers add the last number to it and update the available position
            // else increment available position and push in the available position

            for (; pend < size;)
            {
                int current = board[row][pend], count = 0;
                while (pend < size && board[row][pend] == current)
                {
                    count++;
                    board[row][pend++] = 0;
                }

                if (avail > -1 && board[row][avail] == current)
                {
                    board[row][avail] = (count + 1) * current;
                }
                else
                {
                    board[row][++avail] = count * current;
                }
            }

            updateAvailableCells();
        }
    }

    void right()
    {
        for (int row = board.size() - 1; row > -1; row--)
        {
            // use 2 ptrs first pointing to the available position and second pointing the pending number
            int avail = board.size() - 1, pend = board.size() - 1;

            // move all the zeros to the left
            for (; pend > -1; pend--)
            {
                if (board[row][pend] != 0)
                {
                    int val = board[row][pend];
                    board[row][pend] = 0;
                    board[row][avail--] = val;
                }
            }

            avail = board.size();
            pend = board.size() - 1;
            // pending logic to add the numbers

            for (; pend > -1;)
            {
                int current = board[row][pend], count = 0;
                while (pend > -1 && board[row][pend] == current)
                {
                    count++;
                    board[row][pend--] = 0;
                }

                if (avail < board.size() && board[row][avail] == current)
                {
                    board[row][avail] = (count + 1) * current;
                }
                else
                {
                    board[row][--avail] = count * current;
                }
            }

            updateAvailableCells();
        }
    }

    void top()
    {
        for (int col = 0; col < board.size(); col++)
        {
            // use 2 ptrs first pointing to the available position and second pointing the pending number
            int avail = 0, pend = 0;

            // move all the zeros to the bottom
            for (; pend < size; pend++)
            {
                if (board[pend][col] != 0)
                {
                    int val = board[pend][col];
                    board[pend][col] = 0;
                    board[avail++][col] = val;
                }
            }

            avail = -1;
            pend = 0;
            // pending logic to add the numbers

            for (; pend < size;)
            {
                int current = board[pend][col], count = 0;
                while (pend < size && board[pend][col] == current)
                {
                    count++;
                    board[pend++][col] = 0;
                }

                if (avail > -1 && board[avail][col] == current)
                {
                    board[avail][col] = (count + 1) * current;
                }
                else
                {
                    board[++avail][col] = count * current;
                }
            }

            updateAvailableCells();
        }
    }

    void bottom()
    {
        for (int col = 0; col < board.size(); col++)
        {
            // use 2 ptrs first pointing to the available position and second pointing the pending number
            int avail = board.size() - 1, pend = board.size() - 1;

            // move all the zeros to the top
            for (; pend > -1; pend--)
            {
                if (board[pend][col] != 0)
                {
                    int val = board[pend][col];
                    board[pend][col] = 0;
                    board[avail--][col] = val;
                }
            }

            avail = board.size();
            pend = board.size() - 1;
            // pending logic to add the numbers

            for (; pend > -1;)
            {
                int current = board[pend][col], count = 0;
                while (pend > -1 && board[pend][col] == current)
                {
                    count++;
                    board[pend--][col] = 0;
                }

                if (avail < board.size() && board[avail][col] == current)
                {
                    board[avail][col] = (count + 1) * current;
                }
                else
                {
                    board[--avail][col] = count * current;
                }
            }

            updateAvailableCells();
        }
    }

    // Board() : Board(4){}  //call the parametrized constructor with size 4

    Board() {}

    Board(int size)
    {
        board = vector<vector<int>>(size, vector<int>(size, 0));
        this->size = size;

        Utility u;
        this->utility = u;

        // initialize the rows and cols
        updateAvailableCells();
        initialize();
        printBoard();
    }

    void updateAvailableCells()
    {
        // cout<<"running updateAvailableCells"<<endl;
        // empty the cells first
        availableCells.clear();

        // update the available cells
        for (int row = 0; row < board.size(); row++)
        {
            for (int col = 0; col < board[0].size(); col++)
            {
                if (board[row][col] == 0)
                {
                    availableCells.push_back({row, col});
                }
            }
        }
    }

    // call this only if the game is not over
    pair<int, int> randomCellPicker()
    {

        int idx = utility.generateRandomNumber(availableCells.size() - 1);
        pair<int, int> p = availableCells[idx];
        int row = p.first, col = p.second;

        // cout<<"erasing : "<<row<<" "<<col<<endl;
        availableCells.erase(availableCells.begin() + idx);

        // cout<<"available cells: "<<endl;
        // for(auto [r,c] : availableCells){
        //     cout<<r<<" "<<c<<endl;
        // }
        return {row, col};
    }

    void printBoard()
    {
        cout << "printing board: " << endl;
        // cells with values 0 are considered empty
        for (int row = 0; row < board.size(); row++)
        {
            for (int col = 0; col < board[0].size(); col++)
            {
                if (board[row][col] == 0)
                {
                    cout << '0' << " ";
                }
                else
                {
                    cout << board[row][col] << " ";
                }
            }
            cout << endl;
        }
    }

    void setCellVal(int row, int col, int val)
    {
        board[row][col] = val;
    }

    int getVal(int row, int col)
    {
        return this->board[row][col];
    }

    int getSize()
    {
        return size;
    }

    int checkGameState()
    {
        // 0 --> game over, 1 --> use wins, 2 --> in progress
        int count = 0; // count of cells occupied
        bool won = false;
        for (int row = 0; row < this->getSize(); row++)
        {
            for (int col = 0; col < this->getSize(); col++)
            {
                if (this->getVal(row, col) != 0)
                {
                    count++;
                }
                if (this->getVal(row, col) == 2048)
                {
                    won = true;
                    break;
                }
            }
        }
        int size = this->getSize();
        if (won)
        {
            return 1;
        }
        else if (count == size * size)
        {
            return 0;
        }
        else
        {
            return 2;
        }
    }
};

class Game
{
private:
    Board board;
    Utility utility;
    vector<int> allowed = {2, 4, 2, 2, 2, 4, 2};

    // tested
    int generateNextNumber()
    {
        int idx = utility.generateRandomNumber(allowed.size() - 1);
        // cout<<"idx: "<<idx<<endl;
        return allowed[idx];
    }

    // not really needed for now
    void resetBoard()
    {
    }

public:
    Game() {}

    Game(int size)
    {
        Utility u;
        this->utility = u;
        // cout<<"utility constructor done"<<endl;
        Board b(size);

        // cout<<"board constructor done"<<endl;
        this->board = b;

        // cout<<"game constructor done"<<endl;
    }

    /*
   The user will make a move by entering a number.
       0 denotes left
       1 denotes right
       2 denotes top
       3 denotes bottom
   */
    void makeMove(int input)
    {
        if (input == 0)
        {
            board.left();
        }
        else if (input == 1)
        {
            board.right();
        }
        else if (input == 2)
        {
            board.top();
        }
        else if (input == 3)
        {
            board.bottom();
        }

        int state = board.checkGameState();
        if (state == 1)
        {
            // win
            cout << "player won" << endl;
            return;
        }

        // add a value to a random cell
        int val = generateNextNumber();
        pair<int, int> p = board.randomCellPicker();
        int r = p.first, c = p.second;

        board.setCellVal(r, c, val);
        // cout<<"setting "<<r<<", "<<c<<" : "<<val<<endl;
        // cout<<"new val: "<<board.getVal(r,c)<<endl;

        // print the board after making the move
        board.printBoard();

        board.updateAvailableCells();
        state = board.checkGameState();
        if (state == 0)
        {
            // game over
            cout << "game over" << endl;
            return;
        }
    }
};

int main()
{
    Game game_2048(4);

    game_2048.makeMove(0);
    game_2048.makeMove(1);
    game_2048.makeMove(2);
    game_2048.makeMove(3);
    game_2048.makeMove(0);
    game_2048.makeMove(1);
    game_2048.makeMove(2);
    game_2048.makeMove(3);
}