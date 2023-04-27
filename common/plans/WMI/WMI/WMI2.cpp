////
//// Created by LIXIN on 2023/4/21.
////
//#include <iostream>
//#include <string>
//#include <windows.h>
//#include <objbase.h>
//#include <wbemidl.h>
//#include <comdef.h>
//#include <mutex>
//
//#pragma comment(lib, "wbemuuid.lib")
//#pragma comment(lib, "comsuppw.lib")
//
//using namespace std;
//
//// wmi namespace
//IWbemLocator  *wmiLocator = NULL;
//IWbemServices *wmiNamespace = 0;
//BSTR wmiPath = SysAllocString(L"root\\wmi");
//HRESULT hr = S_OK;
//
//// brightnessAdjustmentClass
//IWbemClassObject *brightnessAdjustmentClass = NULL;
//IWbemClassObject *brightnessAdjustmentClassInstant = NULL;
//IWbemClassObject *brightnessAdjustmentClassObject = NULL;
//IWbemClassObject *brightnessAdjustmentMethodInstant = NULL;
//IEnumWbemClassObject *brightnessAdjustmentClassEnum = NULL;
//
//BSTR brightnessAdjustmentClassPath = SysAllocString(L"WmiMonitorBrightnessMethods");
//BSTR brightnessAdjustmentMethodName = SysAllocString(L"WmiSetBrightness");
//BSTR brightnessAdjustmentMethodArgName1 = SysAllocString(L"Timeout");
//BSTR brightnessAdjustmentMethodArgName2 = SysAllocString(L"Brightness");
//BSTR brightnessAdjustmentClassQuery = SysAllocString(L"Select * from WmiMonitorBrightnessMethods");
//
//
//// brightnessClass
//IEnumWbemClassObject *brightnessClassEnum = NULL;
////IWbemClassObject *brightnessClass = NULL;
//IWbemClassObject *brightnessClassObject = NULL;
//
//BSTR brightnessClassPath = SysAllocString(L"WmiMonitorBrightness");
//BSTR brightnessVariableName = SysAllocString(L"CurrentBrightness");
//BSTR brightnessQuery = SysAllocString(L"Select * from WmiMonitorBrightness");
//
//int Init()
//{
//    // Initialize COM and connect up to CIMOM
//    hr = CoInitialize(0);
//    if (FAILED(hr))
//        return -1;
//    hr = CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_DEFAULT, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, NULL);
//    if (FAILED(hr))
//        return -1;
//    hr = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, (LPVOID *)&wmiLocator);
//    if (FAILED(hr))
//        return -1;
//    hr = wmiLocator->ConnectServer(wmiPath, NULL, NULL, NULL, 0, NULL, NULL, &wmiNamespace);
//    if (hr != WBEM_S_NO_ERROR)
//        return -1;
//    hr = CoSetProxyBlanket(wmiNamespace, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL, RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE);
//    if (hr != WBEM_S_NO_ERROR)
//        return -1;
//
//
//    // query variable enum
//    hr = wmiNamespace->ExecQuery(_bstr_t(L"WQL"), brightnessAdjustmentClassQuery, WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, NULL, &brightnessAdjustmentClassEnum);
//    if (hr != WBEM_S_NO_ERROR)
//        return -1;
//    /*hr = wmiNamespace->ExecQuery(_bstr_t(L"WQL"), brightnessQuery, WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, NULL, &brightnessClassEnum);
//    if (hr != WBEM_S_NO_ERROR)
//        return -1;*/
//
//
//    // get class obj
//    ULONG ret;
//    hr = brightnessAdjustmentClassEnum->Next(WBEM_INFINITE, 1, &brightnessAdjustmentClassObject, &ret);
//    if (hr != WBEM_S_NO_ERROR)
//        return -1;
//
//    hr = wmiNamespace->GetObject(brightnessAdjustmentClassPath, 0, NULL, &brightnessAdjustmentClass, NULL);
//    if (hr != WBEM_S_NO_ERROR)
//        return -1;
//
//    // get brightness adjust method instant
//    hr = brightnessAdjustmentClass->GetMethod(brightnessAdjustmentMethodName, 0, &brightnessAdjustmentClassInstant, NULL);
//    if (hr != WBEM_S_NO_ERROR)
//        return -1;
//    hr = brightnessAdjustmentClassInstant->SpawnInstance(0, &brightnessAdjustmentMethodInstant);
//    if (hr != WBEM_S_NO_ERROR)
//        return -1;
//
//    return 0;
//}
//
//int GetBrightness()
//{
//    // 每次都需要查询
//    if (brightnessClassEnum) brightnessClassEnum->Release();
//    if (brightnessClassObject) brightnessClassObject->Release();
//
//    HRESULT hr = wmiNamespace->ExecQuery(_bstr_t(L"WQL"), brightnessQuery, WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, NULL, &brightnessClassEnum);
//    if (hr != WBEM_S_NO_ERROR)
//        return -1;
//
//    DWORD ret;
//    hr = brightnessClassEnum->Next(WBEM_INFINITE, 1, &brightnessClassObject, &ret);
//    if (hr != WBEM_S_NO_ERROR)
//        return -1;
//
//    // 获得亮度值
//    VARIANT brightnessValue;
//    VariantInit(&brightnessValue);
//    hr = brightnessClassObject->Get(brightnessVariableName, 0, &brightnessValue, 0, 0);
//    if (hr != WBEM_S_NO_ERROR)
//        return -1;
//    return brightnessValue.uintVal;
//}
//
//int SetBrightness(int brightness)
//{
//    // 亮度范围是 [0, 100]
//    brightness = min(max(brightness, 0), 100);
//    // write brightness adjust params
//    VARIANT var;
//    VariantInit(&var);
//    V_VT(&var) = VT_BSTR;
//
//    // write timeout param
//    V_BSTR(&var) = SysAllocString(L"0");
//    hr = brightnessAdjustmentMethodInstant->Put(brightnessAdjustmentMethodArgName1, 0, &var, CIM_UINT32);
//    if (hr != WBEM_S_NO_ERROR)
//        return -1;
//
//    // write brightness param
//    V_BSTR(&var) = SysAllocString(to_wstring(brightness).c_str());
//    hr = brightnessAdjustmentMethodInstant->Put(brightnessAdjustmentMethodArgName2, 0, &var, CIM_UINT8);
//    VariantClear(&var);
//    if (hr != WBEM_S_NO_ERROR)
//        return -1;
//
//    // 获取 method instant path 和 execute
//    VARIANT pathVariable;
//    VariantInit(&pathVariable);
//    hr = brightnessAdjustmentClassObject->Get(_bstr_t(L"__PATH"), 0, &pathVariable, NULL, NULL);
//    if (hr != WBEM_S_NO_ERROR)
//        return -1;
//    hr =wmiNamespace->ExecMethod(pathVariable.bstrVal, brightnessAdjustmentMethodName, 0, NULL, brightnessAdjustmentMethodInstant, NULL, NULL);
//    VariantClear(&pathVariable);
//    if (hr != WBEM_S_NO_ERROR)
//        return -1;
//    return 0;
//}
//
//void Cleanup()
//{
//    // 清理
//    SysFreeString(wmiPath);
//    SysFreeString(brightnessAdjustmentClassPath);
//    SysFreeString(brightnessAdjustmentMethodName);
//    SysFreeString(brightnessAdjustmentMethodArgName1);
//    SysFreeString(brightnessAdjustmentMethodArgName2);
//    SysFreeString(brightnessAdjustmentClassQuery);
//
//    SysFreeString(brightnessClassPath);
//    SysFreeString(brightnessVariableName);
//    SysFreeString(brightnessQuery);
//
//    brightnessAdjustmentClass->Release();
//    brightnessAdjustmentMethodInstant->Release();
//    brightnessAdjustmentClassInstant->Release();
//    brightnessAdjustmentClassObject->Release();
//    brightnessAdjustmentClassEnum->Release();
//
//    //if (brightnessClass) brightnessClass->Release();
//    brightnessClassObject->Release();
//    brightnessClassEnum->Release();
//
//    wmiNamespace->Release();
//    wmiLocator->Release();
//
//    CoUninitialize();
//}
//
//// 亮度进阶值
//const int incrementValue = 5;
//
//void IncreaseBrightness()
//{
//    // 提升一阶亮度
//    int i = GetBrightness();
//    SetBrightness(i + incrementValue);
//}
//void ReduceBrightness()
//{
//    // 降低一阶亮度
//    int i = GetBrightness();
//    SetBrightness(i - incrementValue);
//}
//
//ATOM hotKeyId1, hotKeyId2;
//
//LRESULT __stdcall WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
//{
//    switch (Msg)
//    {
//        case WM_DESTROY:
//            PostQuitMessage(0);
//            break;
//        case WM_HOTKEY:
//            if (wParam == hotKeyId1)
//                PostMessageA(0, WM_USER + 1, 0, 0);
//            else if (wParam == hotKeyId2)
//                PostMessageA(0, WM_USER + 2, 0, 0);
//            break;
//    }
//    return DefWindowProcA(hWnd, Msg, wParam, lParam);
//}
////
////int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
////{
////    // 全局唯一
////    auto m = CreateMutexA(NULL, TRUE, "{CAF16FA1-1E0C-419B-9D45-99B5719ED318}");
////
////    if (ERROR_ALREADY_EXISTS == GetLastError())
////        return -1;
////
////    int r = Init();
////    if (r != 0)
////        return -1;
////
////    //创建隐藏窗口，仅用于捕获热键
////    WNDCLASSEX wcex;
////    memset(&wcex, 0, sizeof(wcex));
////    wcex.cbSize     = sizeof(wcex);
////    wcex.style      = 0;
////    wcex.lpfnWndProc= WndProc;
////    wcex.cbClsExtra = 0;
////    wcex.cbWndExtra = 0;
////    wcex.hInstance  = GetModuleHandleA(0);
////    wcex.hIcon      = 0;
////    wcex.hCursor    = 0;
////    wcex.hbrBackground  = 0;
////    wcex.lpszMenuName   = 0;
////    wcex.lpszClassName  = "test";
////    if (RegisterClassExA(&wcex) == FALSE)
////        return -1;
////    HWND hwnd = CreateWindowA("test", "", WS_EX_NOACTIVATE, 0, 0, 0, 0, 0, 0, GetModuleHandleA(0), 0);
////    ShowWindow(hwnd, SW_HIDE);
////
////    //注册热键
////    hotKeyId1 = GlobalAddAtom("MonitorBrightnessAdjuster1") - 0xc000;
////    hotKeyId2 = GlobalAddAtom("MonitorBrightnessAdjuster2") - 0xc000;
////    int b = RegisterHotKey(hwnd, hotKeyId1, MOD_SHIFT | MOD_ALT, VK_F8);
////    b = RegisterHotKey(hwnd, hotKeyId2, MOD_SHIFT | MOD_ALT, VK_F9);
////
////    MSG msg;
////    while (GetMessageA(&msg, 0, 0, 0))
////    {
////        TranslateMessage(&msg);
////        DispatchMessageW(&msg);
////        if (msg.message == WM_USER + 1)
////            ReduceBrightness();
////        else if (msg.message == WM_USER + 2)
////            IncreaseBrightness();
////    }
////    // 一般不会到达这里，一般是直接任务管理器结束此程序
////    // 清理
////    UnregisterHotKey(hwnd, hotKeyId1);
////    UnregisterHotKey(hwnd, hotKeyId2);
////    GlobalDeleteAtom(hotKeyId1);
////    GlobalDeleteAtom(hotKeyId2);
////    Cleanup();
////    return 0;
////}