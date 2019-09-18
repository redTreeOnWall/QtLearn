#include <list>
#include <map>
#include <stack>
#include <string>
#include <vector>

class Met;
class VoidLisp;


enum MetType {
  sg,   // sign   a , b ,var ...
  ls,   // list  (a 1 3 ), (b) (+ 1 2)
  num,  //  3, 4,55.5, -1
  str,  // "name is Li " ...
  nil,   // delfuat
  loc, // local val
};

#define VOID_DBL_MAX 1.7976931348623158e+308 /* max value */
#define VOID_DBL_MIN 2.2250738585072014e-308 /* min positive value */
double string_to_number(std::string *num);

class Met {
 public:
  Met(VoidLisp *);
  Met(VoidLisp *,double num);
  Met(VoidLisp *,std::string *content);
  Met(VoidLisp *,int listSize);
  std::vector<Met *> *mets = NULL;
  std::string *name = NULL;
  double num = 0;
  MetType type;
  std::string *toString();
  bool gcMarked = false;
  void gcMark();

 private:
  int printIndex;
  // VoidLisp *lisper;
};

typedef std::vector<Met *> VoidLispParam;
typedef Met *(*LispFunction)(const std::vector<Met *> &params,
                             VoidLisp *lisper);
class LispRuntimeStatckElem {
 public:
 LispRuntimeStatckElem(VoidLisp *lisper);
 ~LispRuntimeStatckElem();
 VoidLisp *lisp = NULL;
 std::map<std::string,long> *localVals =NULL;
 VoidLispParam params;
 std::list<Met *> paramsOrigin;
 void setLocalVal(const std::string &,Met *);
 Met *getLocalVal(const std::string &);
 int statckIndex;
};

class VoidLisp {
 public:
  VoidLisp();
  ~VoidLisp();
  std::vector<std::string *> *GetToken(std::string *s);
  void AddFunction(const std::string &funcName, const LispFunction func);
  Met *GetTree(std::string *s);
  Met *Eval(std::string *s);
  Met *RunFunction(const std::string &funcName);
  Met *exec(Met *m);
  std::list<LispRuntimeStatckElem *> runtimeStatck;
  int stackIndex=0;
  std::list<Met *> *allMet = NULL;
  void addMet(Met *);
  void gc();

 private:
  void pushTokenToTokens(std::vector<std::string *> *tokens, std::vector<char> *token);
  Met *GetAst(std::vector<std::string *> *tokens);
  std::map<std::string, long> funcs;
  void initLib();
};
