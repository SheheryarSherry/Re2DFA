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

 

