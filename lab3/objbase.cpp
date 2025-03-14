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
    // Реализация методов класса IUnknown
    virtual ULONG _stdcall AddRef() { return 0; };
    virtual ULONG _stdcall Release() { return 0; };

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

    // Реализация интерфейса IX
    virtual void _stdcall Fx() { std::cout << "CA::Fx" << std::endl; };

    // Реализация интерфейса IY
    virtual void _stdcall Fy() { std::cout << "CA::Fy" << std::endl; };

};

// Функция создания компонента
IUnknown* CreateInstance() {
    IUnknown* pI = static_cast<IX*>(new CA);
    pI->AddRef();
    return pI;
}

BOOL SameComponents(IX* pIX, IY* pIY)
{
    IUnknown* pI1 = NULL;
    IUnknown* pI2 = NULL;

    // Получить указатель на IUnknown через pIX
    pIX->QueryInterface(IID_IUnknown1, (void**)&pI1);

    // Получить указатель на IUnknown через pIY
    pIY->QueryInterface(IID_IUnknown1, (void**)&pI2);

    // Сравнить полученные указатели
    return pI1 == pI2;
}

void f(IX* pIX)
{
    IX* pIX2 = NULL;

    // Запросить IX через IX
    HRESULT hr = pIX->QueryInterface(IID_IX, (void**)&pIX2);

    assert(SUCCEEDED(hr)); // Запрос должен быть успешным
}

void f2(IX* pIX)
{
    HRESULT hr;

    IX* pIX2 = NULL;
    IY* pIY = NULL;

    // Получить IY через IX
    hr = pIX->QueryInterface(IID_IY, (void**)&pIY);

    if (SUCCEEDED(hr))
    {
        // Получить IX через IY
        hr = pIY->QueryInterface(IID_IX, (void**)&pIX2);

        // QueryInterface должна отработать успешно
        assert(SUCCEEDED(hr));
    }
}

void f3(IX* pIX)
{
    HRESULT hr;
    IY* pIY = NULL;

    // Запросить IY у IX
    hr = pIX->QueryInterface(IID_IY, (void**)&pIY);

    if (SUCCEEDED(hr))
    {
        IZ* pIZ = NULL;

        // Запросить IZ и IY
        hr = pIY->QueryInterface(IID_IZ, (void**)&pIZ);

        if (SUCCEEDED(hr))
        {
            // Запросить IZ у IX
            hr = pIX->QueryInterface(IID_IZ, (void**)&pIZ);
            assert(SUCCEEDED(hr)); // Это должно работать
        }
    }
}





// Клиент
int main() {

    setlocale(LC_ALL, "Russian");
    cout << "Hello, world!" << endl;

    HRESULT hr;

    cout << "Client: get pointer to IUnknown" << endl;
    IUnknown* pIUnkonwn = CreateInstance();

    cout << "\nClient: get pointer to IX" << endl;
    IX* pIX = NULL;
    hr = pIUnkonwn->QueryInterface(IID_IX, (void**)&pIX);
    if (SUCCEEDED(hr)) {
        cout << "Client: IX received successfully" << endl;
        pIX->Fx(); // использовать интерфейс IX
    }

    cout << "\nClient: get pointer to IY" << endl;
    IY* pIY = NULL;
    hr = pIUnkonwn->QueryInterface(IID_IY, (void**)&pIY);
    if (SUCCEEDED(hr)) {
        cout << "Client: IY received successfully" << endl;
        pIY->Fy(); // использовать интерфейс IY
    };

    cout << "\nClient: Get unsupported interface" << endl;
    IZ* pIZ = NULL;
    hr = pIUnkonwn->QueryInterface(IID_IZ, (void**)&pIZ);
    if (SUCCEEDED(hr)) {
        cout << "Client: interface IZ get succesfully" << endl;
    }
    else {
        cout << "Client: Can not get interface IZ" << endl;
    }  
    cout << "\nКлиент: Получить указатель на IY через IX" << endl;
    IY* pIYfromIX = NULL;
    hr = pIX->QueryInterface(IID_IY, (void**)&pIYfromIX);
    if (SUCCEEDED(hr)) {
        cout << "Клиент: указатель на IY получен успешно" << endl;
        pIYfromIX->Fy();
    }
    else {
        cout << "Клиент: Невозможно получить указатель на IY через IX" << endl;
    };

    cout << "\nКлиент: Получить указатель на IUnknown через IY" << endl;
    IUnknown* pIUnknownFromIY = NULL;
    hr = pIY->QueryInterface(IID_IUnknown1, (void**)&pIUnknownFromIY);
    if (SUCCEEDED(hr)) {
        cout << "Равны два указателя?" << endl;
        if (pIUnknownFromIY == pIUnkonwn) {
            cout << "ДА" << endl;
        }
        else {
            cout << "HET" << endl;
        }
    };
  

    // Тест функции f
    cout << "\nТест f()" << endl;
    try {
        f(pIX);
        cout << "f() выполнена успешно" << endl;
    }
    catch (...) {
        cout << "Ошибка при выполнении f()" << endl;
    }

    // Тест функции f2
    cout << "\nТест f2()" << endl;
    try {
        f2(pIX);
        cout << "f2() выполнена успешно" << endl;
    }
    catch (...) {
        cout << "Ошибка при выполнении f2()" << endl;
    }

    // Тест функции f3
    cout << "\nТест f3()" << endl;
    try {
        f3(pIX);
        cout << "f3() выполнена успешно" << endl;
    }
    catch (...) {
        cout << "Ошибка при выполнении f3()" << endl;
    }

    // Тест функции SameComponents
    cout << "\nТест SameComponents()" << endl;
    bool result = SameComponents(pIX, pIY);
    cout << "Результат сравнения компонентов: " << (result ? "указатели совпадают" : "указатели различаются") << endl;

    // Добавьте очистку памяти после тестов
    if (pIX != NULL) pIX->Release();
    if (pIY != NULL) pIY->Release();

    // Удалить компонент
    delete pIUnkonwn;

    return 0;

}

