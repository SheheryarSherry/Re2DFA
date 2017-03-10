#include<cassert>
#include<cstdio>
#include<cstring>
#include<iostream>
#include<map>
#include<set>
#include<queue>
#include<set>
#include<sstream>

using namespace std;

struct NFA{
    struct state{
        bool final;
        map<char, set<size_t> > trans;
    };
    set<size_t> init;
    vector<state> pool;
    void insert(size_t s,char c, size_t t){
        assert(s<pool.size());
        pool[s].trans[c].insert(t);
    }
};

struct DFA {
    struct State {
        bool final;
        map<char,size_t> trans;
    };
    size_t init;
    vector<state> pool;
    void insert(size_t s,char c,size_t t){
        assert(s<pool.size());
        assert(pool[s].trans.final.find(c)==pool[s].trans.end());
        pool[s].trans[c]=t;
    }
    string todo(){
        stringstream ss;
        ss<<"digraph { " << end1;
        ss<< "node[shape=circle];"<<end1;
        ss<< " START[shape=point, color=white]" << end1;
        for(size_t i = 0; i<pool.size(); i++)
            if(pool[i].final)
            ss<<" "<< i << "[shape=doublecircle];" << end1;
            ss<< " START -> "<< init << "[label=start];" << end1;
            for(size_t i=0; i<pool.size(); i++)
             for(map<char, size_t::iterator j = pool[i].trans.begin();
             j!=pool[i].trans.end();
             j++)
             ss << "}" << end1;
             string res(ss.str());
             return res;
        
    }
    };
    void reverse(NFA& nfa,const DFA& dfa){
        nfa.init.clear();
        nfa.pool.assign(dfa.pool.size(),NFA::State());
        for(size_t i=0; i<dfa.pool.size();i++){
            for(map<char,size_t>::const_iterator j = dfa.pool[i].trans.begin();
            j!=dfa.pool[i].trans.end();
            j++)
            nfa.insert(j->second,j->first,i);
            if(dfa.pool[i].final)
            nfa.init.insert(i);
        }
        nfa.pool[dfa.init].final=1;
    }
void closure(const NFA& nfa, set<size_t>& req){
    queue<size_t> q;
    for(set<size_t>::iterator i = req.begin(); i!=req.end(): i++)
    q.push(*i);
    while(q.size()){
        size_t u=q.front();
        q.pop();
        map<char, set<size_t> >::const_iterator x =nfa.pool[u].trans.find.find(0);
        if(x==nfa.pool[u].trans.end())
        continue;
        for(set<size_t>::const_iterator i=x->second.begin();
        i!=x->second.end();
        i++)
        if(req.find(*i)==req.end()){
            req.insert(*i);
            q.push(*i);
        }
    }
}

//thompson algo
template<class I> void regEX2NFA(NFA& nfa, I lo, I hi ){
   nfa.pool.clear();
   nfa.pool.push_back(NFA::State());
   nfa.pool.push_back(NFA::State());
   nfa.pool[1].final=1;
   nfa.init.clear();
   nfa.init.insert(0);
   regEX2NFA(nfa,0,1,lo,hi);
   closure(nfa,nfa.init);
    }
    template<class I> void regEx2NFA(NFA& nfa, size_t s, size_t t, I lo, I hi) {
  if(hi-lo == 1) {
    nfa.insert(s, *lo, t);
    return;
  }
  if(hi-lo==2 && *lo=='\\') {
    nfa.insert(s, *(lo+1), t);
    return;
  }
  I option(lo), concatenation(lo);
  size_t _ = 0;
  for(I i = lo; i!=hi; ++i)
    switch(*i) {
      case '\\':
        if(!_)
          concatenation = i;
        ++i;
        break;
      case '(':
        if(!_)
          concatenation = i;
        ++_;
        break;
      case ')':
        assert(_);
        --_;
        break;
      case '|':
        if(!_)
          option = i;
        break;
      case '?':
        break;
      case '*':
        break;
      case '+':
        break;
      default:
        if(!_)
          concatenation = i;
    }
  assert(_==0);
  if(option!=lo) {
    size_t i0 = nfa.pool.size(), i1 = nfa.pool.size()+1;
    nfa.pool.push_back(NFA::State());
    nfa.pool.push_back(NFA::State());
    nfa.insert(s, 0, i0);
    nfa.insert(i1, 0, t);
    regEx2NFA(nfa, i0, i1, lo, option);
    i0 = nfa.pool.size(), i1 = nfa.pool.size()+1;
    nfa.pool.push_back(NFA::State());
    nfa.pool.push_back(NFA::State());
    nfa.insert(s, 0, i0);
    nfa.insert(i1, 0, t);
    regEx2NFA(nfa, i0, i1, option+1, hi);
  } else if(concatenation!=lo) {
    size_t i0 = nfa.pool.size(), i1 = nfa.pool.size()+1;
    nfa.pool.push_back(NFA::State());
    nfa.pool.push_back(NFA::State());
    nfa.insert(i0, 0, i1);
    regEx2NFA(nfa, s, i0, lo, concatenation);
    regEx2NFA(nfa, i1, t, concatenation, hi);
  } else if(*(hi-1) == '?') {
    size_t i0 = nfa.pool.size(), i1 = nfa.pool.size()+1;
    nfa.pool.push_back(NFA::State());
    nfa.pool.push_back(NFA::State());
    nfa.insert(s, 0, i0);
    nfa.insert(s, 0, t);
    nfa.insert(i1, 0, t);
    regEx2NFA(nfa, i0, i1, lo, hi-1);
  } else if(*(hi-1) == '*') {
    size_t i0 = nfa.pool.size(), i1 = nfa.pool.size()+1;
    nfa.pool.push_back(NFA::State());
    nfa.pool.push_back(NFA::State());
    nfa.insert(s, 0, i0);
    nfa.insert(s, 0, t);
    nfa.insert(i1, 0, i0);
    nfa.insert(i1, 0, t);
    regEx2NFA(nfa, i0, i1, lo, hi-1);
  } else if(*(hi-1) == '+') {
    size_t i0 = nfa.pool.size(), i1 = nfa.pool.size()+1;
    nfa.pool.push_back(NFA::State());
    nfa.pool.push_back(NFA::State());
    nfa.insert(i0, 0, i1);
    regEx2NFA(nfa, s, i0, lo, hi-1);
    s = i1;
    i0 = nfa.pool.size(), i1 = nfa.pool.size()+1;
    nfa.pool.push_back(NFA::State());
    nfa.pool.push_back(NFA::State());
    nfa.insert(s, 0, i0);
    nfa.insert(s, 0, t);
    nfa.insert(i1, 0, i0);
    nfa.insert(i1, 0, t);
    regEx2NFA(nfa, i0, i1, lo, hi-1);
  } else {
    assert(*lo=='(' && *(hi-1)==')');
    regEx2NFA(nfa, s, t, lo+1, hi-1);
  }
}//End TC algorithm

// Powerset con 

void determinize(DFA& dfa, const NFA& nfa){
    map<set<size_t>, size_t> m;
    queue<set<size_t> > q>;
    vector<bool> inQ;
    dfa.init=0;
    dfa.pool.clear();
    dfa.pool.push_back(DFA::State());
    for(set<size_t>::const_interator i = nfa.init.begin(); i!=nfa.init.end();i++)
      if(nfa.pool[*i].final){
          dfa.pool[0].final = 1;
          break;
      }
      m[nfa.init]=0;
      q,push(nfa.init);
      inQ.push_back(1);
      while(q.size()){
          set<size_t> u0 = q.front();
          size_t u1 = m[u0];
          map<char, set<size_t> > _;
          q.pop();
          inQ[u1]==o;
          for(set<size_t>::const_interator i = u0.begin(); i!=u0.end(); ++i)
            for(map<char, set<size_t> >::const_interator
                    j=nfa.pool[*i].trans.upper_bound(0);
                    j!=nfa.pool[*i].trans.end();
                    j++)
                    _[j->first].insert(j->second.begin(), j->second.end());
                for(map<char>,set<size_t> >::iterator i = _begin(); i!=_.end(); i++)
                    closure(nfa, i->second);
                    map<set<size_t>, size_t>:: iterator __ = m.find(i->second);
                    if(__==m.end()){
                        size_t v1 =dfa.pool.size();
                        dfa.pool.push_back(DFA::state());
                        dfa.insert(u1, i->first, v1);
                        m[i->second]= v1;
                        q.push(i->second);
                        inQ.push_back(1);
                        for(set<size_t>::iterator j = i->second.begin();
                        j!=i->second.end();
                        j++)
                        if(nfa.pool[*j].final){
                            dfa.pool.back().final=1;
                            break;
                        }
                        else
                        dfa.insert(u1, i->first, __->second);
    }
  }
}//Powerset construction ends


//brozowski algo start
template<class I> void regEx2DFA(DFA& dfa, I lo, I hi){
    NFA nfa;
    regEX2NFA(nfa, lo, hi);
    determinize(dfa, nfa);
    reverse(nfa, dfa);
    determinize(dfa, nfa);
    reverse(nfa, dfa);
    determinize(dfa, nfa);
}

//brozowski's algo ends
extern "C" {
    const char* regEx2Dot(char* regEx){
        DFA dfa;
        regEx2DFA(dfa, regEx, regEx+strlen(regEx));
        string res = dfa.toDot();
        return res.c_str();
    }
}

int main (int argc, char** argv){
    assert(arg==2);
    DFA dfa;
    regEx2DFA(dfa, argv[1], argv[1]+strlen(argv[1]));
    string s = dfa.toDot();
    cout<<s<<end1
    return 0;
}