import urllib.request
import re
from bs4 import BeautifulSoup

def add(d,key,value):
    if(key in d):
        d[key].append(value)
    else:
        d.update({key:[value]})

def GenDicts(l):
    Ds = {}
    S = {}
    Dl = {}
    for i in l:
        try:
            html = urllib.request.urlopen('http://www.bgprevoz.rs/linije/red-voznje/dnevne-linije/' + str(i))
            soup = BeautifulSoup(html, "html.parser")
            tags = soup('table')
            a = re.findall(r'<tr>\n<td>(.+)</td>\n<td>(.+)</td>\n<td>(.+)</td>\n<td>(.+)</td>\n</tr>',str(tags[0]))
            for el in a:
                add(S,el[1],el[3])
                add(Dl,i,el[3])
                add(Ds,el[3],i)
            a = re.findall(r'<tr>\n<td>(.+)</td>\n<td>(.+)</td>\n<td>(.+)</td>\n<td>(.+)</td>\n</tr>',str(tags[1]))
            for el in a:
                add(S,el[1],el[3])
                add(Dl,i,el[3])
                add(Ds,el[3],i)
                    
        except:
            continue
    return Ds,Dl,S

def Distance(Dl,l,a,b):
    b1 = b2 = True
    i = 0
    while b1 or b2:
        if(Dl[l][i] == a):
            b1 = False
            x = i
        if(Dl[l][i] == b):
            b2 = False
            y = i
        i+=1
    return abs(x-y)
def FindIndirectRoute(Ds,Dl,S,a,b):
    T = {}
    res = []
    possible = False
    for el in Ds:
        T[el] = ''
    T[a] = 'start'
    Q = [a]
    P = [0]
    while(Q != []):
        for x in Ds[Q[0]]:
            for y in Dl[x]:
                if(y == b):
                    T[y] = Q[0]
                    possible = True
                    break
                elif(T[y] == '') and (P[0] <= 3):
                    P.append(P[0]+1)
                    Q.append(y)
                    T[y] = Q[0]
            if(possible):
                break
        if(possible):
            break
        del(P[0])
        del(Q[0])
    if(possible):
        x = b
        while(x != 'start'):
            t = x
            x = T[x]
            if(x != 'start'):
                res.insert(0,FindDirectRoute(Ds,Dl,S,x,t))
    return res
        
        
def FindDirectRoute(Ds,Dl,S,a,b):
    res = []
    temp = {}
    if(a == ''):
        return res
    elif(a[0] == '#'):
        x = [a]
    elif(a in S):
        x = S[a]
    else:
        return res

    if(b == ''):
        return res
    elif(b[0] == '#'):
        y = [b]
    elif(b in S):
        y = S[b]
    else:
        return res
    for i in x:
        for j in y:
            for el in Ds[i]:
                if el in Ds[j]:
                    add(temp,el,Distance(Dl,el,i,j))
    for el in temp:
        i = 0
        m = temp[el][0]
        while i<len(temp[el]):
            if(temp[el][i] < m):
                m = temp[el][i]
            i+=1
        res.append((el,m))
    if(res == []):
        for i in x:
            for j in y:
                res = FindIndirectRoute(Ds,Dl,S,i,j)
                if(res != []):
                    return res
    return res

(Ds,Dl,S) = GenDicts([str(i) for i in range(50)])
print(S)
ok = True
while ok:
    try:
        a = str(input('Pocetna: '))
        b = str(input('Krajnja: '))
        print(FindDirectRoute(Ds,Dl,S,a,b))
        if (a=='') or (b==''):
            ok = False
    except Exception as e:
        print(e)
        continue

print('eGFDBND')
