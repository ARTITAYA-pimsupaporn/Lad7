#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<windows.h>
#include<time.h>
#include<conio.h>
int score = 0;
void setcursor(bool visible) // ลบ cursor 0 = ลบ , 1 = มี
{
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO lpCursor;
	lpCursor.bVisible = visible;
	lpCursor.dwSize = 20;
	SetConsoleCursorInfo(console, &lpCursor);
}
void gotoxy(int x, int y) // พิกัด
{
	COORD c = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}
char cursor(int x, int y) { // ตรวจสอบการชน
	HANDLE hStd = GetStdHandle(STD_OUTPUT_HANDLE);
	char buf[2]; COORD c = { x,y }; DWORD num_read;
	if (
		!ReadConsoleOutputCharacter(hStd, (LPTSTR)buf, 1, c, (LPDWORD)&num_read))

		return '\0';
	else
		return buf[0];

}
void erase_ship(int x, int y)
{
	gotoxy(x, y);
	printf("         ");
}
void draw_ship(int x, int y) // วาดยาน
{
	gotoxy(x, y); printf(" <-0-> ");
}
void draw_bullet(int x, int y)	// วาดกระสุน
{
	gotoxy(x, y);
	printf("A");
}
void clear_bullet(int x, int y)  // ลบกระสุนที่เคลื่อนที่
{
	gotoxy(x, y); 
	printf(" ");
}
void addScore()
{
	score+=10;
	gotoxy(76, 0);
	printf(" %d", score);
}
int randrange(int start, int stop)
{
	return rand() % (stop - start + 1) + start;
}
int main() {
	char ch = '.';
	srand(time(NULL)); //สุ่มไม่ให้ชุดเดิม time(NULL) จะได้ตัวเลขใหม่ 
	int x = 38, y = 20;
	int direction = 0;
	int i = 0;
	int bx[5], by[5];
	bool bulletStatus[5] = {0,0,0,0,0};
	int score = 0;
	setcursor(0);
	gotoxy(x, y);
	draw_ship(x, y);
	

	//#################### random #########################
	for (int i = 0; i < 20; i++)
	{
		gotoxy(randrange(10, 70), randrange(2, 5)); //  x ตัวอักษร y บรรทัด
		printf("*");
	}
	//#################### move ############################
	do {
		if (_kbhit()) {
			ch = _getch();
			if (ch == 'a') { direction = -1; }
			if (ch == 'd') { direction = 1; }
			if (ch == 's') { direction = 0; }
			if (ch == ' ')
			{
				for (int i = 0; i < 5; i++)
				{
					if (!bulletStatus[i])
					{
						bulletStatus[i] = 1;
						bx[i] = x + 3;
						by[i] = y - 1;
						break;
					}
				}

			}
			fflush(stdin);
		}
		if (direction != 0) {
			if (direction == -1 && x > 0) {			// กด a
				erase_ship(x, y);
				draw_ship(--x, y);
			}
			else if (direction == 1 && x < 71) {	// กด d
				erase_ship(x, y);
				draw_ship(++x, y);
			}
		}
		else
		{
			draw_ship(x, y);  // กด s
		}
	
		// ################# กระสุน + เช็คการชน ######################
		for (int i = 0; i < 5; i++)
		{
			if (bulletStatus[i])
			{
				clear_bullet(bx[i], by[i]);
				Beep(700, 25); // คอมทุกคนจะมี speaker (ความถี่ , millis)
				
				if (cursor(bx[i], by[i] - 1) == '*')
				{
					
					addScore();
					Beep(500, 25);
					gotoxy(bx[i], by[i] - 1);  // ต้องมีตำแหน่งที่ลบ
					printf(" ");
					gotoxy(randrange(10, 70), randrange(2, 5)); 
					printf("*");
					bulletStatus[i] = 0;   // รีเชต กระสุนเมื่อชน
				}
				else if (by[i] >= 2)
				{
					draw_bullet(bx[i], --by[i]);
				}
				else 
				{
					bulletStatus[i] = 0;
				}
			}
		}
		gotoxy(68, 0);
		printf("SCORE : ");
		Sleep(100);

	} while (ch != 'x');
	return 0;
}