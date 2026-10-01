#include <bits/stdc++.h>
using namespace std;
struct Update {int pos,time;};
struct Query {int pos,l,r,id;};
// Position divide-and-conquer; each node stores all time intervals [i,j).
struct Block {
    struct Node {int l,r,mid,left=-1,right=-1,u; vector<int> lp,rp,table;};
    vector<Node> nodes; vector<int> pref,weight;
    Block(const vector<int>& w,const vector<int>& positions):weight(w) {
        pref.resize(w.size()+1); for(int i=0;i<(int)w.size();i++) pref[i+1]=pref[i]+w[i];
        build(0,w.size()-1,positions);
    }
    int build(int l,int r,const vector<int>& events) {
        int id=nodes.size(); nodes.emplace_back();
        int u=events.size(),mid=l;
        vector<int> cnt(r-l+1,1); for(int p:events) ++cnt[p-l];
        int target=(u+r-l+1)/2,acc=0;
        while(mid<r && acc+cnt[mid-l]<=target) {acc+=cnt[mid-l]; ++mid;}
        nodes[id].l=l; nodes[id].r=r; nodes[id].mid=mid; nodes[id].u=u;
        vector<int> le,re,lp(u+1),rp(u+1);
        for(int i=0;i<u;i++) {
            if(events[i]<mid) le.push_back(events[i]);
            if(events[i]>mid) re.push_back(events[i]);
            lp[i+1]=le.size(); rp[i+1]=re.size();
        }
        int left=-1,right=-1;
        if(l<mid) left=build(l,mid-1,le);
        if(mid<r) right=build(mid+1,r,re);
        auto &z=nodes[id]; z.left=left; z.right=right; z.lp=move(lp); z.rp=move(rp);
        z.table.resize((u+1)*(u+1));
        int wl=pref[mid]-pref[l];
        for(int i=0;i<=u;i++) for(int j=i;j<=u;j++) {
            int li=z.lp[i],lj=z.lp[j],ri=z.rp[i],rj=z.rp[j],v=0;
            if(left!=-1) {auto &ch=nodes[left]; v=ch.table[li*(ch.u+1)+lj];}
            if(((j-i)-(lj-li))&1) v=wl-v;
            if(((j-i)-(lj-li))&1) v+=weight[mid];
            if(right!=-1) {auto &ch=nodes[right]; v+=ch.table[ri*(ch.u+1)+rj];}
            z.table[i*(u+1)+j]=v;
        }
        return id;
    }
    int get(int id,int i,int j,int x) {
        if(id==-1) return 0;
        auto &z=nodes[id]; if(x<z.l) return 0;
        if(x>=z.r) return z.table[i*(z.u+1)+j];
        int li=z.lp[i],lj=z.lp[j],ri=z.rp[i],rj=z.rp[j];
        int v=get(z.left,li,lj,x);
        if(((j-i)-(lj-li))&1) v=pref[min(x+1,z.mid)]-pref[z.l]-v;
        if(x>=z.mid) {
            if(((j-i)-(lj-li))&1) v+=weight[z.mid];
            v+=get(z.right,ri,rj,x);
        }
        return v;
    }
};
int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n,m,q; cin>>n>>m>>q;
    vector<int> parent(n+1),w(n+1),o(m+1),x(q),l(q),r(q),mass(n+1,1),heavy(n+1),top(n+1),pos(n+1);
    vector<vector<int>> children(n+1),chain(n+1);
    for(int i=2;i<=n;i++) {cin>>parent[i]; children[parent[i]].push_back(i);}
    for(int i=1;i<=n;i++) cin>>w[i];
    for(int i=1;i<=m;i++) {cin>>o[i]; ++mass[o[i]];}
    for(int i=0;i<q;i++) {cin>>l[i]>>r[i]>>x[i]; ++mass[x[i]];}
    // Include update/query endpoints in subtree mass for the heavy-child choice.
    for(int u=n;u>=1;u--) for(int v:children[u]) {mass[u]+=mass[v]; if(!heavy[u] || mass[v]>mass[heavy[u]]) heavy[u]=v;}
    for(int u=1;u<=n;u++) {
        top[u]=(parent[u] && heavy[parent[u]]==u?top[parent[u]]:u);
        pos[u]=chain[top[u]].size(); chain[top[u]].push_back(u);
    }
    vector<vector<Update>> updates(n+1); vector<vector<Query>> queries(n+1);
    for(int i=1;i<=m;i++) for(int u=o[i];u;u=parent[top[u]]) updates[top[u]].push_back({pos[u],i});
    for(int i=0;i<q;i++) for(int u=x[i];u;u=parent[top[u]]) queries[top[u]].push_back({pos[u],l[i],r[i],i});
    vector<int> ans(q); const int B=1800;
    for(int h=1;h<=n;h++) if(!chain[h].empty() && !queries[h].empty()) {
        int len=chain[h].size(); vector<int> load(len,1),prefix(len+1);
        for(auto z:updates[h]) ++load[z.pos];
        vector<int> chainTimes;
        for(auto z:updates[h]) chainTimes.push_back(z.time);
        for(auto &z:queries[h]) {
            z.l=lower_bound(chainTimes.begin(),chainTimes.end(),z.l)-chainTimes.begin();
            z.r=upper_bound(chainTimes.begin(),chainTimes.end(),z.r)-chainTimes.begin();
        }
        sort(queries[h].begin(),queries[h].end(),[](const Query &a,const Query &b) {return a.pos>b.pos;});
        vector<int> localRank(chainTimes.size()+1);
        vector<unsigned char> aboveParity(chainTimes.size()+1);
        for(auto z:queries[h]) ++load[z.pos];
        for(int i=0;i<len;i++) prefix[i+1]=prefix[i]+w[chain[h][i]];
        vector<pair<int,int>> ranges;
        for(int a=0;a<len;) {
            int b=a,total=load[a];
            while(b+1<len && total+load[b+1]<=B) total+=load[++b];
            ranges.push_back({a,b}); a=b+1;
        }
        vector<int> which(len); for(int i=0;i<(int)ranges.size();i++) for(int p=ranges[i].first;p<=ranges[i].second;p++) which[p]=i;
        vector<vector<Update>> local(ranges.size());
        for(auto z:updates[h]) local[which[z.pos]].push_back(z);
        for(int bi=(int)ranges.size()-1;bi>=0;bi--) {
            auto [a,b]=ranges[bi]; auto &ev=local[bi];
            vector<int> positions,weights;
            for(auto z:ev) positions.push_back(z.pos-a);
            for(int i=0;i<(int)updates[h].size();i++) {
                int p=updates[h][i].pos;
                localRank[i+1]=localRank[i]+(a<=p && p<=b);
                aboveParity[i+1]=aboveParity[i]^(p>b);
            }
            for(int p=a;p<=b;p++) weights.push_back(w[chain[h][p]]);
            unique_ptr<Block> block; if(a!=b) block=make_unique<Block>(weights,positions);
            // Most queries cover the entire block: bypass the recursive prefix query.
            int qi=0,fullSum=prefix[b+1]-prefix[a],dimension=ev.size()+1;
            const int *table=block?block->nodes[0].table.data():nullptr;
            for(;qi<(int)queries[h].size() && queries[h][qi].pos>=b;qi++) {
                const auto &z=queries[h][qi];
                int i=localRank[z.l],j=localRank[z.r];
                int value=table?table[i*dimension+j]:((j-i)&1)*weights[0];
                if(aboveParity[z.r]^aboveParity[z.l]) value=fullSum-value;
                ans[z.id]+=value;
            }
            for(;qi<(int)queries[h].size() && queries[h][qi].pos>=a;qi++) {
                const auto &z=queries[h][qi];
                int i=localRank[z.l],j=localRank[z.r],sum=prefix[z.pos+1]-prefix[a];
                int value=block->get(0,i,j,z.pos-a);
                if(aboveParity[z.r]^aboveParity[z.l]) value=sum-value;
                ans[z.id]+=value;
            }
        }
    }
    for(int v:ans) cout<<v<<'\n';
}
