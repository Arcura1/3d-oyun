#include <iostream>
#include <Windows.h>
#include <chrono>
using namespace std;
int eheight=40,ewidth=120;//ekran büyüüklüðü

float karakterx=8.0f;//karkater kordinatlarý
float karaktery = 8.0f;//karkater kordinatlarý

float karaktera = 0.0f;//karkaterin baktýðý yön

int nmapheight = 16;//mapin yüksekliði
int nmapwidth = 16;//mapin geniþliði

float Ffov = 3.14159/4;//karakterin gödüþ açýsý
float maxuzak = 16.0f;


int main() {
	//bufffer
	wchar_t *ekran = new wchar_t[ewidth*eheight];
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER,NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesyazýldý = 0;
	//buffer
	
	////içindeki harita////
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


	////içindeki harita////


	//////zaman ayýraçlarý////////
	auto tp1 = chrono::system_clock::now();
	auto tp2 = chrono::system_clock::now();
	//////zaman ayýraçlarý////////

	//gameloop
	while (1) {
		if (GetAsyncKeyState((unsigned short)'O') & 0x8000) {
			break;//çýkýþ
		}

		//////zaman için////////

		tp2 = chrono::system_clock::now();
		chrono::duration<float> elapsedTime = tp2 - tp1;
		tp1 = tp2;
		float felapsedTime = elapsedTime.count();
		
		/////zaman için////////
		
		
		////kontroller için/////
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
		
		////kontroller için/////
		
		
		/////render/////
		for (int x = 0; x < ewidth; x++) {
			float rayacý = (karaktera - Ffov / 2.0f) + (((float)x / (float)ewidth) * Ffov);
			
			float duvaraUzaklýk = 0.0f;
			
			bool flag1 = false;
			
			float vektx = sinf(rayacý);
			float vekty = cosf(rayacý);
			
			while (!flag1 ) {
				duvaraUzaklýk += 0.1f;

				int ntestx = (int)(karakterx + vektx * duvaraUzaklýk);
				int ntesty = (int)(karaktery + vekty * duvaraUzaklýk);

				if (ntestx < 0 || ntestx >= nmapwidth || ntesty < 0 || ntesty >= nmapheight) {
					flag1 = true;
					duvaraUzaklýk = maxuzak;
				}
				else {
					if (map[ntesty * nmapheight + ntestx] == '#') {
						flag1 = true;



					}
				}
			}
			
			
			int nCelling = (float)eheight / 2.0 - eheight / (float)duvaraUzaklýk;
			int efloor = eheight - nCelling;

			short nshade=' ';
			if (duvaraUzaklýk <= maxuzak/4.0)        nshade = 0x2588;
			else if (duvaraUzaklýk <= maxuzak / 3.0) nshade = 0x2593;
			else if (duvaraUzaklýk <= maxuzak / 2.0) nshade = 0x2592;
			else if (duvaraUzaklýk <= maxuzak)       nshade = 0x2591;
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
	
		///swprintf_s(ekran, 40, L"X=%3.2f,Y=%3.2f,FPS=%3.2f",karakterx,karaktery,1.0f/felapsedTime); // fps ve deðerleri göstermesi için
		for (int mnx = 0;mnx<nmapheight; mnx++) {
			for (int mns = 0; mns < nmapwidth; mns++) {
				ekran[(mns + 1) * ewidth + mnx] = map[mns * nmapwidth + mnx];
			}
		} 
		ekran[((int)karaktery+1)*ewidth+(int)karakterx]='P';
		ekran[ewidth*eheight - 1] = '\0';
		WriteConsoleOutputCharacter(hConsole, ekran, ewidth * eheight, { 0,0 }, &dwBytesyazýldý);
		/////render///
		
	}
	//gameloop


}