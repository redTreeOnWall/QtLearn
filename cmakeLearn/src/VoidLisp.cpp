#include "VoidLisp.hpp"
#include <math.h>
#include <iostream>
#include <stack>

using namespace std;

bool isStringNum(std::string *num) {
  if (num == NULL || num->size() == 0 || num->size() >= 10000) {
    return false;
  }

  auto *s = num->c_str();
  char first = s[0];
  bool isSign = first == '-';
  if (isSign) {
    if (isSign && num->length() == 1) {
      return false;
    }
  }
  for (int i = (isSign ? 1 : 0); i < num->size(); i++) {
    char c = s[i];
    bool isN = c >= 48 && c <= 57;
    if (!isN) {
      //不是数字
      return false;
    }
  }
  return true;
}
double string_to_number(std::string *num) {
  double res = 0;
  if (num == NULL || num->size() == 0) {
    return res;
  }

  if (num->size() >= 10000) {
    return VOID_DBL_MAX;
  }
  std::string n = *num;
  bool isAdd = !(n[0] == '-');

  std::vector<char> pointLeft;
  std::vector<char> pointRight;

  bool isLeft = true;
  int startIndex = isAdd ? 0 : 1;
  for (int i = startIndex; i < n.length(); i++) {
    char c = n[i];
    if (c == '.') {
      isLeft = false;
      continue;
    }
    if (isLeft) {
      pointLeft.push_back(c);
    } else {
      pointRight.push_back(c);
    }
  }

  for (int i = 0; i < pointLeft.size(); i++) {
    char c = pointLeft[pointLeft.size() - 1 - i];
    res = res + (c - 48) * (pow(10, i));
  }

  for (int i = 0; i < pointRight.size(); i++) {
    char c = pointRight[i];
    res = res + (c - 48) / (pow(10, i + 1));
  }

  if (!isAdd) {
    res = -1 * res;
  }

  return res;
};

Met::Met(VoidLisp *l) {
  this->type = MetType::nil;
  l->addMet(this);
}
Met::Met(VoidLisp *l,double num) {
  this->num = num;
  this->type = MetType::num;
  l->addMet(this);
}
Met::Met(VoidLisp *l,int listSize) {
  this->mets = new vector<Met *>();
  this->type = MetType::ls;
  l->addMet(this);
}

Met::Met(VoidLisp *l,string *content) {
  this->type = MetType::str;
  string &os = *content;
  string *s = new string(os);
  this->name = s;
  l->addMet(this);
};

string *Met::toString() {
  int index = 0;
  string *reP;
  if (type == MetType::ls) {
    if (mets == 0 || mets->size() == 0) {
      throw "met size is zero";
    }
    index++;

    vector<string *> str;
    str.push_back(new string("( "));
    for (auto m : *mets) {
      auto *s = m->toString();
      str.push_back(s);
      str.push_back(new string(" "));
    }
    str.push_back(new string(") "));

    int count = 0;
    for (auto *as : str) {
      count = count + as->length();
    }
    auto *resStrP = new string(count, 0);
    string &resStr = *resStrP;

    int ind = 0;
    for (auto *as : str) {
      count = count + as->length();
      string &asStr = *as;
      for (char ss : asStr) {
        if (ind < count) {
          resStr[ind] = ss;
          ind++;
        }
      }
      delete as;
      as = NULL;
    }
    reP = resStrP;
  } else if (type == MetType::sg) {
    reP = new string(*(this->name));
  } else if (type == MetType::num) {
    if (num == floor(num)) {
      auto s = to_string((int)num);
      reP = new string(s);
    } else {
      auto s = to_string(num);
      reP = new string(s);
    }
  } else if (type == MetType::str) {
    string &s = *(this->name);
    reP = new string(s);
  } else if (type == MetType::nil) {
    reP = new string("nil");
  }
  return reP;
}


LispRuntimeStatckElem :: LispRuntimeStatckElem(VoidLisp *lisper){
  this->lisp = lisper;
  lisp ->runtimeStatck.push_back(this);
  lisp->stackIndex ++;
  this->statckIndex = lisp->stackIndex;
};

LispRuntimeStatckElem :: ~LispRuntimeStatckElem(){
  lisp -> runtimeStatck.pop_back();
  this->localVals->clear();
  delete this->localVals;
  lisp->stackIndex --;
};

void LispRuntimeStatckElem :: setLocalVal(const string &key, Met *m){
  // if(lisp->runtimeStatck.empty() || lisp->runtimeStatck.end() == lisp->runtimeStatck.begin()){
  //   throw "paren is empty";
  // }

  LispRuntimeStatckElem *parentElem  = *(--(--lisp->runtimeStatck.end()));

  if(parentElem->localVals ==NULL){
    parentElem -> localVals =  new map<string,long>();
  }

  string keyS = key;
  (*(parentElem -> localVals))[keyS] = (long)m;
};

Met *LispRuntimeStatckElem :: getLocalVal(const string &key){

  if(lisp->runtimeStatck.empty() || lisp->runtimeStatck.end() == lisp->runtimeStatck.begin()){
    throw "paren is empty";
  }

  auto &sta =  lisp ->runtimeStatck;
  int ii =0;
  for(auto it = sta.end()-- ; it != sta.begin() ;){
    --it;
    LispRuntimeStatckElem *parentElem  = *it;
    if(parentElem->localVals !=NULL){
      map<string,long> &mp =  *(parentElem -> localVals);
      if(mp.find(key) != mp.end()){
        return (Met *) mp[key];
      }
    }
  }

  string err =  "sign "+ key + " is not define";
  throw err;
}

void VoidLisp ::pushTokenToTokens(vector<string *> *tokens, vector<char> *token) {
  int l = token->size();
  if (l == 0) {
    return;
  }
  auto *newToken = new string(l, 0);
  for (int i = 0; i < l; i++) {
    (*newToken)[i] = (*token)[i];
  }

  token->clear();
  tokens->push_back(newToken);
}

VoidLisp::VoidLisp() { 
  this->allMet =  new list<Met *>();
  initLib();
}

VoidLisp::~VoidLisp() { 
  gc();
  this->allMet->clear();
  delete allMet;
  allMet = NULL;
}

vector<string *> *VoidLisp ::GetToken(string *s) {
  auto tokens = new vector<string *>();
  string &li = *s;
  auto *token = new vector<char>();
  bool isInString = false;
  bool isInAno = false;
  for (int i = 0; i < li.length(); i++) {
    char c = li[i];

    if (isInAno) {
      if (c == '\n') {
        isInAno = false;
      }
      continue;
    }

    if (isInString) {
      token->push_back(c);
      if (c == '"') {
        pushTokenToTokens(tokens, token);
        isInString = false;
      }
      continue;
    }

    // not in ano or string ↓

    if (c == ';') {
      // start ano
      isInAno = true;
      pushTokenToTokens(tokens, token);
      continue;
    }

    if (c == '"') {
      // start string
      isInString = true;
      pushTokenToTokens(tokens, token);
      token->push_back(c);
      continue;
    }

    if (c == ' ' || c == '\n' || c == '\t') {
      pushTokenToTokens(tokens, token);
      continue;
    }

    if (c == '(' || c == ')') {
      pushTokenToTokens(tokens, token);
      token->push_back(c);
      pushTokenToTokens(tokens, token);
      continue;
    }
    token->push_back(c);
  }

  pushTokenToTokens(tokens, token);

  delete token;
  token = NULL;
  return tokens;
}

Met *VoidLisp::GetAst(std::vector<std::string *> *tokens) {
  stack<Met *> sta;
  Met *root = new Met(this,0);
  sta.push(root);
  auto &ts = *tokens;
  for (int i = 0; i < tokens->size(); i++) {
    auto *tp = ts[i];
    string &t = *tp;
    char c = t[0];
    if (c == '(') {
      auto *m = new Met(this,0);
      if (sta.empty()) {
        cout << "err: to much ')'" << endl;
        throw "err: to much ')'";
      }
      sta.top()->mets->push_back(m);
      sta.push(m);
    } else if (c == ')') {
      sta.pop();
    } else {
      auto *me = new Met(this);
      if (c == '"' && t.size() >= 2 && t[t.size() - 1] == '"') {
        me->type = MetType::str;
        me->name = new string();
        string &nameS = *(me->name);
        for (int ci = 1; ci < t.size() - 1; ci++) {
          nameS += t[ci];
        }
      } else if (isStringNum(tp)) {
        me->type = MetType ::num;
        me->num = string_to_number(tp);
      } else {
        me->type = MetType::sg;
        me->name = new string(t);
      }
      if (sta.empty()) {
        cout << "err: to much ')'" << endl;
        throw "err: to much ')'";
      }
      sta.top()->mets->push_back(me);
    }
  }
  for (auto *s : *tokens) {
    delete s;
    s = NULL;
  }
  delete tokens;

  sta.pop();
  if (!sta.empty()) {
    cout << "err: to much '(' " << endl;
    throw "err: to much '('";
  }
  if (root == NULL) {
    root = new Met(this,0);
  }
  if (root->mets->size() != 1) {
    cout << "err: code must be in one () " << endl;
    throw "err: to much '('";
  }
  return root;
}

void VoidLisp::AddFunction(const string &funcName, const LispFunction func) {
  long p = (long)func;
  this->funcs[funcName] = p;
};

Met *VoidLisp::RunFunction(const std::string &funcName) {
  VoidLispParam &params = runtimeStatck.back()->params;
  if (funcs.find(funcName) == funcs.end()) {
    cout << "err function : " << funcName << " not exist" << endl;
    throw "err";
  }
  LispFunction f = (LispFunction)(funcs[funcName]);
  Met *m = f(params, this);
  return m;
}


Met *VoidLisp:: exec(Met *m){
  ///------
  if (m->type == MetType::ls) {

    LispRuntimeStatckElem stack(this);

    if (m->mets == 0 || m->mets->size() == 0) {
      cout << "met size is zero" << endl;
      m->type = MetType::nil;
      return m;
    }

    for(auto mmm : *m->mets){
      this->runtimeStatck.back()->paramsOrigin.push_back(mmm);
    }

    vector<Met *> &ms = (*(m->mets));

    Met *key =ms[0];
    if(key ->type != MetType :: sg){
      key =exec( key);
    }

    if (key->type != MetType::sg) {
      string n = *(m->name);
      cout << n << endl;
      cout << "err! function name _" << n << "_is not a sign" << endl;
      throw "err";
    }

    bool isLet = false;
    if(key->name->size() == 3){
      string keyStr = *key->name;
      if(keyStr == "let"){
          isLet = true;
      }
    }


    if ((*(key->name))[0] == '\'') {
      return ms[1];
    }

    //参数对象 进入方法栈里;
    VoidLispParam &params = runtimeStatck.back()->params;

    int originLength = ms.size();
    int thisLength = originLength;
    for (int i = 1; i < ms.size(); i++) {
      Met &mi = *(ms[i]);
      if (mi.type == MetType::sg && (*mi.name)[0] == '\'') {
        if (i < ms.size() - 1) {
          i++;
          Met *next = ms[i];
          params.push_back(next);
        }
      } else {
        Met *paramMet;
        paramMet =(isLet && i==1) ? ms[i] :  exec( ms[i]);
        params.push_back(paramMet);
      }
      int passedCount = i +1;
      while (thisLength > (originLength - passedCount)) {
        this ->runtimeStatck.back() ->paramsOrigin.pop_front();
        thisLength = thisLength-1;
      }
    }

    auto *resMet = this->RunFunction(*(key->name));

    return resMet;

  } else if (m->type == MetType::sg) {
    Met * local = this->runtimeStatck.back() -> getLocalVal(*(m->name)) ;
    if(local != NULL){
      return local;
    }
    return m;
  } else if (m->type == MetType::num) {
    return m;
  } else if (m->type == MetType::str) {
    return m;
  } else if (m->type == MetType::nil) {
    return m;
  }
  return m;
}

Met *VoidLisp::GetTree(string *s) {
  auto *tokens = GetToken(s);
  auto *tree = GetAst(tokens);
  return tree;
}

Met *VoidLisp::Eval(string *s) {
  Met *treeAll = GetTree(s);
  Met *tree = (*(treeAll->mets))[0];
  // root = tree;
  string *treeStr = tree->toString();
  Met *res = exec(tree);
  string *resStr = res->toString();
  std::cout << "> " << *resStr << endl;
}

void VoidLisp::gc() {
  /*
    gc : 
    1. mark all the useful object (param, local vals in statck and globle vals)
    2. relese objects not marked;(foreach all the objects)
   */

  //mark;
  stack<Met *>  * loopStackP = new stack<Met *>();
  stack<Met *>  & loopStack =*loopStackP;

  for( auto *elem : runtimeStatck){
    for(auto p: elem -> params){
      loopStack.push(p);
    }

    for(auto p: elem -> paramsOrigin){
      loopStack.push(p);
    }

    if(elem -> localVals != NULL){
      for(auto it  = elem ->localVals->begin();it != elem -> localVals ->end();++it){
        auto *p =(Met *) it->second;
        loopStack.push(p);
      }
    }
    while(!loopStack.empty()){
      Met *e = loopStack.top();
      loopStack.pop();
      if(e->gcMarked){
        continue;
      }
      e->gcMarked = true;
      // cout << "marked obj: " <<*(e->toString()) <<endl;
      if(e->mets != NULL){
        for(auto em : *(e->mets)){
          loopStack.push(em);
        }
      }
    }
  }

  // release
  for(auto it  = allMet->begin() ; it != allMet->end();){
    Met * m = *it;
    if(!m->gcMarked){
      allMet->erase(it++); // delete and to next
      delete m;
    }else{
      ++it;
    }
  }
}

void VoidLisp::addMet(Met * m){
  this->allMet -> push_back(m);
}

//----------------------------------------------------------------------------------------------------

Met *LispPrint(const VoidLispParam &params, VoidLisp *lisper) {
  string ss ;
  for (Met *m : params) {
    string *s = m->toString();
    ss+= *s;
    delete s;
  }
  cout << ss << endl;
  return params.back();
}

Met *add(const VoidLispParam &params, VoidLisp *lisper) {
  double res = 0;
  for (Met *m : params) {
    res = res + m->num;
  }
  return new Met(lisper,res);
}

Met *mul(const VoidLispParam &params, VoidLisp *lisper) {
  double res = 1;
  for (Met *m : params) {
    res = res * m->num;
  }
  return new Met(lisper,res);
}

Met *sub(const VoidLispParam &params, VoidLisp *lisper) {
  double res = 0;
  if (params.size() > 2) {
    res = (*params[0]).num;
    for (int i = 1; i < params.size(); i++) {
      auto *m = params[i];
      res = res - m->num;
    }
  }
  return new Met(lisper,res);
}


Met *div(const VoidLispParam &params, VoidLisp *lisper) {

  double res = 0;
  if (params.size() > 2) {
    res = (*params[0]).num;
    for (int i = 1; i < params.size(); i++) {
      auto *m = params[i];
      res = res / m->num;
    }
  }
  return new Met(lisper,res);
}

Met *let(const VoidLispParam &params, VoidLisp *lisper) {
  if(params.size() !=2 || params[0]->type != MetType::sg){
    cout <<"let is not correct used:should use like: (let a 1000)"<<endl;
    throw "err";
  }
  lisper->runtimeStatck.back() ->setLocalVal(*(params[0]->name),params[1]);
  return (params[1]);
}

Met *run(const VoidLispParam &params, VoidLisp *lisper) {
  return params.back();
}

Met *gcAll(const VoidLispParam &params, VoidLisp *lisper) {
  lisper->gc();
  return new Met(lisper);
}

Met *allMetNum(const VoidLispParam &params, VoidLisp *lisper) {
  return new Met(lisper,(double) lisper->allMet->size());
}






void VoidLisp::initLib() {
  this->AddFunction("println", LispPrint);
  this->AddFunction("+", add);
  this->AddFunction("-", add);
  this->AddFunction("*", mul);
  this->AddFunction("/", div);
  this->AddFunction("let", let);
  this->AddFunction("run", run);
  this->AddFunction("gc", gcAll);
  this->AddFunction("allMetNum", allMetNum);
};