#include <bits/stdc++.h>
using namespace std; using Cl=chrono::steady_clock; using ll = long long;


// small helper function to print label + value
///user defined template function template
template<typename T> void pr(const string &l,const T &v){ cout<<l<<": "<<v<<'\n'; }




// buff = Buffered Reader // reads file chunk by chunk using given buffer size
class buff{
 public:
  string hi; ll bye; ifstream in;

  buff(const string &f,ll bs):hi(f),bye(bs){
   if(bye<256*1024||bye>1024*1024) throw runtime_error("buffer should between 256 to 1024 KB");
   in.open(hi,ios::binary);
  if(!in.is_open()) throw runtime_error("cannot open file");
  }

  bool nxt(string &o){
    if(!in.good()) return false;
    vector<char> b(bye); in.read(b.data(),bye);
    streamsize g=in.gcount();
    if(g<=0) return false;
  o.assign(b.data(),b.data()+g);
    return true;
  }
};


// toke = Tokenizer
class toke{
 public:

  void run(const string &c,string &cr,const function<void(const string&)> &h){
    string t; if(!cr.empty()) t.swap(cr);

    for(unsigned char ch:c){      if(isalnum(ch)) t.push_back(tolower(ch));
      else{
       if(!t.empty()){ h(t); t.clear(); }
      }
    }
    cr=move(t);}

  void end(string &cr,const function<void(const string&)> &h){
   if(!cr.empty()){ h(cr); cr.clear(); }
  }
};


//versioned Index
class ver{
 public:
  ll bye; unordered_map<string,unordered_map<string,ll>> mp;

  ver(ll bs=512*1024):bye(bs){}

  void build(const string &v,const string &f){
   if(mp.count(v)) return;
    buff r(f,bye); toke z; string cr,ch;

    while(r.nxt(ch)){
    z.run(ch,cr,[&](const string &w){ add(v,w); });
    }
    z.end(cr,[&](const string &w){ add(v,w); });
  }




  ll get(const string &v,const string &w)const{
    auto it=mp.find(v);
    if(it==mp.end()) throw runtime_error("version not found");
    auto it2=it->second.find(w);
    if(it2==it->second.end()) return 0;
    return it2->second;
  }

  vector<pair<string,ll>> top(const string &v,ll k)const{
    auto it=mp.find(v);
    if(it==mp.end()) throw runtime_error("version not found");

   vector<pair<string,ll>> a(it->second.begin(),it->second.end());

    sort(a.begin(),a.end(),[](auto &x,auto &y){
      if(x.second!=y.second) return x.second>y.second;
      return x.first<y.first;
    });




    if(k<a.size()) a.resize(k);
    return a;
  }

  void add(const string &v,const string &w){ mp[v][w]++; }
  void add(const string &v, const string &w, ll cnt) { 
    if (cnt <= 0) return;
    mp[v][w] += cnt; 
  }
};


// Base query class
class Q{
public:
  virtual ~Q()=default;
  virtual void run(ver &i)const=0;
};


// convert string to lowercase
string low(const string &s){
    string r;
    for(unsigned char c : s){
        if(isalnum(c))
            r.push_back(tolower(c));
    }
    return r;
}





// Word Query
class WQ:public Q{
 public:
  string v_,w_;
  WQ(string v,string w):v_(v),w_(low(w)){}

 void run(ver &i)const override{
    ll f=i.get(v_,w_);
  pr("Version",v_);
    cout<<"Word '"<<w_<<"' = "<<f<<'\n';
  }
};


// Top Query
class TQ:public Q{
 public: string v_; ll k_;
  TQ(string v,ll k):v_(v),k_(k){}

  void run(ver &i)const override{
   auto a=i.top(v_,k_);
    pr("Version",v_);
    for(ll j=0;j<a.size();j++)
      cout<<j+1<<". "<<a[j].first<<" -> "<<a[j].second<<'\n';
  }
};


// Diff Query
class DiffQ:public Q{
 public:
  string v1_,v2_,w_;
  DiffQ(string a,string b,string w):v1_(a),v2_(b),w_(low(w)){}

  void run(ver &i)const override{
    ll c1=i.get(v1_,w_);
 ll c2=i.get(v2_,w_);
    pr("Version",v1_);
    cout<<"Word '"<<w_<<"' = "<<c1<<'\n';  pr("Version",v2_);
    cout<<"Word '"<<w_<<"' = "<<c2<<'\n';
    cout<<"Difference ("<<v2_<<" - "<<v1_<<") = "<<((long long)c2 - (long long)c1)<<'\n';
  }
};


// QP
class QP{
 public:
  ver &i_;
  QP(ver &i):i_(i){}
  void exec(unique_ptr<Q> q){ q->run(i_); }
};

 

string usage_all(){
    return
    "Correct Syntax:\n"
    "\n"
    "1. Word Count Query:\n"
    "   .\\240650_devarsh.exe --file <file> --version <version_name> --buffer <buffer_size> --query word --word <word>\n"
    "   Example:\n"
    "   .\\240650_devarsh.exe --file test_logs.txt --version v1 --buffer 1023 --query word --word error\n"
    "\n"
    "2. Top-K Query:\n"
    "   .\\240650_devarsh.exe --file <file> --version <version_name> --buffer <buffer_size> --query top --top <K>\n"
    "   Example:\n"
    "   .\\240650_devarsh.exe --file test_logs.txt --version v2 --buffer 512 --query top --top 20\n"
    "\n"
    "3. Difference Query:\n"
    "   .\\240650_devarsh.exe --file1 <file1> --version1 <version1> --file2 <file2> --version2 <version2> --buffer <buffer_size> --query diff --word <word>\n"
    "   Example:\n"
    "   .\\240650_devarsh.exe --file1 test_logs.txt --version1 v1 --file2 verbose_logs.txt --version2 v2 --buffer 512 --query diff --word error\n"
    "\n"
    "Note:\n"
    "  Buffer size must be between 256 and 1024 KB\n";
}

bool file_exists(const string &path){
    if(path.empty()) return false;
    ifstream f(path, ios::binary);
    return f.good();
}

void validation(const string &qt,const string &f, const string &v,const string &f1, const string &v1,const string &f2, const string &v2,ll buffer_kb,const string &query_word,ll top_k){
    if(qt.empty()){
        throw runtime_error(string("Missing --query flag.\n") + usage_all());
    }

    if(qt != "word" && qt != "top" && qt != "diff"){
        throw runtime_error(string("Unknown --query value: '") + qt + "'.\n" + usage_all());
    }

    // buffer check (KB)
    if(buffer_kb < 256 || buffer_kb > 1024){
        throw runtime_error("Invalid --buffer: must be between 256 and 1024 (KB).\n" + usage_all());
    }

    if(qt == "word"){
        if(f.empty() || v.empty() || query_word.empty()){
            throw runtime_error(string("Missing argument for 'word' query.\n") + usage_all());
        }
        if(!file_exists(f)){
            throw runtime_error(string("File not found for --file: '") + f + "'.");
        }
    } else if(qt == "top"){
        if(f.empty() || v.empty()){
            throw runtime_error(string("Missing argument for 'top' query.\n") + usage_all());
        }
        if(top_k <= 0) throw runtime_error("Invalid --top: must be > 0.\n" + usage_all());
        if(!file_exists(f)){
            throw runtime_error(string("File not found for --file: '") + f + "'.");
        }
    } else if(qt == "diff"){
        if(f1.empty() || v1.empty() || f2.empty() || v2.empty() || query_word.empty()){
            throw runtime_error(string("Missing argument for 'diff' query.\n") + usage_all());
        }
        if(v1 == v2){
            throw runtime_error(string("Versions for diff must be distinct (version1 != version2)."));
        }
        if(!file_exists(f1)){
            throw runtime_error(string("File not found for --file1: '") + f1 + "'.");
        }
        if(!file_exists(f2)){
            throw runtime_error(string("File not found for --file2: '") + f2 + "'.");
        }
    }
}

 
int main(int argc,char **argv){
  ios::sync_with_stdio(false); cin.tie(nullptr);

 try{
    auto total_start = Cl::now();

    string f,v,f1,v1,f2,v2; string qt;
    ll buffer_kb = 512;
    string query_word; ll top_k = 10;

    for(int i=1;i<argc;i++){
      string a=argv[i];
     if(a=="--file" && i+1<argc){ f=argv[++i]; }
      else if(a=="--version" && i+1<argc){ v=argv[++i]; }
      else if(a=="--file1" && i+1<argc){ f1=argv[++i]; }
    else if(a=="--version1" && i+1<argc){ v1=argv[++i]; }
      else if(a=="--file2" && i+1<argc){ f2=argv[++i]; }
    else if(a=="--version2" && i+1<argc){ v2=argv[++i]; }
      else if(a=="--buffer" && i+1<argc){ buffer_kb=stoul(argv[++i]); }
      else if(a=="--query" && i+1<argc){ qt=argv[++i]; }
      else if(a=="--word" && i+1<argc){ query_word=argv[++i]; }
      else if(a=="--top" && i+1<argc){ top_k=stoul(argv[++i]); }
    }
       
    validation(qt, f, v, f1, v1, f2, v2, buffer_kb, query_word, top_k);
    ll byebytes = buffer_kb * 1024ULL; ver i(byebytes);
    pr("Buffer Size (KB)", buffer_kb);
    unique_ptr<Q> q;

    if(qt=="word"){ i.build(v,f); q=make_unique<WQ>(v,query_word); }


    else if(qt=="top"){ i.build(v,f); q=make_unique<TQ>(v,top_k); }
    
    else if(qt=="diff"){ i.build(v1,f1); i.build(v2,f2); q=make_unique<DiffQ>(v1,v2,query_word); }
   else{ throw runtime_error("unknown query"); }

    QP p(i); p.exec(move(q));

    auto total_end = Cl::now();
    chrono::duration<double> total_d = total_end - total_start;
    pr("Total Execution time (s)", total_d.count());

  }catch(exception &e){ cerr<<"error: "<<e.what()<<'\n'; }
}
