#include <iostream>
#include <Windows.h>
#include <chrono>
using namespace std;
int eheight=40,ewidth=120;//ekran b�y��kl���

float karakterx=8.0f;//karkater kordinatlar�
float karaktery = 8.0f;//karkater kordinatlar�

float karaktera = 0.0f;//karkaterin bakt��� y�n

int nmapheight = 16;//mapin y�ksekli�i
int nmapwidth = 16;//mapin geni�li�i

float Ffov = 3.14159/4;//karakterin g�d�� a��s�
float maxuzak = 16.0f;


int main() {
	//bufffer
	wchar_t *ekran = new wchar_t[ewidth*eheight];
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER,NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesyaz�ld� = 0;
	//buffer
	
	////i�indeki harita////
	wstring map;

	map += L"################";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#...........#..#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..........#####";
	map += L"#..............#";
	map += L"#..............#";
	map += L"################";


	////i�indeki harita////


	//////zaman ay�ra�lar�////////
	auto tp1 = chrono::system_clock::now();
	auto tp2 = chrono::system_clock::now();
	//////zaman ay�ra�lar�////////

	//gameloop
	while (1) {
		if (GetAsyncKeyState((unsigned short)'O') & 0x8000) {
			break;//��k��
		}

		//////zaman i�in////////

		tp2 = chrono::system_clock::now();
		chrono::duration<float> elapsedTime = tp2 - tp1;
		tp1 = tp2;
		float felapsedTime = elapsedTime.count();
		
		/////zaman i�in////////
		
		
		////kontroller i�in/////
		if (GetAsyncKeyState((unsigned short)'W') & 0x8000) {
			
			karakterx += sinf(karaktera) * 3.0 * felapsedTime;
			karaktery += cosf(karaktera) * 3.0 * felapsedTime;
			if (map[(int)karaktery * nmapwidth + (int)karakterx] == '#') {
				karakterx -= sinf(karaktera) * 3.0 * felapsedTime;
				karaktery -= cosf(karaktera) * 3.0 * felapsedTime;
			}

		}
		if (GetAsyncKeyState((unsigned short)'S') & 0x8000) {

			karakterx -= sinf(karaktera) * 3.0 * felapsedTime;
			karaktery -= cosf(karaktera) * 3.0 * felapsedTime;
			if (map[(int)karaktery * nmapwidth + (int)karakterx] == '#') {
				karakterx += sinf(karaktera) * 3.0 * felapsedTime;
				karaktery += cosf(karaktera) * 3.0 * felapsedTime;
			}

		}
		if (GetAsyncKeyState((unsigned short)'Q') & 0x8000) {
			karaktera -= (1.0)*felapsedTime;
		}
		if (GetAsyncKeyState((unsigned short)'E') & 0x8000) {
			karaktera += (1.0) * felapsedTime;
		}
		if (GetAsyncKeyState((unsigned short)'A') & 0x8000) {
			karakterx += sinf(karaktera - 3.14159 / 2) * 3.0 * felapsedTime;
			karaktery += cosf(karaktera - 3.14159 / 2) * 3.0 * felapsedTime;
			if (map[(int)karaktery * nmapwidth + (int)karakterx] == '#') {
				karakterx -= sinf(karaktera - 3.14159 / 2) * 3.0 * felapsedTime;
				karaktery -= cosf(karaktera - 3.14159 / 2) * 3.0 * felapsedTime;
			}
		}
		if (GetAsyncKeyState((unsigned short)'D') & 0x8000) {
			karakterx += sinf(karaktera + 3.14159 / 2) * 3.0 * felapsedTime;
			karaktery += cosf(karaktera + 3.14159 / 2) * 3.0 * felapsedTime;
			if (map[(int)karaktery * nmapwidth + (int)karakterx] == '#') {
				karakterx -= sinf(karaktera + 3.14159 / 2) * 3.0 * felapsedTime;
				karaktery -= cosf(karaktera + 3.14159 / 2) * 3.0 * felapsedTime;
			}
		}
		
		////kontroller i�in/////
		
		
		/////render/////
		for (int x = 0; x < ewidth; x++) {
			float rayac� = (karaktera - Ffov / 2.0f) + (((float)x / (float)ewidth) * Ffov);
			
			float duvaraUzakl�k = 0.0f;
			
			bool flag1 = false;
			
			float vektx = sinf(rayac�);
			float vekty = cosf(rayac�);
			
			while (!flag1 ) {
				duvaraUzakl�k += 0.1f;

				int ntestx = (int)(karakterx + vektx * duvaraUzakl�k);
				int ntesty = (int)(karaktery + vekty * duvaraUzakl�k);

				if (ntestx < 0 || ntestx >= nmapwidth || ntesty < 0 || ntesty >= nmapheight) {
					flag1 = true;
					duvaraUzakl�k = maxuzak;
				}
				else {
					if (map[ntesty * nmapheight + ntestx] == '#') {
						flag1 = true;



					}
				}
			}
			
			
			int nCelling = (float)eheight / 2.0 - eheight / (float)duvaraUzakl�k;
			int efloor = eheight - nCelling;

			short nshade=' ';
			if (duvaraUzakl�k <= maxuzak/4.0)        nshade = 0x2588;
			else if (duvaraUzakl�k <= maxuzak / 3.0) nshade = 0x2593;
			else if (duvaraUzakl�k <= maxuzak / 2.0) nshade = 0x2592;
			else if (duvaraUzakl�k <= maxuzak)       nshade = 0x2591;
			else									 nshade = ' ';
			

			for (int y = 0;y < eheight; y++) {
				if (y < nCelling) {
					ekran[y * ewidth + x] = ' ';
				}
				else if (y > nCelling && y < efloor) {
					ekran[y * ewidth + x] = nshade;
				}
				else {
					float b = 1.0f - (((float)y - (eheight / 2.0f)) / ((float)eheight / 2.0f));
					if (b < 0.25)        nshade = '#';
					else if (b < 0.50) nshade = 'x';
					else if (b < 0.75) nshade = '.';
					else if (b < 0.9)       nshade = '-';
					else ;
					
					ekran[y * ewidth + x] = nshade;
					
				}
			}

		
		}
	
		///swprintf_s(ekran, 40, L"X=%3.2f,Y=%3.2f,FPS=%3.2f",karakterx,karaktery,1.0f/felapsedTime); // fps ve de�erleri g�stermesi i�in
		for (int mnx = 0;mnx<nmapheight; mnx++) {
			for (int mns = 0; mns < nmapwidth; mns++) {
				ekran[(mns + 1) * ewidth + mnx] = map[mns * nmapwidth + mnx];
			}
		} 
		ekran[((int)karaktery+1)*ewidth+(int)karakterx]='P';
		ekran[ewidth*eheight - 1] = '\0';
		WriteConsoleOutputCharacter(hConsole, ekran, ewidth * eheight, { 0,0 }, &dwBytesyaz�ld�);
		/////render///
		
	}
	//gameloop


}