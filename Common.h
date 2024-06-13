#pragma once

#ifndef __COMMON_H__
#define __COMMON_H__
#include "FlyCapture2GUI.h"
#include"FlyCapture2Defs.h"
#include"FlyCapture2.h"
#include"FlyCapture2Platform.h"

#include <streambuf>
#include <string>
#include<vector>
#include<iostream>
#include<fstream>
#include<sstream>
#include<thread>
#include<Windows.h>
#include<mutex>


#include"QtCore/Qdebug"


struct CamPropertyData {
	float value;
	bool isAuto;
	bool onOff;
	bool onePush;
	CamPropertyData() {
		value = 0;
		isAuto = false;
		onOff = false;
		onePush = false;
	}
	CamPropertyData(float v, bool a, bool of, bool push) {
		value = v;
		isAuto = a;
		of = onOff;
		onePush = push;
	}
};
//static QOut qout;
#endif