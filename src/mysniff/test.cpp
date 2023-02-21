#include "stdafx.h"
#include <iostream>
#include <functional>
using namespace std;

typedef std::function<void()> fp;
void g_fun()
{
	cout << "g_fun()" << endl;
}
class A {
public:
	static void A_fun_static()
	{
		cout << "A_fun_static()" << endl;
	}
	void A_fun()
	{
		cout << "A_fun()" << endl;
	}
	void A_fun_int(int i)
	{
		cout << "A_fun_int()" << i << endl;
	}
	//非静态类成员，因为含有this指针，所以需要使用bind
	void init()
	{
		fp  fp1 = std::bind(&A::A_fun, this);
		fp1();
	}
	void init2()
	{
		typedef std::function<void(int)> fpi;
		//对于参数要使用占位符 std::placeholders::_1 
		fpi f = std::bind(&A::A_fun_int, this, std::placeholders::_1);
		f(5);
	}
};
int main2() {
	//绑定到全局函数  
	fp f2 = fp(&g_fun);
	f2();
	//绑定到类静态成员函数
	fp f1 = fp(&A::A_fun_static);
	f1();

	A().init();
	A().init2();
	return 0;
}