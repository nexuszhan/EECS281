// Project Identifier: B99292359FFD910ED13A7E6C7F9705B8742F0D79
#include <iostream>
#include <vector>
#include <deque>
#include <getopt.h>
#include <string>
#include <cctype>

using namespace std;

struct coordinate
{
    int room;
    int row;
    int col;
};

class needHelp {};
class unspecifiedScheme {};
class doublespecifiedScheme {};
class unknowChar {};
class invalidRoom {};
class invalidRow {};
class invalidCol {};

// Read the command line
string getMode(int argc, char* argv[])
{
    int choice;
    int option_index = 0;
    option long_options[] = 
    {
        { "stack", no_argument, nullptr, 's' },
        { "queue", no_argument, nullptr, 'q' },
        { "output", required_argument, nullptr, 'o'},
        { "help",  no_argument, nullptr, 'h' },
        { nullptr, 0, nullptr, '\0' }
    };

    string scheme="";
    string out_mode="M";
    while ((choice = getopt_long(argc, argv, "sqo:h", long_options, &option_index)) != -1)
    {
        switch (choice)
        {
            case 's':
                scheme += "s";
                break;

            case 'q':
                scheme += "q";
                break;

            case 'o':
                out_mode = optarg;
                break;

            case 'h':
                throw needHelp();
        } 
    }

    if (scheme.length() == 0)
        throw unspecifiedScheme();
    if (scheme.length() == 2)
        throw doublespecifiedScheme();
    
    return (scheme+out_mode);
}

// Read map from stdin
void getMap(const char & input_mode, const int & rooms, const int & length, 
            vector<vector<vector<char>>> & map, coordinate & start,coordinate & end)
{
    string map_line="";
    if (input_mode == 'M')
    {
        int room_cnt = 0;
        int row = 0; // Current room and row
        while (getline(cin,map_line))
        {
            if (map_line.length()>2)
            {
                if (map_line[0] == '/' && map_line[1] == '/')
                    continue;
            }
            
            for (int i=0; i<length; i++)
            {
                char ch = map_line[i];
                
                if (ch=='.' || ch=='#' || ch=='!' || ch=='S' || ch=='C' || isdigit(ch))
                {
                    map[room_cnt][row][i] = ch;
                    if (ch == 'S')
                    {
                        start.room = room_cnt;
                        start.row = row;
                        start.col = i;
                    }
                    if (ch == 'C')
                    {
                        end.room = room_cnt;
                        end.row = row;
                        end.col = i;
                    }
                }
                else
                    throw unknowChar();
            }
            row++;
            if (row == length)
            {
                room_cnt++;
                row = 0;
            }
        }
    }
    else if (input_mode == 'L')
    {
        while (getline(cin,map_line))
        {
            if (map_line.length() > 2)
            {
                if (map_line[0] == '/' && map_line[1] == '/')
                    continue;
            }
            
            size_t comma1 = map_line.find(',');
            int room = atoi(map_line.substr(1,comma1-1).c_str());
            if (room < 0 || room >= rooms)
                throw invalidRoom();
            
            size_t comma2 = map_line.find(',',comma1+1);
            int row = atoi(map_line.substr(comma1+1,comma2-comma1-1).c_str());
            if (row < 0 || row >= length)
                throw invalidRow();
            
            size_t comma3 = map_line.find(',',comma2+1);
            int col = atoi(map_line.substr(comma2+1,comma3-comma2-1).c_str());
            if (col < 0 || col >= length)
                throw invalidCol();
            
            char ch = map_line[comma3+1];
            if (ch=='.' || ch=='#' || ch=='!' || ch=='S' || ch=='C' || isdigit(ch))
            {
                map[room][row][col] = ch;
                if (ch == 'S')
                {
                    start.room = room;
                    start.row = row;
                    start.col = col;
                }
                if (ch == 'C')
                {
                    end.room = room;
                    end.row = row;
                    end.col = col;
                }
            }                
            else
                throw unknowChar();
        }
    }
}

bool updateContainer(deque<coordinate> & search_container,const vector<vector<vector<char>>> & map,
                    vector<vector<vector<char>>> & last_tiles, 
                    const int & room, const int & row, const int & col, int & discovered_tiles, 
                    const char & dir)
{
    char ch = map[room][row][col];
    if (ch=='.' || ch=='C' || isdigit(ch))
    {
        coordinate tmp = {room,row,col};
        search_container.push_back(tmp);

        last_tiles[room][row][col] = dir;

        discovered_tiles++;
        if (ch=='C')
            return true;
    }
    return false;
}

char convertRoomType(int room)
{
    if (room==0)
        return '0';
    else if (room==1)
        return '1';
    else if (room==2)
        return '2';
    else if (room==3)
        return '3';
    else if (room==4)
        return '4';
    else if (room==5)
        return '5';
    else if (room==6)
        return '6';
    else if (room==7)
        return '7';
    else if (room==8)
        return '8';
    else if (room==9)
        return '9';
    return 'a'; // not supposed to happen
}

void printMap(const int & rooms, const int & length, const vector<vector<vector<char>>> & map)
{
    for (int i=0; i<rooms; i++)
    {
        cout << "//castle room " << i << "\n";
        for (int j=0; j<length; j++)
        {
            for (int k=0; k<length; k++)
                cout << map[i][j][k];
            cout << "\n";
        }
    }
}

int main(int argc,char* argv[])
{
    ios_base::sync_with_stdio(false);

    string mode;
    try
    {
        // Handle command line
        mode = getMode(argc,argv); // "sM"or"sL"or"qM"or"qL"
    }
    catch(needHelp)
    {
        cout << "This program figures out a path from the given starting point to a destination.\n";
        return 0;
    }
    catch(unspecifiedScheme)
    {
        cerr << "Stack or queue must be specified\n" << flush;
        return 1;
    }
    catch(doublespecifiedScheme)
    {
        cerr << "Stack or queue can only be specified once\n" << flush;
        return 1;
    }
    
    
    // Read first three lines
    char input_mode;
    int rooms,length;
    cin >> input_mode >> rooms >> length;
    cin.get(); // eliminate the remaining '\n' in buffer

    vector<vector<vector<char>>> map(rooms,vector<vector<char>>(length,vector<char>(length,'.')));
    coordinate start,end;
    try
    {
        getMap(input_mode,rooms,length,map,start,end);
    }
    catch(unknowChar)
    {
        cerr << "Unknown map character\n" << flush;
        return 1;
    }
    catch(invalidRoom)
    {
        cerr << "Invalid room number\n" << flush;
        return 1;
    }
    catch(invalidRow)
    {
        cerr << "Invalid row number\n" << flush;
        return 1;
    }
    catch(invalidCol)
    {
        cerr << "Invalid column number\n" << flush;
        return 1;
    }

    deque<coordinate> search_container;
    search_container.push_back(start);

    // The coordination in last_tiles indicates the previous tile before the pos
    vector<vector<vector<char>>> last_tiles(rooms,vector<vector<char>>(length,vector<char>(length,'*')));
    last_tiles[start.room][start.row][start.col] = 'x';
    bool found = false;
    int discovered_tiles = 1;
    while (!search_container.empty())
    {
        coordinate prev;
        if (mode[0] == 'q')
        {
            prev = search_container.front();
            search_container.pop_front();
        }
        else
        {
            prev = search_container.back();
            search_container.pop_back();
        }
        int room = prev.room;
        int row = prev.row;
        int col = prev.col;

        if (isdigit(map[room][row][col]))
        {   // The position is a warp pipe
            int new_room = (map[room][row][col] - '0');
            if (new_room<rooms && last_tiles[new_room][row][col]=='*')
            {
                char prev_room = convertRoomType(room); // The exact room leads to the new_room
                if (updateContainer(search_container, map, last_tiles, new_room, 
                                    row, col, discovered_tiles, prev_room))
                {
                    found = true;
                    break;
                }
            }
        }
        else
        {
            // north
            if ((row-1)>=0 && last_tiles[room][row-1][col]=='*')
            {
                if (updateContainer(search_container, map, last_tiles, room, 
                                    row-1, col, discovered_tiles, 'n'))
                {
                    found = true;
                    break;
                }
            }

            // east
            if ((col+1)<=(length-1) && last_tiles[room][row][col+1]=='*')
            {
                if (updateContainer(search_container, map, last_tiles, room, 
                                    row, col+1, discovered_tiles, 'e'))
                {
                    found = true;
                    break;
                }
            }

            // south
            if ((row+1)<=(length-1) && last_tiles[room][row+1][col]=='*')
            {
                if (updateContainer(search_container, map, last_tiles, room, 
                                    row+1, col, discovered_tiles, 's'))
                {
                    found = true;
                    break;
                }
            }

            // west
            if ((col-1)>=0 && last_tiles[room][row][col-1]=='*')
            {
                if (updateContainer(search_container, map, last_tiles, room, 
                                    row, col-1, discovered_tiles, 'w'))
                {
                    found = true;
                    break;
                }
            }
        }
    }

    if (found)
    {
        deque<int> pipe_rooms; // Hold where a pipe leads to
        int cur_room = end.room;
        int cur_row = end.row;
        int cur_col = end.col;
        while (!(cur_room==start.room && cur_row==start.row && cur_col==start.col))
        {
            char last_dir = last_tiles[cur_room][cur_row][cur_col];
            if (isdigit(last_dir))
            {
                pipe_rooms.push_front(cur_room);
                cur_room = (last_dir-'0');
                map[cur_room][cur_row][cur_col] = 'p';
            }
            else
            {
                if (last_dir == 'n')
                    map[cur_room][++cur_row][cur_col] = 'n';
                else if (last_dir == 'e')
                    map[cur_room][cur_row][--cur_col] = 'e';
                else if (last_dir == 's')
                    map[cur_room][--cur_row][cur_col] = 's';
                else if (last_dir == 'w')
                    map[cur_room][cur_row][++cur_col] = 'w';
            }
        }
        
        if (mode[1]=='M')
        {
            cout << "Start in room " << start.room << ", row " << start.row 
                 << ", column " << start.col << "\n";
            printMap(rooms,length,map);
        }
        else
        {
            cout << "Path taken:\n";
            cur_room = start.room;
            cur_row = start.row;
            cur_col = start.col;
            while (!(cur_room==end.room && cur_row==end.row && cur_col==end.col))
            {
                char ch = map[cur_room][cur_row][cur_col];
                cout << "(" << cur_room << "," << cur_row << "," << cur_col << ","
                     << ch << ")\n";
                if (ch=='n')
                    cur_row--;
                else if (ch=='e')
                    cur_col++;
                else if (ch=='s')
                    cur_row++;
                else if (ch=='w')
                    cur_col--;
                else if (ch=='p')
                {
                    cur_room = pipe_rooms.front();
                    pipe_rooms.pop_front();
                }
            }
        }
    }
    else
        cout << "No solution, " << discovered_tiles << " tiles discovered.\n";

    return 0;
}