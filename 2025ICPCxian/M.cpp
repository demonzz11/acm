#include <bits/stdc++.h>
using namespace std;
using ll=long long;
const ll MOD=1000000007;
int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n,m; cin>>n>>m;
    ll total=1,one=0,large=1; bool allOne=true;
    ll choices=max(m-n+1,0);
    for(int i=0,x;i<n;i++) {
        cin>>x;
        if(x==-1) total=total*m%MOD;
        if(x!=-1 && x!=1) allOne=false;
        ll waysOne=(x==-1 || x==1),waysLarge=(x==-1?choices:(x>=n));
        if(i==0 || i==n-1) waysOne=0;
        ll nextOne=large*waysOne%MOD,nextLarge=(one+large)*waysLarge%MOD;
        one=nextOne; large=nextLarge;
    }
    ll bad=(large+(n%2 && allOne))%MOD;
    cout<<(total-bad+MOD)%MOD<<'\n';
}
