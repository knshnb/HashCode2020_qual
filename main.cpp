#include <bits/stdc++.h>  // clang-format off
using namespace std;
#define REP2(i, n) for (int i = 0, max_i = (n); i < max_i; i++)
#define REP3(i, a, b) for (int i = (a), max_i = (b); i < max_i; i++)
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

mt19937 rnd(chrono::steady_clock::now().time_since_epoch().count());

const char *files[] = {"data/a_example.txt",       "data/b_read_on.txt",       "data/c_incunabula.txt",
                       "data/d_tough_choices.txt", "data/e_so_many_books.txt", "data/f_libraries_of_the_world.txt"};

int B, L, D;
vector<int> scores;
vector<vector<int>> books;
vector<int> signup_days;
vector<int> parallel;

int calc_point(const vector<int> &library_order, const vector<vector<int>> &book_orders) {
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

int best_point = -1;
vector<int> best_library_order;
vector<vector<int>> best_book_orders;

void hill_climb(int times = 1000) {
    REP(t, times) {
        if (t % 100 == 0) cerr << "t: " << t << endl;
        auto library_order = best_library_order;
        int i = rnd() % L, j = rnd() % L;
        swap(library_order[i], library_order[j]);
        auto book_orders = best_book_orders;
        int nxt_point = calc_point(library_order, book_orders);
        if (nxt_point > best_point) {
            best_library_order = move(library_order);
            best_book_orders = move(book_orders);
            best_point = nxt_point;
            cerr << "nxt_point: " << best_point << endl;
        }
    }
}

void initialize_for_C() {
    // book
    best_book_orders = books;
    for (int l = 0; l < L; l++) {
        sort(best_book_orders[l].begin(), best_book_orders[l].end(),
             [&](int i, int j) { return scores[i] > scores[j]; });
    }

    // library
    best_library_order.resize(L);
    for (int i = 0; i < L; i++) best_library_order[i] = i;
    sort(best_library_order.begin(), best_library_order.end(),
         [&](auto &i, auto &j) { return signup_days[i] < signup_days[j]; });
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

    initialize_for_C();
    best_point = calc_point(best_library_order, best_book_orders);
    cerr << "score: " << best_point << endl;

    hill_climb();

    // // output
    // cout << L << endl;
    // for (int i = 0; i < L; i++) {
    //     int a = best_library_order[i];
    //     cout << a << ' ' << books[a].size() << '\n';
    //     for (int b : best_book_orders[a]) {
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
