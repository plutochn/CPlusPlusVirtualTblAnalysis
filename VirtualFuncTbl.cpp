// VirtualFuncTbl.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

typedef long long UUID;
typedef unsigned int uint64_t;

class IUnknownEx
{
public:
	virtual void* Query(UUID& uuid, uint64_t queryVer) = 0;
	virtual void Release() = 0;
};

class IView :   public IUnknownEx
{
public:
	virtual void UpdateView() = 0;
};

class IDoc :   public IUnknownEx
{
public:
	virtual void UpdateDoc() = 0;
};

#define OUT_FUNCNAME(func) cout<<"func: "<<#func<<endl;

class CApp : public IView, public IDoc
{
public:
	virtual void* Query(UUID& uuid, uint64_t queryVer)
	{
		if (uuid == 0)
		{
			return static_cast<IView*>(this);
		}
		if (uuid == 1)
		{
			return static_cast<IDoc*>(this);
		}
		if (uuid == 2)
		{
			return static_cast<IUnknownEx*>(static_cast<IView*>(this));
		}
		if (uuid == 3)
		{
			return static_cast<IUnknownEx*>(static_cast<IDoc*>(this));
		}
		return NULL;
	}

	virtual void Release()
	{
		OUT_FUNCNAME(Release)
	}

	virtual void UpdateView()
	{
		OUT_FUNCNAME(UpdateView)
	}

	virtual void UpdateDoc()
	{
		OUT_FUNCNAME(UpdateDoc)
	}
};

int _tmain(int argc, _TCHAR* argv[])
{
	cout<<"_____________________________________"<<endl;
	cout<<"This is a virtual func tbl test demo!"<<endl;
	cout<<"_____________________________________"<<endl;
	cout<<endl;

	CApp* pApp = new CApp();
	pApp->UpdateDoc();
	pApp->UpdateView();
	pApp->Release();

	UUID uuid = 0;
	IView* pView = (IView*) pApp->Query(uuid,0);
	uuid++;

	IDoc*  pDoc = (IDoc*)  pApp->Query(uuid,1);
	uuid++;

	IUnknownEx*  pUnknownEx0 = (IUnknownEx*)  pApp->Query(uuid,2);
	uuid++;

	IUnknownEx*  pUnknownEx1 = (IUnknownEx*)  pApp->Query(uuid,3);
	uuid++;
	//error C2594: “static_cast”: 从“CApp *”到“IUnknownEx *”的转换不明确
	//IUnknownEx* pUnknownEx =  static_cast<IUnknownEx*>(pApp);
	// 解决方法 改成虚继承
	// class IView : virtual public IUnknownEx
	// class IDoc : virtual public IUnknownEx
	uuid = 0;
	IView* pView1 = (IView*) pUnknownEx1->Query(uuid,0);
	uuid++;

	IDoc*  pDoc1 = (IDoc*)  pUnknownEx1->Query(uuid,1);
	uuid++;

	uuid = 0;
	IView* pView2 = (IView*) pUnknownEx0->Query(uuid,0);
	uuid++;

	IDoc*  pDoc2 = (IDoc*)  pUnknownEx0->Query(uuid,1);
	uuid++;
	/**
	    CApp 内存布局
		offset				value
		------				-----
		0					IView vtbl_ponier --> 指向 Query stub 0
												  指向 Release stub 0

		4					IDoc  vtbl_ponier --> 指向 Query stub 1
												  指向 Release stub 1
		------------------------------------------------------
		Query stub 1

		CApp::Query:
		013311F4  jmp         CApp::Query (013351B3h)  

		013351B3  sub         ecx,4  
		013351B6  jmp         CApp::Query (013313B6h)  

		013313B6  jmp         CApp::Query (01335070h) 


		-----------------------------------------------------
		Query stub 0
		011C13B6  jmp         CApp::Query (011C5070h) */
	return 0;
}

