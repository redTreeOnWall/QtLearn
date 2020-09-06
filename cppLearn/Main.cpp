#include <stdio.h>
#include <memory>
#include <iostream>
using namespace std;

class TestClass{
    public:
    TestClass(){
        std::cout << "new " << std::endl;
    }
    int age;
    char* name;
    void Speak();
    ~TestClass(){
        std::cout << "delete " << this->name << std::endl;
    }
};

void TestClass :: Speak(){
    std::cout << "hello I'm  " << this->name << ". I'm " << this->age<< "years old." <<  std::endl;
}

void PointTest(){
    printf("指针测试");
    TestClass lee = TestClass();
    lee.age = 25;
    lee.name = "lee";

    TestClass* iris = new TestClass();
    iris->age = 25;
    iris->name = "iris";

    TestClass& i = lee;

    TestClass i2 = lee;

    TestClass& i3 = i;

    TestClass i4 = i3;


    /*


    TestClass i4 = i3;

    TestClass i4 // 分配栈内存
    = i3 //赋值 结构体

    同样的 

    TestClass& i3 = i;
    TestClass& i3 //分配栈内存 不过由于分配的是指针 因此占用的空间不大
    = i; //赋值 指针数值

     */


    long iPointer = (long) (&i);
    long i2Pointer = (long) (&i2);
    long leePointer = (long) (&lee);
    long i3Pointer = (long) (&i3);
    long i4Pointer = (long) (&i4);


    printf("leePointer:%d\n",leePointer);
    printf("iPointer:%d\n",iPointer);
    printf("i2Pointer:%d\n",i2Pointer);
    printf("i3Pointer:%d\n",i3Pointer);
    printf("i4Pointer:%d\n",i4Pointer);

    printf("lee is name:%s age:%d \n",lee.name,lee.age);
    printf("iris is name:%s age:%d \n",iris->name,iris->age);
    printf("lee is name:%s age:%d \n",i.name,i.age);
}

shared_ptr<TestClass> TestSmartPointer2(std::shared_ptr<TestClass> t){
    t->name = "lee2";
    shared_ptr<TestClass> p2(new TestClass());
    p2->name = "ting";
    p2 -> age = 26;
    return p2;
}
void TestSmartPointer(){
    printf("智能指针测试\n");
    std::shared_ptr<TestClass> p(new TestClass());
    p->age = 25;
    p->name = "lee";
    std::cout << p->name  << std::endl;
    auto p2 = TestSmartPointer2(p);
    std::cout << p->name  << std::endl;

    auto p3 = p2;
    cout << "p2 use count:" << p2.use_count() << endl;
    std::cout << p2->name  << std::endl;

    p3 -> Speak();

    weak_ptr<TestClass> wp(p);
    wp.lock()->Speak();
}


int main(){
    // PointTest();
    TestSmartPointer();

    return 0;
}