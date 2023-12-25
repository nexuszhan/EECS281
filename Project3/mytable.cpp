// Project Identifier: C0F4DFE8B340D81183C208F70F9D2D797908754D

#include "mytable.h"

void my_table::myInsert(const string & table_name, size_t N)
{
    size_t start = table.size();
    table.reserve(start + N);
    size_t col_num = colname_map.size();
    for (size_t i=0; i<N; i++)
    {
        vector<TableEntry> row;
        row.reserve(col_num);
        for (size_t j=0; j<col_num; j++)
        {
            switch (types[j])
            {
                case EntryType::String:
                {
                    string val;
                    cin >> val;
                    TableEntry value(val);
                    row.emplace_back(value);

                    if (j == hashed_col_index)
                    {
                        if (hash_or_bst == 'h') // hash
                            _hash[value].emplace_back(i+start);
                        else if (hash_or_bst == 'b') // bst
                            _bst[value].emplace_back(i+start);
                    }
                    break;
                }
                case EntryType::Double:
                {
                    double val;
                    cin >> val;
                    TableEntry value(val);
                    row.emplace_back(value);

                    if (j == hashed_col_index)
                    {
                        if (hash_or_bst == 'h') // hash
                            _hash[value].emplace_back(i+start);
                        else if (hash_or_bst == 'b') // bst
                            _bst[value].emplace_back(i+start);
                    }
                    break;
                }
                case EntryType::Int:
                {
                    int val;
                    cin >> val;
                    TableEntry value(val);
                    row.emplace_back(value);

                    if (j == hashed_col_index)
                    {
                        if (hash_or_bst == 'h') // hash
                            _hash[value].emplace_back(i+start);
                        else if (hash_or_bst == 'b') // bst
                            _bst[value].emplace_back(i+start);
                    }
                    break;
                }
                case EntryType::Bool:
                {
                    bool val;
                    cin >> val;
                    TableEntry value(val);
                    row.emplace_back(value);

                    if (j == hashed_col_index)
                    {
                        if (hash_or_bst == 'h') // hash
                            _hash[value].emplace_back(i+start);
                        else if (hash_or_bst == 'b') // bst
                            _bst[value].emplace_back(i+start);
                    }
                    break;
                }
                default:
                    break;
            }
        }
        table.emplace_back(row);
    }
    cout << "Added " << N << " rows to " << table_name << " from position " << start << " to " << start+N-1 << "\n";
}

void my_table::myPrint(const string & table_name, bool quiet)
{
    size_t N;
    cin >> N;
    vector<size_t> indices;
    vector<string> print_col_names; // !!!
    indices.reserve(N);
    print_col_names.reserve(N);
    for (size_t i=0; i<N; i++)
    {
        string name;
        cin >> name;

        auto index = checkColumnName(name, table_name, "PRINT");
        if (index == colname_map.end()) return;

        if (!quiet)
        {
            indices.emplace_back(index->second);
            print_col_names.emplace_back(name);
        }
    }
        
    string colname;
    cin >> colname;
    if (colname == "ALL")
    {
        if (!quiet)
        {
            for (string & name : print_col_names)
                cout << name << " ";
            cout << "\n";

           for (vector<TableEntry> row : table)
            {
                for (size_t i : indices)
                    cout << row[i] << " ";
                cout << "\n";
            }
        }
        cout << "Printed " << table.size() << " matching rows from " << table_name << "\n";
    }
    else
    {
        cin >> colname;
       
        auto index = checkColumnName(colname, table_name, "PRINT");
        if (index == colname_map.end()) return;
        
        if (!quiet)
        {
            for (string & name : print_col_names)
                cout << name << " ";
            cout << "\n";
        }
        char op;
        cin >> op;
        
        size_t col_index = index->second;
        size_t cnt = 0;
        switch (types[col_index])
        {
            case EntryType::String:
            {
                string val;
                cin >> val;
                TableEntry value(val);

                cnt = printConditional(op, col_index, value, indices, quiet);
                break;
            }
            case EntryType::Double:
            {
                double val;
                cin >> val;
                TableEntry value(val);

                cnt = printConditional(op, col_index, value, indices, quiet);
                break;
            }
            case EntryType::Int:
            {
                int val;
                cin >> val;
                TableEntry value(val);

                cnt = printConditional(op, col_index, value, indices, quiet);
                break;
            }
            case EntryType::Bool:
            {
                bool val;
                cin >> val;
                TableEntry value(val);

                cnt = printConditional(op, col_index, value, indices, quiet);
                break;
            }
            default:
                break;
        }
        cout << "Printed " << cnt << " matching rows from " << table_name << "\n";
    }
}

size_t my_table::printConditional(char op, size_t col_index, TableEntry value, const vector<size_t> & indices, bool quiet)
{
    size_t cnt = 0;

    if (hash_or_bst == 'b' && col_index == hashed_col_index) // bst index on conditional column
    {
        switch (op)
        {
            case '=':
            {
                auto it = _bst.find(value);
                if (it != _bst.end())
                {
                    cnt += it->second.size();
                    if (!quiet)
                    {
                        for (int target_row : it->second)
                        {
                            for (size_t i : indices)
                                cout << table[size_t(target_row)][i] << " ";
                            cout << "\n";
                        }
                    }
                }
                break;
            }
            case '<':
            {
                for (auto it=_bst.begin(); it!=_bst.end() && it->first < value; it++)
                {   
                    cnt += it->second.size();
                    if (!quiet)
                    {
                        for (int target_row : it->second)
                        {
                            for (size_t i : indices)
                                cout << table[size_t(target_row)][i] << " ";
                            cout << "\n";
                        }
                    }
                }
                break;
            }
            case '>':
            {
                for (auto it = _bst.upper_bound(value); it!=_bst.end(); it++)
                {
                    cnt += it->second.size();
                    if (!quiet)
                    {
                        for (int target_row : it->second)
                        {
                            for (size_t i : indices)
                                cout << table[size_t(target_row)][i] << " ";
                            cout << "\n";
                        }
                    }
                }
                break;
            }
            default:
                break;
        }   
    }
    else if (hash_or_bst == 'h' && col_index == hashed_col_index && op == '=')
    {
        auto it = _hash.find(value);

        if (it != _hash.end())
        {
            cnt += it->second.size();
            if (!quiet)
            {
                for (int target_row : it->second)
                {
                    for (size_t i : indices)
                        cout << table[size_t(target_row)][i] << " ";
                    cout << "\n";
                }
            }
        }
    }
    else 
    {
        for (vector<TableEntry> & row : table)
        {
            bool satisfy = false;
            switch (op)
            {
                case '=':
                {
                    if (row[col_index] == value)
                        satisfy = true;
                    break;
                }
                case '<':
                {
                    if (row[col_index] < value)
                        satisfy = true;
                    break;
                }
                case '>':
                {
                    if (row[col_index] > value)
                        satisfy = true;
                    break;
                }
                default:
                    break;
            }
            if (satisfy)
            {
                if (!quiet)
                {
                    for (size_t i : indices)
                        cout << row[i] << " ";
                    cout << "\n";
                }
                cnt++;
            }
        }
    }
    return cnt;
}

void my_table::myDelete(const string & table_name)
{
    string name;
    cin >> name;
    cin >> name;
    
    auto index = checkColumnName(name, table_name, "DELETE");
    if (index == colname_map.end()) return;
    size_t col_index = index->second;

    size_t N = 0;
    char op;
    cin >> op;
    switch (types[col_index])
    {
        case EntryType::String:
        {
            string val;
            cin >> val;
            TableEntry value(val);
            N = deleteOp(op, col_index, value);
            break;
        }
        case EntryType::Double:
        {
            double val;
            cin >> val;
            TableEntry value(val);
            N = deleteOp(op, col_index, value);
            break;
        }
        case EntryType::Int:
        {
            int val;
            cin >> val;
            TableEntry value(val);
            N = deleteOp(op, col_index, value);
            break;
        }
        case EntryType::Bool:
        {
            bool val;
            cin >> val;
            TableEntry value(val);
            N = deleteOp(op, col_index, value);
            break;
        }
            
        default:
            break;
    }
    cout << "Deleted " << N << " rows from " << table_name << "\n";
}

size_t my_table::deleteOp(char op, size_t col_index, TableEntry val)
{
    size_t cnt = 0;

    if (col_index == hashed_col_index && hash_or_bst != 'n') // index can be used
    {
        priority_queue<int> pq;
        if (hash_or_bst == 'h') // hash
        {
            auto it = _hash.begin();

            if (op == '=')
            {
                it = _hash.find(val);
                if (it != _hash.end())
                {
                    for (int i : it->second)
                        pq.push(i);
                    cnt += it->second.size();
                }
            }
            else
            {
                while (it != _hash.end())
                {
                    bool satisfy = false;
                    switch (op)
                    {
                        case '<':
                        {
                            if (it->first < val)
                                satisfy = true;
                            break;
                        }
                        case '>':
                        {
                            if (it->first > val)
                                satisfy = true;
                            break;
                        }
                        default:
                            break;
                    }

                    if (satisfy)
                    {
                        for (int i : it->second)
                            pq.push(i);
                        cnt += it->second.size();
                    }
                    it++;
                }
            }
        }
        else // bst
        {
            switch (op)
            {
                case '=':
                {
                    auto it = _bst.find(val);
                    if (it != _bst.end())
                    {
                        for (int i : it->second)
                            pq.push(i);
                        cnt += _bst[val].size();
                    }
                    break;
                }
                case '<':
                {
                    auto it = _bst.begin();
                    while (it != _bst.end() && it->first < val)
                    {
                        for (int i : it->second)
                            pq.push(i);
                        cnt += it->second.size();
                        it++;
                    }
                    break;
                }
                case '>':
                {
                    auto it = _bst.upper_bound(val);
                    while (it != _bst.end())
                    {
                        for (int i : it->second)
                            pq.push(i);
                        cnt += it->second.size();
                        it++;
                    }
                    break;
                }
                default:
                    break;
            }
        }
        while (!pq.empty())
        {
            table.erase(table.begin() + pq.top());
            pq.pop();
        }
    }
    else // cannot use index generated
    {   
        auto bound = table.begin();
        switch (op)
        {
            case '=':
            {
                bound = remove_if(table.begin(), table.end(), [val, col_index](vector<TableEntry> v){return v[col_index] == val;});
                break;
            }
            case '<':
            {
                bound = remove_if(table.begin(), table.end(), [val, col_index](vector<TableEntry> v){return v[col_index] < val;});
                break;
            }
            case '>':
            {
                bound = remove_if(table.begin(), table.end(), [val, col_index](vector<TableEntry> v){return v[col_index] > val;});
                break;
            }
            default:
                break;
        }

        cnt += (-size_t(bound - table.end()));
        table.erase(bound, table.end());
    }
    // update bst or hash index
    if (hash_or_bst == 'b' && cnt > 0)
    {
        _bst.clear();
        for (size_t i=0; i<table.size(); i++)
            _bst[table[i][hashed_col_index]].emplace_back(i);
    }
    else if (hash_or_bst == 'h' && cnt > 0)
    {
        _hash.clear();
        for (size_t i=0; i<table.size(); i++)
            _hash[table[i][hashed_col_index]].emplace_back(i);
    }

    return cnt;
}

void my_table::myGenerate(const string & table_name)
{
    string index_type;
    cin >> index_type;
    string colname;
    cin >> colname; cin >> colname; cin >> colname;

    auto index = checkColumnName(colname, table_name, "GENERATE");
    if (index == colname_map.end()) return;
    size_t col_index = index->second;

    if (!(col_index == hashed_col_index && hash_or_bst == index_type[0]))
    {
        hashed_col_index = col_index;
        if (hash_or_bst == 'h')
            _hash.clear();
        else if (hash_or_bst == 'b')
            _bst.clear();
        if (index_type == "hash")
        {
            hash_or_bst = 'h';
            for (size_t i=0; i<table.size(); i++)
                _hash[table[i][col_index]].emplace_back(i);
        }
        else //if (index_type == "bst")
        {
            hash_or_bst = 'b';
            for (size_t i=0; i<table.size(); i++)
                _bst[table[i][col_index]].emplace_back(i);
        }
    }

    cout << "Created " << index_type << " index for table " << table_name <<  " on column " << colname << "\n";
}

void my_table::myJoin(my_table* mt2, const string & table_name1, const string & table_name2, bool quiet)
{
    string colname1;
    cin >> colname1;
    cin >> colname1;
    auto index1 = checkColumnName(colname1, table_name1, "JOIN");
    if (index1 == colname_map.end()) return;

    string colname2;
    cin >> colname2;
    cin >> colname2;
    auto index2 = mt2->checkColumnName(colname2, table_name2, "JOIN");
    if (index2 == mt2->colname_map.end()) return;

    size_t N;
    string print_colname;
    vector<char> which_table;
    vector<size_t> indices;
    vector<string> print_colnames;
    cin >> print_colname;
    cin >> print_colname;
    cin >> N;
    which_table.reserve(N);
    indices.reserve(N);
    print_colnames.reserve(N);
    for (size_t i=0; i<N; i++)
    {
        cin >> print_colname;
        char ch;
        cin >> ch;

        unordered_map<string, size_t>::iterator index;
        if (ch == '1')
        {
            index = checkColumnName(print_colname, table_name1, "JOIN");
            if (index == colname_map.end()) return;
        }
        else
        {
            index = mt2->checkColumnName(print_colname, table_name2, "JOIN");
            if (index == mt2->colname_map.end()) return;
        }
        
        if (!quiet)
        {
            indices.emplace_back(index->second);
            which_table.emplace_back(ch);
            print_colnames.emplace_back(print_colname);
        }
    }
    if (!quiet)
    {
        for (string & name : print_colnames)
            cout << name << " ";
        cout << "\n";
    }
        
    size_t col_index1 = index1->second;
    size_t col_index2 = index2->second;
    size_t cnt = 0;
    vector<vector<TableEntry>> & table2 = mt2->table;
    if (col_index2 == mt2->hashed_col_index && mt2->hash_or_bst != 'n') // can use index for the second table
    {
        if (mt2->hash_or_bst == 'h') // hash
        {
            for (auto & row1 : table)
            {
                TableEntry value1 = row1[col_index1];
                auto it = mt2->_hash.find(value1);
                if (it != mt2->_hash.end())
                {
                    cnt += it->second.size();
                    if (!quiet)
                    {
                        for (int row2_index : it->second)
                        {
                            vector<TableEntry> & row2 = table2[size_t(row2_index)];
                            
                            for (size_t i=0; i<indices.size(); i++)
                            {
                                if (which_table[i] == '1')
                                    cout << row1[indices[i]] << " ";
                                else
                                    cout << row2[indices[i]] << " ";
                            }
                            cout << "\n";
                        }
                    }
                }
            }
        }
        else // bst
        {
            for (auto & row1 : table)
            {
                TableEntry value1 = row1[col_index1];
                auto it = mt2->_bst.find(value1);
                if (it != mt2->_bst.end())
                {
                    cnt += it->second.size();
                    if (!quiet)
                    {
                        for (int row2_index : it->second)
                        {
                            vector<TableEntry> & row2 = table2[size_t(row2_index)];
                        
                            for (size_t i=0; i<indices.size(); i++)
                            {
                                if (which_table[i] == '1')
                                    cout << row1[indices[i]] << " ";
                                else
                                    cout << row2[indices[i]] << " ";
                            }
                            cout << "\n";
                        }
                    }
                }
            }
        }
    }
    else // cannot use index for second table
    {
        unordered_map<TableEntry, vector<size_t>> tmp_map;
        for (size_t row2_index=0; row2_index<mt2->table.size(); row2_index++)
            tmp_map[table2[row2_index][col_index2]].emplace_back(row2_index);
        
        for (auto & row1 : table)
        {
            TableEntry value1 = row1[col_index1];
            auto it = tmp_map.find(value1);
            if (it != tmp_map.end())
            {
                cnt += it->second.size();
                if (!quiet)
                {
                    for (size_t i : it->second)
                    {
                        vector<TableEntry> & row2 = table2[i];
                        for (size_t j=0; j<indices.size(); j++)
                        {
                            switch (which_table[j])
                            {
                                case '1':
                                    cout << row1[indices[j]] << " ";
                                    break;
                                case '2':
                                    cout << row2[indices[j]] << " ";
                                    break;
                                default:
                                    break;
                            }
                        }
                        cout << "\n";
                    }
                }      
            }
            
        }
    }
    cout << "Printed " << cnt << " rows from joining " << table_name1 << " to " << table_name2 << "\n";
}