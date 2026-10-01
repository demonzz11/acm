#include <bits/stdc++.h>
using namespace std;
int id(char c) { return c=='C'?0:c=='W'?1:2; }
bool feasible(const array<int,3>& cnt,int len,int left,int right) {
    if(!len) return left<0 || right<0 || left!=right;
    for(int c=0;c<3;c++) if(2*cnt[c]-len+(left==c)+(right==c)>1) return false;
    return true;
}
int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int t; cin>>t;
    while(t--) {
        int n; string s; cin>>n>>s;
        int first=n,last=-1; array<int,3> total{};
        for(int i=0;i<n;i++) {++total[id(s[i])]; if(i+1<n && s[i]==s[i+1]) {first=min(first,i); last=i;}}
        if(last==-1) {cout<<"Beautiful\n"; continue;}
        if(*max_element(total.begin(),total.end())>(n+1)/2) {cout<<"Impossible\n"; continue;}
        array<int,3> cnt{};
        int r=-1,best=n+1,L=0,R=n-1;
        for(int l=0;l<=first+1;l++) {
            while(r<max(l,last)) ++cnt[id(s[++r])];
            int left=l?id(s[l-1]):-1;
            while(!feasible(cnt,r-l+1,left,r+1<n?id(s[r+1]):-1) && r+1<n) ++cnt[id(s[++r])];
            if(feasible(cnt,r-l+1,left,r+1<n?id(s[r+1]):-1) && r-l+1<best) {best=r-l+1; L=l; R=r;}
            --cnt[id(s[l])];
        }
        cnt={}; for(int i=L;i<=R;i++) ++cnt[id(s[i])];
        int left=L?id(s[L-1]):-1,right=R+1<n?id(s[R+1]):-1;
        const string colors="CWP";
        for(int i=L;i<=R;i++) {
            for(int c=0;c<3;c++) if(cnt[c] && c!=left) {
                --cnt[c];
                if(feasible(cnt,R-i,c,right)) {s[i]=colors[c]; left=c; break;}
                ++cnt[c];
            }
        }
        cout<<"Possible\n"<<L+1<<' '<<R+1<<'\n'<<s<<'\n';
    }
}
