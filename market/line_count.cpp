#include <iostream>
#include <string>
#include <vector>
#include <fstream>
using namespace std;

void line_count(const vector<string>& names) {
    int counter = 0;
    for(int i = 0; i < names.size(); i++) {
        ifstream file(names[i].c_str());
        if(!file.good()) {
            cout << "File not found\n";
            continue;
        }
        cout << "File " << names[i] << " is being read...\n";
        string line;
        while (getline(file, line)) {
            counter++;
        }
    }

    cout << "There are " << counter << " lines of code in these files\n";


}

int main()
{
    vector<string> filenames;
    filenames.push_back("main.cpp");
    filenames.push_back("market.cpp");
    filenames.push_back("product.cpp");
    filenames.push_back("offer.cpp");
    filenames.push_back("user.cpp");
    filenames.push_back("misc_funcs.cpp");
    filenames.push_back("sort_methods.cpp");
    filenames.push_back("processing.cpp");
    filenames.push_back("print.cpp");
    filenames.push_back("find_and_check.cpp");
    filenames.push_back("market.hpp");
    filenames.push_back("product.hpp");
    filenames.push_back("offer.hpp");
    filenames.push_back("misc_funcs.hpp");
    filenames.push_back("sort_methods.hpp");
    filenames.push_back("user.hpp");
    filenames.push_back("exceptions.hpp");
    line_count(filenames);
}