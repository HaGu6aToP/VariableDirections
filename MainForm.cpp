#include "MainForm.h"


#define ConstantsClicked 1
#define ConstantsEnter 2
#define ConstantsLoad 3
#define ConstantOnButtonClick 4
#define AlgoritmStart 5

#define TextBufferSize 256

CRITICAL_SECTION cs;
#define WM_CALC_FINISHED()

using MyClasses::Methods::VariableDirections;

char buffer[TextBufferSize];

OPENFILENAMEA ofn;
char filename[256];

static HGLRC hRC; // Постоянный контекст рендеринга
static HDC hDC; // Приватный контекст устройства GDI
BOOL keys[256], working = FALSE;
HWND hChildWnd1;

HWND aEdit;
HWND bEdit;
HWND tEdit;
HWND c1Edit;
HWND c2Edit;
HWND h1Edit;
HWND h2Edit;
HWND tauEdit;

MyClasses::Graphics::Tape t{0, nullptr, 0, 0};



double C1 = 1, C2 = 1, A = 1, B = 1, T = 0.4, h1 = 0.25, h2 = 0.25, tau = 0.0625;
MyClasses::Array<MyClasses::Array<double>> u{};

double f(double x, double y, double t) { return C1 * t * pow(M_E, -((x - 7 * A / 15) * (x - 7 * A / 15) + (y - 8 * B / 15) * (y - 8 * B / 15))); }
double u0(double x, double y) { return 0; }
double g1(double x, double t) { return 0; }
double g2(double y, double t) { return C2 * t * sin(M_PI * y / B); }
double g3(double x, double t) { return 0; }
double g4(double y, double t) { return C2 * t * sin(M_PI * y / B); }

void delarr(int len, double** triangularTape) {

	for (int i = 0; i < len; i++)
		delete triangularTape[i];
	delete triangularTape;
}

void thread(void* pParams) {
	MyClasses::Array<MyClasses::Array<double>> *u = (MyClasses::Array<MyClasses::Array<double>>*)pParams;


	for (int k = 1; k < VariableDirections::NT_2; k += 2) {
		EnterCriticalSection(&cs);
		(*u) = VariableDirections::firstStep(k, (*u));
		(*u) = VariableDirections::secondStep(k + 1, (*u));
		std::cout << (*u);
		LeaveCriticalSection(&cs);
		Sleep(1000);
	}


	t = MyClasses::Graphics::triangularTape((*u), VariableDirections::X, VariableDirections::Y);
	working = FALSE;
	MessageBox(NULL, L"Success", L"Result", MB_OK);
	_endthread();
}

void setOpenFileParams(HWND hWnd) {
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFile = filename;
	ofn.nMaxFile = sizeof(filename);
	ofn.lpstrFilter = "*.txt";
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

}

void loadData(LPCSTR path) {
	HANDLE fileToLoad = CreateFileA(
		path,
		GENERIC_READ,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);
	DWORD bytesIterated;
	ReadFile(fileToLoad, buffer, 256, &bytesIterated, NULL);

	char buffer2[256];
	int res[5];
	int i = 0, j = 0, k = 0;
	for (i = 0; i < 256; i++) {
		if (buffer[i] == '\0' || buffer[i] == '\n') {
			buffer2[j] = '\0';
			res[k] = std::atoi(buffer2);
			break;
		}
		if (buffer[i] != ' ') {
			buffer2[j] = buffer[i];
			j++;
		}
		else {
			buffer2[j] = '\0';
			res[k] = std::atoi(buffer2);
			k++;
			j = 0;
		}
	}

	A = res[0];
	B = res[1];
	T = res[2];
	C1 = res[3];
	C2 = res[4];

	
}

void myMenu(HWND hWnd) {
	HMENU hMenu = CreateMenu();
	HMENU hSubMenu1 = CreateMenu();
	HMENU hSubMenu2 = CreateMenu();
	
	AppendMenu(hSubMenu1, MF_STRING, ConstantsEnter, L"Ввести");
	AppendMenu(hSubMenu1, MF_STRING, ConstantsLoad, L"Загрузить");
	AppendMenu(hSubMenu2, MF_STRING, AlgoritmStart, L"Запустить");

	AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hSubMenu1, L"Константы");
	AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hSubMenu2, L"Алгоритм");

	SetMenu(hWnd, hMenu);
}


void ConstantWndAddWidgets(HWND hWnd) {
	CreateWindow(L"static", L"a:", WS_VISIBLE | WS_CHILD, 5, 25, 300, 20, hWnd, NULL, NULL, NULL);
	aEdit = CreateWindow(L"edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 30, 25, 100, 20, hWnd, NULL, NULL, NULL);

	CreateWindow(L"static", L"b:", WS_VISIBLE | WS_CHILD, 5, 45, 300, 20, hWnd, NULL, NULL, NULL);
	bEdit = CreateWindow(L"edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 30, 45, 100, 20, hWnd, NULL, NULL, NULL);

	CreateWindow(L"static", L"T:", WS_VISIBLE | WS_CHILD, 5, 65, 300, 20, hWnd, NULL, NULL, NULL);
	tEdit = CreateWindow(L"edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 30, 65, 100, 20, hWnd, NULL, NULL, NULL);

	CreateWindow(L"static", L"C1:", WS_VISIBLE | WS_CHILD, 5, 85, 300, 20, hWnd, NULL, NULL, NULL);
	c1Edit = CreateWindow(L"edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 30, 85, 100, 20, hWnd, NULL, NULL, NULL);

	CreateWindow(L"static", L"C2:", WS_VISIBLE | WS_CHILD, 5, 105, 300, 20, hWnd, NULL, NULL, NULL);
	c2Edit = CreateWindow(L"edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 30, 105, 100, 20, hWnd, NULL, NULL, NULL);


	CreateWindow(L"static", L"h1:", WS_VISIBLE | WS_CHILD, 5, 125, 300, 20, hWnd, NULL, NULL, NULL);
	h1Edit = CreateWindow(L"edit", L"0.0625", WS_VISIBLE | WS_CHILD | WS_BORDER, 30, 125, 100, 20, hWnd, NULL, NULL, NULL);

	CreateWindow(L"static", L"h2:", WS_VISIBLE | WS_CHILD, 5, 145, 300, 20, hWnd, NULL, NULL, NULL);
	h2Edit = CreateWindow(L"edit", L"0.0625", WS_VISIBLE | WS_CHILD | WS_BORDER, 30, 145, 100, 20, hWnd, NULL, NULL, NULL);

	CreateWindow(L"static", L"tau:", WS_VISIBLE | WS_CHILD, 5, 165, 300, 20, hWnd, NULL, NULL, NULL);
	tauEdit = CreateWindow(L"edit", L"0.0625", WS_VISIBLE | WS_CHILD | WS_BORDER, 30, 165, 100, 20, hWnd, NULL, NULL, NULL);


	CreateWindow(L"button", L"Ввести", WS_VISIBLE | WS_CHILD | ES_CENTER, 0, 185, 300, 20, hWnd, (HMENU)ConstantOnButtonClick, NULL, NULL);
}

static std::pair<bool, HWND> addWindow(const std::wstring&& winClass, const std::wstring&& title, HWND hParrentHwnd, const WNDPROC callback) {
	UnregisterClass(winClass.c_str(), GetModuleHandle(nullptr));
	WNDCLASSEX wc{ sizeof(WNDCLASSEX) };
	HWND hWindow{};
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.lpfnWndProc = callback;
	wc.lpszClassName = winClass.c_str();
	wc.style = CS_VREDRAW | CS_HREDRAW;

	const auto create_window = [&hWindow, &winClass, &title, &hParrentHwnd]() ->std::pair<bool, HWND> {
		hWindow = CreateWindow(winClass.c_str(), title.c_str(), WS_OVERLAPPEDWINDOW, 0, 0, 300, 240, hParrentHwnd, nullptr, nullptr, nullptr);
		if (!hWindow)
			return { true, nullptr };

		ShowWindow(hWindow, SW_SHOWDEFAULT);
		UpdateWindow(hWindow);
		return { true, hWindow };
	};

	if (!RegisterClassEx(&wc))
		return create_window();

	return create_window();
}

float triangularTape[][3] = { {0.0, 0.0, 0.25}, {0.0, 0.25, 0.32100635417193535}, {0.25, 0.0, 0.32100635417193535}, {0.25, 0.25, 0.4047071351143498}, {0.5, 0.0, 0.41218031767503205}, {0.5, 0.25, 0.5177415429526928}, {0.75, 0.0, 0.5292500041531687}, {0.75, 0.25, 0.671610114862716}, {1.0, 0.0, 0.6795704571147613}, {1.0, 0.25, 0.8725857393654602}, {1.0, 0.25, 0.8725857393654602}, {1.0, 0.5, 1.1204222675845161}, {1.0, 0.5, 1.1204222675845161}, {1.0, 0.25, 0.8725857393654602}, {0.75, 0.5, 0.8679790096882578}, {0.75, 0.25, 0.671610114862716}, {0.5, 0.5, 0.6682904281454302}, {0.5, 0.25, 0.5177415429526928}, {0.25, 0.5, 0.522623464080277}, {0.25, 0.25, 0.4047071351143498}, {0.0, 0.5, 0.41218031767503205}, {0.0, 0.25, 0.32100635417193535}, {0.0, 0.5, 0.41218031767503205}, {0.0, 0.5, 0.41218031767503205}, {0.0, 0.5, 0.41218031767503205}, {0.0, 0.75, 0.5292500041531687}, {0.25, 0.5, 0.522623464080277}, {0.25, 0.75, 0.6859580344935808}, {0.5, 0.5, 0.6682904281454302}, {0.5, 0.75, 0.8783531759659525}, {0.75, 0.5, 0.8679790096882578}, {0.75, 0.75, 1.136503832093116}, {1.0, 0.5, 1.1204222675845161}, {1.0, 0.75, 1.4386506690014325}, {1.0, 0.75, 1.4386506690014325}, {1.0, 1.0, 1.8472640247326624}, {1.0, 1.0, 1.8472640247326624}, {1.0, 0.75, 1.4386506690014325}, {0.75, 1.0, 1.4386506690014325}, {0.75, 0.75, 1.136503832093116}, {0.5, 1.0, 1.1204222675845161}, {0.5, 0.75, 0.8783531759659525}, {0.25, 1.0, 0.8725857393654602}, {0.25, 0.75, 0.6859580344935808}, {0.0, 1.0, 0.6795704571147613}, {0.0, 0.75, 0.5292500041531687}, {0.0, 1.0, 0.6795704571147613}, {0.0, 1.0, 0.6795704571147613} };
void show() {
	glLineWidth(2);
	//glBegin(GL_TRIANGLES);
	//for (int i = 2; i < 48; i += 1) {
	//	for (int j = 0; j < 3; j++) {
	//		if (std::abs(triangularTape[i - j][2] - 0.25) < 0.25 * 2)
	//			glColor3f(0, 0, 1);
	//		else if (std::abs(triangularTape[i - j][2] - 1.8472640247326624) < 1.8472640247326624 / 2)
	//			glColor3f(1, 0, 0);
	//		else
	//			glColor3f(1, 1, 1);

	//		glVertex3f(triangularTape[i - j][0], triangularTape[i - j][1], triangularTape[i - j][2]);
	//	}
	//}
	//glEnd();

	glBegin(GL_LINES);
	glColor3f(1, 0, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(10, 0, 0);

	glColor3f(0, 1, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 10, 0);

	glColor3f(0, 0, 1);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 10);
	glEnd();
}

// Вызвать после создания окна GL
GLvoid InitGL(GLsizei Width, GLsizei Height) {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // Очистка экрана в черный цвет
	glClearDepth(1.0); // Разрешить очистку буфера глубины
	glDepthFunc(GL_LESS); // Тип теста глубины
	glEnable(GL_DEPTH_TEST); // Разрешить тест глубины

	glShadeModel(GL_SMOOTH); // Разрешить плавное цветовое сглаживание
	glMatrixMode(GL_PROJECTION); // Выбор матрицы проекции
	glLoadIdentity(); // Сброс матрицы проекции
	gluPerspective(45.0f, (GLfloat)Width / (GLfloat)Height, 0.1f, 100.0f); // Вычислить соотношение геометричиских размеров для окна
	glMatrixMode(GL_MODELVIEW); // Выбор матрицы просмотра модели

	MyClasses::Graphics::World::addFunction(show);
}

// Функция масштабирования сцены при изменении размеров окна
GLvoid ReSizeGLScene(GLsizei Width, GLsizei Height) {
	if (Height == 0)
		Height = 1; // Предотвращение деления на 0

	glViewport(0, 0, Width, Height); // Сброс текущей области вывода и перспективных преобразований
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity(); // Сброс матрицы проекции
	gluPerspective(45.0f, (GLfloat)Width / (GLfloat)Height, 0.1f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
}

// Рисование
GLvoid DrawGLScene(GLvoid) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Очистка экрана и буфера глубины
	glLoadIdentity(); //Сброс просмотра

	//glOrtho(-10, 10, -10, 10, -10, 10);
	glTranslatef(0.0f, 0.0f, -7.0f); // Сдвинемся влево на 1.5 единицы и // в экран на 6.0

	glPushMatrix();

	MyClasses::Graphics::Camera::MoveCamera();
	MyClasses::Graphics::World::ShowWorld();

	
	if (working && (u.len() > 0)) {
		EnterCriticalSection(&cs);
		MyClasses::Graphics::Tape t = MyClasses::Graphics::triangularTape(u, VariableDirections::X, VariableDirections::Y);
		MyClasses::Graphics::drowTriangularTape(t);
		LeaveCriticalSection(&cs);
		delarr(t.len, t.triangularTape);
		t.triangularTape = nullptr;
		Sleep(500);
	}
	else if (!working && t.triangularTape != nullptr)
		MyClasses::Graphics::drowTriangularTape(t);

	glPopMatrix(); 
}

//Установка формата пикселей
int SetWindowPixelFormat()
{
	int m_GLPixelIndex;
	PIXELFORMATDESCRIPTOR pfd;


	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;

	pfd.dwFlags = PFD_DRAW_TO_WINDOW |
		PFD_SUPPORT_OPENGL |
		PFD_DOUBLEBUFFER;

	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cRedBits = 8;
	pfd.cRedShift = 16;
	pfd.cGreenBits = 8;
	pfd.cGreenShift = 8;
	pfd.cBlueBits = 8;
	pfd.cBlueShift = 0;
	pfd.cAlphaBits = 0;
	pfd.cAlphaShift = 0;
	pfd.cAccumBits = 64;
	pfd.cAccumRedBits = 16;
	pfd.cAccumGreenBits = 16;
	pfd.cAccumBlueBits = 16;
	pfd.cAccumAlphaBits = 0;
	pfd.cDepthBits = 32;
	pfd.cStencilBits = 8;
	pfd.cAuxBuffers = 0;
	pfd.iLayerType = PFD_MAIN_PLANE;
	pfd.bReserved = 0;
	pfd.dwLayerMask = 0;
	pfd.dwVisibleMask = 0;
	pfd.dwDamageMask = 0;



	m_GLPixelIndex = ChoosePixelFormat(hDC, &pfd);
	if (m_GLPixelIndex == 0) // Let's choose a default index.
	{
		m_GLPixelIndex = 1;
		if (DescribePixelFormat(hDC, m_GLPixelIndex, sizeof(PIXELFORMATDESCRIPTOR), &pfd) == 0)
			return 0;
	}


	if (SetPixelFormat(hDC, m_GLPixelIndex, &pfd) == FALSE)
		return 0;


	return 1;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	RECT Screen; // Размеры окна
	//GLuint PixelFormat;
	//static PIXELFORMATDESCRIPTOR pfd = {
	//	sizeof(PIXELFORMATDESCRIPTOR), // Размер структуры
	//	1, // ?
	//	PFD_DRAW_TO_WINDOW | // Формат для окна
	//	PFD_SUPPORT_OPENGL | // Формат для OpenGl
	//	PFD_DOUBLEBUFFER, // Формат для двойного буфера
	//	32, // Выбор 16 бит глубины цвета
	//	0, 0, 0, 0, 0, 0, // ?
	//	0, // Нет буфера прозрачности
	//	0, // ?
	//	0, // Нет буфера аккумуляции
	//	0, 0, 0, 0, // ?
	//	32, // 16 битный Z-буффер
	//	0, // Нет буфера траффарета
	//	0, // ?
	//	PFD_MAIN_PLANE, // Главный слой рисования
	//	0, // ?
	//	0, 0, 0 // ?
	//};

	// Обработка сообщений
	switch (message) {
	case WM_CREATE:
	{
		hDC = GetDC(hWnd);
		SetWindowPixelFormat();
		hRC = wglCreateContext(hDC); 

		if (!hRC) {
			MessageBox(0, L"Can't create a GL rendering", L"Error", MB_OK | MB_ICONERROR);
			PostQuitMessage(0);
			break;
		}

		if (!wglMakeCurrent(hDC, hRC)) {
			MessageBox(0, L"Can't activate GLRC", L"Error", MB_OK | MB_ICONERROR);
			PostQuitMessage(0);
			break;
		}

		GetClientRect(hWnd, &Screen);
		InitGL(Screen.right, Screen.bottom);

		myMenu(hWnd);
		setOpenFileParams(hWnd);

		break;
	}

	case WM_KEYDOWN:
		keys[wParam] = TRUE;

		break;
	case WM_KEYUP:
		keys[wParam] = FALSE;
		break;

	case WM_DESTROY:
	case WM_CLOSE:
		ChangeDisplaySettings(NULL, 0);

		wglMakeCurrent(hDC, NULL);
		wglDeleteContext(hRC);
		ReleaseDC(hWnd, hDC);

		PostQuitMessage(0);

		DeleteCriticalSection(&cs);
		
		break;

	case WM_SIZE:
		ReSizeGLScene(LOWORD(lParam), HIWORD(lParam));

		break;

	case WM_COMMAND: 
	{
		BOOL flag;
		int wmId = LOWORD(wParam);
		switch (wmId) {
		case ConstantsLoad: {
			if (GetOpenFileNameA(&ofn)) { loadData(filename); }
			break;
		}

		case AlgoritmStart:{

			MyClasses::Methods::VariableDirectionsParams params = {
				A,
				B,
				T,
				h1,
				h2,
				tau,
				f,
				u0,
				g1,
				g2,
				g3,
				g4
			};

			VariableDirections::init(params);


			u.setLength(VariableDirections::NY);
			for (int j = 0; j < VariableDirections::NY; j++)
				u[j].setLength(VariableDirections::NX);

			for (int j = 0; j < VariableDirections::NY; j++)
				for (int i = 0; i < VariableDirections::NX; i++)
					u[j][i] = VariableDirections::params.u0(VariableDirections::X[i], VariableDirections::Y[j]);

			working = TRUE;

			std::cout << u;
			InitializeCriticalSection(&cs);

			//MyClasses::Graphics::Tape t = MyClasses::Graphics::triangularTape(u, VariableDirections::X, VariableDirections::Y);
			//MyClasses::Graphics::drowTriangularTape(t);


			_beginthread(thread, 4096, &u);
			//while (working) {
			//	EnterCriticalSection(&cs);

			//	MyClasses::Graphics::Tape t = MyClasses::Graphics::triangularTape(u, VariableDirections::X, VariableDirections::Y);
			//	MyClasses::Graphics::drowTriangularTape(t);

			//	LeaveCriticalSection(&cs);
			// 	Sleep(1500);
			//}


			break;
		}
		case ConstantsEnter:
			const auto res = addWindow(L"ChildWindow1", L"Ввод", hWnd, [](HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)->LRESULT {
				switch (message) {
				case WM_CREATE:
					ConstantWndAddWidgets(hWnd);
					break;

				case WM_COMMAND: {
					int wmId = LOWORD(wParam);
					switch (wmId) {
					case ConstantOnButtonClick:
						GetWindowTextA(aEdit, buffer, TextBufferSize);
						A = std::atoi(buffer);
						if (strchr(buffer, '.'))
							A += std::atof(strchr(buffer, '.'));
						

						GetWindowTextA(bEdit, buffer, TextBufferSize);
						B = std::atoi(buffer);
						if (strchr(buffer, '.'))
							B += std::atof(strchr(buffer, '.'));

						GetWindowTextA(tEdit, buffer, TextBufferSize);
						T = std::atoi(buffer);
						if (strchr(buffer, '.'))
							T += std::atof(strchr(buffer, '.'));

						GetWindowTextA(c1Edit, buffer, TextBufferSize);
						C1 = std::atoi(buffer);
						if (strchr(buffer, '.'))
							C1 += std::atof(strchr(buffer, '.'));

						GetWindowTextA(c2Edit, buffer, TextBufferSize);
						C2 = std::atoi(buffer);
						if (strchr(buffer, '.'))
							C2 += std::atof(strchr(buffer, '.'));

						GetWindowTextA(h1Edit, buffer, TextBufferSize);
						h1 = std::atoi(buffer);
						if (strchr(buffer, '.'))
							h1 += std::atof(strchr(buffer, '.'));

						GetWindowTextA(h2Edit, buffer, TextBufferSize);
						h2 = std::atoi(buffer);
						if (strchr(buffer, '.'))
							h2 += std::atof(strchr(buffer, '.'));

						GetWindowTextA(tauEdit, buffer, TextBufferSize);
						tau = std::atoi(buffer);
						if (strchr(buffer, '.'))
							tau += std::atof(strchr(buffer, '.'));

						SetWindowTextA(aEdit, "");
						SetWindowTextA(bEdit, "");
						SetWindowTextA(tEdit, "");
						SetWindowTextA(c1Edit, "");
						SetWindowTextA(c2Edit, "");
						SetWindowTextA(h1Edit, "");
						SetWindowTextA(h2Edit, "");
						SetWindowTextA(tauEdit, "");
					}
				}
				}
				return DefWindowProc(hWnd, message, wParam, lParam);
				});
			hChildWnd1 = res.second;
		}
	}

	default:
		return (DefWindowProc(hWnd, message, wParam, lParam));
	}
	return 0;
}

// Создание окна

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd) {
	MSG msg; // Структура сообщения Windows
	//WNDCLASS wc; // Структура класса Windows для установки типа окна
	//HWND hWnd; // Сохранение дескриптора окна

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	WNDCLASS wc{};
	//wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.lpszClassName = L"WindowClass";
	//RegisterClass(&wc);

	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC; // Перерисовывание окна при перемещении, скрытый DC окна
	//wc.lpfnWndProc = (WNDPROC)WndProc; // Перехват сообщений
	wc.cbClsExtra = 0; 
	wc.cbWndExtra = 0; 
	//wc.hInstance = hInstance; 
	wc.hIcon = NULL; 
	//wc.hCursor = LoadCursor(NULL, IDC_ARROW); 
	wc.hbrBackground = NULL; 
	wc.lpszMenuName = NULL; 
	//wc.lpszClassName = L"OpenGL WinClass";

	// Регистрация окна
	if (!RegisterClass(&wc)) { 
		MessageBox(0, L"Failed To Register The Window Class.", L"Error", MB_OK | MB_ICONERROR); return FALSE; 
	}

	//auto hWnd = CreateWindow(L"OpenGL WinClass", L"Jeff Molofee's GL Code Tutorial … NeHe '99", // Заголовок вверху окна
	//	WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 0, 0, // Позиция окна на экране 
	//	640, 480, // Ширина и высота окна 
	//	NULL, NULL, hInstance, NULL);

	auto hWnd = CreateWindow(wc.lpszClassName, L"App", WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT, NULL, CW_USEDEFAULT, NULL, nullptr, nullptr, hInstance, nullptr);

	ShowWindow(hWnd, nShowCmd);
	UpdateWindow(hWnd);


	if (!hWnd) {
		MessageBox(0, L"Window Creation Error.", L"Error", MB_OK | MB_ICONERROR); 
		return FALSE; 
	}

	DEVMODE dmScreenSettings; // Режим работы
	
	//memset(&dmScreenSettings, 0, sizeof(DEVMODE)); // Очистка для хранения установок 
	//dmScreenSettings.dmSize = sizeof(DEVMODE); // Размер структуры Devmode 
	////dmScreenSettings.dmPelsWidth = 640; // Ширина экрана 
	////dmScreenSettings.dmPelsHeight = 480; // Высота экрана 
	//dmScreenSettings.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT; // Режим Пиксела 
	//ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN); // Переключение в полный экран

	//ShowWindow(hWnd, SW_SHOW); 
	//UpdateWindow(hWnd); 
	SetFocus(hWnd);

	glFrustum(-1, 1, -1, 1, 2, 8); //Перспективная проекция
	while (1) {
		// Обработка всех сообщений 
		while (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
			if (GetMessage(&msg, NULL, 0, 0)) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else 
				return TRUE;
		}

		DrawGLScene(); // Нарисовать сцену 
		SwapBuffers(hDC); // Переключить буфер экрана 
		if (keys[VK_ESCAPE])
			SendMessage(hWnd, WM_CLOSE, 0, 0); // Если ESC - выйти 
	}

	
}

void trim(char* s)
{
	// удаляем пробелы и табы с начала строки:
	int i = 0, j;
	while ((s[i] == ' ') || (s[i] == '\t'))
	{
		i++;
	}
	if (i > 0)
	{
		for (j = 0; j < strlen(s); j++)
		{
			s[j] = s[j + i];
		}
		s[j] = '\0';
	}

	// удаляем пробелы и табы с конца строки:
	i = strlen(s) - 1;
	while ((s[i] == ' ') || (s[i] == '\t'))
	{
		i--;
	}
	if (i < (strlen(s) - 1))
	{
		s[i + 1] = '\0';
	}
}

