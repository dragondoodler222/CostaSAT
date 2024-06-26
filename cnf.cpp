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
    // cout << "Row Col Checked. Yay!\n";

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
        // cout << II;
        // cout << "\n";
    }

    // int strlen = 6 + to_string(pow(N, 2)).length() + 1 + to_string(num_clauses).length();
    out.seekp(0);
    out << "p cnf " << pow(N, 2) << " " << num_clauses;
    out.close();
}

void gen_cnf_slope(int N) {
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

    int var_index = pow(N, 2) + 1;

    for (int m_x = 1; m_x < N; m_x++) {
        for (int m_y = 1; m_y < N; m_y++) {
            if (m_x == N - 1 && m_y == N - 1) continue;

            int additional_vars = 0;

            for (int i = 0; i < N - m_x; i++) {
                for (int j = 0; j < N - m_y; j++) {
                    // Point 1: i, j
                    // Point 2: i + mx, j + my
                    // -P_(1,2) = -x[i][j] v -x[i + mx][j + my]
                    if (i == 0 && j == 0) {
                        out << "-" << IDs[i][j]  << " -" << IDs[i + m_x][j + m_y] << " " << var_index + additional_vars << " 0\n";
                        additional_vars++;
                        num_clauses++;
                        continue;
                    }

                    if (i == N - m_x - 1 && j == N - m_y - 1) {
                        out << "-" << IDs[i][j]  << " -" << IDs[i + m_x][j + m_y] << " -" << var_index + additional_vars - 1 << " 0\n";
                        // additional_vars++;
                        num_clauses++;
                        continue;
                    }

                    out << "-" << var_index + additional_vars - 1 << " " << var_index + additional_vars << " 0\n";
                    out << "-" << IDs[i][j]  << " -" << IDs[i + m_x][j + m_y] << " " << var_index + additional_vars << " 0\n";
                    out << "-" << IDs[i][j]  << " -" << IDs[i + m_x][j + m_y] << " -" << var_index + additional_vars - 1 << " 0\n";
                    num_clauses += 3;

                    additional_vars++;

                }
            }

            var_index += additional_vars;

        }
    }

    for (int m_x = 1; m_x < N; m_x++) {
        for (int m_y = 1; m_y < N; m_y++) {
            if (m_x == N - 1 && m_y == N - 1) continue;

            int additional_vars = 0;

            for (int i = 0; i < N - m_x; i++) {
                for (int j = N - 1; j >= m_y; j--) {
                    // Point 1: i, j
                    // Point 2: i + mx, j + my
                    // -P_(1,2) = -x[i][j] v -x[i + mx][j + my]
                    if (i == 0 && j == N - 1) {
                        out << "-" << IDs[i][j]  << " -" << IDs[i + m_x][j - m_y] << " " << var_index + additional_vars << " 0\n";
                        additional_vars++;
                        num_clauses++;
                        continue;
                    }

                    if (i == N - m_x - 1 && j == m_y) {
                        out << "-" << IDs[i][j]  << " -" << IDs[i + m_x][j - m_y] << " -" << var_index + additional_vars - 1 << " 0\n";
                        // additional_vars++;
                        num_clauses++;
                        continue;
                    }

                    out << "-" << var_index + additional_vars - 1 << " " << var_index + additional_vars << " 0\n";
                    out << "-" << IDs[i][j]  << " -" << IDs[i + m_x][j - m_y] << " " << var_index + additional_vars << " 0\n";
                    out << "-" << IDs[i][j]  << " -" << IDs[i + m_x][j - m_y] << " -" << var_index + additional_vars - 1 << " 0\n";
                    num_clauses += 3;

                    additional_vars++;

                }
            }

            var_index += additional_vars;

        }
    }

    // int strlen = 6 + to_string(pow(N, 2)).length() + 1 + to_string(num_clauses).length();
    out.seekp(0);
    out << "p cnf " << var_index - 1 << " " << num_clauses;
    out.close();
    // cout << N << ", " << num_clauses << "\n";
    // cout << N << ", " << var_index - 1 << "\n";

}


void gen_cnf_slope_bin(int N) {
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

    int var_index = pow(N, 2) + 1;

    for (int m_x = 1; m_x < N; m_x++) {
        for (int m_y = 1; m_y < N; m_y++) {
            if (m_x == N - 1 && m_y == N - 1) continue;

            int additional_vars = ceil(log2((N - m_x)*(N - m_y)));

            for (int i = 0; i < N - m_x; i++) {
                for (int j = 0; j < N - m_y; j++) {
                    // Point 1: i, j
                    // Point 2: i + mx, j + my
                    // -P_(1,2) = -x[i][j] v -x[i + mx][j + my]
                    int val = i*(N-m_y) + j;
                    for (int k = 0; k < additional_vars; k++) {
                        if (val & 1)
                            out << "-" << IDs[i][j]  << " -" << IDs[i + m_x][j + m_y] << " " << var_index + k << " 0\n";
                        else
                            out << "-" << IDs[i][j]  << " -" << IDs[i + m_x][j + m_y] << " -" << var_index + k << " 0\n";

                        val = val >> 1;
                        num_clauses ++;
                    }

                }
            }

            var_index += additional_vars;

        }
    }

    for (int m_x = 1; m_x < N; m_x++) {
        for (int m_y = 1; m_y < N; m_y++) {
            if (m_x == N - 1 && m_y == N - 1) continue;

            int additional_vars = ceil(log2((N - m_x)*(N - m_y)));

            for (int i = 0; i < N - m_x; i++) {
                for (int j = N - 1; j >= m_y; j--) {
                    // Point 1: i, j
                    // Point 2: i + mx, j + my
                    // -P_(1,2) = -x[i][j] v -x[i + mx][j + my]
                    int val = i*(N-m_y) + (N - 1 - j);
                    for (int k = 0; k < additional_vars; k++) {
                        if (val & 1)
                            out << "-" << IDs[i][j]  << " -" << IDs[i + m_x][j - m_y] << " " << var_index + k << " 0\n";
                        else
                            out << "-" << IDs[i][j]  << " -" << IDs[i + m_x][j - m_y] << " -" << var_index + k << " 0\n";

                        val = val >> 1;
                        num_clauses ++;
                    }

                }
            }

            var_index += additional_vars;

        }
    }

    // int strlen = 6 + to_string(pow(N, 2)).length() + 1 + to_string(num_clauses).length();
    out.seekp(0);
    out << "p cnf " << var_index - 1 << " " << num_clauses;
    out.close();
    // cout << N << ", " << num_clauses << "\n";
    // cout << N << ", " << var_index - 1 << "\n";

}

int main(int argc, char* argv[]) {
    if (stoi(argv[2]) == 0) {
        gen_cnf(stoi(argv[1]));
    }

    if (stoi(argv[2]) == 1) {
        gen_cnf_slope(stoi(argv[1]));
    }

    if (stoi(argv[2]) == 2) {
        gen_cnf_slope_bin(stoi(argv[1]));
    }

}
