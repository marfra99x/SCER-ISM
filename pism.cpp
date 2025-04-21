#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

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
        printf("%lld - %lld\n", u, pairU[u]);
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

        int index = (int)c - 'A';
        int bit = createBitwiseText(text[i]);

        all_bitmask[index] &= bit;
    }

    n = pattern.size();
    for (int i = 0; i < sigma_p_x_len; i++)
    {
        char c = sigma_p_x[i];
        cout << i << " " << c - 'A' << ": ";
        printBinary(all_bitmask[c - 'A']);
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
            int c = sigma_p_x[i] - 'A';
            int v = sigma_p_y[j] - 'A';
            if (all_bitmask[c] & (1 << v))
            {
                int u = c + 'A' - 'F';
                graph.addEdge(u, v);
                cout << "v -> " << (char)(c + 'A') << " " << (char)(v + 'A') << endl;
            }
        }
    }
    int res = graph.hopcroftKarp();

    cout << "m:" << res << endl;

    if (res == sigma_p_x_len)
        return true;
    return false;
}

int main()
{
    // Example indeterminate text where each position can represent multiple characters
    vector<string> y = {"A", "b", "CD", "ACE"};
    string x = "FbGH"; // X, Y, Z are parameterized, 'b' is fixed

    int sigma_p_y_len = 4;
    int sigma_p_x_len = 3;
    string sigma_p_y = "ACDE";
    string sigma_p_x = "FGH";

    // Check if pattern p-matches the indeterminate text
    cout << (matchesAt(y, x, sigma_p_y_len, sigma_p_x_len, sigma_p_y, sigma_p_x) ? "P-Match" : "No Match") << endl;

    return 0;
}
