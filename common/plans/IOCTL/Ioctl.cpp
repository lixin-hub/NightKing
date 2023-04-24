//
// Created by LIXIN on 2023/4/19.
//DDC_CI/CI只在支持的硬件才有效
//

#include "Ioctl.h"
#include <windows.h>
#include <iostream>
#include <Ntddvdeo.h>
#include "QDebug"
#define _CRT_SECURE_NO_WARNINGS
#define IOCTL_VIDEO_QUERY_SUPPORTED_BRIGHTNESS  CTL_CODE(FILE_DEVICE_VIDEO, 0x125, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_VIDEO_QUERY_DISPLAY_BRIGHTNESS 	CTL_CODE(FILE_DEVICE_VIDEO, 0x126, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_VIDEO_SET_DISPLAY_BRIGHTNESS 		CTL_CODE(FILE_DEVICE_VIDEO, 0x127, METHOD_BUFFERED, FILE_ANY_ACCESS)

/**
 * @param level 调节笔记本亮度值
 * @return 是否调节成功
 */
bool Ioctl::setBrightness(int level)
{
    HANDLE h;
    DWORD nOutBufferSize = 256;
    BYTE SupportedBrightness[256];
    DWORD g_supportedLevelCount;
    DISPLAY_BRIGHTNESS DisplayBrightness;

    memset(SupportedBrightness, 0, sizeof(SupportedBrightness));

    /* use createfile function to open lcd.
    * url from microsoft about IOCTL code.
    *  http://msdn.microsoft.com/en-us/library/windows/desktop/aa372703%28v=vs.85%29.aspx
    */

    /*
    * char* 和 wchar_t* 互相转换参考博客
    * https://www.cnblogs.com/icqw/p/4614877.html
    */
    /*
    * c++代码参考博客
    * https://blog.csdn.net/weixin_34111819/article/details/86328019
    */
    char temchar[] = "\\\\.\\LCD";
    char* szSour = temchar;
    WCHAR Temp[128] = { 0 };
    mbstowcs(Temp, szSour, strlen(szSour));
    h = CreateFileW(Temp, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, 0);
    if (h == INVALID_HANDLE_VALUE) {
        qDebug()<<"Open \\\\.\\LCD error";
        return false;
    }

    /* Query for display supported level */
    if (!DeviceIoControl(h, IOCTL_VIDEO_QUERY_SUPPORTED_BRIGHTNESS, NULL, 0, SupportedBrightness, nOutBufferSize, &g_supportedLevelCount, NULL)) {
        qDebug()<<"IOCTL_VIDEO_QUERY_SUPPORTED_BRIGHTNESS error - %d, - buffer: %d ", g_supportedLevelCount, nOutBufferSize;
        return false;
    }
    if (g_supportedLevelCount == 0)  	/* 0 means not supported */
    {
        qDebug()<<"\nLCD not support LEVEL COUNT", g_supportedLevelCount;
        return false;
    }

    DisplayBrightness.ucDisplayPolicy = 0;
    DisplayBrightness.ucACBrightness = level;
    DisplayBrightness.ucDCBrightness = level;

    /* Set display backlight level */
    nOutBufferSize = sizeof(DisplayBrightness);
    if (!DeviceIoControl(h, IOCTL_VIDEO_SET_DISPLAY_BRIGHTNESS, (DISPLAY_BRIGHTNESS*)&DisplayBrightness, nOutBufferSize, NULL, 0, &nOutBufferSize, NULL)) {
        qDebug()<<"IOCTL_VIDEO_SET_SUPPORTED_BRIGHTNESS error - %d, - buffer: %d ", g_supportedLevelCount, nOutBufferSize;
        return false;
    }

//    Sleep(500); /* delay for some time while wmi event changed */
    nOutBufferSize = sizeof(DisplayBrightness);
    if (!DeviceIoControl(h, IOCTL_VIDEO_QUERY_DISPLAY_BRIGHTNESS, NULL, 0, (DISPLAY_BRIGHTNESS*)&DisplayBrightness, nOutBufferSize, &nOutBufferSize, NULL)) {
        qDebug()<<"IOCTL_VIDEO_QUERY_SUPPORTED_BRIGHTNESS error - %d, - buffer: %d ", g_supportedLevelCount, nOutBufferSize;
        return false;
    }
    qDebug()<<"\nBrightness_AC: %d\nBrightness_DC: %d", DisplayBrightness.ucACBrightness, DisplayBrightness.ucDCBrightness;
}
