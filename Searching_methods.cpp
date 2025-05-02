//#include <bits/stdc++.h>
#include <iostream>
#include <vector>

using namespace std;

int sequentialSearch(vector<int>& v, int target)
{
    for (int i = 0; i < v.size(); i++) {
        if (target == v[i]) return i;
    }
    cout << "the target is not found\n";
    return -1;
}

int recursiveSequentialSearch(vector<int>& v, int target, int index = 0)
{
    if (index == v.size()) {
        cout << "the target is not found\n";
        return -1;
    }
    if (v[index] == target) return index;
    return recursiveSequentialSearch(v, target, ++index);
}

int binarySearch(vector<int>& v, int target, int l, int r)
{
    while (l <= r) {
        int mid = l + (r - l) / 2;
        if (v[mid] == target) return mid;
        else if (v[mid] < target)l = ++mid;
        else r = --mid;
    }
    cout << "the target is not found\n";
    return -1;
}

int recursiveBinarySearch(vector<int>& v, int target, int l, int r)
{
    if (l > r) {
        cout << "the target is not found\n";
        return -1;
    }
    int mid = l + (r - l) / 2;
    if (v[mid] == target) return mid;
    if (v[mid] < target) return recursiveBinarySearch(v, target, ++mid, r);
    return recursiveBinarySearch(v, target, l, --mid);
}



int main()
{
    vector <int> v = { 1, 3, 5, 6, 10 , 11, 12, 12, 16 };
    cout << sequentialSearch(v, 4) << '\n';
    cout << recursiveSequentialSearch(v, 6) << '\n';
    cout << binarySearch(v, 11, 0, v.size() - 1) << '\n';
    cout << recursiveBinarySearch(v, 1, 0, v.size() - 1) << '\n';
    return 0;
}
