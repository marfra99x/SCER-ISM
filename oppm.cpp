#include <iostream>
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

// Function to compute rank transformation
vector<int> sortedIndexes(const vector<int> &x)
{
    vector<int> idx(x.size());
    iota(idx.begin(), idx.end(), 0);
    sort(idx.begin(), idx.end(), [&x](int i, int j)
         { return x[i] < x[j]; });
    return idx;
}

// Function to check if a number is a power of two
bool isPowerOfTwo(int num)
{
    return (num & (num - 1)) == 0;
}

// Function to find the smallest valid match in an indeterminate position
int findSmallestValid(int t, int prev)
{
    t &= -prev;    // Remove all values smaller than prev
    return t & -t; // Extract the smallest remaining power of two
}

// Function to determine order-preserving match
bool orderPreservingMatch(const vector<int> &S, const vector<int> &T)
{
    int n = S.size();
    vector<int> rhoS = sortedIndexes(S);
    vector<int> selectedT(n);

    for (int i = 0; i < n; i++)
    {
        int index = rhoS[i];
        int t = T[index];

        if (isPowerOfTwo(t))
        {
            if (t < selectedT[rhoS[i - 1]])
                return false;
            selectedT[index] = t; // Determinate value
        }
        else
        {
            if (i == 0)
            {
                selectedT[index] = t & -t; // Pick the smallest power of two
            }
            else
            {
                selectedT[index] = findSmallestValid(t, selectedT[rhoS[i - 1]]);
                if (selectedT[index] == 0)
                    return false; // No valid selection exists
            }
        }
    }

    // return sortedIndexes(selectedT) == rhoS;
    return true;
}

// Function to permute vector based on given order
vector<vector<int> > permute(const vector<vector<int> > &y, const vector<int> &pi)
{
    vector<vector<int> > y_pi(y.size());
    for (size_t i = 0; i < y.size(); i++)
    {
        y_pi[i] = y[pi[i]];
    }
    return y_pi;
}

// Function to compute μOPPM
bool muOPPM(const vector<int> &x, const vector<vector<int> > &y)
{
    int m = x.size();
    vector<int> pi = sortedIndexes(x); // Get the sorted index order

    vector<int> x_pi(m);
    vector<vector<int> > y_pi = permute(y, pi);

    for (int i = 0; i < m; i++) // permute
    {
        x_pi[i] = x[pi[i]];
    }

    vector<vector<int> > y_prime;
    y_prime.push_back(y_pi[0]);
    int j = 0;

    // Process intersections for equal elements
    for (int i = 1; i < m; i++)
    {
        if (x_pi[i] == x_pi[i - 1])
        {
            set<int> intersection;
            set_intersection(y_prime[j].begin(), y_prime[j].end(), y_pi[i].begin(), y_pi[i].end(),
                             inserter(intersection, intersection.begin()));
            y_prime[j] = vector<int>(intersection.begin(), intersection.end());
        }
        else
        {
            j++;
            y_prime.push_back(y_pi[i]);
        }
    }

    int s = y_prime.size();
    int nextMin = -1;

    // Checking monotonic properties
    for (int i = 0; i < s; i++)
    {
        auto it = upper_bound(y_prime[i].begin(), y_prime[i].end(), nextMin);
        if (it == y_prime[i].end())
            return false;
        nextMin = *it;
    }

    return true;
}

int main()
{
    const int size = 100000;
    auto start_time = high_resolution_clock::now();
    auto end_time = high_resolution_clock::now();
    auto new_time = duration<double, milli>(end_time - start_time).count();
    auto old_time = duration<double, milli>(end_time - start_time).count();
    new_time = 0;
    old_time = 0;
    int minExponent = 1;
    int maxExponent = 30;
    int r = 2;

    for (int tsk = 0; tsk < 50; tsk++)
    {
        cout << tsk << endl;
        vector<int> S(size);
        vector<int> T(size);
        for(int i=0; i<size; i++){
            int exponent = minExponent + (std::rand() % (maxExponent - minExponent + 1));
            int powerOfTwo = 1 << exponent;
            S[i] = T[i] = powerOfTwo;
        }


        vector<vector<int> > y(size);
        vector<int> x(size); // Determinate string
        
        for (int i = 0; i < size; i++)
        {
            y[i].resize(r);
            for(int j=0; j<r; j++){
                int exponent = minExponent + (std::rand() % (maxExponent - minExponent + 1));
                int powerOfTwo = 1 << exponent;
                T[i] |= powerOfTwo;
                y[i][j] = exponent;
            }
            sort(y[i].begin(), y[i].end());
            x[i] = y[i][0];
        }
        start_time = high_resolution_clock::now();
        bool match = orderPreservingMatch(S, T);
        end_time = high_resolution_clock::now();
        new_time += duration<double, milli>(end_time - start_time).count();
        if(!match)cout << "error1"<<endl;
        

        start_time = high_resolution_clock::now();
        match = muOPPM(x, y);
        end_time = high_resolution_clock::now();
        old_time += duration<double, milli>(end_time - start_time).count();
        if(!match)cout << "error2"<<endl;

    }
    
    cout << "Searching Time NEW: " << new_time/500.0 << " ms" << endl;
    cout << "Searching Time OLD: " << old_time/500.0 << " ms" << endl;
    cout << endl
         << endl
         << "New is: " << (old_time - new_time) / old_time * 100 << "\% faster" << endl;
    return 0;
}