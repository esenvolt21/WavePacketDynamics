
// ModelTask4.h: главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CModelTask4App:
// Сведения о реализации этого класса: ModelTask4.cpp
//

class CModelTask4App : public CWinApp
{
public:
	CModelTask4App();

// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
};

extern CModelTask4App theApp;
