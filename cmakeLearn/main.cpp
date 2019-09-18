#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <vector>
#include "./src/Test1.hpp"
#include "./src/VoidLisp.hpp"
using namespace std;
// https://www.ibm.com/developerworks/cn/linux/l-cn-cmake/index.html   //cmaker
// 使用

void TestColection() {
  // vector
  vector<int> vi;
  for (int i = 0; i < 10; i++) {
    vi.push_back(i);
  }

  for (int i = 0; i < vi.size(); i++) {
    int n = vi[i];
    cout << i << ":" << n << endl;
  }
  // vector object pointer
  vector<Test1 *> vo;
  for (int i = 0; i < 10; i++) {
    Test1 *t = new Test1();
    auto t2 = new Test1();
    t->id = i;
    t2->id = i * 100;
    vo.push_back(t);
    vo.push_back(t2);
  }
  vector<Test1 *>::iterator it;
  for (it = vo.begin(); it != vo.end(); it++) {
    Test1 *p = *it;
    cout << p->id << endl;
  }
}

class TestC {
 public:
  TestC(int id) { this->id = id; }
  int id;
};

void TestPoiterAndRef() {
  // string *s =  new string("12345");
  // string s2 = *s;
  // auto s3  = s->c_str();
  // // s->c_str()[0] = 'a';
  // // s2.c_str()[0] = 'b';
  // void *vs3 =(void *) s3;
  // ((char *)(vs3))[0] = 'a';

  // cout << (s->c_str()) << endl;
  // cout << (s2.c_str()) << endl;
  // cout << s3 << endl;

  // auto c1 =new TestC(1);
  // auto c2 = TestC(2);
  // auto c3 = *c1;
  // TestC &c4 = (*c1);
  // c1->id = 0;
  // (*c1).id =5;

  // cout << c1->id << endl;
  // cout << c2.id << endl;
  // cout << c3.id << endl;
  // cout << c4.id << endl;
}

void TestVoidLisp() {
  ifstream fin("../res/test.lisp", ios::in);
  if (!fin) {
    cout << "文件打开失败！" << endl;
    exit(1);
  }
  string text;
  while (!fin.eof()) {
    int id = fin.get();
    if (id > 0) {
      text += (char)(id);
    } else {
      break;
    }
  }
  fin.close();
  try {
    auto *lisper = new VoidLisp();
    lisper->Eval(&text);
  } catch (std::exception &e) {
    cout << "exeption" << endl;
    cout << e.what() << endl;
  } catch (const char *msg) {
    cout << "exeption" << endl;
    cout << msg << endl;
  } catch (std::string msg) {
    cout << "exeption" << endl;
    cout << msg << endl;
  }

}

int TestFun(int a) { return a + a; }
void TestFunctionPointer() {
  typedef int (*FUC)(int);
  FUC fuc = TestFun;  // now fuc is a function pointer

  int n6 = fuc(3);
  cout << n6 << endl;

  long fp = (long)fuc;  // 64 bit system
  cout << "fp:" << fp << endl;

  FUC fuc2 = (FUC)(fp);

  int fuc2Res = fuc2(3);  // 6
  cout << "fuc2Res:" << fuc2Res << endl;

  std::map<string, long> mp;
  // // mp.insert(std::pair<string,int (int)>( "nn6",*fuc));
  // std:: map<string,string> p;
  // p.insert(std::pair<string,string>("1","2"));
}

int main() {
  // std::cout << "Hello world" <<std::endl;
  // Test1 *t =  new Test1();
  // t->Say();
  // t->Loop();

  // TestColection();

  // TestFunctionPointer();

  TestVoidLisp();

  return 0;
}
