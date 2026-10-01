#include <bits/stdc++.h>
using namespace std;
int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n,m,q; cin>>n>>m>>q;
    vector<int> head(n+1,-1),to(2*m),next(2*m); int ec=0;
    auto edge=[&](int u,int v) {to[ec]=v; next[ec]=head[u]; head[u]=ec++;};
    for(int i=0,u,v;i<m;i++) {cin>>u>>v; edge(u,v); edge(v,u);}
    vector<int> degree(n+1),xr(n+1),f(n+1),limit(n+1);
    vector<char> active(n+1); int bad=0;
    auto adjust=[&](int u,int delta) {bad-=(f[u]>=3); f[u]+=delta; bad+=(f[u]>=3);};
    auto insert=[&](int u) {
        active[u]=1;
        for(int e=head[u];e!=-1;e=next[e]) {
            int v=to[e]; if(!active[v]) continue;
            if(degree[v]==1) {adjust(xr[v],1); adjust(u,1);}
            else if(degree[v]>=2) adjust(u,1);
            ++degree[v]; xr[v]^=u; ++degree[u]; xr[u]^=v;
        }
        if(degree[u]>=2) for(int e=head[u];e!=-1;e=next[e]) if(active[to[e]]) adjust(to[e],1);
    };
    auto erase=[&](int u) {
        bad-=(f[u]>=3); active[u]=0;
        for(int e=head[u];e!=-1;e=next[e]) {
            int v=to[e]; if(!active[v]) continue;
            if(degree[u]>=2) adjust(v,-1);
            if(degree[v]==2) adjust(xr[v]^u,-1);
            --degree[v]; xr[v]^=u;
        }
        degree[u]=xr[u]=f[u]=0;
    };
    int r=0;
    for(int l=1;l<=n;l++) {
        while(r<n && bad==0) insert(++r);
        limit[l]=r-(bad!=0);
        erase(l);
    }
    while(q--) {int l,r; cin>>l>>r; cout<<(r<=limit[l]?"Yes":"No")<<'\n';}
}
