#include <raylib.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Cursorposition.h"

typedef struct tilestruct { Color color; Rectangle rect; } tiles;
typedef struct enemynames { Rectangle hitbox; char name[4]; } names;

names enemynames[100];
tiles tile[625];

Color cursorcolor;

int linearray[2][1000];
int totallines = 0;
int totalmobs = 5;
int totalenemies = 0;
int pos = 0;
int pos0 = 0;
int pos1 = 0;
int pos2 = 0;
int pos3 = 0;
int dimcol = 0;
int drawdims = 0;


Rectangle enemyspawner;
Rectangle players[100];
Rectangle textbox;
Rectangle textbox0;
Rectangle textbox1;
Rectangle textbox2;
Rectangle textbox3;
Rectangle clrscreen;
Rectangle savebutton;
Rectangle loadbutton;
Rectangle undobutton;
Rectangle dimbutton;

char totalsaves;
char enemyname[5];
char p0size[3];
char p1size[3];
char p2size[3];
char p3size[3];

void initiateplayers();
void initiateenemyspawner();

void checkdimbutton(Vector2 mousepos)
{
	if (CheckCollisionPointRec(mousepos, dimbutton))
	{
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		{
			if (drawdims == 0)
			{
				drawdims = 1;
				dimcol = 1;
			}
			else
			{
				drawdims = 0;
				dimcol = 0;
			}
		}
	}
}


void drawdimbutton()
{
	if (dimcol == 0)
		DrawRectangleRec(dimbutton, RED);
	else if (dimcol == 1)
		DrawRectangleRec(dimbutton, GREEN);
	DrawText("DIM", dimbutton.x + 20, dimbutton.y + 15, 20, BLACK);
}

void initiatedimbutton()
{
	dimbutton = (Rectangle){600,20,80,50};
}

void checkundobutton(Vector2 mousepos)
{
	if (CheckCollisionPointRec(mousepos, undobutton))
	{
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		{
			if(totallines>0)
			totallines -= 1;
		}
	}
}

void drawundobutton()
{
	DrawRectangleRec(undobutton, RED);
	DrawText("Undo", undobutton.x + 3, undobutton.y + 10, 10, BLACK);
}


void initialiseundobutton()
{
	undobutton = (Rectangle){ 910,80,30,30 };
}


void checkload(Vector2 mousepos)
{
	int loadfilefound = 0;
	char loadfilenamebuffer[100];
	char desiredloadfile[100];
	char truefile[100];

	if (CheckCollisionPointRec(mousepos, loadbutton))
	{
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		{
			system("cls");
			FILE* file;
			file = fopen("filenames.txt", "r");
			while (fscanf(file, "%s", loadfilenamebuffer) != EOF)
			{
				printf("%s\n", loadfilenamebuffer);
			}
			fseek(file, 0, SEEK_SET);
			printf("\n\nWhich file do you wish to load?: ");
			scanf("%s", desiredloadfile);
			while (fscanf(file, "%s", loadfilenamebuffer) != EOF)
			{
				if (strstr(loadfilenamebuffer,desiredloadfile))
				{
					loadfilefound = 1;
					break;
				}
			}
			if (loadfilefound == 0)
			{
				fclose(file);
				checkload(mousepos);
			}
			else
			{
				int checker = 1;
				int loadbuffer;
				FILE* loadfile;
				loadfile = fopen(loadfilenamebuffer, "r");
				totallines = 0;
				int x = 0;
				while (fscanf(loadfile,"%d", &loadbuffer) != EOF)
				{
						if (checker % 2 == 1)
						{
							linearray[0][x] = loadbuffer;
						}
						else
						{
							linearray[1][x] = loadbuffer;
							totallines += 1;
							x += 1;
						}
						checker += 1;
				}
				printf("\n\nFILE LOADED");
				fclose(loadfile);
				fclose(file);
			}
		}
	}
}


void drawloadbutton()
{
	DrawRectangleRec(loadbutton, RED);
	DrawText("LOAD", loadbutton.x + 10, loadbutton.y + 15, 20, BLACK);
}

void initialiseloadbutton()
{
	loadbutton = (Rectangle){ 700,20,80,50 };
}

void checksave(Vector2 mousepos)
{
	char savefilename[100];

	if (CheckCollisionPointRec(mousepos, savebutton))
	{
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		{
			system("cls");
			printf("Enter save file name:(file must contain '.txt')\n%s",save);
			do
			{
				printf("%s\033[0K",reset);
				scanf("%s", savefilename);
			} while (!strstr(savefilename, ".txt"));
			FILE* file;
			FILE* file2;
			printf("\nFILE SAVED");


			file2 = fopen("filenames.txt", "a");
			fprintf(file2, "%s\n", savefilename);


			file=fopen(savefilename, "a");
			for (int x = 0; x < totallines; x++)
			{
				fprintf(file, "%d %d\n", linearray[0][x],linearray[1][x]);
			}
			fclose(file);
			fclose(file2);
		}
	}
}

void drawsavebutton()
{
	DrawRectangleRec(savebutton, RED);
	DrawText("SAVE", savebutton.x + 10, savebutton.y + 15, 20, BLACK);
}

void initialisesavebutton()
{
	savebutton = (Rectangle){ 800,20,80,50 };
}

void initialisesavenumber()
{
	FILE* file;
	file = fopen("savenum.txt", "r");
	fscanf(file, "%c", &totalsaves);
	fclose(file);
}

void drawnodes()
{
	for (int x = 0; x < totallines; x++)
	{
		DrawCircle(tile[linearray[0][x]].rect.x+20, tile[linearray[0][x]].rect.y+20, 10, BLACK);
		DrawCircle(tile[linearray[1][x]].rect.x + 20, tile[linearray[1][x]].rect.y + 20, 10, BLACK);
	}
}



void drawdistances()
{
	int distance;
	float positionx;
	float positiony;
	for (int x = 0; x < totallines; x++)
	{
		distance = (abs(tile[linearray[0][x]].rect.x- tile[linearray[1][x]].rect.x)+ abs(tile[linearray[0][x]].rect.y - tile[linearray[1][x]].rect.y ))/8;
		positionx = (tile[linearray[0][x]].rect.x + tile[linearray[1][x]].rect.x) / 2;
		positiony= (tile[linearray[0][x]].rect.y + tile[linearray[1][x]].rect.y) / 2;

		if (tile[linearray[0][x]].rect.x == tile[linearray[1][x]].rect.x && tile[linearray[0][x]].rect.x >= 500)
			positionx +=30;
		else if (tile[linearray[0][x]].rect.x == tile[linearray[1][x]].rect.x && tile[linearray[0][x]].rect.x <= 500)
			positionx -=10;
		else if (tile[linearray[0][x]].rect.y == tile[linearray[1][x]].rect.y && tile[linearray[0][x]].rect.y >= 500)
			positiony += 30;

		DrawText(TextFormat("%d", distance), positionx, positiony, 15, RED);
	}
}

void checkclearscreen(Vector2 mousepos)
{
	if (CheckCollisionPointRec(mousepos, clrscreen))
	{
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		{
			for (int x = 0; x < 100; x++)
			{
				for (int y = 0; y <= 3; y++)
				{
					enemynames[x].name[y] = ' ';
				}
				enemynames[x].hitbox = (Rectangle){ -100,-100,0,0 };
				players[x] = (Rectangle){ 0,0,0,0 };
			}
			totallines = 0;
			totalmobs = 5;
			pos = 0;
			pos0 = 0;
			pos1 = 0;
			pos2 = 0;
			pos3 = 0;
			p0size[0] = 1;
			p1size[0] = 1;
			p2size[0] = 1;
			p3size[0] = 1;
			initiateplayers();
			initiateenemyspawner();
		}
	}
}

void initiateclearscreen()
{
	clrscreen = (Rectangle){900,20,50,50};
}

void drawclearscreen()
{
	DrawRectangleRec(clrscreen, RED);
	DrawText("CLR", clrscreen.x+5, clrscreen.y + 15, 20, BLACK);
}


void changesizes()
{
	if (p0size[0] == '1')
	{
		players[0].width = 30;
		players[0].height = 30;
	}
	else if(p0size[0] == '2')
	{
		players[0].width = 70;
		players[0].height = 70;
	}
	else if (p0size[0] == '3')
	{
		players[0].width = 110;
		players[0].height = 110;
	}


	if (p1size[0] == '1')
	{
		players[1].width = 30;
		players[1].height = 30;
	}
	else if (p1size[0] == '2')
	{
		players[1].width = 70;
		players[1].height = 70;
	}
	else if (p1size[0] == '3')
	{
		players[1].width = 110;
		players[1].height = 110;
	}


	if (p2size[0] == '1')
	{
		players[2].width = 30;
		players[2].height = 30;
	}
	else if (p2size[0] == '2')
	{
		players[2].width = 70;
		players[2].height = 70;
	}
	else if (p2size[0] == '3')
	{
		players[2].width = 110;
		players[2].height = 110;
	}


	if (p3size[0] == '1')
	{
		players[3].width = 30;
		players[3].height = 30;
	}
	else if (p3size[0] == '1')
	{
		players[3].width = 70;
		players[3].height = 70;
	}
	else if (p3size[0] == '1')
	{
		players[3].width = 110;
		players[3].height = 110;
	}
}


void drawenemynames()
{
	for(int e=0;e<100;e++)
	DrawText(enemynames[e].name,enemynames[e].hitbox.x,enemynames[e].hitbox.y,15,BLACK);
}


void checktextboxinput(Vector2 mousepos)
{
	char key = GetKeyPressed();
	if (CheckCollisionPointRec(mousepos, textbox))
	{
		if (key >= 65 && key <= 90)
		{
			if (pos == 3)
			{
				pos = 0;
				enemyname[0] = enemyname[1] = enemyname[2] = ' ';
				enemynames[totalmobs - 1].name[0] = enemynames[totalmobs - 1].name[1] = enemynames[totalmobs - 1].name[2] = ' ';
			}
			else
			{
				enemyname[pos] = key;
				enemynames[totalmobs - 1].name[pos] = key;
				pos++;
			}
		}
	}
	else if (CheckCollisionPointRec(mousepos, textbox0))
	{
		if (key >= 48 && key <= 57)
		{
			if (pos0 == 1)
			{
				pos0 = 0;
				p0size[0] = ' ';
			}
			else
			{
				p0size[pos0] = key;
				pos0++;
			}
		}
	}
	else if (CheckCollisionPointRec(mousepos, textbox1))
	{
		if (key >= 48 && key <= 57)
		{
			if (pos1 == 1)
			{
				pos1 = 0;
				p1size[0] = ' ';
			}
			else
			{
				p1size[pos1] = key;
				pos1++;
			}
		}
	}
	else if (CheckCollisionPointRec(mousepos, textbox2))
	{
		if (key >= 48 && key <= 57)
		{
			if (pos2 == 1)
			{
				pos2 = 0;
				p2size[0] = ' ';
			}
			else
			{
				p2size[pos2] = key;
				pos2++;
			}
		}
	}
	else if (CheckCollisionPointRec(mousepos, textbox3))
	{
		if (key >= 48 && key <= 57)
		{
			if (pos3 == 1)
			{
				pos3 = 0;
				p3size[0] = ' ';
			}
			else
			{
				p3size[pos3] = key;
				pos3++;
			}
		}
	}
}

void drawtextbox()
{
	DrawRectangle(815, 920, 110, 40, RED);
	DrawRectangleRec(textbox, WHITE);
	DrawRectangleRec(textbox0, WHITE);
	DrawRectangleRec(textbox1, WHITE);
	DrawRectangleRec(textbox2, WHITE);
	DrawRectangleRec(textbox3, WHITE);
	DrawText(enemyname, 845, 925, 20, BLACK);
	DrawText(p3size, 865, 845, 20, BLACK);
	DrawText(p2size, 865, 805, 20, BLACK);
	DrawText(p1size, 865, 765, 20, BLACK);
	DrawText(p0size, 865, 725, 20, BLACK);
}

void initiatetextbox()
{
	textbox = (Rectangle){ 820,925,100,30 };
	textbox3 = (Rectangle){ 820,845,100,30 };
	textbox2 = (Rectangle){ 820,805,100,30 };
	textbox1 = (Rectangle){820,765,100,30 };
	textbox0 = (Rectangle){ 820,725,100,30 };
}

void initiateenemyspawner()
{
	enemyspawner = (Rectangle){ 935,925,30,30 };
	players[totalmobs-1] = enemyspawner;
}

void drawplayernames()
{
	DrawText("P", players[0].x+10, players[0].y+10, 10, BLACK);
	DrawText("C", players[1].x + 10, players[1].y + 10, 10, BLACK);
	DrawText("A", players[2].x + 10, players[2].y + 10, 10, BLACK);
	DrawText("G", players[3].x + 10, players[3].y + 10, 10, BLACK);

}



void checkplayers(Vector2 mousepos)
{
	static int picked = -1;


	for (int t = 0; t < 625; t++)
	{
			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && picked != -1)
			{
				if (CheckCollisionPointRec(mousepos, tile[t].rect))
				{
					if (picked >= 4 && picked < totalmobs - 1)
					{
						players[picked].x = tile[t].rect.x + 5;
						players[picked].y = tile[t].rect.y + 5;
						enemynames[picked].hitbox.x = players[picked].x;
						enemynames[picked].hitbox.y = players[picked].y;
						picked = -1;
					}
					else 
					{
						players[picked].x = tile[t].rect.x + 5;
						players[picked].y = tile[t].rect.y + 5;
						picked = -1;
					}
				}
				pos = 0;
			}
	}


	for (int p = 0; p <totalmobs; p++)
	{
		if (CheckCollisionPointRec(mousepos, players[p]))
		{
			if (IsMouseButtonPressed( MOUSE_BUTTON_LEFT ))
			{
				if (picked == -1)
				{
					enemyname[0]=enemyname[1]=enemyname[2]=' ';
					picked = p;
					if (picked ==totalmobs-1)
					{
						totalmobs += 1;
						totalenemies += 1;
						initiateenemyspawner();
					}
				}
			}
		}
	}


	if (picked != -1)
	{
		if (picked <= 3)
		{
			players[picked].x = mousepos.x + 20;
			players[picked].y = mousepos.y + 20;
		}
		else if(picked>=4&&picked<totalmobs-1)
		{
			players[picked].x = mousepos.x + 20;
			players[picked].y = mousepos.y + 20;
			enemynames[picked].hitbox.x = players[picked].x;
			enemynames[picked].hitbox.y = players[picked].y;
		}
	}
}



void drawplayersandenemy()
{
	int x;
	for (x = 0; x <=3; x++)
	{
		DrawRectangleRec(players[x], GREEN);
	}
	for(x=4;x<100;x++)
	DrawRectangleRec(players[x], RED);
}

void initiateplayers()
{
	players[0] = (Rectangle){935,725,30,30};
	players[1] = (Rectangle){935,765,30,30};
	players[2] = (Rectangle){935,805,30,30};
	players[3] = (Rectangle){935,845,30,30};
}

void drawselectedlines()
{
	Rectangle startingtile, endingtile;
	for (int l = 0; l <totallines; l++)
	{
		startingtile = tile[linearray[0][l]].rect;
		endingtile = tile[linearray[1][l]].rect;
		Vector2 startposition = { startingtile.x + startingtile.width / 2, startingtile.y + startingtile.height / 2 };
		Vector2 endposition = { endingtile.x + endingtile.width / 2, endingtile.y + endingtile.height / 2 };
		DrawLineEx(startposition,endposition,4,BLACK);
	}
}



void checktiles(Vector2 mousepos)
{
	static int selectpos = 0;
	for (int t = 0; t < 625; t++)
	{
		if (CheckCollisionPointRec(mousepos, tile[t].rect))
		{
			if (selectpos == 1)
			{
				Rectangle startingtile, endingtile;
				linearray[1][totallines] = t;
				startingtile = tile[linearray[0][totallines]].rect;
				endingtile = tile[linearray[1][totallines]].rect;
				Vector2 startposition = { startingtile.x + startingtile.width / 2, startingtile.y + startingtile.height / 2 };
				Vector2 endposition = { endingtile.x + endingtile.width / 2, endingtile.y + endingtile.height / 2 };
				DrawLineEx(startposition, endposition, 4, BLACK);
				
				
				int distance;
				float positionx;
				float positiony;
				distance = (abs(tile[linearray[0][totallines]].rect.x - tile[linearray[1][totallines]].rect.x) + abs(tile[linearray[0][totallines]].rect.y - tile[linearray[1][totallines]].rect.y)) / 8;
				positionx = (tile[linearray[0][totallines]].rect.x + tile[linearray[1][totallines]].rect.x) / 2;
				positiony = (tile[linearray[0][totallines]].rect.y + tile[linearray[1][totallines]].rect.y) / 2;

				if (tile[linearray[0][totallines]].rect.x == tile[linearray[1][totallines]].rect.x && tile[linearray[0][totallines]].rect.x >= 500)
					positionx += 30;
				else if (tile[linearray[0][totallines]].rect.x == tile[linearray[1][totallines]].rect.x && tile[linearray[0][totallines]].rect.x <= 500)
					positionx -= 10;
				else if (tile[linearray[0][totallines]].rect.y == tile[linearray[1][totallines]].rect.y && tile[linearray[0][totallines]].rect.y >= 500)
					positiony += 30;

				DrawText(TextFormat("%d", distance), positionx, positiony, 15, RED);
			}

			if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
			{
					if (selectpos == 0)
					{
						linearray[0][totallines] = t;
						selectpos =1;
					}
					else if (selectpos== 1)
					{
						linearray[1][totallines] = t;
						totallines += 1;
						selectpos =0;
					}
			}
		}
	}
}

void drawgrid()
{
	for (int x = 0; x <= 1000; x += 40)
	{
		DrawLine(x, 0, x, 1000, BLACK);
	}
	for (int y = 0; y <= 1000; y += 40)
	{
		DrawLine(0, y, 1000, y, BLACK);
	}
}

void displaytiles()
{
	for (int t = 0; t < 625; t++)
	{
		DrawRectangleRec(tile[t].rect, tile[t].color);
	}
}

void initialisetiles()
{
	int t = 0;
	for (int x = 0; x <= 960; x += 40)
	{
		for (int y = 0; y <= 960; y += 40)
		{
			tile[t].rect.x = x;
			tile[t].rect.y = y;
			tile[t].color = (Color){238,215,161,255};
			tile[t].rect.width = 40;
			tile[t].rect.height = 40;
			t += 1;
		}
	}
}


int main(void)
{
	InitWindow(1000, 1000, "DnD Map Editor");
	system("cls");
	initialisetiles();
	initiateplayers();
	initiateenemyspawner();
	initiatetextbox();
	initiateclearscreen();
	initialisesavenumber();
	initialisesavebutton();
	initialiseloadbutton();
	initialiseundobutton();
	initiatedimbutton();

	Vector2 mousepos;

	while (!WindowShouldClose())
	{
		mousepos = GetMousePosition();

		BeginDrawing();
		ClearBackground(WHITE);

		displaytiles();
		drawgrid();
		
		drawplayersandenemy();
		drawplayernames();
		drawtextbox();
		drawenemynames();
		drawclearscreen();
		if(drawdims==1)
			drawdistances();
		drawsavebutton();
		drawloadbutton();
		drawundobutton();
		drawdimbutton();

		checktiles(mousepos);
		checkplayers(mousepos);
		checktextboxinput(mousepos);
		checkclearscreen(mousepos);
		checksave(mousepos);
		checkload(mousepos);
		checkundobutton(mousepos);
		checkdimbutton(mousepos);

		changesizes();

		drawselectedlines();
		drawnodes();

		EndDrawing();
	}
	CloseWindow();
}