#include <iostream>
#include "cpq.tab.hpp"

using namespace std;

extern FILE *yyin;

int main(int argc, const char *argv[])
{
    if (argc > 1) {
        yyin = fopen(argv[1], "r");
        if (!yyin) {
            perror("fopen");
            return -1;
        }
    } else {
        cout << "Usage: tree <in_file>" << endl;
        return -1;
    }
    yyparse();
    return 0;
}