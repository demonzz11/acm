#!/usr/bin/env python3
"""C++17 builds, official samples, independent oracles, real G interaction, stress tests.

Requires only Python 3.10+ and g++. No judge login or online submission is performed.
"""
from __future__ import annotations
import argparse
from collections import deque
from concurrent.futures import ThreadPoolExecutor
from fractions import Fraction
from functools import lru_cache
from heapq import heappop, heappush
from itertools import combinations, combinations_with_replacement, permutations
import json
import math
from pathlib import Path
import queue
import random
import shutil
import subprocess
import sys
import tempfile
import threading
import time

ROOT = Path(__file__).resolve().parent
sys.path.insert(0, str(ROOT / 'tests'))
from meteorite_oracle import brute as meteorite_brute
MOD = 998244353
RNG = random.Random(20241865)
BIN = None
COUNTS = {}

def run(letter, data, timeout=90):
    result = subprocess.run([str(BIN / (letter + '.exe'))], input=data, text=True,
                            capture_output=True, timeout=timeout)
    assert result.returncode == 0, (letter, result.returncode, result.stderr)
    return result.stdout.split()

def batch(letter, cases, expected):
    actual = run(letter, str(len(cases)) + '\n' + '\n'.join(cases) + '\n')
    wanted = list(map(str, expected))
    assert actual == wanted, (letter, next(((i, a, b) for i, (a, b) in enumerate(zip(actual, wanted)) if a != b), None), len(actual), len(wanted))
    COUNTS[letter] = len(cases)

def samples():
    cases = json.loads((ROOT / 'tests/samples.json').read_text(encoding='utf-8'))
    for letter, items in cases.items():
        for data, expected in items:
            actual = run(letter, data)
            if expected is None:
                continue  # B constructions are checked independently below.
            if letter == 'K':
                wanted = list(map(float, expected.split()))
                assert len(actual) == len(wanted)
                assert all(abs(float(a)-b) <= 1e-6*max(1, abs(b)) for a,b in zip(actual,wanted))
            else:
                assert actual == expected.split(), (letter, actual, expected)
    print('Official static samples: all 12 non-interactive problems passed.', flush=True)

def check_A():
    cases, expected = [], []
    for n in range(1,7):
        for m in range(1,9):
            count = 0
            for lengths in combinations_with_replacement(range(1,m+1), n):
                lengths = lengths[::-1]
                for mask in range(1 << (n-1)):
                    height = lengths[0] + sum(lengths[i+1] for i in range(n-1) if mask >> i & 1)
                    count += height == m
            cases.append(f'{n} {m}'); expected.append(count)
    batch('A', cases, expected)

def check_B():
    cases = [(n,m) for n in range(1,36) for m in range(1,36)]
    tokens = iter(run('B', str(len(cases))+'\n'+'\n'.join(f'{n} {m}' for n,m in cases)+'\n'))
    for n,m in cases:
        verdict = next(tokens)
        if verdict == 'No':
            assert math.gcd(n,m) > 1
        else:
            assert verdict == 'Yes' and math.gcd(n,m) == 1
            a = [int(next(tokens)) for _ in range(n)]
            b = [int(next(tokens)) for _ in range(m)]
            assert all(0 <= x < n*m for x in a+b)
            assert len({x*y%(n*m) for x in a for y in b}) == n*m
    assert next(tokens, None) is None
    COUNTS['B'] = len(cases)

def check_C():
    cases, expected = [], []
    # Include failures, but also force viable initial positions in most random games.
    games = []
    for _ in range(1600):
        n,m = RNG.randrange(1,6), RNG.randrange(1,6)
        a = [[RNG.choice([0,0,0,1,2,3]) for _ in range(n)] for _ in range(m)]
        c = [RNG.randrange(2) for _ in range(n)]
        if not any(c): c[RNG.randrange(n)] = 1
        if RNG.random() < .8:
            for i in range(n):
                if c[i]: a[0][i] = 0
            for row in a:
                if all(row): row[RNG.randrange(n)] = 0
        games.append((a,c))
    for bits in range(1<<9):
        a = [[bits >> (t*3+i) & 1 for i in range(3)] for t in range(3)]
        for mask in (1,3,7): games.append((a,[mask>>i&1 for i in range(3)]))
    for a,c in games:
        cases.append(f'{len(c)} {len(a)}\n'+' '.join(map(str,c))+'\n'+'\n'.join(' '.join(map(str,row)) for row in a))
        expected.append(meteorite_brute(a,c))
    batch('C', cases, expected)

@lru_cache(None)
def game_winner(a,b,turn):
    current = a if turn == 0 else b
    before = sum(x*y for x,y in zip(a,b))
    for i in range(len(a)):
        for j in range(i):
            nxt = list(current); nxt[i],nxt[j] = nxt[j],nxt[i]
            aa,bb = (tuple(nxt),b) if turn == 0 else (a,tuple(nxt))
            if sum(x*y for x,y in zip(aa,bb)) > before:
                if game_winner(aa,bb,1-turn) == turn:
                    return turn
    return 1-turn

def check_D():
    cases, expected = [], []
    for _ in range(1200):
        n=RNG.randrange(1,6); a=RNG.sample(range(1,n+1),n);b=RNG.sample(range(1,n+1),n)
        text=f'{n}\n'+ ' '.join(map(str,a))+'\n'+' '.join(map(str,b))
        answer='AB'[game_winner(tuple(a),tuple(b),0)]
        for _ in range(n-1):
            t=RNG.randrange(2);l=RNG.randrange(n);r=RNG.randrange(l,n);d=RNG.randrange(1,n+1)
            text+=f'\n{"AB"[t]} {l+1} {r+1} {d}'
            p=a if t==0 else b;sub=p[l:r+1];shift=d%len(sub);p[l:r+1]=sub[shift:]+sub[:shift]
            answer+='AB'[game_winner(tuple(a),tuple(b),0)]
        cases.append(text);expected.append(answer)
    batch('D',cases,expected)

def operations(cost):
    return [(1<<i,cost[0]) for i in range(4)]+[(3,cost[1]),(12,cost[1]),(5,cost[2]),(10,cost[2]),(15,cost[3])]

def light_brute(initial,cost):
    targets={v^15:i for i,v in enumerate(initial)}
    full=(1<<len(initial))-1;dist={(0,0):0};heap=[(0,0,0)]
    while heap:
        d,visited,cur=heappop(heap)
        if dist[visited,cur]!=d:continue
        if visited==full:return d
        for op,w in operations(cost):
            nxt=cur^op;mask=visited|(1<<targets[nxt] if nxt in targets else 0)
            if d+w<dist.get((mask,nxt),10**18):
                dist[mask,nxt]=d+w;heappush(heap,(d+w,mask,nxt))

def grid_text(v):
    s=''.join(str(v>>i&1) for i in range(4))
    return s[:2]+'\n'+s[2:]

def check_E():
    count=0
    for _ in range(5):
        costs=[RNG.randrange(1,15) for _ in range(4)]
        cases=[RNG.sample(range(16),RNG.randrange(1,7)) for _ in range(45)]
        data=f'{len(cases)} '+' '.join(map(str,costs))+'\n'
        for s in cases:data+=str(len(s))+'\n'+'\n'.join(map(grid_text,s))+'\n'
        actual=list(map(int,run('E',data)))
        expected=[light_brute(s,costs) for s in cases]
        assert actual==expected,(costs,actual,expected)
        count+=len(cases)
    COUNTS['E']=count

def cube_edges(n,cost):
    edges=[]
    for free in range(1<<n):
        fixed=((1<<n)-1)^free;sub=fixed
        while True:
            face=0
            for vertex in range(1<<n):
                if vertex&fixed==sub:face|=1<<vertex
            edges.append((face,cost[free]))
            if sub==0:break
            sub=(sub-1)&fixed
    return edges

def cube_reference(n,cost):
    edges=cube_edges(n,cost);states=1<<(1<<n)
    # Prim on the entire state graph: independent of SOS and implicit pivot counts.
    best=[10**18]*states;seen=[False]*states;best[0]=0;mst=0
    for _ in range(states):
        u=min((i for i in range(states) if not seen[i]),key=best.__getitem__)
        seen[u]=True;mst+=best[u]
        for mask,w in edges:best[u^mask]=min(best[u^mask],w)
    # Construct a valid positive-time visit of every state using explicit cheapest basis.
    pivots={};basis=[]
    for vector,w in sorted(edges,key=lambda e:e[1]):
        v=vector
        while v and v.bit_length()-1 in pivots:v^=pivots[v.bit_length()-1]
        if v:pivots[v.bit_length()-1]=v;basis.append((vector,w))
    assert len(basis)==1<<n
    cur=basis[0][0];visited={cur};total=basis[0][1]
    for step in range(1,states):
        bit=(step&-step).bit_length()-1;vector,w=basis[bit];cur^=vector;total+=w;visited.add(cur)
    assert len(visited)==states and total==mst+min(cost)
    return total%MOD

def check_F():
    cases,expected=[],[]
    for n in (2,3):
        for _ in range(15):
            cost=[RNG.randrange(1,20) for _ in range(1<<n)]
            cases.append(str(n)+'\n'+' '.join(map(str,cost)))
            expected.append(cube_reference(n,cost))
    batch('F',cases,expected)

def hull(points):
    points=sorted(set(points))
    def cross(a,b,c):return (b[0]-a[0])*(c[1]-a[1])-(b[1]-a[1])*(c[0]-a[0])
    low=[]
    for p in points:
        while len(low)>1 and cross(low[-2],low[-1],p)<=0:low.pop()
        low.append(p)
    high=[]
    for p in reversed(points):
        while len(high)>1 and cross(high[-2],high[-1],p)<=0:high.pop()
        high.append(p)
    return low[:-1]+high[:-1]

def cross_section(poly,x):
    ys=[]
    for a,b in zip(poly,poly[1:]+poly[:1]):
        if a[0]==b[0]:continue
        if min(a[0],b[0])<=x<max(a[0],b[0]):
            ys.append(Fraction(a[1])+Fraction(b[1]-a[1],b[0]-a[0])*(x-a[0]))
    ys.sort();assert len(ys)%2==0
    return sum((ys[i+1]-ys[i] for i in range(0,len(ys),2)),Fraction(0))

def interact_G(polygons):
    p=subprocess.Popen([str(BIN/'G.exe')],stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True,bufsize=1)
    output=queue.Queue()
    def reader():
        for line in p.stdout:output.put(line)
        output.put(None)
    threading.Thread(target=reader,daemon=True).start()
    def send(text):p.stdin.write(text);p.stdin.flush()
    send(str(len(polygons))+'\n')
    try:
        for poly in polygons:
            shuffled=RNG.sample(poly,len(poly));send(str(len(poly))+'\n'+'\n'.join(f'{x} {y}' for x,y in shuffled)+'\n')
            queries=0
            area=abs(sum(a[0]*b[1]-a[1]*b[0] for a,b in zip(poly,poly[1:]+poly[:1])))
            while True:
                line=output.get(timeout=10);assert line is not None,'G exited prematurely'
                words=line.split();assert len(words)==3,line
                a,b=map(int,words[1:])
                if words[0]=='?':
                    queries+=1;assert queries<=len(poly)-2
                    assert 1<=b<=1000 and 0<=Fraction(a,b)<=1000
                    result=cross_section(poly,Fraction(a,b));send(f'{result.numerator} {result.denominator}\n')
                else:
                    assert words[0]=='!' and b>0 and math.gcd(a,b)==1
                    assert Fraction(a,b)==Fraction(area,2),(poly,line,area)
                    break
        p.stdin.close();assert p.wait(timeout=10)==0
    finally:
        if p.poll() is None:p.kill()

def check_G():
    polygons=[[(0,0),(1,1),(1,3),(3,0)],[(0,0),(999,1000),(1000,999)],
              [(0,0),(7,0),(7,2),(3,2),(3,7),(0,7)],
              [(0,0),(9,0),(9,9),(6,9),(6,3),(3,3),(3,9),(0,9)]]
    for _ in range(70):
        p=hull([(RNG.randrange(1001),RNG.randrange(1001)) for _ in range(RNG.randrange(4,50))])
        if len(p)>=3:polygons.append(p)
    # Distinct x; large rational denominators and many independent edge slopes.
    chain=[(x,(x*x)%997) for x in range(1,151)]
    # Use convex hulls for guaranteed simple extra cases; fixed concave cases are above.
    polygons.append(hull(chain))
    chunk=[];size=0
    for p in polygons:
        if size+len(p)>1000:interact_G(chunk);chunk=[];size=0
        chunk.append(p);size+=len(p)
    if chunk:interact_G(chunk)
    COUNTS['G']=len(polygons)

def guide_brute(n,scenic):
    pairs=list(combinations(range(n),2));fixed=sum(1<<pairs.index(tuple(sorted(e))) for e in scenic)
    optional=[i for i in range(len(pairs)) if not(fixed>>i&1)]
    answer=0
    for mask in range(1<<len(optional)):
        chosen=fixed
        for j,i in enumerate(optional):
            if mask>>j&1:chosen|=1<<i
        g=[[] for _ in range(n)]
        for i,(u,v) in enumerate(pairs):
            if chosen>>i&1:g[u].append((v,i));g[v].append((u,i))
        seen={0};tree=0
        def dfs(u):
            nonlocal tree
            for v,i in sorted(g[u]):
                if v not in seen:seen.add(v);tree|=1<<i;dfs(v)
        dfs(0)
        answer+=(tree&fixed)==fixed and (tree&~fixed).bit_count()<=1
    return answer

def check_H():
    count=0
    for n in range(2,7):
        for _ in range(18):
            edges=[(i,RNG.randrange(i)) for i in range(1,n)];edges.pop(RNG.randrange(n-1))
            perm=RNG.sample(range(n),n);edges=[(perm[u],perm[v]) for u,v in edges]
            data=str(n)+'\n'+'\n'.join(f'{u+1} {v+1}' for u,v in edges)+'\n'
            assert run('H',data)==[str(guide_brute(n,edges))],(n,edges)
            count+=1
    COUNTS['H']=count

def pruning_brute(n,a):
    base=1<<n;dist=[0]*(2*base);depth=[0]*(2*base)
    for v in range(2,2*base):dist[v]=dist[v//2]+a[v-2];depth[v]=depth[v//2]+1
    edges=list(range(2,2*base))
    for count in range(n+1):
        for selected in combinations(edges,count):
            release=sorted(depth[v] for v in selected)
            # Assign chosen edges to the latest count available days.
            if any(d>n-count+i+1 for i,d in enumerate(release)):continue
            signatures=[]
            for leaf in range(base,2*base):
                ancestors=set();u=leaf
                while u>1:ancestors.add(u);u//=2
                signatures.append((dist[leaf],tuple(v in ancestors for v in selected)))
            if len(set(signatures))==base:return count
    return -1

def check_I():
    cases,expected=[],[]
    for n in range(1,4):
        for _ in range(160):
            a=[RNG.randrange(1,7) for _ in range((1<<(n+1))-2)]
            cases.append(str(n)+'\n'+' '.join(map(str,a)));expected.append(pruning_brute(n,a))
    for _ in range(24):
        n=4;a=[RNG.randrange(1,31) for _ in range(30)]
        cases.append(str(n)+'\n'+' '.join(map(str,a)));expected.append(pruning_brute(n,a))
    batch('I',cases,expected)

def check_J():
    for _ in range(24):
        a=RNG.sample(range(1,101),8);players=list(range(8));runner=None
        while len(players)>1:
            if len(players)==2:runner=min(players,key=lambda i:a[i])
            players=[max(players[i:i+2],key=lambda j:a[j]) for i in range(0,len(players),2)]
        expected=[f'T{players[0]}','beats',f'T{runner}']
        assert run('J','\n'.join(f'T{i} {a[i]}' for i in range(8))+'\n')==expected
    COUNTS['J']=24

def pinball_sample(poly,samples=800):
    n=len(poly);best=[0.]*(n+1)
    def cross(a,b):return a[0]*b[1]-a[1]*b[0]
    def sub(a,b):return a[0]-b[0],a[1]-b[1]
    for _ in range(samples):
        # Convex combinations ensure the starting point lies inside.
        weights=[RNG.random() for _ in poly];total=sum(weights)
        p=tuple(sum(w*v[i] for w,v in zip(weights,poly))/total for i in (0,1))
        angle=RNG.random()*2*math.pi;d=math.cos(angle),math.sin(angle);used=set();distance=0
        for k in range(n+1):
            hit=None
            for i in range(n):
                a,b=poly[i],poly[(i+1)%n];e=sub(b,a);den=cross(d,e)
                if abs(den)<1e-12:continue
                ap=sub(a,p);t=cross(ap,e)/den;s=cross(ap,d)/den
                if t>1e-8 and -1e-9<=s<=1+1e-9 and (hit is None or t<hit[0]):hit=t,i
            if hit is None:break
            t,i=hit;distance+=t;best[k]=max(best[k],distance)
            if i in used:break
            used.add(i);p=p[0]+t*d[0],p[1]+t*d[1]
            e=sub(poly[(i+1)%n],poly[i]);factor=2*(d[0]*e[0]+d[1]*e[1])/(e[0]*e[0]+e[1]*e[1])
            d=factor*e[0]-d[0],factor*e[1]-d[1]
    for i in range(1,n+1):best[i]=max(best[i],best[i-1])
    return best

def check_K():
    polygons=[[(0,0),(5,0),(5,3),(0,3)],[(4,0),(0,3),(0,-1)],
              [(-7,0),(-3,-5),(4,-6),(8,1),(2,7),(-4,6)]]
    for _ in range(5):
        p=hull([(RNG.randrange(-20,21),RNG.randrange(-20,21)) for _ in range(8)])
        if 3<=len(p)<=6 and all((b[0]-a[0])*(c[1]-a[1])!=(b[1]-a[1])*(c[0]-a[0]) for a,b,c in combinations(p,3)):
            polygons.append(p)
    for poly in polygons:
        data=lambda p:str(len(p))+'\n'+'\n'.join(f'{x} {y}' for x,y in p)+'\n'
        actual=list(map(float,run('K',data(poly))))
        diameter=max(math.dist(a,b) for a in poly for b in poly)
        assert abs(actual[0]-diameter)<1e-7
        assert all(actual[i]+1e-7>=actual[i-1] for i in range(1,len(actual)))
        assert all(a<=(i+1)*diameter+1e-7 for i,a in enumerate(actual))
        lower=pinball_sample(poly)
        assert all(a+1e-7>=b for a,b in zip(actual,lower)),(poly,actual,lower)
        transformed=[(2*y+17,-2*x-11) for x,y in poly]
        other=list(map(float,run('K',data(transformed))))
        assert all(abs(b-2*a)<2e-6*max(1,b) for a,b in zip(actual,other))
    COUNTS['K']=len(polygons)*2

def standings(submissions,p,L=None,R=None):
    solved=[set(),set()];score=[0,0];wrong={}
    for team,problem,t,verdict in submissions:
        if problem in solved[team]:continue
        key=team,problem
        if verdict:
            solved[team].add(problem)
            if L is not None:t=t-max(0,min(t,R)-L)
            score[team]+=t+p*wrong.get(key,0)
        else:wrong[key]=wrong.get(key,0)+1
    return min(range(2),key=lambda i:(-len(solved[i]),score[i],i))

def contest_brute(submissions,p):
    original=standings(submissions,p);end=max(s[2] for s in submissions)
    for length in range(1,end+1):
        for l in range(end-length+1):
            if standings(submissions,p,l,l+length)!=original:return f'{l} {l+length}'
    return '-1'

def check_L():
    cases,expected=[],[]
    for _ in range(5000):
        n=RNG.randrange(1,23);p=RNG.randrange(1,11);subs=[]
        times=sorted(RNG.randrange(1,16) for _ in range(n))
        for t in times:subs.append((RNG.randrange(2),RNG.randrange(1,4),t,RNG.randrange(2)))
        cases.append(f'{n} {p}\n'+'\n'.join(f'{a+1} {b} {t} {d}' for a,b,t,d in subs))
        expected.append(contest_brute(subs,p))
    actual=run('L',str(len(cases))+'\n'+'\n'.join(cases)+'\n')
    wanted=' '.join(expected).split();assert actual==wanted,('L mismatch',next(((i,a,b) for i,(a,b) in enumerate(zip(actual,wanted)) if a!=b),None))
    COUNTS['L']=len(cases)

def hotel_brute(n,operations):
    g=[[] for _ in range(n)];reach=[[i==j for j in range(n)] for i in range(n)]
    for a,b in operations:g[a%n].append(((a+b)%n,b));reach[a%n][(a+b)%n]=True
    for k in range(n):
        for i in range(n):
            for j in range(n):reach[i][j]|=reach[i][k] and reach[k][j]
    bad=set()
    for start in range(n):
        def dfs(u,seen,total):
            for v,w in g[u]:
                if v==start and total+w!=0:bad.add(start)
                elif v not in seen:dfs(v,seen|{v},total+w)
        dfs(start,{start},0)
    return [any(reach[i][j] for j in bad) for i in range(n)]

def check_M():
    for _ in range(180):
        n=RNG.randrange(1,8);ops=[(RNG.randrange(-15,16),RNG.randrange(-15,16)) for _ in range(RNG.randrange(1,15))]
        queries=list(range(-n,n));data=f'{n} {len(ops)} {len(queries)}\n'+'\n'.join(f'{a} {b}' for a,b in ops)+'\n'+'\n'.join(map(str,queries))+'\n'
        possible=hotel_brute(n,ops);expected=['Yes' if possible[x%n] else 'No' for x in queries]
        assert run('M',data)==expected,(n,ops)
    COUNTS['M']=180

def stress():
    checks=[]
    def timed(label,letter,data,expected=None):
        started=time.monotonic();actual=run(letter,data,180)
        if expected is not None:assert actual==list(map(str,expected)),(label,actual[:20])
        checks.append((label,round(time.monotonic()-started,3)))
    timed('A: 2000 x 2000','A','1\n2000 2000\n',[204576309])
    n=500000;p=list(range(1,n+1));row=' '.join(map(str,p))
    timed('D: n=500000','D',f'1\n{n}\n{row}\n{row}\n'+'A 1 1 1\n'*(n-1),['B'*n])
    n=1000000
    timed('C: n=1000000,m=1','C',f'1\n{n} 1\n'+'1 '*n+'\n'+'0 '*n+'\n',[0])
    n=1000;m=1000;rows=[]
    for t in range(m):rows.append(' '.join('0' if (i+t)%7==0 else '1' for i in range(n)))
    c=' '.join('1' if i%7==0 else '0' for i in range(n))
    timed('C: 1000 x 1000 mixed','C',f'1\n{n} {m}\n{c}\n'+'\n'.join(rows)+'\n')
    timed('F: n=20 all unit costs','F','1\n20\n'+'1 '*(1<<20)+'\n',[pow(2,1<<20,MOD)])
    n=200000
    timed('H: 199999-node star + isolated vertex','H',f'{n}\n'+'\n'.join(f'1 {v}' for v in range(2,n))+'\n',[2*n-2])
    timed('H: ascending chain + isolated vertex','H',f'{n}\n'+'\n'.join(f'{v} {v+1}' for v in range(1,n-1))+'\n',[pow(2,n*(n-1)//2-(n-2),MOD)])
    n=10;timed('I: ten maximum-depth all-equal trees','I','10\n'+(str(n)+'\n'+'1 '*((1<<(n+1))-2)+'\n')*10,[-1]*10)
    n=400000;rows=[]
    for i in range(n//2-1):
        for team in (1,2):rows.append(f'{team} {i+1} {(i+1)*2000000} 1')
    rows.extend(['2 200000 999999000000 1','1 200000 1000000000000 1'])
    data=f'1\n{n} 1000000000000\n'+'\n'.join(rows)+'\n'
    timed('L: 400000 submissions, large timestamps','L',data,[999999000000,1000000000000])
    n=500000;ops='\n'.join(f'{i} 1' for i in range(n))
    timed('M: one 500000-residue nonzero cycle','M',f'{n} {n} 1\n{ops}\n0\n',['Yes'])
    for label,seconds in checks:print(f'STRESS {label}: {seconds:.3f}s',flush=True)

def main():
    global BIN
    parser=argparse.ArgumentParser();parser.add_argument('--stress',action='store_true');parser.add_argument('--original',action='store_true');parser.add_argument('--only',default='ABCDEFGHIJKLM');parser.add_argument('--cxx',default=shutil.which('g++') or 'g++');args=parser.parse_args()
    start=time.monotonic()
    with tempfile.TemporaryDirectory(prefix='shenyang1865-') as temp:
        BIN=Path(temp)
        def compile_one(source):
            result=subprocess.run([args.cxx,'-std=c++17','-O2','-Wall','-Wextra','-Wshadow',str(source),'-o',str(BIN/(source.stem+'.exe'))],capture_output=True,text=True)
            assert result.returncode==0 and not result.stderr,(source.name,result.stderr)
        with ThreadPoolExecutor(max_workers=3) as pool:list(pool.map(compile_one,sorted((ROOT/'solutions').glob('*.cpp'))))
        print('Compiled A-M with C++17; no warnings.',flush=True)
        samples()
        for letter in args.only:
            at=time.monotonic();globals()['check_'+letter]()
            print(f'{letter}: {COUNTS[letter]} independent checks passed ({time.monotonic()-at:.2f}s)',flush=True)
        if args.stress:stress()
        if args.original:
            saved_counts=COUNTS.copy();reference_bin=BIN;BIN=Path(temp)/'original';BIN.mkdir()
            for letter in 'BD':
                compile_one(ROOT/(letter+'.cpp'))
                globals()['check_'+letter]()
                print(f'Original {letter}.cpp: {COUNTS[letter]} independent checks passed.',flush=True)
            BIN=reference_bin;COUNTS.clear();COUNTS.update(saved_counts)
    print(f'PASS: {sum(COUNTS.values())} checks; elapsed {time.monotonic()-start:.2f}s. Local verification only.',flush=True)

if __name__=='__main__':main()
