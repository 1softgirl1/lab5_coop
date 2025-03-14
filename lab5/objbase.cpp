#include <iostream>
#include <unknwn.h>
#include "objbase.h"
#include <cassert>

using namespace std;

// ����������
interface IX : IUnknown {
    virtual void _stdcall Fx() = 0;
};

interface IY : IUnknown {
    virtual void _stdcall Fy() = 0;
};

interface IZ : IUnknown {
    virtual void _stdcall Fz() = 0;
};

// ���������
class CA : public IX, public IY {
public:

    virtual HRESULT _stdcall QueryInterface(const IID& iid, void** ppv) {
        if (iid == IID_IUnknown1) {
            cout << "QueryInterface: ��������� ��������� �� IUnknown" << endl;
            *ppv = static_cast<IX*>(this);
        }
        else if (iid == IID_IX) {
            cout << "QueryInterface: ��������� ��������� �� IX" << endl;
            *ppv = static_cast<IX*>(this);
        }
        else if (iid == IID_IY) {
            cout << "QueryInterface: ��������� ��������� �� IY" << endl;
            *ppv = static_cast<IY*>(this);
        }
        else {
            cout << "��������� �� ��������������" << endl;
            *ppv = NULL;
            return E_NOINTERFACE;
        }
        reinterpret_cast<IUnknown*>(*ppv)->AddRef();
        return S_OK;
    }
    // �����������
    CA() : m_cRef(0) {};

    // ����������
    ~CA() { cout << "CA: destroed..." << endl; };

    // ���������� ���������� IX
    virtual void _stdcall Fx() { std::cout << "CA::Fx" << std::endl; };

    // ���������� ���������� IY
    virtual void _stdcall Fy() { std::cout << "CA::Fy" << std::endl; };

    virtual ULONG _stdcall AddRef() {
        cout << "CA: AddRef = " << m_cRef + 1 << endl;
        return ++m_cRef;
    };

    virtual ULONG _stdcall Release() {
        cout << "CA: Release = " << m_cRef - 1 << endl;
        if (--m_cRef == 0) {
            delete this;
            return 0;
        }
        return --m_cRef;
    };
private:
    // ������� ������
    long m_cRef;
};

// ������� �������� ����������
IUnknown* CreateInstance() {
    IUnknown* pI = static_cast<IX*>(new CA);
    pI->AddRef();
    return pI;
}


// ������
int main() {

    setlocale(LC_ALL, "Russian");
    // ������� ���������
    IUnknown* pIUnknown1 = CreateInstance();

    IX* pIX = NULL;  // ������� pIX
    HRESULT hr = pIUnknown1->QueryInterface(IID_IX, (void**)&pIX);

    pIUnknown1->Release();  // ��������� ������ � IUnknown

    if (SUCCEEDED(hr))
    {
        pIX->Fx();  // ������������ ��������� IX

        IX* pIX2 = pIX; // ������� ����� pIX
        pIX2->AddRef();  // ��������� ������� ������
        pIX2->Fx();     // ���-�� ������ ��� ������ pIX2
        pIX2->Release(); // ��������� ������ � pIX2

        pIX->Release();  // ��������� ������ � pIX
    }

    return 0;

}

