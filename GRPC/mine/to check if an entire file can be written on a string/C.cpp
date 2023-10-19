#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main() {
    string s = "";
    string s1;

    // Open input file "in.txt" and read from it
    ifstream input_file("in.txt");
    if (!input_file.is_open()) {
        cerr << "Error opening input file 'in.txt'" << endl;
        return 1;
    }

    while (getline(input_file, s1)) {
        s += s1 + "\n";
    }
    input_file.close(); // Close the input file

    // Open output file "C1.cpp" and write to it
    ofstream output_file("C1.cpp");
    if (!output_file.is_open()) {
        cerr << "Error opening output file 'C1.cpp'" << endl;
        return 1;
    }

    output_file << s; // Write the contents of the string to the output file
    output_file.close(); // Close the output file
}
