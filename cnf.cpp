#include <iostream>
#include <cmath>
#include <random>
#include <chrono>
#include <fstream>

using namespace std;

void gen_cnf(int N) {
    ofstream out;
    out.open("out.cnf");
    int num_clauses = 0;

    out << "                                        \n";
    int **IDs = new int*[N];
    for (int i = 0; i < N; i++) {
        IDs[i] = new int[N];
        for (int j = 0; j < N; j++) {
            IDs[i][j] = i * N + j + 1;
        }
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            out << IDs[i][j] << " ";
        }
        out << "0\n";
        num_clauses++;

        for (int j = 0; j < N; j++) {
            for (int k = 0; k < j; k++) {
                out << "-" << IDs[i][j] << " -" << IDs[i][k] << " 0\n";
                num_clauses++;
            }
        }
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            out << IDs[j][i] << " ";
        }
        out << "0\n";
        num_clauses++;

        for (int j = 0; j < N; j++) {
            for (int k = 0; k < j; k++) {
                out << "-" << IDs[j][i] << " -" << IDs[k][i] << " 0\n";
                num_clauses++;
            }
        }
    }
    cout << "Row Col Checked. Yay!\n";

    for (int II = 0; II < pow(N, 2); II++) {
        for (int JJ = 0; JJ < II; JJ++) {
            for (int KK = 0; KK < JJ; KK++) {
                int x1 = II/N;
                int y1 = II%N;

                int x2 = JJ/N;
                int y2 = JJ%N;

                int x3 = KK/N;
                int y3 = KK%N;

                for (int i = 0; i < 3; i++) {
                    int x4;
                    int y4;
                    switch(i) {
                        case 0:
                        x4 = x3 + x1 - x2;
                        y4 = y3 + y1 - y2;
                        break;
                        case 1:
                        x4 = x2 + x3 - x1;
                        y4 = y2 + y3 - y1;
                        case 2:
                        x4 = x1 + x2 - x3;
                        y4 = y1 + y2 - y3;
                        break;
                    }
                    if (0 <= x4 && 0 <= y4 && x4 < N && y4 < N) {
                        int LL = N * x4 + y4;
                        if (max(max(II, JJ), max(KK, LL)) == LL) {
                            num_clauses++;
                            out << "-" << IDs[x1][y1] << " -" << IDs[x2][y2] << " -" << IDs[x3][y3] << " -" << IDs[x4][y4] << " 0\n";
                        }
                    }
                }
            }
        }
        cout << II;
        cout << "\n";
    }

    // int strlen = 6 + to_string(pow(N, 2)).length() + 1 + to_string(num_clauses).length();
    out.seekp(0);
    out << "p cnf " << pow(N, 2) << " " << num_clauses;
    out.close();
}

int main() {
    gen_cnf(32);
}
