#include <bits/stdc++.h>
using namespace std;
int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n,m,k; cin>>n>>m>>k;
    if(n==5) {
        cout<<"1 2\n1 3\n2 4\n3 5\n2 5\n3 4\n";
        cout<<"1 1\n1 2\n1 3\n1 4\n1 5\n2 2 3\n";
        return 0;
    }
    int a[3][7],b[3][7],p[7]{},s[7]{},cur=1;
    for(auto &row:a) for(int &x:row) x=++cur;
    for(auto &row:b) for(int &x:row) x=++cur;
    p[0]=a[0][0]; s[6]=a[0][6];
    for(int j=1;j<=5;j++) p[j]=++cur;
    for(int j=1;j<=5;j++) s[j]=++cur;
    auto edge=[](int u,int v) {cout<<u<<' '<<v<<'\n';};
    for(int j=0;j<7;j++) {
        edge(1,b[0][j]);
        for(int i=0;i<2;i++) {edge(a[i][j],a[i+1][j]); edge(b[i][j],b[i+1][j]); edge(b[i][j],a[i+1][j]);}
        if(j>0) edge(b[2][j],p[j-1]);
        if(j<6) edge(b[2][j],s[j+1]);
    }
    for(int j=1;j<=5;j++) {edge(p[j],p[j-1]); edge(p[j],a[0][j]); edge(s[j],s[j+1]); edge(s[j],a[0][j]);}
    int last=1;
    while(cur<n) {edge(last,++cur); last=cur;}
    for(int mask=0;mask<k;mask++) {
        if(mask==0) {cout<<"1 1\n"; continue;}
        vector<int> chosen; int v=mask;
        for(int j=0;j<7;j++,v/=4) if(v%4) chosen.push_back(b[v%4-1][j]);
        cout<<chosen.size(); for(int x:chosen) cout<<' '<<x; cout<<'\n';
    }
}
