//
// Created by LIXIN on 2023/4/20.
//主要针对笔记本电脑
//

#include <wbemcli.h>
#include <unistd.h>
#include "WMI.h"
#include "QApplication"
#include "common/monitor/mymonitors.h"

using namespace std;
static bool isInit = false;
// wmi namespace
IWbemLocator *wmiLocator = NULL;
IWbemServices *wmiNamespace = 0;
BSTR wmiPath = SysAllocString(L"root\\wmi");
HRESULT hr = S_OK;

// brightnessAdjustmentClass
IWbemClassObject *brightnessAdjustmentClass = NULL;
IWbemClassObject *brightnessAdjustmentClassInstant = NULL;
IWbemClassObject *brightnessAdjustmentClassObject = NULL;
IWbemClassObject *brightnessAdjustmentMethodInstant = NULL;
IEnumWbemClassObject *brightnessAdjustmentClassEnum = NULL;

BSTR brightnessAdjustmentClassPath = SysAllocString(L"WmiMonitorBrightnessMethods");
BSTR brightnessAdjustmentMethodName = SysAllocString(L"WmiSetBrightness");
BSTR brightnessAdjustmentMethodArgName1 = SysAllocString(L"Timeout");
BSTR brightnessAdjustmentMethodArgName2 = SysAllocString(L"Brightness");
BSTR brightnessAdjustmentClassQuery = SysAllocString(L"Select * from WmiMonitorBrightnessMethods");


// brightnessClass
IEnumWbemClassObject *brightnessClassEnum = NULL;
//IWbemClassObject *brightnessClass = NULL;
IWbemClassObject *brightnessClassObject = NULL;

BSTR brightnessClassPath = SysAllocString(L"WmiMonitorBrightness");
BSTR brightnessVariableName = SysAllocString(L"CurrentBrightness");
BSTR brightnessQuery = SysAllocString(L"Select * from WmiMonitorBrightness");

void Cleanup() {
    // 清理
    SysFreeString(wmiPath);
    SysFreeString(brightnessAdjustmentClassPath);
    SysFreeString(brightnessAdjustmentMethodName);
    SysFreeString(brightnessAdjustmentMethodArgName1);
    SysFreeString(brightnessAdjustmentMethodArgName2);
    SysFreeString(brightnessAdjustmentClassQuery);

    SysFreeString(brightnessClassPath);
    SysFreeString(brightnessVariableName);
    SysFreeString(brightnessQuery);

    brightnessAdjustmentClass->Release();
    brightnessAdjustmentMethodInstant->Release();
    brightnessAdjustmentClassInstant->Release();
    brightnessAdjustmentClassObject->Release();
    brightnessAdjustmentClassEnum->Release();

    //if (brightnessClass) brightnessClass->Release();
    brightnessClassObject->Release();
    brightnessClassEnum->Release();

    wmiNamespace->Release();
    wmiLocator->Release();

    CoUninitialize();
}

int Init() {
    qDebug() << "初始化COM";
    // Initialize COM and connect up to CIMOM
    hr = CoInitialize(0);
    if (FAILED(hr))
        return -1;
    hr = CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_DEFAULT, RPC_C_IMP_LEVEL_IMPERSONATE, NULL,
                              EOAC_NONE, NULL);
    if (FAILED(hr))
        return -1;
    hr = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, (LPVOID * ) & wmiLocator);
    if (FAILED(hr))
        return -1;
    hr = wmiLocator->ConnectServer(wmiPath, NULL, NULL, NULL, 0, NULL, NULL, &wmiNamespace);
    if (hr != WBEM_S_NO_ERROR)
        return -1;
    hr = CoSetProxyBlanket(wmiNamespace, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL, RPC_C_AUTHN_LEVEL_CALL,
                           RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE);
    if (hr != WBEM_S_NO_ERROR)
        return -1;


    // query variable enum
    hr = wmiNamespace->ExecQuery(_bstr_t(L"WQL"), brightnessAdjustmentClassQuery,
                                 WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, NULL,
                                 &brightnessAdjustmentClassEnum);
    if (hr != WBEM_S_NO_ERROR)
        return -1;
    /*hr = wmiNamespace->ExecQuery(_bstr_t(L"WQL"), brightnessQuery, WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, NULL, &brightnessClassEnum);
    if (hr != WBEM_S_NO_ERROR)
        return -1;*/


    // get class obj
    ULONG ret;
    hr = brightnessAdjustmentClassEnum->Next(WBEM_INFINITE, 1, &brightnessAdjustmentClassObject, &ret);
    if (hr != WBEM_S_NO_ERROR)
        return -1;

    hr = wmiNamespace->GetObject(brightnessAdjustmentClassPath, 0, NULL, &brightnessAdjustmentClass, NULL);
    if (hr != WBEM_S_NO_ERROR)
        return -1;

    // get brightness adjust method instant
    hr = brightnessAdjustmentClass->GetMethod(brightnessAdjustmentMethodName, 0, &brightnessAdjustmentClassInstant,
                                              NULL);
    if (hr != WBEM_S_NO_ERROR)
        return -1;
    hr = brightnessAdjustmentClassInstant->SpawnInstance(0, &brightnessAdjustmentMethodInstant);
    if (hr != WBEM_S_NO_ERROR)
        return -1;
    return 0;
}

int SetBrightness(int brightness) {
    // 亮度范围是 [0, 100]
    brightness = min(max(brightness, 0), 100);
    // write brightness adjust params
    VARIANT var;
    VariantInit(&var);
    V_VT(&var) = VT_BSTR;

    // write timeout param
    V_BSTR(&var) = SysAllocString(L"0");
    hr = brightnessAdjustmentMethodInstant->Put(brightnessAdjustmentMethodArgName1, 0, &var, CIM_UINT32);
    if (hr != WBEM_S_NO_ERROR)
        return -1;

    // write brightness param
    V_BSTR(&var) = SysAllocString(to_wstring(brightness).c_str());
    hr = brightnessAdjustmentMethodInstant->Put(brightnessAdjustmentMethodArgName2, 0, &var, CIM_UINT8);
    VariantClear(&var);
    if (hr != WBEM_S_NO_ERROR)
        return -1;

    // 获取 method instant path 和 execute
    VARIANT pathVariable;
    VariantInit(&pathVariable);
    hr = brightnessAdjustmentClassObject->Get(_bstr_t(L"__PATH"), 0, &pathVariable, NULL, NULL);
    if (hr != WBEM_S_NO_ERROR)
        return -1;
    hr = wmiNamespace->ExecMethod(pathVariable.bstrVal, brightnessAdjustmentMethodName, 0, NULL,
                                  brightnessAdjustmentMethodInstant, NULL, NULL);
    VariantClear(&pathVariable);
    if (hr != WBEM_S_NO_ERROR)
        return -1;
    return 0;
}

int GetBrightness(){
// 每次都需要查询
    if (brightnessClassEnum) brightnessClassEnum->Release();
    if (brightnessClassObject) brightnessClassObject->Release();


    HRESULT hr = wmiNamespace->ExecQuery(_bstr_t(L"WQL"), brightnessQuery,
                                         WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, NULL,
                                         &brightnessClassEnum);

    if (hr != WBEM_S_NO_ERROR)
        return -1;

    DWORD ret;
    hr = brightnessClassEnum->Next(WBEM_INFINITE, 1, &brightnessClassObject, &ret);
    if (hr != WBEM_S_NO_ERROR)
        return -1;

    // 获得亮度值
    VARIANT brightnessValue;
    VariantInit(&brightnessValue);
    hr = brightnessClassObject->Get(brightnessVariableName, 0, &brightnessValue, 0, 0);
    if (hr != WBEM_S_NO_ERROR)
        return -1;
    return brightnessValue.uintVal;
}

bool WMI::setBrightness(int bValue) {
    if (!isInit) {
        Init();
        isInit = true;
    }
    if (SetBrightness(bValue) == 0) {
        return true;
    }
    return false;
}

bool WMI::getBrightness(MonitorBrightness &monitorBrightness) {
    isInit = false;
    Init();
    int value = GetBrightness();
    monitorBrightness.currentBrightness = value;
    return value >= 0;
}

WMI::~WMI() {
    Cleanup();
}


/*
void cleanUp(BSTR path, BSTR ClassPath, BSTR MethodName, BSTR ArgName0, BSTR ArgName1, BSTR bstrQuery,
             IWbemClassObject *pClass, IWbemClassObject *pInInst, IWbemClassObject *pInClass, IWbemLocator *pLocator,
             IWbemServices *pNamespace) {
    SysFreeString(path);
    SysFreeString(ClassPath);
    SysFreeString(MethodName);
    SysFreeString(ArgName0);
    SysFreeString(ArgName1);
    SysFreeString(bstrQuery);

    if (pClass) pClass->Release();
    if (pInInst) pInInst->Release();
    if (pInClass) pInClass->Release();
    if (pLocator) pLocator->Release();
    if (pNamespace) pNamespace->Release();
    CoUninitialize();
};

bool WMI::setBrightness(int bValue) {
    IWbemLocator *pLocator = NULL;
    IWbemServices *pNamespace = 0;
    IWbemClassObject *pClass = NULL;
    IWbemClassObject *pInClass = NULL;
    IWbemClassObject *pInInst = NULL;
    IEnumWbemClassObject *pEnum = NULL;
    HRESULT hr = S_OK;
    BSTR path = SysAllocString(L"root\\wmi");
    BSTR ClassPath = SysAllocString(L"WmiMonitorBrightnessMethods");
    BSTR MethodName = SysAllocString(L"WmiSetBrightness");
    BSTR ArgName0 = SysAllocString(L"Timeout");
    BSTR ArgName1 = SysAllocString(L"Brightness");
    BSTR bstrQuery = SysAllocString(L"Select * from WmiMonitorBrightnessMethods");

    if (!path || !ClassPath || !MethodName || !ArgName0) {
        qDebug() << "SysAllocString failed. Out of memory.";
        { qDebug() << "\n[-] Err: xxx"; }
        cleanUp(path, ClassPath, MethodName, ArgName0, ArgName1, bstrQuery, pClass, pInInst, pInClass, pLocator,
                pNamespace);
        return false;
    }

    // Initialize COM and connect up to CIMOM

    hr = CoInitialize(0);
    if (FAILED(hr)) {
        qDebug() << "CoInitialize returned 0x%x:", hr;
        { qDebug() << "\n[-] Err: CoInitialize"; }
        cleanUp(path, ClassPath, MethodName, ArgName0, ArgName1, bstrQuery, pClass, pInInst, pInClass, pLocator,
                pNamespace);
        return false;
    }

    //  NOTE:
    //  When using asynchronous WMI API's remotely in an environment where the "Local System" account
    //  has no network identity (such as non-Kerberos domains), the authentication level of
    //  RPC_C_AUTHN_LEVEL_NONE is needed. However, lowering the authentication level to
    //  RPC_C_AUTHN_LEVEL_NONE makes your application less secure. It is wise to
    // use semi-synchronous API's for accessing WMI data and events instead of the asynchronous ones.
//        hr = CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_DEFAULT, RPC_C_IMP_LEVEL_IMPERSONATE, NULL,
//                                  EOAC_NONE, //change to EOAC_NONE if you change dwAuthnLevel to RPC_C_AUTHN_LEVEL_NONE
//                                  NULL);
//        if (FAILED(hr)) {
//            qDebug() << "CoInitializeSecurity returned 0x:" << hr;
//            { qDebug() << "\n[-] Err: CoInitializeSecurity"; }
//            cleanUp(path, ClassPath, MethodName, ArgName0, ArgName1, bstrQuery, pClass, pInInst, pInClass, pLocator,
//                    pNamespace);
//            return false;
//        }
    hr = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, (LPVOID * ) & pLocator);
    if (FAILED(hr)) {
        qDebug() << "CoCreateInstance returned 0x:" << hr;
        { qDebug() << "\n[-] Err: CoCreateInstance"; }
        cleanUp(path, ClassPath, MethodName, ArgName0, ArgName1, bstrQuery, pClass, pInInst, pInClass, pLocator,
                pNamespace);
        return false;
    }
    hr = pLocator->ConnectServer(path, NULL, NULL, NULL, 0, NULL, NULL, &pNamespace);
    if (hr != WBEM_S_NO_ERROR) {
        qDebug() << "\n[-] Err: ConnectServer";
        cleanUp(path, ClassPath, MethodName, ArgName0, ArgName1, bstrQuery, pClass, pInInst, pInClass, pLocator,
                pNamespace);
        return false;
    }

    hr = CoSetProxyBlanket(pNamespace, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL, RPC_C_AUTHN_LEVEL_CALL,
                           RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE);

    if (hr != WBEM_S_NO_ERROR) {
        qDebug() << "\n[-] Err: CoSetProxyBlanket";
        cleanUp(path, ClassPath, MethodName, ArgName0, ArgName1, bstrQuery, pClass, pInInst, pInClass, pLocator,
                pNamespace);
        return false;
    }

    hr = pNamespace->ExecQuery(_bstr_t(L"WQL"), //Query Language
                               bstrQuery, //Query to Execute
                               WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, //Make a semi-synchronous call
                               NULL, //Context
                               &pEnum
//Enumeration Interface
    );

    if (hr != WBEM_S_NO_ERROR) {
        qDebug() << "\n[-] Err: ExecQuery";
        cleanUp(path, ClassPath, MethodName, ArgName0, ArgName1, bstrQuery, pClass, pInInst, pInClass, pLocator,
                pNamespace);
        return false;
    }

    hr = WBEM_S_NO_ERROR;

    ULONG ulReturned;
    IWbemClassObject *pObj;
    DWORD retVal = 0;

    //Get the Next Object from the collection
    hr = pEnum->Next(WBEM_INFINITE, //Timeout
                     1, //No of objects requested
                     &pObj, //Returned Object
                     &ulReturned
//No of object returned
    );

    if (hr != WBEM_S_NO_ERROR) {
        qDebug() << "\n[-] Err: Next";
        cleanUp(path, ClassPath, MethodName, ArgName0, ArgName1, bstrQuery, pClass, pInInst, pInClass, pLocator,
                pNamespace);
        return false;
    }

    // Get the class object
    hr = pNamespace->GetObject(ClassPath, 0, NULL, &pClass, NULL);
    if (hr != WBEM_S_NO_ERROR) {
        qDebug() << "\n[-] Err: GetObject";
        cleanUp(path, ClassPath, MethodName, ArgName0, ArgName1, bstrQuery, pClass, pInInst, pInClass, pLocator,
                pNamespace);
        return false;
    }

    // Get the input argument and set the property
    hr = pClass->GetMethod(MethodName, 0, &pInClass, NULL);
    if (hr != WBEM_S_NO_ERROR) {
        qDebug() << "\n[-] Err: GetMethod";
        cleanUp(path, ClassPath, MethodName, ArgName0, ArgName1, bstrQuery, pClass, pInInst, pInClass, pLocator,
                pNamespace);
        return false;
    }

    hr = pInClass->SpawnInstance(0, &pInInst);
    if (hr != WBEM_S_NO_ERROR) {
        qDebug() << "\n[-] Err: SpawnInstance";
        cleanUp(path, ClassPath, MethodName, ArgName0, ArgName1, bstrQuery, pClass, pInInst, pInClass, pLocator,
                pNamespace);
        return false;
    }

    VARIANT var1;
    VariantInit(&var1);

    V_VT(&var1) = VT_BSTR;
    V_BSTR(&var1) = SysAllocString(L"0");
    hr = pInInst->Put(ArgName0, 0, &var1, CIM_UINT32); //CIM_UINT64

    //var1.vt = VT_I4;
    //var1.ullVal = 0;
    //hr = pInInst->Put(ArgName0, 0, &var1, 0);
    VariantClear(&var1);
    if (hr != WBEM_S_NO_ERROR) {
        qDebug() << "\n[-] Err: Put ArgName0";
        cleanUp(path, ClassPath, MethodName, ArgName0, ArgName1, bstrQuery, pClass, pInInst, pInClass, pLocator,
                pNamespace);
        return false;
    }

    VARIANT var;
    VariantInit(&var);

    V_VT(&var) = VT_BSTR;

    char *p = new char[3];
//    数字转换为字符串
    sprintf(p, "%d", bValue);
    //将char 转换为2个字节
    OLECHAR *str = (OLECHAR *) malloc(((strlen(p) + 1) * sizeof(OLECHAR)));
    for (int i = 0; i < strlen(p); i++) {
        str[i] = p[i];
    }
    str[strlen(p) + 1] = '\0';
    V_BSTR(&var) = SysAllocString(str);
    hr = pInInst->Put(ArgName1, 0, &var, CIM_UINT8);

    //var.vt=VT_UI1;
    //var.uiVal = 100;
    //hr = pInInst->Put(ArgName1, 0, &var, 0);
    VariantClear(&var);
    if (hr != WBEM_S_NO_ERROR) {
        qDebug() << "\n[-] Err: Put ArgName1";
        cleanUp(path, ClassPath, MethodName, ArgName0, ArgName1, bstrQuery, pClass, pInInst, pInClass, pLocator,
                pNamespace);
        return false;
    }
    // Call the method

    VARIANT pathVariable;
    VariantInit(&pathVariable);

    hr = pObj->Get(_bstr_t(L"__PATH"), 0, &pathVariable, NULL, NULL);
    if (hr != WBEM_S_NO_ERROR) {
        qDebug() << "\n[-] Err: Get";
        cleanUp(path, ClassPath, MethodName, ArgName0, ArgName1, bstrQuery, pClass, pInInst, pInClass, pLocator,
                pNamespace);
        return false;
    }

    hr = pNamespace->ExecMethod(pathVariable.bstrVal, MethodName, 0, NULL, pInInst, NULL, NULL);
    VariantClear(&pathVariable);
    if (hr != WBEM_S_NO_ERROR) {
        qDebug() << "\n[-] Err: ExecMethod";
        cleanUp(path, ClassPath, MethodName, ArgName0, ArgName1, bstrQuery, pClass, pInInst, pInClass, pLocator,
                pNamespace);
        return false;
    }
// Free up
    cleanUp(path, ClassPath, MethodName, ArgName0, ArgName1, bstrQuery, pClass, pInInst, pInClass, pLocator,
            pNamespace);
    return true;

}*/
