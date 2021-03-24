
// ModelTask4Dlg.h: файл заголовка
//

#pragma once

#include <vector>
#include <complex>

using namespace std;

// Диалоговое окно CModelTask4Dlg
class CModelTask4Dlg : public CDialogEx
{
// Создание
public:
	CModelTask4Dlg(CWnd* pParent = nullptr);	// стандартный конструктор

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MODELTASK4_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


// Реализация
protected:
	HICON m_hIcon;

	DWORD dwThread;
	HANDLE hThread;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	//области рисования
	CWnd* PicWndGraph;
	CDC* PicDcGraph;
	CRect PicGraph;

	//области рисования
	CWnd* PicWndEvolution;
	CDC* PicDcEvolution;
	CRect PicEvolution;

	double Min = -10, Max = 10, Min1 = -10, Max1 = 10, Min2 = -10, Max2 = 10;
	double xx0 = -10, xxmax = 10, yy0 = 0, yymax = 0, xxi = 0, yyi = 0;

	double xpGraph = 0, ypGraph = 0,			//коэфициенты пересчета
		xminGraph = -1, xmaxGraph = 1,			//максисимальное и минимальное значение х 
		yminGraph = -0.5, ymaxGraph = 5;			//максисимальное и минимальное значение y
	double mnGraph = -0.1, mxGraph = 1;					//коэффициенты масштабирования

	double xpEvolution = 0, ypEvolution = 0,			//коэфициенты пересчета
		xminEvolution = -1, xmaxEvolution = 1,			//максисимальное и минимальное значение х 
		yminEvolution = -0.5, ymaxEvolution = 5;			//максисимальное и минимальное значение y
	double mnEvolution = -0.1, mxEvolution = 1;					//коэффициенты масштабирования

public:
	afx_msg void OnBnClickedСalculate();
	afx_msg void OnBnClickedDrawYama();
	afx_msg void OnBnClickedDrawKupol();
	afx_msg void OnBnClickedStartEvolution();
	afx_msg void OnBnClickedBPF();
	afx_msg void OnNMCustomdrawSlider1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMCustomdrawSlider2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedVosstPsi();
	afx_msg void OnBnClickedSbros();

	double amplituda;
	double t0_param;
	double dispersia;
	double a_GU;
	double x_step;
	int t_max;
	double t_step;
	CButton U_x;
	CButton Psi0_x;
	CButton real;
	CButton image;
	CButton modul;
	CButton StartStop;
	CButton recording;
	CButton BPFchek;
	CButton VosstPsi;
	CButton raschet;
	CSliderCtrl sliderBPF;
	CSliderCtrl koord_peak;
	
	int scale = 1;
	double defaultX0 = 0.0;
	double defaultY0 = 0.0;
	double PI = 3.1415926;
	bool ellipsBPF = false;
	char znach[1000];

	BOOL bRunTh = false;		// переменная, показывающая, запущен ли процесс
	CString start = L"Продолжить";		// строки с именем кнопки для каждого случая
	CString stop = L"Пауза";

	vector<double> ish_sign;
	vector<double> potenc_func;
	vector<vector<complex<double>>> evolution;
	vector<vector<complex<double>>> evolutionBPF;

	double GaussKupol(double t);
	void DrawDC(CDC* WinDc, CRect WinPic, double AbsMax);
	void DrawGraph(vector<double>& x, CDC* WinDc, CRect WinxmaxGraphc);
	void DrawEvolution(vector<complex<double>> vec, CDC* WinDc, CRect WinxmaxGraphc);
	void DrawBPF(vector<complex<double>> vec, CDC* WinDc, CRect WinxmaxGraphc);
	void Mashtab(vector<double>& solve_buff, int dim, double* mmin, double* mmax);
	double PotenсFunc(double x);
	complex<double> funcAkBk();
	complex<double> funcCk(double xk);
	complex<double> funcDk(vector<complex<double>> psi, double xk, int k);
	vector<complex<double>> alpha(int size);
	vector<complex<double>> beta(vector<complex<double>> psi, vector<complex<double>> alpha);
	vector<complex<double>> Psi(vector<complex<double>> alpha, vector<complex<double>> beta);
	void AnimationInformation();
	void fourea(vector<complex<double>> data, int n, int is);
};


