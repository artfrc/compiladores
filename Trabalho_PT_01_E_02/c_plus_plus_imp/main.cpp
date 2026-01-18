#include <bits/stdc++.h>
using namespace std;

#define NAME_FILE "file.txt"

string getContentFile() {
            
    try {
        ifstream file(NAME_FILE);

        if (!file.is_open()) {
            throw runtime_error("Nao foi possivel abrir o arquivo: "  NAME_FILE);
        }

        stringstream buffer;
        buffer << file.rdbuf();

        return buffer.str();
    }
    catch (const exception& e) {
        cerr << "Erro: " << e.what() << endl;
        throw;
    }
}

int main() {

    string buffer = getContentFile();
    cout << buffer << endl;

}