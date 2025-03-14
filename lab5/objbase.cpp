#include <iostream>
#include <unknwn.h>
#include "objbase.h"
#include <cassert>

using namespace std;

// Интерфейсы
interface IX : IUnknown {
    virtual void _stdcall Fx() = 0;
};

interface IY : IUnknown {
    virtual void _stdcall Fy() = 0;
};

interface IZ : IUnknown {
    virtual void _stdcall Fz() = 0;
};

// Компонент
class CA : public IX, public IY {
public:

    virtual HRESULT _stdcall QueryInterface(const IID& iid, void** ppv) {
        if (iid == IID_IUnknown1) {
            cout << "QueryInterface: возвращаю указатель на IUnknown" << endl;
            *ppv = static_cast<IX*>(this);
        }
        else if (iid == IID_IX) {
            cout << "QueryInterface: возвращаю указатель на IX" << endl;
            *ppv = static_cast<IX*>(this);
        }
        else if (iid == IID_IY) {
            cout << "QueryInterface: возвращаю указатель на IY" << endl;
            *ppv = static_cast<IY*>(this);
        }
        else {
            cout << "Интерфейс не поддерживается" << endl;
            *ppv = NULL;
            return E_NOINTERFACE;
        }
        reinterpret_cast<IUnknown*>(*ppv)->AddRef();
        return S_OK;
    }
    // Конструктор
    CA() : m_cRef(0) {};

    // Деструктор
    ~CA() { cout << "CA: destroed..." << endl; };

    // Реализация интерфейса IX
    virtual void _stdcall Fx() { std::cout << "CA::Fx" << std::endl; };

    // Реализация интерфейса IY
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
    // Счетчик ссылок
    long m_cRef;
};

// Функция создания компонента
IUnknown* CreateInstance() {
    IUnknown* pI = static_cast<IX*>(new CA);
    pI->AddRef();
    return pI;
}


// Клиент
int main() {

    setlocale(LC_ALL, "Russian");
    // Создать компонент
    IUnknown* pIUnknown1 = CreateInstance();

    IX* pIX = NULL;  // Создать pIX
    HRESULT hr = pIUnknown1->QueryInterface(IID_IX, (void**)&pIX);

    pIUnknown1->Release();  // Завершить работу с IUnknown

    if (SUCCEEDED(hr))
    {
        pIX->Fx();  // Использовать интерфейс IX

        IX* pIX2 = pIX; // Создать копию pIX
        pIX2->AddRef();  // Увеличить счетчик ссылок
        pIX2->Fx();     // Что-то делать при помощи pIX2
        pIX2->Release(); // Завершить работу с pIX2

        pIX->Release();  // Завершить работу с pIX
    }

    return 0;

}

