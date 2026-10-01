#include <bits/stdc++.h>
using namespace std;
int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n; cin>>n; vector<int>a(n); for(int &x:a) cin>>x;
    sort(a.begin(),a.end()); int hi=0,lo=0;
    for(int x:a) hi+=(hi>=x?1:-1);
    for(auto it=a.rbegin();it!=a.rend();++it) lo+=(lo>=*it?1:-1);
    cout<<hi<<' '<<lo<<'\n';
}
