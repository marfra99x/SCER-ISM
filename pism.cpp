#include <iostream>
#include <vector>
#include <random>
#include <unordered_map>
#include <cstdlib> // for srand, rand
#include <vector>
#include <bitset>
#include <algorithm>
#include <fstream>
#include <set>
#include <map>
#include <sstream>
#include <numeric> // Include this for iota
#include <ctime>   // For clock timing
#include <chrono>  // High precision timing

using namespace std;
using namespace chrono;

const int FIXED_ALPHABET_SIZE = 12; // 'a-l'
const int PARAM_ALPHABET_SIZE = 12; // 'A-L'
const int TOTAL_ALPHABET_SIZE = 24; // 12 fixed + 12 parameterized

#define NIL 0
#define INF INT_MAX
typedef long long ll;
typedef pair<long, long> pr;
class BipGraph
{
    ll m, n;
    vector<vector<ll>> adj;
    vector<ll> pairU, pairV, dist;

public:
    BipGraph(ll, ll);
    void addEdge(ll, ll);
    bool bfs();
    bool dfs(ll);
    ll hopcroftKarp();
};
ll BipGraph::hopcroftKarp()
{
    for (ll u = 0; u < m; u++)
        pairU[u] = NIL;
    for (ll v = 0; v < n; v++)
        pairV[v] = NIL;
    ll result = 0;
    while (bfs())
    {
        for (ll u = 1; u <= m; u++)
            if (pairU[u] == NIL && dfs(u))
            {
                result++;
            }
    }
    for (ll u = 0; u < m; u++)
    {
        // printf("%lld - %lld\n", u, pairU[u]);
    }
    return result;
}
bool BipGraph::bfs()
{
    queue<ll> Q;
    for (ll u = 1; u <= m; u++)
    {
        if (pairU[u] == NIL)
        {
            dist[u] = 0;
            Q.push(u);
        }
        else
            dist[u] = INF;
    }
    dist[NIL] = INF;
    while (!Q.empty())
    {
        ll u = Q.front();
        Q.pop();
        if (dist[u] < dist[NIL])
        {
            for (int i = 0; i < adj[u].size(); ++i)
            {
                ll v = adj[u][i];
                if (dist[pairV[v]] == INF)
                {
                    dist[pairV[v]] = dist[u] + 1;
                    Q.push(pairV[v]);
                }
            }
        }
    }
    return (dist[NIL] != INF);
}
bool BipGraph::dfs(ll u)
{
    if (u != NIL)
    {
        for (int i = 0; i < adj[u].size(); ++i)
        {
            ll v = adj[u][i];
            if (dist[pairV[v]] == dist[u] + 1)
            {
                if (dfs(pairV[v]) == true)
                {
                    pairV[v] = u;
                    pairU[u] = v;
                    return true;
                }
            }
        }
        dist[u] = INF;
        return false;
    }
    return true;
}
BipGraph::BipGraph(ll m, ll n)
{
    this->m = m;
    this->n = n;
    adj.resize(m + 1);
    pairU.resize(m + 1);
    pairV.resize(n + 1);
    dist.resize(m + 1);
}
void BipGraph::addEdge(ll u, ll v)
{
    adj[u].push_back(v);
}

bool isFixed(char x)
{
    return (x >= 'a' && x <= 'z');
}

void printBinary(int num, int bits = 8)
{
    cout << bitset<8>(num) << endl; // Prints binary with 24 bits (default)
}

// Function to create a bitwise representation of an indeterminate string
int createBitwiseText(const string &indeterminateText)
{
    int bitwiseText = 0;

    for (char c : indeterminateText)
    {
        bitwiseText |= (1 << (c - 'A'));
    }
    return bitwiseText;
}

// Function to check if the pattern p-matches the indeterminate text
bool matchesAt(const vector<string> &text, const string &pattern, int sigma_p_y_len, int sigma_p_x_len, string sigma_p_y, string sigma_p_x)
{

    char f_x = sigma_p_x[0];
    char f_y = sigma_p_y[0];

    // cout << "f: " << f_y << " " << f_x << endl;
    int n = pattern.size();
    int all_bitmask[32];

    for (int i = 0; i < 32; i++)
    {
        all_bitmask[i] = (1 << (PARAM_ALPHABET_SIZE + 1)) - 1;
    }

    for (int i = 0; i < n; i++)
    {
        char c = pattern[i];
        if (isFixed(c))
        {
            if (c != text[i][0])
                return false;
            continue;
        }

        int index = (int)c - f_y;
        int bit = createBitwiseText(text[i]);

        all_bitmask[index] &= bit;
    }

    n = pattern.size();
    for (int i = 0; i < sigma_p_x_len; i++)
    {
        char c = sigma_p_x[i];
        // cout << i << " " << c - 'A' << ": ";
        // printBinary(all_bitmask[c - 'A']);
        if (!isFixed(c) && all_bitmask[c - 'A'] == 0)
        {
            return false;
        }
    }

    ios_base::sync_with_stdio(false);

    BipGraph graph(sigma_p_x_len, sigma_p_y_len);

    for (int i = 0; i < sigma_p_x_len; i++)
    {
        for (int j = 0; j < sigma_p_y_len; j++)
        {
            int c = sigma_p_x[i] - f_y;
            int v = sigma_p_y[j] - f_y;
            if (all_bitmask[c] & (1 << v))
            {
                int u = c + f_y - f_x;
                graph.addEdge(u, v);
                // cout << "v -> " << (char)(c + 'A') << " " << (char)(v + 'A') << endl;
            }
        }
    }

    int res = graph.hopcroftKarp();

    // cout << "m:" << res << endl;

    if (res == sigma_p_x_len)
        return true;
    return false;
}

int main()
{
    int n_tsk = 100;
    auto start_time = high_resolution_clock::now();
    auto end_time = high_resolution_clock::now();
    auto total_time = duration<double, milli>(end_time - start_time).count();
    for (int tsk = 0; tsk < n_tsk; tsk++)
    {
        cout << tsk << endl;
        srand(time(NULL)); // seeds the PRNG with current time
        int n = 100000;
        // Example indeterminate text where each position can represent multiple characters
        vector<string> y; //= {"A", "b", "CD", "ACE"};
        string x;         //= "FbGH"; // X, Y, Z are parameterized, 'b' is fixed

        int sigma_p_y_len = 12;
        int sigma_p_x_len = 12;
        string sigma_p_y = "ABCDEFGHIJKL";
        string sigma_p_x = "MNOPQRSTUVWX";

        y.resize(n);

        for (int i = 0; i < n; i++)
        {
            int r = rand() % 10;
            // cout << "_" << i << " " << r << endl;
            if (r == 0) // generate solid
            {
                char c = rand() % 12 + 'a';
                y[i] = c;
                x += c;
            }
            else // generate indeterminate
            {
                int k = rand() % 12 + 1;
                string shuffled = sigma_p_y;
                random_device rd;
                mt19937 gen(rd());
                shuffle(shuffled.begin(), shuffled.end(), gen);
                // cout << k << endl;
                // cout << shuffled << endl;

                y[i] = shuffled.substr(0, k);
                // cout << y[i] << endl;
                x += shuffled[0] + 12;

                // cout << "-> " << y[i] << " - " << x[i] << endl;
            }
        }

        // cout << "here" << endl;

        // cout << x << endl;

        /*for(int i=0; i<n; i++){
            cout << y[i] << endl;
        }*/

        // Check if pattern p-matches the indeterminate text

        start_time = high_resolution_clock::now();
        bool res = matchesAt(y, x, sigma_p_y_len, sigma_p_x_len, sigma_p_y, sigma_p_x);
        end_time = high_resolution_clock::now();
        total_time += duration<double, milli>(end_time - start_time).count();

        /*
        if (res)
            cout << "P-Match Found" << endl;
        else
            cout << "No P-Match" << endl;
        */
    }

    cout << "Searching Time: " << total_time / (double)(n_tsk) << " ms" << endl;
    return 0;
}
