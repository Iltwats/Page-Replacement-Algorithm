// Longest Distance First Page Replacement Algorithm implementation
#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <cstring>

using namespace std;

// Function to find the maximum element in an array
int maxm(int[], int);

// Function to find the number of distinct pages
int distinct(int[], int, int, int *, int[]);

// Function to determine the position of the frame where a new page will be inserted in case of a page fault
int func(int[], int[], int, int, int, unordered_map<int, int> &);

int main()
{
    cout << "This is LDF page replacement algorithm" << endl;

    int frames, n, pages[100], c5, dist[100];

    // Read the number of frames
    cout << "Enter the number of frames: ";
    cin >> frames;

    // Read the number of inputs
    cout << "Enter the number of inputs: ";
    cin >> n;

    // Read the reference pages serially
    cout << "Enter the reference pages serially" << endl;
    for (int i = 0; i < n; i++)
        cin >> pages[i];

    // Find the number of distinct pages and store them in the 'dist' array
    int d = distinct(pages, n, frames, &c5, dist);

    // Sort the 'dist' array in ascending order
    sort(dist, dist + d);

    // Create a map to store the index of each distinct page in the 'dist' array
    unordered_map<int, int> map;
    for (int i = 0; i < d; i++)
        map[dist[i]] = i;

    int faults = 1;
    int matrix[frames + 1][n];
    memset(matrix, -1, sizeof(matrix));

    // Initialize the first element in the matrix with the first page reference
    matrix[0][0] = pages[0];

    // Set the last element in the matrix as a marker for the first page reference
    matrix[frames][0] = -3;

    // Create a set to keep track of the pages in the frames
    unordered_set<int> s{pages[0]};

    int si = 0;
    for (int j = 1; j < n; j++)
    {
        int temp[frames];
        int x = pages[j];

        // Copy the previous column of the matrix to the current column
        for (int i = 0; i < frames; i++)
            matrix[i][j] = matrix[i][j - 1];

        // Check if the current page is not present in the set
        if (s.find(x) == s.end())
        {
            // If the number of page references is less than or equal to c5
            if (j <= c5)
            {
                // Increment the frame index and insert the page into the frame
                si++;
                matrix[si][j] = x;
                s.insert(x);
            }
            else
            {
                // Copy the previous column of the matrix to the temporary array
                for (int i = 0; i < frames; i++)
                    temp[i] = matrix[i][j - 1];

                // Find the position where the new page should be inserted using the 'func' function
                int pos = func(temp, dist, x, d, n, map);

                // Insert the new page into the frame at the determined position
                matrix[pos][j] = x;

                // Update the set of pages in the frame by adding the new page and removing the replaced page
                s.insert(x);
                s.erase(temp[pos]);
            }

            // Increment the page fault count
            faults++;

            // Set the last element in the matrix as a marker for a page fault
            matrix[frames][j] = -3;
        }
        else
        {
            // Set the last element in the matrix as a marker for a page hit
            matrix[frames][j] = -2;
        }
    }

    // Print the matrix
    for (int i = 0; i < frames + 1; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (matrix[i][j] == -1)
                cout << "  ";
            else if (matrix[i][j] == -2)
                cout << "H ";
            else if (matrix[i][j] == -3)
                cout << "M ";
            else
                cout << matrix[i][j] << " ";
        }
        cout << "\n";
    }

    // Print the number of page faults
    cout << "Page faults: " << faults << endl;
    return 0;
}

// Function to determine the position of the frame where a new page will be inserted in case of a page fault
int func(int temp[], int dist[], int x, int d, int n, unordered_map<int, int> &map)
{
    int xp = map[x];
    int a[n], b[n];
    for (int i = 0; i < n; i++)
    {
        int p = map[temp[i]];
        a[i] = (d + p - xp) % d;
        b[i] = (d + xp - p) % d;
    }
    int m = -1, pos = -1;
    for (int i = 0; i < n; i++)
    {
        int mp = min(a[i], b[i]);
        if (mp >= m)
        {
            if (mp == m)
            {
                if (b[i] < b[pos])
                    pos = i;
            }
            else
                pos = i;
            m = mp;
        }
    }
    return pos;
}

// Function to find the number of distinct pages
int distinct(int array[], int n, int n5, int *c5, int dist[])
{
    unordered_set<int> s;
    bool ind = true;
    int res = 0;
    for (int i = 0; i < n; i++)
    {
        if (s.find(array[i]) == s.end())
        {
            s.insert(array[i]);
            dist[res] = array[i];
            res++;
        }
        if (ind && res == n5)
        {
            *c5 = i;
            ind = false;
        }
    }
    return res;
}

// Function to find the maximum element in an array
int maxm(int array[], int n)
{
    int m = array[0];
    for (int i = 1; i < n; i++)
    {
        if (array[i] > m)
        {
            m = array[i];
        }
    }
    return m;
}

/*
Input/Output:
1. 
    This is LDF page replacement algorithm
    Enter the number of frames: 5
    Enter the number of inputs: 15
    Enter the reference pages serially
    3 2 1 4 5 3 2 1 4 5 3 2 1 4 5
    3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 
      2 2 2 2 2 2 2 2 2 2 2 2 2 2 
        1 1 1 1 1 1 1 1 1 1 1 1 1 
          4 4 4 4 4 4 4 4 4 4 4 4 
            5 5 5 5 5 5 5 5 5 5 5 
    M M M M M H H H H H H H H H H 
    Page faults: 5

2. 
    This is LDF page replacement algorithm
    Enter the number of frames: 4
    Enter the number of inputs: 8
    Enter the reference pages serially
    1 2 5 3 6 3 2 8   
    1 1 1 1 1 1 1 1 
      2 2 2 6 6 2 2 
        5 5 5 5 5 5 
          3 3 3 3 8 
    M M M M M H M M 
    Page faults: 7
*/