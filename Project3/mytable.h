// Project Identifier: C0F4DFE8B340D81183C208F70F9D2D797908754D

#ifndef MYTABLE_H
#define MYTABLE_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <queue>
#include "TableEntry.h"

using namespace std;

class my_table 
{
    private:
        unordered_map<TableEntry, vector<int>> _hash;
        map<TableEntry, vector<int>> _bst;
        size_t hashed_col_index;
        unordered_map<string, size_t> colname_map;
        vector<EntryType> types;
        vector<vector<TableEntry>> table;
        char hash_or_bst; // 'n' for no index, 'h' for hash, 'b' for bst

        friend class my_database;
    
    public:
        void myInsert(const string & table_name, size_t N);

        void myPrint(const string & table_name, bool quiet);
        size_t printConditional(char op, size_t col_index, TableEntry value, const vector<size_t> & indices, bool quiet);

        void myDelete(const string & table_name);
        size_t deleteOp(char op, size_t col_index, TableEntry val);

        void myGenerate(const string & table_name);
        
        void myJoin(my_table* mt2, const string & table_name1, const string & table_name2, bool quiet);

        // check whehter col_name is in table
        unordered_map<string, size_t>::iterator checkColumnName(const string & col_name, const string & table_name, const string & command)
        {
            auto it = colname_map.find(col_name);
            if (it == colname_map.end())
            {
                cout << "Error during " << command << ": " << col_name << " does not name a column in " << table_name << "\n";
    
                string tmp;
                getline(cin, tmp);
            }
            return it;
        }
};

class my_database
{
    unordered_map<string, my_table*> database;

    public:
        void create()
        {
            string table_name;
            cin >> table_name;
            if (database.find(table_name) != database.end())
            {
                cout << "Error during CREATE: Cannot create already existing table " << table_name << "\n";
                string tmp;
                getline(cin,tmp);
                return;
            }
            my_table* mt = new my_table;
            database[table_name] = mt;
            size_t N;
            cin >> N;
            mt->types.resize(N);
            mt->hash_or_bst = 'n';
            for (size_t i=0; i<N; i++)
            {
                string type;
                cin >> type;
                switch (type[0])
                {
                    case 's':
                        mt->types[i] = (EntryType::String);
                        break;
                    case 'd':
                        mt->types[i] = (EntryType::Double);
                        break;
                    case 'i':
                        mt->types[i] = (EntryType::Int);
                        break;
                    case 'b':
                        mt->types[i] = (EntryType::Bool);
                        break;
                    default:
                        break;
                }
            }

            mt->colname_map.reserve(N);
            cout << "New table " << table_name << " with column(s) ";
            for (size_t i=0; i<N; i++)
            {
                string name;
                cin >> name;
                mt->colname_map[name] = i;
                cout << name << " ";
            }
            cout << "created\n";
        }

        void remove()
        {
            string table_name;
            cin >> table_name;
            my_table* mt = fetchTable(table_name, "REMOVE");
            if (!mt) return;

            delete mt;
            database.erase(table_name);
            cout << "Table " << table_name << " deleted\n";
        }

        // check whether table_name is in database (not for CREATE)
        // If so, return a pointer to the table; otherwise return nullptr        
        my_table* fetchTable(const string & table_name, const string & command)
        {
            if (database.find(table_name) == database.end())
            {
                cout << "Error during " << command << ": " << table_name << " does not name a table in the database\n";
                if (command != "REMOVE")
                {
                    string tmp;
                    getline(cin, tmp);
                }
                return nullptr;
            }
            return database[table_name];
        }

        void clearTables()
        {
            for (auto it=database.begin(); it!=database.end(); it++)
                delete it->second;
        }
};


#endif