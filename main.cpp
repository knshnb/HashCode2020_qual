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
vector<vector<int>> books;  // 各libraryについてscoreが大きい順にソートされている
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

vector<vector<int>> optimize_book_orders(const vector<int> &library_order) {
    vector<int> nums(L);  // 各libraryが選べる本の個数
    int time = 0;
    for (int l : library_order) {
        time += signup_days[l];
        int tmp_time = time;
        for (int i = 0; i < books[l].size(); i++) {
            if (i % parallel[l] == 0) {
                if (++tmp_time > D) break;
            }
            nums[l]++;
        }
        assert(nums[l] <= books[l].size());
    }

    vector<vector<int>> book_orders(L);
    vector<int> used(B);
    auto lib = library_order;
    sort(lib.begin(), lib.end(), [&](int i, int j) {
        return books[i].size() - nums[i] < books[j].size() - nums[j];
    });
    for (int l : lib) {
        int cnt = 0;
        for (int b : books[l]) {
            if (book_orders[l].size() == nums[l]) break;
            if (used[b]) continue;
            used[b] = true;
            book_orders[l].push_back(b);
        }
    }
    return book_orders;
}

void hill_climb(int times = 1000000) {
    REP(t, times) {
        if (t % 100 == 0) cerr << "t: " << t << endl;
        auto library_order = best_library_order;
        int i = rnd() % 150, j = rnd() % L;
        swap(library_order[i], library_order[j]);
        auto book_orders = optimize_book_orders(library_order);
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

    // library
    best_library_order.resize(L);
    for (int i = 0; i < L; i++) best_library_order[i] = i;
    sort(best_library_order.begin(), best_library_order.end(),
         [&](auto &i, auto &j) { return signup_days[i] < signup_days[j]; });
}

void initialize_for_D() {
    // book
    best_book_orders = books;
    for (int l = 0; l < L; l++) {
        sort(best_book_orders[l].begin(), best_book_orders[l].end(),
             [&](int i, int j) { return scores[i] > scores[j]; });
    }

    // library
    best_library_order.resize(L);
    vector<bool> used(B);
    set<int> ok;
    for (int i = 0; i < L; i++) ok.insert(i);
    vector<vector<int>> used_books(L);
    for (int _ = 0; _ < L; _++) {
        vector<pair<int, int>> bookcnt;
        for (int i : ok) {
            int cnt = 0;
            for (int a : books[i]) {
                if (!used[a]) cnt++;
            }
            bookcnt.emplace_back(cnt, i);
        }
        assert(bookcnt.size() == L - _);
        sort(bookcnt.rbegin(), bookcnt.rend());
        int lib = bookcnt.front().second;
        best_library_order[_] = lib;
        for (int a : books[lib]) {
            if (used[a]) continue;
            used[a] = true;
            used_books[lib].push_back(a);
        }
        ok.erase(lib);
    }
}

void initialize_for_E() {
    // book
    best_book_orders = books;
    for (int l = 0; l < L; l++) {
        sort(best_book_orders[l].begin(), best_book_orders[l].end(),
             [&](int i, int j) { return scores[i] > scores[j]; });
    }
    // library
    best_library_order.resize(L);
    for (int i = 0; i < L; i++) best_library_order[i] = i;
    vector<pair<int, int>> values;
    vector<int> order(L);
    set<int> ok;
    for (int i = 0; i < L; i++) {
        ok.insert(i);
    }
    int cum_time = 0;
    for (int _ = 0; _ < 200; _++) {
        values.clear();
        for (int i : ok) {
            sort(books[i].begin(), books[i].end(), [&](auto &a, auto &b) { return scores[a] > scores[b]; });
            int time = cum_time + signup_days[i];
            int idx = 0;
            int value = 0;
            while (time < D && idx < books[i].size()) {
                time++;
                for (int j = 0; j < parallel[i] && idx < books[i].size(); j++) {
                    value += scores[books[i][idx]];
                    idx++;
                }
            }
            values.emplace_back(value, i);
        }
        sort(values.begin(), values.end(),
             [&](auto &a, auto &b) { return a.first / signup_days[a.second] > b.first / signup_days[b.second]; });
        int lib = values.front().second;
        order[_] = lib;
        assert(values.size() == ok.size());
        assert(values.size() == L - _);
        ok.erase(lib);
        cum_time += signup_days[lib];
    }
    for (int i = 0; i < 200; i++) best_library_order[i] = order[i];
    values.erase(values.begin());
    assert(values.size() == L - 200);
    for (int i = 200; i < L; i++) best_library_order[i] = values[i - 200].second;
}

void initialize_for_F() {
    // book
    best_book_orders = books;
    for (int l = 0; l < L; l++) {
        sort(best_book_orders[l].begin(), best_book_orders[l].end(),
             [&](int i, int j) { return scores[i] > scores[j]; });
    }

    // library
    vector<long long> libpower(L, 0);
    for (int i = 0; i < L; i++) {
        for (int j = 0; j < books[i].size(); j++) {
            libpower[i] += scores[books[i][j]];
        }
    }

    best_library_order.resize(L);
    for (int i = 0; i < L; i++) best_library_order[i] = i;
    sort(best_library_order.begin(), best_library_order.end(),
         [&](auto &i, auto &j) { return signup_days[i] * libpower[j] < signup_days[j] * libpower[i]; });
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
    for (int l = 0; l < L; l++) {
        sort(books[l].begin(), books[l].end(), [&](int i, int j) { return scores[i] > scores[j]; });
    }

    initialize_for_E();
    best_point = calc_point(best_library_order, best_book_orders);
    cerr << "score: " << best_point << endl;

    hill_climb();

    // output
    cout << L << endl;
    for (int i = 0; i < L; i++) {
        int l = best_library_order[i];
        cout << l << ' ' << max(1, (int)best_book_orders[l].size()) << '\n';
        if (best_book_orders[l].size() == 0) {
            cout << books[l][0] << ' ';
        }
        for (int b : best_book_orders[l]) {
            cout << b << ' ';
        }
        cout << '\n';
    }
}
int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    int n;
    cin >> n;
    assert(freopen(files[n], "r", stdin));
    solve();
}
