#include <bits/stdc++.h>  // clang-format off
using namespace std;
#define REP2(i, n) for (Int i = 0, max_i = (n); i < max_i; i++)
#define REP3(i, a, b) for (Int i = (a), max_i = (b); i < max_i; i++)
#define OVERLOAD2(_1, _2, _3, name, ...) name
#define REP(...) OVERLOAD2(__VA_ARGS__, REP3, REP2)(__VA_ARGS__)
struct SetupIO { SetupIO() { cin.tie(nullptr), ios::sync_with_stdio(false), cout << fixed << setprecision(13); } } setup_io;
#ifndef _MY_DEBUG
#define dump(...)
#endif  // clang-format on

/**
 *    author:  knshnb
 *    created: Fri Feb 21 02:24:38 JST 2020
 **/

const char *files[] = {"data/a_example.txt",       "data/b_read_on.txt",       "data/c_incunabula.txt",
                       "data/d_tough_choices.txt", "data/e_so_many_books.txt", "data/f_libraries_of_the_world.txt"};

int B, L, D;
vector<int> scores;
vector<vector<int>> books;
vector<int> signup_days;
vector<int> parallel;

int calc_point(const vector<int>& library_order, const vector<vector<int>>& book_orders) {
    vector<int> used(B);
    int time = 0;
    for (int l : library_order) {
        time += signup_days[l];
        int tmp_time = time;
        for (int i = 0; i < book_orders[l].size(); i++) {
            if (i % parallel[l] == 0) {
                if (++tmp_time > D) break;
            }
            used[book_orders[l][i]] = true;
        }
    }
    int score = 0;
    for (int b = 0; b < B; b++) {
        if (used[b]) score += scores[b];
    }
    return score;
}

void solve() {
    // INPUT READ BEGIN
    cin >> B >> L >> D;
    scores.resize(B);
    books.resize(L);
    signup_days.resize(L);
    parallel.resize(L);
    for (int i = 0; i < B; i++) cin >> scores[i];
    for (int i = 0; i < L; i++) {
        int a;
        cin >> a >> signup_days[i] >> parallel[i];
        books[i].resize(a);
        for (int j = 0; j < a; j++) cin >> books[i][j];
    }
    // INPUT READ END
    // signup の時間が短い順に
    vector<int> order(L);
    for (int i = 0; i < L; i++) order[i] = i;
    sort(order.begin(), order.end(), [&](auto &i, auto &j) { return signup_days[i] < signup_days[j]; });

    cout << "score: " << calc_point(order, books) << endl;

    // output
    // cout << L << endl;
    // for (int i = 0; i < L; i++) {
    //     int a = order[i];
    //     cout << a << ' ' << books[a].size() << '\n';
    //     for (int b : books[a]) {
    //         cout << b << ' ';
    //     }
    //     cout << '\n';
    // }
}
int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    int n;
    cin >> n;
    assert(freopen(files[n], "r", stdin));
    solve();
}
