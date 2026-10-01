#include <bits/stdc++.h>
using namespace std;
struct Dinic {
    struct Edge {int v,next,cap;};
    int n; vector<int> head,level,cur; vector<Edge> edges;
    Dinic(int n):n(n),head(n,-1),level(n),cur(n) {}
    void add(int u,int v,int cap) {edges.push_back({v,head[u],cap}); head[u]=edges.size()-1; edges.push_back({u,head[v],0}); head[v]=edges.size()-1;}
    bool bfs(int s,int t) {
        fill(level.begin(),level.end(),-1); vector<int>q(n); int l=0,r=0; q[r++]=s; level[s]=0;
        while(l<r) {int u=q[l++]; for(int e=head[u];e!=-1;e=edges[e].next) if(edges[e].cap && level[edges[e].v]<0) {int v=edges[e].v; level[v]=level[u]+1; q[r++]=v;}}
        return level[t]>=0;
    }
    int dfs(int u,int t,int flow) {
        if(u==t) return flow;
        for(int &e=cur[u];e!=-1;e=edges[e].next) {auto &z=edges[e]; if(z.cap && level[z.v]==level[u]+1) {
            int sent=dfs(z.v,t,min(flow,z.cap)); if(sent) {z.cap-=sent; edges[e^1].cap+=sent; return sent;}
        }}
        return 0;
    }
    int flow(int s,int t) {int total=0; while(bfs(s,t)) {cur=head; while(int f=dfs(s,t,INT_MAX)) total+=f;} return total;}
};
int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int t; cin>>t;
    while(t--) {
        int n; cin>>n; vector<int>a(n),b(n),count(n); bool ok=true;
        for(int &x:a) cin>>x;
        for(int i=0;i<n;i++) {cin>>b[i]; ++count[b[i]]; if((a[i]&b[i])!=b[i]) ok=false;}
        if(a==b) {cout<<"Yes\n"; continue;}
        if(!ok) {cout<<"No\n"; continue;}
        int s=n,t=n+1; Dinic d(n+2);
        d.edges.reserve(2*n*10);
        for(int mask=0;mask<n;mask++) {
            d.add(s,mask,1); if(count[mask]) d.add(mask,t,count[mask]);
            for(int bits=mask;bits;bits&=bits-1) d.add(mask,mask^(bits&-bits),n);
        }
        cout<<(d.flow(s,t)==n?"Yes":"No")<<'\n';
    }
}
