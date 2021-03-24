
// ModelTask4Dlg.cpp: файл реализации
//

#include "pch.h"
#include "framework.h"
#include "ModelTask4.h"
#include "ModelTask4Dlg.h"
#include "afxdialogex.h"
#include <fstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define DOTSGRAPH(x,y) (xpGraph*((x)-xminGraph)),(ypGraph*((y)-ymaxGraph)) // макрос перевода координат для графика сигналa
#define DOTSANIMATION(x,y) (xpEvolution*((x)-xminEvolution)),(ypEvolution*((y)-ymaxEvolution)) // макрос перевода координат для графика сигналa

complex<double> i_mnim(0.0, 1.0);

// Диалоговое окно CModelTask4Dlg

CModelTask4Dlg::CModelTask4Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MODELTASK4_DIALOG, pParent)
	, amplituda(5)
	, t0_param(0)
	, dispersia(2.5)
	, a_GU(10)
	, x_step(0.1)
	, t_max(256)
	, t_step(0.2)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CModelTask4Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, amplituda);
	DDX_Text(pDX, IDC_EDIT2, t0_param);
	DDX_Text(pDX, IDC_EDIT3, dispersia);
	DDX_Text(pDX, IDC_EDIT4, a_GU);
	DDX_Text(pDX, IDC_EDIT5, x_step);
	DDX_Text(pDX, IDC_EDIT6, t_max);
	DDX_Text(pDX, IDC_EDIT7, t_step);
	DDX_Control(pDX, IDC_BUTTON1, StartStop);
	DDX_Control(pDX, IDC_RADIO9, U_x);
	DDX_Control(pDX, IDC_RADIO10, Psi0_x);
	DDX_Control(pDX, IDC_RADIO1, real);
	DDX_Control(pDX, IDC_RADIO2, image);
	DDX_Control(pDX, IDC_RADIO3, modul);
	DDX_Control(pDX, IDC_CHECK1, recording);
	DDX_Control(pDX, IDC_SLIDER1, sliderBPF);
	DDX_Control(pDX, IDC_CHECK2, BPFchek);
	DDX_Control(pDX, IDC_SLIDER2, koord_peak);
	DDX_Control(pDX, IDC_CHECK3, VosstPsi);
	DDX_Control(pDX, IDC_RADIO8, raschet);
}

BEGIN_MESSAGE_MAP(CModelTask4Dlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CModelTask4Dlg::OnBnClickedStartEvolution)
	ON_BN_CLICKED(IDC_RADIO8, &CModelTask4Dlg::OnBnClickedСalculate)
	ON_BN_CLICKED(IDC_RADIO9, &CModelTask4Dlg::OnBnClickedDrawYama)
	ON_BN_CLICKED(IDC_RADIO10, &CModelTask4Dlg::OnBnClickedDrawKupol)
	ON_BN_CLICKED(IDC_CHECK2, &CModelTask4Dlg::OnBnClickedBPF)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &CModelTask4Dlg::OnNMCustomdrawSlider1)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER2, &CModelTask4Dlg::OnNMCustomdrawSlider2)
	ON_BN_CLICKED(IDC_CHECK3, &CModelTask4Dlg::OnBnClickedVosstPsi)
	ON_BN_CLICKED(IDC_BUTTON2, &CModelTask4Dlg::OnBnClickedSbros)
END_MESSAGE_MAP()


// Обработчики сообщений CModelTask4Dlg

BOOL CModelTask4Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	// TODO: добавьте дополнительную инициализацию

	// TODO: добавьте дополнительную инициализацию
	//для  картинки
	PicWndGraph = GetDlgItem(IDC_KUPOL);
	PicDcGraph = PicWndGraph->GetDC();
	PicWndGraph->GetClientRect(&PicGraph);

	//для  картинки
	PicWndEvolution = GetDlgItem(IDC_EVOLUT);
	PicDcEvolution = PicWndEvolution->GetDC();
	PicWndEvolution->GetClientRect(&PicEvolution);

	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок.  Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void CModelTask4Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
		DrawDC(PicDcGraph, PicGraph, 50);
		DrawDC(PicDcEvolution, PicEvolution, 50);
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR CModelTask4Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CModelTask4Dlg::Mashtab(vector<double>& solve_buff, int dim, double* mmin, double* mmax)		//определяем функцию масштабирования
{
	*mmin = *mmax = solve_buff[0];

	for (int i = 0; i < dim; i++)
	{
		if (*mmin > solve_buff[i]) *mmin = solve_buff[i];
		if (*mmax < solve_buff[i]) *mmax = solve_buff[i];
	}
}

void CModelTask4Dlg::DrawDC(CDC* WinDc, CRect WinxmaxGraphc, double AbsMax)
{
	//ГРАФИК СИГНАЛА

	//область построения
	xminGraph = -AbsMax * 0.08;			//минимальное значение х
	xmaxGraph = AbsMax * 1.25;			//максимальное значение х
	yminGraph = -Max1 * 0.1;			//минимальное значение y
	ymaxGraph = Max1 * 1.15;		//максимальное значение y

	// создание контекста устройства
	CBitmap bmp;
	CDC* MemDc;
	MemDc = new CDC;
	MemDc->CreateCompatibleDC(WinDc);

	double widthX = WinxmaxGraphc.Width() * scale;
	double heightY = WinxmaxGraphc.Height() * scale;
	xpGraph = (widthX / (xmaxGraph - xminGraph));			//Коэффициенты пересчёта координат по Х
	ypGraph = -(heightY / (ymaxGraph - yminGraph));			//Коэффициенты пересчёта координат по У

	bmp.CreateCompatibleBitmap(WinDc, widthX, heightY);
	CBitmap* pBmp = (CBitmap*)MemDc->SelectObject(&bmp);

	// заливка фона графика белым цветом
	MemDc->FillSolidRect(WinxmaxGraphc, RGB(0, 0, 0));

	CPen setka_pen;
	setka_pen.CreatePen(		//для сетки
		PS_DOT,					//пунктирная
		1,						//толщина 1 пиксель
		RGB(71, 74, 81));			//цвет  grey

	CPen osi_pen;
	osi_pen.CreatePen(		//для сетки
		PS_SOLID,				//сплошная линия
		3,						//толщина 3 пикселя
		RGB(255, 255, 255));			//цвет white

	MemDc->SelectObject(&osi_pen);

	//создаём Ось Y
	MemDc->MoveTo(DOTSGRAPH(defaultX0, ymaxGraph));
	MemDc->LineTo(DOTSGRAPH(defaultX0, yminGraph));
	//создаём Ось Х
	MemDc->MoveTo(DOTSGRAPH(xminGraph, -defaultY0));
	MemDc->LineTo(DOTSGRAPH(xmaxGraph, -defaultY0));

	MemDc->SelectObject(&setka_pen);

	//отрисовка сетки по y
	for (double x = xminGraph; x <= xmaxGraph; x += xmaxGraph / scale / 8)
	{
		if (x != 0)
		{
			MemDc->MoveTo(DOTSGRAPH(x, ymaxGraph));
			MemDc->LineTo(DOTSGRAPH(x, yminGraph));
		}
	}
	//отрисовка сетки по x
	for (double y = yminGraph; y <= ymaxGraph; y += ymaxGraph / scale / 4)
	{
		if (y != 0)
		{
			MemDc->MoveTo(DOTSGRAPH(xminGraph, y));
			MemDc->LineTo(DOTSGRAPH(xmaxGraph, y));
		}
	}

	// установка прозрачного фона текста
	MemDc->SetBkMode(TRANSPARENT);
	MemDc->SetTextColor(RGB(255, 255, 255));

	// установка шрифта
	CFont font;
	font.CreateFontW(16, 0, 0, 0, FW_HEAVY, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS || CLIP_LH_ANGLES, DEFAULT_QUALITY, DEFAULT_PITCH, _T("Century Gothic"));
	MemDc->SelectObject(&font);

	//подпись осей
	MemDc->TextOutW(DOTSGRAPH(5, ymaxGraph - 0.03), _T("φ"));	//Y
	MemDc->TextOutW(DOTSGRAPH(xmaxGraph - 15, 0.1), _T("х"));		//X

	//по Y с шагом 5
	for (double i = 0; i <= ymaxGraph; i += ymaxGraph / scale / 8)
	{
		CString str;
		if (i != 0)
		{
			str.Format(_T("%.2f"), i / scale + defaultY0 / scale);
			MemDc->TextOutW(DOTSGRAPH(defaultX0 + xminGraph / 2, i + 0.03 * ymaxGraph), str);
		}
	}
	//по X с шагом 0.5
	for (double j = xminGraph; j <= xmaxGraph; j += xmaxGraph / scale / 8)
	{
		double o = 0.0 / scale;
		CString str;
		if (j != o)
		{
			str.Format(_T("%.0f"), j / scale - defaultX0 / scale);
			MemDc->TextOutW(DOTSGRAPH(j - xmaxGraph / 100, -defaultY0 - 0.02), str);
		}
	}

	// вывод на экран
	WinDc->BitBlt(0, 0, widthX, heightY, MemDc, 0, 0, SRCCOPY);
	delete MemDc;
}

void CModelTask4Dlg::DrawGraph(vector<double>& x, CDC* WinDc, CRect WinxmaxGraphc)
{
	if (U_x.GetCheck() == BST_CHECKED)
	{
		//ГРАФИК СИГНАЛА
		xminGraph = -a_GU * 1.2;
		xmaxGraph = a_GU * 1.2;
		yminGraph = -amplituda * 1.15;			//минимальное значение y
		ymaxGraph = amplituda * 0.25;
	}
	else if (Psi0_x.GetCheck() == BST_CHECKED)
	{
		//ГРАФИК СИГНАЛА
		xminGraph = -a_GU * 1.2;
		xmaxGraph = a_GU * 1.2;
		yminGraph = -0.15;			//минимальное значение y
		ymaxGraph = 1.15;
	}

	// создание контекста устройства
	CBitmap bmp;
	CDC* MemDc;
	MemDc = new CDC;
	MemDc->CreateCompatibleDC(WinDc);

	double widthX = WinxmaxGraphc.Width();
	double heightY = WinxmaxGraphc.Height();
	xpGraph = (widthX / (xmaxGraph - xminGraph));			//Коэффициенты пересчёта координат по Х
	ypGraph = -(heightY / (ymaxGraph - yminGraph));			//Коэффициенты пересчёта координат по У

	bmp.CreateCompatibleBitmap(WinDc, widthX, heightY);
	CBitmap* pBmp = (CBitmap*)MemDc->SelectObject(&bmp);

	// заливка фона графика белым цветом
	MemDc->FillSolidRect(WinxmaxGraphc, RGB(0, 0, 0));

	CPen setka_pen;
	setka_pen.CreatePen(		//для сетки
		PS_DOT,					//пунктирная
		1,						//толщина 1 пиксель
		RGB(71, 74, 81));			//цвет  grey

	CPen osi_pen;
	osi_pen.CreatePen(		//для сетки
		PS_SOLID,				//сплошная линия
		3,						//толщина 3 пикселя
		RGB(255, 255, 255));			//цвет white

	CPen graph_pen;
	graph_pen.CreatePen(
		PS_SOLID,				//сплошная линия
		3,						//толщина 2 пикселя
		RGB(148, 0, 211));			//цвет blue

	CPen graph_pen2;
	graph_pen2.CreatePen(
		PS_SOLID,				//сплошная линия
		3,						//толщина 2 пикселя
		RGB(255, 255, 0));			//цвет blue

	MemDc->SelectObject(&setka_pen);
	//отрисовка сетки по y
	for (double x = xminGraph; x <= xmaxGraph; x += xmaxGraph / scale / 8)
	{
		if (x != 0)
		{
			MemDc->MoveTo(DOTSGRAPH(x, ymaxGraph));
			MemDc->LineTo(DOTSGRAPH(x, yminGraph));
		}
	}
	if (U_x.GetCheck() == BST_CHECKED)
	{
		//отрисовка сетки по x
		for (double y = yminGraph; y <= ymaxGraph; y += (-yminGraph / scale / 4))
		{
			if (y != 0)
			{
				MemDc->MoveTo(DOTSGRAPH(xminGraph, y));
				MemDc->LineTo(DOTSGRAPH(xmaxGraph, y));
			}
		}
	}
	else if (Psi0_x.GetCheck() == BST_CHECKED)
	{
		//отрисовка сетки по x
		for (double y = yminGraph; y <= ymaxGraph; y += ymaxGraph / scale / 4)
		{
			if (y != 0)
			{
				MemDc->MoveTo(DOTSGRAPH(xminGraph, y));
				MemDc->LineTo(DOTSGRAPH(xmaxGraph, y));
			}
		}
	}

	// установка прозрачного фона текста
	MemDc->SetBkMode(TRANSPARENT);
	MemDc->SetTextColor(RGB(255, 255, 255));

	// установка шрифта
	CFont font;
	font.CreateFontW(16, 0, 0, 0, FW_HEAVY, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS || CLIP_LH_ANGLES, DEFAULT_QUALITY, DEFAULT_PITCH, _T("Century Gothic"));

	//по X с шагом 0.5
	for (double j = xminGraph; j <= xmaxGraph; j++)
	{
		MemDc->SelectObject(&font);
		double o = 0.0 / scale;
		CString str;
		if ((int)j != o)
		{
			str.Format(_T("%.0f"), (int)j / scale - defaultX0 / scale);
			MemDc->TextOutW(DOTSGRAPH((int)j, defaultY0 - ymaxGraph * 0.02), str);

			MemDc->SelectObject(&osi_pen);
			MemDc->MoveTo(DOTSGRAPH((int)j, ymaxGraph * 0.03));
			MemDc->LineTo(DOTSGRAPH((int)j, -ymaxGraph * 0.03));
		}
	}

	//создаём Ось Y
	MemDc->MoveTo(DOTSGRAPH(defaultX0, ymaxGraph));
	MemDc->LineTo(DOTSGRAPH(defaultX0, yminGraph));
	//создаём Ось Х
	MemDc->MoveTo(DOTSGRAPH(xminGraph, -defaultY0));
	MemDc->LineTo(DOTSGRAPH(xmaxGraph, -defaultY0));

	//подпись осей
	MemDc->TextOutW(DOTSGRAPH(xmaxGraph * 0.03, ymaxGraph * 0.95), _T("φ"));	//Y
	MemDc->TextOutW(DOTSGRAPH(xmaxGraph * 0.95, ymaxGraph * 0.25), _T("x"));		//X

	xxi = -a_GU;
	// отрисовка
	MemDc->MoveTo(DOTSGRAPH(xxi, x[0]));
	for (int i = 0; i < x.size(); i++)
	{
		if (U_x.GetCheck() == BST_CHECKED)
		{
			MemDc->SelectObject(&graph_pen2);
			MemDc->LineTo(DOTSGRAPH(xxi, x[i]));
		}
		else if (Psi0_x.GetCheck() == BST_CHECKED)
		{
			MemDc->SelectObject(&graph_pen);
			MemDc->LineTo(DOTSGRAPH(xxi, x[i]));
		}
		xxi += x_step;
	}

	// вывод на экран
	WinDc->BitBlt(0, 0, widthX, heightY, MemDc, 0, 0, SRCCOPY);
	delete MemDc;
}

void CModelTask4Dlg::DrawEvolution(vector<complex<double>> vec, CDC* WinDc, CRect WinxmaxGraphc)
{
	vector<double> anim;

	for (int i = 0; i < vec.size(); i++)
	{
		if (real.GetCheck() == BST_CHECKED)
		{
			anim.push_back(vec[i].real());
		}
		else if (image.GetCheck() == BST_CHECKED)
		{
			anim.push_back(vec[i].imag());
		}
		else if (modul.GetCheck() == BST_CHECKED)
		{
			anim.push_back(sqrt(vec[i].real() * vec[i].real() + vec[i].imag() * vec[i].imag()));
		}
		else
		{
			MessageBox(L"Вот балбес, выбери, что рисовать!", L"Information", MB_OK || MB_ICONERROR);
			break;
		}
	}

	Mashtab(anim, anim.size(), &Min1, &Max1);
	//ГРАФИК СИГНАЛА
	xminEvolution = -a_GU * 1.2;
	xmaxEvolution = a_GU * 1.2;
	yminEvolution = (Min1 - Max1) * 1.05;			//минимальное значение y
	ymaxEvolution = (Max1 - Min1) * 1.05;

	// создание контекста устройства
	CBitmap bmp;
	CDC* MemDc;
	MemDc = new CDC;
	MemDc->CreateCompatibleDC(WinDc);

	double widthX = WinxmaxGraphc.Width();
	double heightY = WinxmaxGraphc.Height();
	xpEvolution = (widthX / (xmaxEvolution - xminEvolution));			//Коэффициенты пересчёта координат по Х
	ypEvolution = -(heightY / (ymaxEvolution - yminEvolution));			//Коэффициенты пересчёта координат по У

	bmp.CreateCompatibleBitmap(WinDc, widthX, heightY);
	CBitmap* pBmp = (CBitmap*)MemDc->SelectObject(&bmp);

	// заливка фона графика белым цветом
	MemDc->FillSolidRect(WinxmaxGraphc, RGB(0, 0, 0));

	CPen setka_pen;
	setka_pen.CreatePen(		//для сетки
		PS_DOT,					//пунктирная
		1,						//толщина 1 пиксель
		RGB(71, 74, 81));			//цвет  grey

	CPen osi_pen;
	osi_pen.CreatePen(		//для сетки
		PS_SOLID,				//сплошная линия
		3,						//толщина 3 пикселя
		RGB(255, 255, 255));			//цвет white

	CPen graph_pen;
	graph_pen.CreatePen(
		PS_SOLID,				//сплошная линия
		3,						//толщина 2 пикселя
		RGB(148, 0, 211));			//цвет blue

	CPen graph_pen2;
	graph_pen2.CreatePen(
		PS_SOLID,				//сплошная линия
		2.5,						//толщина 2 пикселя
		RGB(255, 255, 0));			//цвет blue

	MemDc->SelectObject(&setka_pen);
	//отрисовка сетки по y
	for (double x = xminEvolution; x <= xmaxEvolution; x += xmaxEvolution / scale / 8)
	{
		if (x != 0)
		{
			MemDc->MoveTo(DOTSANIMATION(x, ymaxEvolution));
			MemDc->LineTo(DOTSANIMATION(x, yminEvolution));
		}
	}
	//отрисовка сетки по x
	for (double y = yminEvolution; y <= ymaxEvolution; y += ymaxEvolution / scale / 4)
	{
		if (y != 0)
		{
			MemDc->MoveTo(DOTSANIMATION(xminEvolution, y));
			MemDc->LineTo(DOTSANIMATION(xmaxEvolution, y));
		}
	}

	CFont font;
	font.CreateFontW(16, 0, 0, 0, FW_HEAVY, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS || CLIP_LH_ANGLES, DEFAULT_QUALITY, DEFAULT_PITCH, _T("Century Gothic"));

	// установка прозрачного фона текста
	MemDc->SetBkMode(TRANSPARENT);
	MemDc->SetTextColor(RGB(255, 255, 255));

	for (double j = xminEvolution; j <= xmaxEvolution; j++)
	{
		MemDc->SelectObject(&font);
		double o = 0.0 / scale;
		CString str;
		if ((int)j != o)
		{
			str.Format(_T("%.0f"), (int)j / scale - defaultX0 / scale);
			MemDc->TextOutW(DOTSANIMATION((int)j, defaultY0 - ymaxEvolution * 0.02), str);

			MemDc->SelectObject(&osi_pen);
			MemDc->MoveTo(DOTSANIMATION((int)j, ymaxEvolution * 0.03));
			MemDc->LineTo(DOTSANIMATION((int)j, -ymaxEvolution * 0.03));
		}
	}

	MemDc->SelectObject(&osi_pen);
	//создаём Ось Y
	MemDc->MoveTo(DOTSANIMATION(defaultX0, ymaxEvolution));
	MemDc->LineTo(DOTSANIMATION(defaultX0, yminEvolution));
	//создаём Ось Х
	MemDc->MoveTo(DOTSANIMATION(xminEvolution, -defaultY0));
	MemDc->LineTo(DOTSANIMATION(xmaxEvolution, -defaultY0));

	//подпись осей
	MemDc->TextOutW(DOTSANIMATION(xmaxEvolution * 0.03, ymaxEvolution * 0.95), _T("φ"));	//Y
	MemDc->TextOutW(DOTSANIMATION(xmaxEvolution * 0.95, ymaxEvolution * 0.25), _T("x"));		//X

	xxi = -a_GU;
	// отрисовка
	MemDc->MoveTo(DOTSANIMATION(xxi, anim[0]));
	for (int i = 0; i < anim.size(); i++)
	{
		MemDc->SelectObject(&graph_pen);
		MemDc->LineTo(DOTSANIMATION(xxi, anim[i]));

		xxi += x_step;
	}

	// вывод на экран
	WinDc->BitBlt(0, 0, widthX, heightY, MemDc, 0, 0, SRCCOPY);
	delete MemDc;
}

void CModelTask4Dlg::DrawBPF(vector<complex<double>> vec, CDC* WinDc, CRect WinxmaxGraphc)
{
	vector<double> anim;

	for (int i = 0; i < vec.size(); i++)
	{
		anim.push_back(vec[i].real());
	}

	Mashtab(anim, anim.size(), &Min, &Max);

	//ГРАФИК СИГНАЛА
	xminGraph = -t_max * 0.15;
	xmaxGraph = t_max * 1.05;
	yminGraph = (Min - Max) * 0.15;;			//минимальное значение y
	ymaxGraph = (Max - Min) * 1.05;;

	// создание контекста устройства
	CBitmap bmp;
	CDC* MemDc;
	MemDc = new CDC;
	MemDc->CreateCompatibleDC(WinDc);

	double widthX = WinxmaxGraphc.Width();
	double heightY = WinxmaxGraphc.Height();
	xpGraph = (widthX / (xmaxGraph - xminGraph));			//Коэффициенты пересчёта координат по Х
	ypGraph = -(heightY / (ymaxGraph - yminGraph));			//Коэффициенты пересчёта координат по У

	bmp.CreateCompatibleBitmap(WinDc, widthX, heightY);
	CBitmap* pBmp = (CBitmap*)MemDc->SelectObject(&bmp);

	// заливка фона графика белым цветом
	MemDc->FillSolidRect(WinxmaxGraphc, RGB(0, 0, 0));

	CPen setka_pen;
	setka_pen.CreatePen(		//для сетки
		PS_DOT,					//пунктирная
		1,						//толщина 1 пиксель
		RGB(71, 74, 81));			//цвет  grey

	CPen osi_pen;
	osi_pen.CreatePen(		//для сетки
		PS_SOLID,				//сплошная линия
		3,						//толщина 3 пикселя
		RGB(255, 255, 255));			//цвет white

	CPen graph_pen;
	graph_pen.CreatePen(
		PS_SOLID,				//сплошная линия
		3,						//толщина 2 пикселя
		RGB(148, 0, 211));			//цвет blue

	CPen graph_pen2;
	graph_pen2.CreatePen(
		PS_SOLID,				//сплошная линия
		3,						//толщина 2 пикселя
		RGB(255, 255, 0));			//цвет blue

	CPen ellips;
	ellips.CreatePen(
		PS_SOLID,				//сплошная линия
		5,						//толщина 2 пикселя
		RGB(255, 255, 0));			//цвет blue

	MemDc->SelectObject(&setka_pen);
	//отрисовка сетки по y
	for (double x = xminGraph; x <= xmaxGraph; x += xmaxGraph / scale / 8)
	{
		if (x != 0)
		{
			MemDc->MoveTo(DOTSGRAPH(x, ymaxGraph));
			MemDc->LineTo(DOTSGRAPH(x, yminGraph));
		}
	}
	//отрисовка сетки по x
	for (double y = yminGraph; y <= ymaxGraph; y += ymaxGraph / scale / 4)
	{
		if (y != 0)
		{
			MemDc->MoveTo(DOTSGRAPH(xminGraph, y));
			MemDc->LineTo(DOTSGRAPH(xmaxGraph, y));
		}
	}

	// установка прозрачного фона текста
	MemDc->SetBkMode(TRANSPARENT);
	MemDc->SetTextColor(RGB(255, 255, 255));

	// установка шрифта
	CFont font;
	font.CreateFontW(16, 0, 0, 0, FW_HEAVY, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS || CLIP_LH_ANGLES, DEFAULT_QUALITY, DEFAULT_PITCH, _T("Century Gothic"));

	//по X с шагом 0.5
	for (double j = xminGraph; j <= xmaxGraph; j += 16)
	{
		MemDc->SelectObject(&font);
		double o = 0.0 / scale;
		CString str;
		if ((int)j != o)
		{
			str.Format(_T("%.0f"), (int)j / scale - defaultX0 / scale);
			MemDc->TextOutW(DOTSGRAPH((int)j, defaultY0 - ymaxGraph * 0.02), str);
		}
	}

	MemDc->SelectObject(&osi_pen);
	//создаём Ось Y
	MemDc->MoveTo(DOTSGRAPH(defaultX0, ymaxGraph));
	MemDc->LineTo(DOTSGRAPH(defaultX0, yminGraph));
	//создаём Ось Х
	MemDc->MoveTo(DOTSGRAPH(xminGraph, -defaultY0));
	MemDc->LineTo(DOTSGRAPH(xmaxGraph, -defaultY0));

	//подпись осей
	MemDc->TextOutW(DOTSGRAPH(xmaxGraph * 0.03, ymaxGraph * 0.95), _T("φ"));	//Y
	MemDc->TextOutW(DOTSGRAPH(xmaxGraph * 0.95, ymaxGraph * 0.25), _T("x"));		//X

	// отрисовка
	MemDc->MoveTo(DOTSGRAPH(0, anim[0]));
	for (int i = 0; i < anim.size(); i++)
	{
		MemDc->SelectObject(&graph_pen);
		MemDc->LineTo(DOTSGRAPH(i, anim[i]));

		if (ellipsBPF == true)
		{
			int position = koord_peak.GetPos();

			MemDc->SelectObject(&ellips);
			MemDc->Ellipse(DOTSGRAPH(position - xmaxGraph * 0.005, anim[position] - ymaxGraph * 0.01),
				DOTSGRAPH(position + xmaxGraph * 0.005, anim[position] + ymaxGraph * 0.01));
		}
	}

	// вывод на экран
	WinDc->BitBlt(0, 0, widthX, heightY, MemDc, 0, 0, SRCCOPY);
	delete MemDc;
}

double CModelTask4Dlg::GaussKupol(double t)			//функция для рассчета сигнала
{
	double s = 0;
	double help = -((t - t0_param) * (t - t0_param));

	s = exp(help / (dispersia * dispersia));

	return s;
}

double CModelTask4Dlg::PotenсFunc(double x)
{
	if (x > -a_GU / 2 && x < a_GU / 2)
	{
		return -amplituda;
	}
	else
	{
		return 0;
	}
}

complex<double> CModelTask4Dlg::funcAkBk()
{
	complex<double> help1 = -i_mnim * t_step;
	complex<double> help2 = 2. * x_step * x_step;

	complex<double> AkorBk = help1 / help2;
	return AkorBk;
}

complex<double> CModelTask4Dlg::funcCk(double xk)
{
	complex<double> help1 = (i_mnim * t_step * PotenсFunc(xk)) / 2.;
	complex<double> help2 = (i_mnim * t_step) / (2. * x_step);
	complex<double> help3 = 1. / x_step;

	complex<double> Ck = 1. + help1 + help2 * (help3 + help3);
	return Ck;
}

complex<double> CModelTask4Dlg::funcDk(vector<complex<double>> psi, double xk, int k)
{
	complex<double> help1 = 1. / x_step;
	complex<double> help2;
	if (k == 0)
	{
		help2 = (psi[k + 1] - psi[k] - psi[k]);
	}
	else if (k == psi.size() - 1)
	{
		help2 = (psi[k] - psi[k] + psi[k - 1]);
	}
	else
	{
		help2 = (psi[k + 1] - psi[k] - psi[k] + psi[k - 1]);
	}

	complex<double> determ = help1 * (help2 / x_step);

	complex<double> help4 = (i_mnim * t_step) / 2.;

	complex<double> Dk = psi[k] + help4 * (determ - PotenсFunc(xk) * psi[k]);

	return Dk;
}

vector<complex<double>> CModelTask4Dlg::alpha(int size)
{
	vector <complex<double>> ak;

	double xk = -a_GU + x_step;

	ak.push_back(0.0);
	for (int k = 1; k < size; k++)
	{
		if (xk <= a_GU)
		{
			complex<double> help1 = -1.0 * funcAkBk();
			complex<double> help2 = funcAkBk() * ak[k - 1];

			ak.push_back(help1 / (funcCk(xk) + help2));
			xk += x_step;
		}
	}

	return ak;
}

vector<complex<double>> CModelTask4Dlg::beta(vector<complex<double>> psi, vector<complex<double>> alpha)
{
	int size = alpha.size();

	vector <complex<double>> bk;

	bk.push_back(0.0);
	int k_iter = 1;
	for (double xk = -a_GU + x_step; xk <= a_GU; xk += x_step)
	{
		if (k_iter < size)
		{
			complex<double> help1 = funcDk(psi, xk, k_iter - 1) - funcAkBk() * bk[k_iter - 1];
			complex<double> help2 = funcCk(xk) + funcAkBk() * alpha[k_iter - 1];

			bk.push_back(help1 / help2);
			k_iter++;
		}
	}
	//bk.push_back(0.0);

	return bk;
}

vector<complex<double>> CModelTask4Dlg::Psi(vector<complex<double>> alpha, vector<complex<double>> beta)
{
	vector <complex<double>> psi;

	int size = alpha.size();

	psi.push_back(0.0);
	for (int k = size - 2; k > 0; k--)
	{
		complex<double> help = alpha[k] * psi.front() + beta[k];
		psi.insert(psi.begin(), help);
	}
	psi.insert(psi.begin(), 0);

	return psi;
}

void CModelTask4Dlg::AnimationInformation()
{
	vector<complex<double>> CurrStep;

	for (int i = 0; i < ish_sign.size(); i++)
	{
		CurrStep.push_back(ish_sign[i]);
	}

	int size = CurrStep.size();
	int iter = 0;

	while (1)
	{
		vector<complex<double>> PrevStep = CurrStep;
		CurrStep.clear();
		vector<complex<double>> Alfa = alpha(size);
		vector<complex<double>> Beta = beta(PrevStep, Alfa);
		CurrStep = Psi(Alfa, Beta);

		DrawEvolution(CurrStep, PicDcEvolution, PicEvolution);

		if (recording.GetCheck() == BST_CHECKED && iter < t_max)
		{
			recording.SetWindowTextW(L"Идет запись");
			evolution.push_back(CurrStep);
			iter++;
		}
		else if (iter == t_max)
		{
			MessageBox(L"Запись успешно завершена!", L"Information", MB_OK || MB_ICONERROR);
			recording.SetWindowTextW(L"Записать данные");
			recording.SetCheck(false);
			iter = 0;
		}
	}
}

void CModelTask4Dlg::fourea(vector<complex<double>> data, int n, int is)
{
	int i, j, istep;
	int m, mmax;
	complex<double> temp;
	double r, r1, theta, w_r, w_i;
	double pi = 3.1415926f;

	r = pi * is;
	j = 0;
	for (i = 0; i < n; i++)
	{
		if (i < j)
		{
			temp = complex<double>(data[j].real(), data[j].imag());
			data[j] = data[i].real();
			data[i] = temp;
		}
		m = n >> 1;
		while (j >= m) { j -= m; m = (m + 1) / 2; }
		j += m;
	}

	mmax = 1;

	while (mmax < n)
	{
		istep = mmax << 1;
		r1 = r / (double)mmax;
		for (m = 0; m < mmax; m++)
		{
			theta = r1 * m;
			w_r = (double)cos((double)theta);
			w_i = (double)sin((double)theta);
			for (i = m; i < n; i += istep)
			{
				j = i + mmax;
				temp = complex<double>(w_r * data[j].real() - w_i * data[j].imag(), w_r * data[j].imag() + w_i * data[j].real());
				data[j] = data[i] - temp;
				data[i] += temp;
			}
		}
		mmax = istep;
	}

	if (is > 0)
		for (i = 0; i < n; i++)
		{
		data[i] /= (double)n;
		}
}

DWORD WINAPI MyProc(PVOID pv)
{
	CModelTask4Dlg* p = (CModelTask4Dlg*)pv;
	p->AnimationInformation();
	return 0;
}

void CModelTask4Dlg::OnBnClickedСalculate()
{
	UpdateData(TRUE);

	//ofstream out("kupol.txt");
	ish_sign.clear();
	potenc_func.clear();
	for (double i = -a_GU; i <= a_GU; i += x_step)
	{
		ish_sign.push_back(GaussKupol(i));	//СЧИТАЕМ ИСХОДНЫЙ СИГНАЛ
		potenc_func.push_back(PotenсFunc(i));
		//out << ish_sign.back() << endl;
	}

	MessageBox(L"Данные рассчитаны", L"Information", MB_OK | MB_ICONINFORMATION);
	//out.close();

	UpdateData(FALSE);
}

void CModelTask4Dlg::OnBnClickedDrawYama()
{
	DrawGraph(potenc_func, PicDcGraph, PicGraph);
}

void CModelTask4Dlg::OnBnClickedDrawKupol()
{
	DrawGraph(ish_sign, PicDcGraph, PicGraph);
}

void CModelTask4Dlg::OnBnClickedStartEvolution()
{
	UpdateData(TRUE);

	if (real.GetCheck() != BST_CHECKED && image.GetCheck() != BST_CHECKED && modul.GetCheck() != BST_CHECKED)
	{
		MessageBox(L"Вот балбес, выбери, что рисовать!", L"Information", MB_OK | MB_ICONERROR);
	}
	else if (!bRunTh)
	{
		StartStop.SetWindowTextW(stop);
		if (hThread == NULL)
		{
			evolution.clear();
			hThread = CreateThread(NULL, 0, MyProc, this, 0, &dwThread);
		}
		else
		{
			ResumeThread(hThread);
		}
		bRunTh = true;
	}
	else
	{
		StartStop.SetWindowTextW(start);
		bRunTh = false;

		SuspendThread(hThread);
	}
}

void fft(vector<complex<double>>& a, bool invert)
{
	double pi = 3.14159265;
	int n = a.size();
	if (n == 1)
		return;

	vector<complex<double>> a0(n / 2), a1(n / 2);
	for (int i = 0; 2 * i < n; i++)
	{
		a0[i] = a[2 * i];
		a1[i] = a[2 * i + 1];
	}
	fft(a0, invert);
	fft(a1, invert);

	double ang = 2 * pi / n * (invert ? -1 : 1);
	complex<double> w(1), wn(cos(ang), sin(ang));
	for (int i = 0; 2 * i < n; i++)
	{
		a[i] = a0[i] + w * a1[i];
		a[i + n / 2] = a0[i] - w * a1[i];
		if (invert)
		{
			a[i] /= 2;
			a[i + n / 2] /= 2;
		}
		w *= wn;
	}
}

void CModelTask4Dlg::OnBnClickedBPF()
{
	int sizeX = evolution[0].size();
	int sizeT = evolution.size();
	int i = 0;

	evolutionBPF.clear();
	evolutionBPF.resize(sizeX);
	for (i = 0; i < sizeX; i++)
	{
		vector<complex<double>> spec;
		for (int j = 0; j < sizeT; j++)
		{
			spec.push_back(evolution[j][i]);
		}

		//fourea(spec, sizeT, -1);
		fft(spec, false);

		for (int j = 0; j < sizeT; j++)
		{
			evolutionBPF[i].push_back(sqrt(spec[j].real() * spec[j].real() + spec[j].imag() * spec[j].imag()));
		}

		if (BPFchek.GetCheck() == BST_CHECKED && i < sizeX)
		{
			BPFchek.SetWindowTextW(L"БПФ в процессе");
		}
	}

	sliderBPF.SetRange(1, sizeX - 2);
	koord_peak.SetRange(0, sizeT - 1);

	if (i == sizeX)
	{
		MessageBox(L"Преобразование Фурье успешно выполнено!", L"Information", MB_OK | MB_ICONINFORMATION);
		BPFchek.SetWindowTextW(L"Выполнить БПФ");
		BPFchek.SetCheck(false);
	}
}

void CModelTask4Dlg::OnNMCustomdrawSlider1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

	if (!evolutionBPF.empty())
	{
		ellipsBPF = false;

		DrawBPF(evolutionBPF[sliderBPF.GetPos()], PicDcEvolution, PicEvolution);
	}

	*pResult = 0;
}

void CModelTask4Dlg::OnNMCustomdrawSlider2(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

	if (!evolutionBPF.empty())
	{
		ellipsBPF = true;
		DrawBPF(evolutionBPF[sliderBPF.GetPos()], PicDcEvolution, PicEvolution);
		VosstPsi.SetCheck(false);
	}

	*pResult = 0;
}

void CModelTask4Dlg::OnBnClickedVosstPsi()
{
	int index = koord_peak.GetPos();

	vector<complex<double>> res;
	for (int j = 0; j < evolutionBPF.size(); j++)
	{
		res.push_back(evolutionBPF[j][koord_peak.GetPos()]);
	}

	DrawEvolution(res, PicDcEvolution, PicEvolution);
}


void CModelTask4Dlg::OnBnClickedSbros()
{
	TerminateThread(hThread, 0);		//убиваем поток
	CloseHandle(hThread);
	hThread = NULL;

	StartStop.SetWindowTextW(L"Старт");
	recording.SetWindowTextW(L"Записать данные");

	U_x.SetCheck(false);
	Psi0_x.SetCheck(false);
	real.SetCheck(false);
	image.SetCheck(false);
	modul.SetCheck(false);
	recording.SetCheck(false);
	BPFchek.SetCheck(false);
	VosstPsi.SetCheck(false);
	raschet.SetCheck(false);

	bRunTh = false;
	DrawDC(PicDcGraph, PicGraph, 50);
	DrawDC(PicDcEvolution, PicEvolution, 50);
}
