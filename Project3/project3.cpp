// Project Identifier: C0F4DFE8B340D81183C208F70F9D2D797908754D

#include "TableEntry.h"
#include "mytable.h"

int main(int argc, char* argv[])
{
    ios_base::sync_with_stdio(false); // you should already have this
    cin >> std::boolalpha;  // add these two lines
    cout << std::boolalpha; // add these two lines

    bool quiet_mode = false;
    if (argc == 3)
    {
        cout << "This is a silly database.\n";
        return 0;
    }
    else if (argc == 2)
    {
        string tmp(argv[1]);
        if (tmp == "-h" || tmp == "--help")
        {
            cout << "This is a silly database.\n";
            return 0;
        }
        quiet_mode = true;
    }

    my_database database;
    do
    {
        cout << "% "; // prompt

        string command;
        cin >> command;
        if (command == "QUIT")
        {
            cout << "Thanks for being silly!\n";
            break;
        }
        else if (command[0] == '#') // comment
        {
            string tmp;
            getline(cin, tmp);
        }
        else if (command == "CREATE")
            database.create();
        else if (command == "REMOVE")
            database.remove();
        else if (command == "INSERT")
        {
            string useless;
            string table_name;
            cin >> useless;
            cin >> table_name;
            size_t N;
            cin >> N;
            cin >> useless;
            my_table* mt = database.fetchTable(table_name, "INSERT");
            if (!mt) continue;

            mt->myInsert(table_name, N);
        }
        else if (command == "PRINT")
        {
            string table_name;
            cin >> table_name;
            cin >> table_name;
            my_table* mt = database.fetchTable(table_name, "PRINT");
            if (!mt) continue;

            mt->myPrint(table_name, quiet_mode);
        }
        else if (command == "DELETE")
        {
            string table_name;
            cin >> table_name;
            cin >> table_name;
            my_table* mt = database.fetchTable(table_name, "DELETE");
            if (!mt) continue;

            mt->myDelete(table_name);
        }
        else if (command == "GENERATE")
        {
            string table_name;
            cin >> table_name;
            cin >> table_name;
            my_table* mt = database.fetchTable(table_name, "GENERATE");
            if (!mt) continue;

            mt->myGenerate(table_name);
        }
        else if (command == "JOIN")
        {
            string table_name1;
            cin >> table_name1;
            my_table* mt1 = database.fetchTable(table_name1, "JOIN");
            if (!mt1) continue;

            string table_name2;
            cin >> table_name2;
            cin >> table_name2;
            my_table* mt2 = database.fetchTable(table_name2, "JOIN");
            if (!mt2) continue;

            mt1->myJoin(mt2, table_name1, table_name2, quiet_mode);
        }
        else
        {
            cout << "Error: unrecognized command\n";
            string tmp;
            getline(cin, tmp);
        }
    } while (true);

    database.clearTables();
    
    return 0;    
}