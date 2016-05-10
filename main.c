#include<GL/glut.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>
#ifdef FMOD
#include"fmod.h"
#endif

GLubyte Texture[27][1024];
char liste_touches[27] =
{ 'A', 'Z', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', 'Q', 'S', 'D', 'F', 'G',
		'H', 'J', 'K', 'L', 'M', 'W', 'X', 'C', 'V', 'B', 'N', ' ' };
float positions_y[2][27] =
{
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0 },
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0 } };
GLuint Texts[27], couleur_texte[3][36][3] =
{ 0, 0, 1 };
int appui_touche = 0, demande = 1, mode = 0, son = 0, affichage = 1, son_lance =
		0, SoundChannel = 0, file_end = 0, offset_ouverture = 0;
FILE *fichier_entree;
#ifdef FMOD
FSOUND_SAMPLE *pSound;
#endif
char affichage_texte[50], texte_demande[3][36];
clock_t temps1, temps2;
int position_texte_actuel = 0, numero_texte_actuel = 0;

void dessiner_touche(unsigned char key, float x, float y, float z);
void Reshape(int width, int height);
void InitGL(void);
void Draw();
void idle();
void Touches(unsigned char key, int x, int y);
void Touches_spe(int key, int x, int y);
void init_alphabet(void);
void init_config(void);
void save_config(void);
void menuPrincipal(int value);
void sortir();
void output_texte(float x, float y, char *p, int numero, int taille, int cmpt);
void output(float x, float y, char *string);
void dessiner_mains(unsigned char key);
void dessiner_paumes(float x, float y, float z, int sens);
void dessiner_doigts(float p1[3], float p2[3], float p3[3], float p4[3],
		int numero, int sens);
void dessiner_pouce(float p1[3], float p2[3], float p3[3], int sens);
#ifdef FMOD
int son_lettre(unsigned char key);
void init_son();
#endif
void Afficher_curseur(int x, int y);
void Cacher_curseur(int x, int y);

void init_config()
{
	FILE *fichier;
	int value;
	if ((fichier = fopen("config.ini", "r")))
	{
		fscanf(fichier, "mode=%d\nson=%d\naffichage=%d", &value, &son,
				&affichage);
		fclose(fichier);
	}
	mode = 3;
	menuPrincipal(value);
}

void save_config()
{
	FILE *fichier;
	if ((fichier = fopen("config.ini", "w")))
	{
		fprintf(fichier, "mode=%d\nson=%d\naffichage=%d", mode, son, affichage);
		fclose(fichier);
	}
}

#ifdef FMOD

void init_son()
{	if (FSOUND_GetVersion() < FMOD_VERSION)
	{
		// Display the error message that shows we need the correct version
		printf("Error : You are using the wrong DLL version!  You should be using FMOD %.02f\n", FMOD_VERSION);

		// Quit the program
		exit(1);
	}
	if (!FSOUND_Init(44100, 32, FSOUND_INIT_GLOBALFOCUS))
	{
		// If we couldn't initialize the sound system we print out the error
		printf("%s\n", FMOD_ErrorString(FSOUND_GetError()));

		// Quit the program
		exit(1);
	}
}

int son_lettre(unsigned char key)
{	char SONG_NAME[32]=
	{	'A','.','w','a','v','\0'};
	if(key!=' ')
	SONG_NAME[0]=key;
	else
	SONG_NAME[0]='_';
	if(son_lance)
	return 0;
	pSound = FSOUND_Sample_Load(FSOUND_FREE,SONG_NAME,0,0);
	if (!pSound)
	{
		// Print the error message to the screen
		printf("%s\n", FMOD_ErrorString(FSOUND_GetError()));

		// Quit the program
		exit(1);
	}
	SoundChannel = FSOUND_PlaySound(FSOUND_FREE,pSound);
	son_lance=1;
}

#endif

void dessiner_mains(unsigned char key)
{
	float MatSpec_main[4] =
	{ 1.f, .0f, .0f, .5f };
	float MatDiff_main[4] =
	{ .0f, .0f, .0f, .5f };
	float p1[10][3], p2[10][3], p3[10][3], p4[10][3];
	float paumes[2][3];
	int numero = 0, espace;

	glEnable(GL_BLEND);     //Active la transparence
	glDepthMask(GL_FALSE); //Desactive l'�criture sur le Zbuffer sans desactiver le test de profondeur.

	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MatDiff_main);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, MatSpec_main);
	paumes[0][0] = -7;
	paumes[0][1] = 2;
	paumes[0][2] = 10;
	paumes[1][0] = 7;
	paumes[1][1] = 2;
	paumes[1][2] = 10;

	switch (key)
	{
		case 'A':
			paumes[0][0] = -9.5;
			paumes[0][2] = 7;
			break;
		case 'B':
			paumes[0][0] = -2;
			paumes[0][2] = 13;
			break;
		case 'C':
			paumes[0][0] = -5.5;
			paumes[0][2] = 13.5;
			break;
		case 'D':
			paumes[0][0] = -6.3;
			paumes[0][2] = 11.5;
			break;
		case 'E':
			paumes[0][0] = -7.2;
			paumes[0][2] = 9;
			break;
		case 'F':
			paumes[0][0] = -5.5;
			paumes[0][2] = 10.8;
			break;
		case 'G':
			paumes[0][0] = -3;
			paumes[0][2] = 10.8;
			break;
		case 'H':
			paumes[1][0] = 2.2;
			paumes[1][2] = 10.8;
			break;
		case 'I':
			paumes[1][0] = 5;
			paumes[1][2] = 9;
			break;
		case 'J':
			paumes[1][0] = 4.5;
			paumes[1][2] = 10;
			break;
		case 'K':
			paumes[1][0] = 5.5;
			paumes[1][2] = 11;
			break;
		case 'L':
			paumes[1][0] = 6.5;
			paumes[1][2] = 10.5;
			break;
		case 'M':
			paumes[1][0] = 8;
			paumes[1][2] = 9.5;
			break;
		case 'N':
			paumes[1][0] = 3;
			paumes[1][2] = 13;
			break;
		case 'O':
			paumes[1][0] = 6;
			paumes[1][2] = 8;
			break;
		case 'P':
			paumes[1][0] = 7;
			paumes[1][2] = 7;
			break;
		case 'Q':
			paumes[0][0] = -9;
			paumes[0][2] = 9;
			break;
		case 'R':
			paumes[0][0] = -6.5;
			paumes[0][2] = 7.5;
			break;
		case 'S':
			paumes[0][0] = -7.5;
			paumes[0][2] = 10.5;
			break;
		case 'T':
			paumes[0][0] = -4;
			paumes[0][2] = 8;
			break;
		case 'U':
			paumes[1][0] = 4;
			paumes[1][2] = 7.5;
			break;
		case 'V':
			paumes[0][0] = -4.5;
			paumes[0][2] = 13;
			break;
		case 'W':
			paumes[0][0] = -8;
			paumes[0][2] = 12;
			break;
		case 'X':
			paumes[0][0] = -7;
			paumes[0][2] = 13;
			break;
		case 'Y':
			paumes[1][0] = 1.5;
			paumes[1][2] = 8;
			break;
		case 'Z':
			paumes[0][0] = -8.5;
			paumes[0][2] = 8;
			break;
		case ' ':
			srand(time(0));
			espace = rand() % 2;
			if (espace == 1)
				paumes[0][2] = 12;
			else
				paumes[1][2] = 12;
			break;
	}
	p1[0][0] = paumes[0][0] - 3.25;
	p1[0][1] = paumes[0][1];
	p1[0][2] = paumes[0][2] - 3;
	p2[0][0] = paumes[0][0] - 3.25;
	p2[0][1] = paumes[0][1] + 0.9;
	p2[0][2] = paumes[0][2] - 4.5;
	p3[0][0] = paumes[0][0] - 3.25;
	p3[0][1] = paumes[0][1] + 0.5;
	p3[0][2] = paumes[0][2] - 6;
	p4[0][0] = paumes[0][0] - 3.25;
	p4[0][1] = paumes[0][1] - 0.1;
	p4[0][2] = paumes[0][2] - 7;

	p1[1][0] = paumes[0][0] - 1.75;
	p1[1][1] = paumes[0][1];
	p1[1][2] = paumes[0][2] - 3;
	p2[1][0] = paumes[0][0] - 1.75;
	p2[1][1] = paumes[0][1] + 0.5;
	p2[1][2] = paumes[0][2] - 5.5;
	p3[1][0] = paumes[0][0] - 1.75;
	p3[1][1] = paumes[0][1] + 0.5;
	p3[1][2] = paumes[0][2] - 7.2;
	p4[1][0] = paumes[0][0] - 1.75;
	p4[1][1] = paumes[0][1] - 0.1;
	p4[1][2] = paumes[0][2] - 8.2;

	p1[2][0] = paumes[0][0] - 0.25;
	p1[2][1] = paumes[0][1];
	p1[2][2] = paumes[0][2] - 3;
	p2[2][0] = paumes[0][0] - 0.25;
	p2[2][1] = paumes[0][1] + 0.5;
	p2[2][2] = paumes[0][2] - 6;
	p3[2][0] = paumes[0][0] - 0.25;
	p3[2][1] = paumes[0][1] + 0.5;
	p3[2][2] = paumes[0][2] - 8;
	p4[2][0] = paumes[0][0] - 0.25;
	p4[2][1] = paumes[0][1] - 0.1;
	p4[2][2] = paumes[0][2] - 9;

	p1[3][0] = paumes[0][0] + 1.25;
	p1[3][1] = paumes[0][1];
	p1[3][2] = paumes[0][2] - 3;
	p2[3][0] = paumes[0][0] + 1.25;
	p2[3][1] = paumes[0][1] + 0.5;
	p2[3][2] = paumes[0][2] - 5.5;
	p3[3][0] = paumes[0][0] + 1.25;
	p3[3][1] = paumes[0][1] + 0.5;
	p3[3][2] = paumes[0][2] - 7.2;
	p4[3][0] = paumes[0][0] + 1.25;
	p4[3][1] = paumes[0][1] - 0.1;
	p4[3][2] = paumes[0][2] - 8.2;

	p1[4][0] = paumes[0][0] + 2.5;
	p1[4][1] = paumes[0][1];
	p1[4][2] = paumes[0][2] - 1.5;
	p2[4][0] = paumes[0][0] + 3.2;
	p2[4][1] = paumes[0][1] - 0.1;
	p2[4][2] = paumes[0][2] - 2.5;
	p3[4][0] = paumes[0][0] + 3.3;
	p3[4][1] = paumes[0][1] - 0.1;
	p3[4][2] = paumes[0][2] - 4.5;

	p1[5][0] = paumes[1][0] + 3.25;
	p1[5][1] = paumes[1][1];
	p1[5][2] = paumes[1][2] - 3;
	p2[5][0] = paumes[1][0] + 3.25;
	p2[5][1] = paumes[1][1] + 0.9;
	p2[5][2] = paumes[1][2] - 4.5;
	p3[5][0] = paumes[1][0] + 3.25;
	p3[5][1] = paumes[1][1] + 0.5;
	p3[5][2] = paumes[1][2] - 6;
	p4[5][0] = paumes[1][0] + 3.25;
	p4[5][1] = paumes[1][1] - 0.1;
	p4[5][2] = paumes[1][2] - 7;

	p1[6][0] = paumes[1][0] + 1.75;
	p1[6][1] = paumes[1][1];
	p1[6][2] = paumes[1][2] - 3;
	p2[6][0] = paumes[1][0] + 1.75;
	p2[6][1] = paumes[1][1] + 0.5;
	p2[6][2] = paumes[1][2] - 5.5;
	p3[6][0] = paumes[1][0] + 1.75;
	p3[6][1] = paumes[1][1] + 0.5;
	p3[6][2] = paumes[1][2] - 7.2;
	p4[6][0] = paumes[1][0] + 1.75;
	p4[6][1] = paumes[1][1] - 0.1;
	p4[6][2] = paumes[1][2] - 8.2;

	p1[7][0] = paumes[1][0] + 0.25;
	p1[7][1] = paumes[1][1];
	p1[7][2] = paumes[1][2] - 3;
	p2[7][0] = paumes[1][0] + 0.25;
	p2[7][1] = paumes[1][1] + 0.5;
	p2[7][2] = paumes[1][2] - 6;
	p3[7][0] = paumes[1][0] + 0.25;
	p3[7][1] = paumes[1][1] + 0.5;
	p3[7][2] = paumes[1][2] - 8;
	p4[7][0] = paumes[1][0] + 0.25;
	p4[7][1] = paumes[1][1] - 0.1;
	p4[7][2] = paumes[1][2] - 9;

	p1[8][0] = paumes[1][0] - 1.25;
	p1[8][1] = paumes[1][1];
	p1[8][2] = paumes[1][2] - 3;
	p2[8][0] = paumes[1][0] - 1.25;
	p2[8][1] = paumes[1][1] + 0.5;
	p2[8][2] = paumes[1][2] - 5.5;
	p3[8][0] = paumes[1][0] - 1.25;
	p3[8][1] = paumes[1][1] + 0.5;
	p3[8][2] = paumes[1][2] - 7.2;
	p4[8][0] = paumes[1][0] - 1.25;
	p4[8][1] = paumes[1][1] - 0.1;
	p4[8][2] = paumes[1][2] - 8.2;

	p1[9][0] = paumes[1][0] - 2.5;
	p1[9][1] = paumes[1][1];
	p1[9][2] = paumes[1][2] - 1.5;
	p2[9][0] = paumes[1][0] - 3.2;
	p2[9][1] = paumes[1][1] - 0.1;
	p2[9][2] = paumes[1][2] - 2.5;
	p3[9][0] = paumes[1][0] - 3.3;
	p3[9][1] = paumes[1][1] - 0.1;
	p3[9][2] = paumes[1][2] - 4.5;

	switch (key)
	{
		case 'A':
		case 'Q':
		case 'W':
			p2[0][1] = 2.2;
			p3[0][1] = 1;
			p4[0][1] = 0.2;
			break;
		case 'B':
		case 'F':
		case 'G':
		case 'R':
		case 'T':
		case 'V':
			p2[3][1] = 2.2;
			p3[3][1] = 1;
			p4[3][1] = 0.2;
			break;
		case 'C':
		case 'D':
		case 'E':
			p2[2][1] = 2.2;
			p3[2][1] = 1;
			p4[2][1] = 0.2;
			break;
		case 'H':
		case 'J':
		case 'N':
		case 'U':
		case 'Y':
			p2[8][1] = 2.2;
			p3[8][1] = 1;
			p4[8][1] = 0.2;
			break;
		case 'I':
		case 'K':
			p4[7][1] = 2.2;
			p3[7][1] = 1;
			p4[7][1] = 0.2;
			break;
		case 'L':
		case 'O':
			p2[6][1] = 2.2;
			p3[6][1] = 1;
			p4[6][1] = 0.2;
			break;
		case 'M':
		case 'P':
			p2[5][1] = 2.2;
			p3[5][1] = 1;
			p4[5][1] = 0.2;
			break;
		case 'S':
		case 'X':
		case 'Z':
			p2[1][1] = 2.2;
			p3[1][1] = 1;
			p4[1][1] = 0.2;
			break;
		case ' ':
			if (espace == 1)
			{
				p2[4][1] = -0.5;
				p3[4][1] = -0.8;
			}
			else
			{
				p2[9][1] = -0.5;
				p3[9][1] = -0.8;
			}
			break;
	}
	dessiner_doigts(p1[3], p2[3], p3[3], p4[3], 3, 1);
	for (numero = 0; numero < 10; numero++)
	{
		if (numero != 4 && numero != 9)
		{
			if (numero < 4)
				dessiner_doigts(p1[numero], p2[numero], p3[numero], p4[numero],
						numero, 1);
			else
				dessiner_doigts(p1[numero], p2[numero], p3[numero], p4[numero],
						numero - 5, -1);
		}
		else
		{
			if (numero == 4)
				dessiner_pouce(p1[4], p2[4], p3[4], 1);
			else
				dessiner_pouce(p1[9], p2[9], p3[9], -1);
		}
	}
	dessiner_paumes(paumes[0][0], paumes[0][1], paumes[0][2], 1);
	dessiner_paumes(paumes[1][0], paumes[1][1], paumes[1][2], -1);

	glDepthMask(GL_TRUE);   //Active l'�criture sur le Zbuffer
	glDisable(GL_BLEND);    //Desactive la transparence
}

void dessiner_paumes(float x, float y, float z, int sens)
{
	glBegin(GL_QUAD_STRIP);

	glNormal3f(0.22 * sens, -0.87, 0.44);
	glVertex3f(2 * sens + x, -0.5 + y, 3 + z);    //Face du dessous
	glNormal3f(-0.22 * sens, -0.87, 0.44);
	glVertex3f(-2 * sens + x, -0.5 + y, 3 + z);
	glNormal3f(0.707 * sens, -0.707, 0);
	glVertex3f(3 * sens + x, -0.75 + y, z);
	glNormal3f(-0.95 * sens, 0, 0.32);
	glVertex3f(-3 * sens + x, -0.75 + y, z);
	glNormal3f(0.26 * sens, -0.79, -0.55);
	glVertex3f(2 * sens + x, -1 + y, -3 + z);
	glNormal3f(-0.31 * sens, -0.94, -0.13);
	glVertex3f(-4 * sens + x, -1 + y, -3 + z);

	glEnd();

	glBegin(GL_QUADS);

	glNormal3f(0.707 * sens, 0.707, 0);
	glVertex3f(3 * sens + x, 0.75 + y, z);      //Face de droite Arri�re
	glNormal3f(0.707 * sens, -0.707, 0);
	glVertex3f(3 * sens + x, -0.75 + y, z);
	glNormal3f(0.22 * sens, -0.87, 0.44);
	glVertex3f(2 * sens + x, -0.5 + y, 3 + z);
	glNormal3f(0.22 * sens, 0.87, 0.44);
	glVertex3f(2 * sens + x, 0.5 + y, 3 + z);

	glNormal3f(-0.31 * sens, 0.94, -0.13);
	glVertex3f(-4 * sens + x, 1 + y, -3 + z);     //Face de gauche
	glNormal3f(-0.31 * sens, -0.94, -0.13);
	glVertex3f(-4 * sens + x, -1 + y, -3 + z);
	glNormal3f(-0.22 * sens, -0.87, 0.44);
	glVertex3f(-2 * sens + x, -0.5 + y, 3 + z);
	glNormal3f(-0.22 * sens, 0.87, 0.44);
	glVertex3f(-2 * sens + x, 0.5 + y, 3 + z);

	glEnd();

	glBegin(GL_QUAD_STRIP);

	glNormal3f(0.22 * sens, 0.87, 0.44);
	glVertex3f(2 * sens + x, 0.5 + y, 3 + z);    //Face du dessus
	glNormal3f(-0.22 * sens, 0.87, 0.44);
	glVertex3f(-2 * sens + x, 0.5 + y, 3 + z);
	glNormal3f(0.707 * sens, 0.707, 0);
	glVertex3f(3 * sens + x, 0.75 + y, z);
	glNormal3f(-0.95 * sens, 0, 0.32);
	glVertex3f(-3 * sens + x, 0.75 + y, z);
	glNormal3f(0.26 * sens, 0.79, -0.55);
	glVertex3f(2 * sens + x, 1 + y, -3 + z);
	glNormal3f(-0.31 * sens, 0.94, -0.13);
	glVertex3f(-4 * sens + x, 1 + y, -3 + z);

	glEnd();

	glBegin(GL_QUADS);

	glNormal3f(0.22 * sens, 0.87, 0.44);
	glVertex3f(2 * sens + x, 0.5 + y, 3 + z);     //Face de derri�re
	glNormal3f(0.22 * sens, -0.87, 0.44);
	glVertex3f(2 * sens + x, -0.5 + y, 3 + z);
	glNormal3f(-0.22 * sens, -0.87, 0.44);
	glVertex3f(-2 * sens + x, -0.5 + y, 3 + z);
	glNormal3f(-0.22 * sens, 0.87, 0.44);
	glVertex3f(-2 * sens + x, 0.5 + y, 3 + z);

	glEnd();
}

void dessiner_pouce(float p1[3], float p2[3], float p3[3], int sens)
{
	glBegin(GL_QUADS);
	glNormal3f(-0.577 * sens, -0.577, -0.577);
	glVertex3f(p3[0] - 0.55 * sens, p3[1] - 0.65, p3[2]); //Doigt Avant face avant
	glNormal3f(0.577 * sens, -0.577, -0.577);
	glVertex3f(p3[0] + 0.55 * sens, p3[1] - 0.65, p3[2]);
	glNormal3f(0.577 * sens, 0.577, -0.577);
	glVertex3f(p3[0] + 0.55 * sens, p3[1] + 0.65, p3[2]);
	glNormal3f(-0.577 * sens, 0.577, -0.577);
	glVertex3f(p3[0] - 0.55 * sens, p3[1] + 0.65, p3[2]);

	glEnd();

	//FACE DE GAUCHE

	glBegin(GL_QUAD_STRIP);
	glNormal3f(-0.577 * sens, -0.577, -0.577);
	glVertex3f(p3[0] - 0.55 * sens, p3[1] - 0.65, p3[2]); //Doigt face de gauche
	glNormal3f(-0.577 * sens, 0.577, -0.577);
	glVertex3f(p3[0] - 0.55 * sens, p3[1] + 0.65, p3[2]);
	glNormal3f(-0.707 * sens, -0.707, 0);
	glVertex3f(p2[0] - 0.65 * sens, p2[1] - 0.65, p2[2]);
	glNormal3f(-0.707 * sens, 0.707, 0);
	glVertex3f(p2[0] - 0.65 * sens, p2[1] + 0.65, p2[2]);
	glNormal3f(-0.707 * sens, -0.707, 0);
	glVertex3f(p1[0] - 0.5 * sens, p1[1] - 1, p1[2] - 1.5); //Doigt Arri�re face de gauche
	glNormal3f(-0.707 * sens, +0.707, 0);
	glVertex3f(p1[0] - 0.5 * sens, p1[1] + 1, p1[2] - 1.5);

	glEnd();

	//FACE DU DESSOUS

	glBegin(GL_QUAD_STRIP);
	glNormal3f(-0.577 * sens, -0.577, -0.577);
	glVertex3f(p3[0] - 0.55 * sens, p3[1] - 0.65, p3[2]);
	glNormal3f(0.577 * sens, -0.577, -0.577);
	glVertex3f(p3[0] + 0.55 * sens, p3[1] - 0.65, p3[2]);
	glNormal3f(-0.707 * sens, -0.707, 0);
	glVertex3f(p2[0] - 0.65 * sens, p2[1] - 0.65, p2[2]); //Doigt Arri�re face du dessous
	glNormal3f(0.707 * sens, -0.707, 0);
	glVertex3f(p2[0] + 0.65 * sens, p2[1] - 0.65, p2[2]);
	glNormal3f(-0.707 * sens, -0.707, 0);
	glVertex3f(p1[0] - 0.5 * sens, p1[1] - 1, p1[2] - 1.5);
	glNormal3f(0.707 * sens, -0.707, 0);
	glVertex3f(p1[0] + 0.5 * sens, p1[1] - 0.75, p1[2] + 1.5);

	glEnd();

	//FACE DE DROITE

	glBegin(GL_QUAD_STRIP);
	glNormal3f(0.577 * sens, -0.577, -0.577);
	glVertex3f(p3[0] + 0.55 * sens, p3[1] - 0.65, p3[2]); //Doigt face de droite
	glNormal3f(0.577 * sens, 0.577, -0.577);
	glVertex3f(p3[0] + 0.55 * sens, p3[1] + 0.65, p3[2]);
	glNormal3f(0.707 * sens, -0.707, 0);
	glVertex3f(p2[0] + 0.65 * sens, p2[1] - 0.65, p2[2]); //Doigt Arri�re face de droite
	glNormal3f(0.707 * sens, 0.707, 0);
	glVertex3f(p2[0] + 0.65 * sens, p2[1] + 0.65, p2[2]);
	glNormal3f(0.707 * sens, -0.707, 0);
	glVertex3f(p1[0] + 0.5 * sens, p1[1] - 0.75, p1[2] + 1.5);
	glNormal3f(0.707 * sens, 0.707, 0);
	glVertex3f(p1[0] + 0.5 * sens, p1[1] + 0.75, p1[2] + 1.5);

	glEnd();

	//FACE DU DESSUS

	glBegin(GL_QUAD_STRIP);
	glNormal3f(-0.577 * sens, 0.577, -0.577);
	glVertex3f(p3[0] - 0.55 * sens, p3[1] + 0.65, p3[2]); //Doigt face du dessus
	glNormal3f(0.577 * sens, 0.577, -0.577);
	glVertex3f(p3[0] + 0.55 * sens, p3[1] + 0.65, p3[2]);
	glNormal3f(-0.707 * sens, 0.707, 0);
	glVertex3f(p2[0] - 0.65 * sens, p2[1] + 0.65, p2[2]);
	glNormal3f(0.707 * sens, 0.707, 0);
	glVertex3f(p2[0] + 0.65 * sens, p2[1] + 0.65, p2[2]); //Doigt Arri�re face du dessus
	glNormal3f(-0.707 * sens, +0.707, 0);
	glVertex3f(p1[0] - 0.5 * sens, p1[1] + 1, p1[2] - 1.5);
	glNormal3f(0.707 * sens, 0.707, 0);
	glVertex3f(p1[0] + 0.5 * sens, p1[1] + 0.75, p1[2] + 1.5);

	glEnd();
}

void dessiner_doigts(float p1[3], float p2[3], float p3[3], float p4[3],
		int numero, int sens)
{
	float largeur[5] =
	{ 0.5, 0.6, 0.6, 0.6 }, v1[3], v2[3];

	glBegin(GL_QUADS);
	glNormal3f(-0.577 * sens, -0.577, -0.577);
	glVertex3f(p4[0] - largeur[numero] * sens, p4[1] - largeur[numero], p4[2]); //Doigt Avant face avant
	glNormal3f(0.577 * sens, -0.577, -0.577);
	glVertex3f(p4[0] + largeur[numero] * sens, p4[1] - largeur[numero], p4[2]);
	glNormal3f(0.577 * sens, 0.577, -0.577);
	glVertex3f(p4[0] + largeur[numero] * sens, p4[1] + largeur[numero], p4[2]);
	glNormal3f(-0.577 * sens, 0.577, -0.577);
	glVertex3f(p4[0] - largeur[numero] * sens, p4[1] + largeur[numero], p4[2]);

	glEnd();

	//FACE DE GAUCHE

	glBegin(GL_QUAD_STRIP);
	glNormal3f(-0.577 * sens, -0.577, -0.577);
	glVertex3f(p4[0] - largeur[numero] * sens, p4[1] - largeur[numero], p4[2]); //Doigt Avant face de gauche
	glNormal3f(-0.577 * sens, 0.577, -0.577);
	glVertex3f(p4[0] - largeur[numero] * sens, p4[1] + largeur[numero], p4[2]);
	glNormal3f(-0.707 * sens, -0.707, 0);
	glVertex3f(p3[0] - largeur[numero] * sens, p3[1] - largeur[numero], p3[2]); //Doigt face de gauche
	glNormal3f(-0.707 * sens, 0.707, 0);
	glVertex3f(p3[0] - largeur[numero] * sens, p3[1] + largeur[numero], p3[2]);
	glNormal3f(-0.707 * sens, -0.707, 0);
	glVertex3f(p2[0] - largeur[numero] * sens, p2[1] - largeur[numero], p2[2]);
	glNormal3f(-0.707 * sens, 0.707, 0);
	glVertex3f(p2[0] - largeur[numero] * sens, p2[1] + largeur[numero], p2[2]);
	glNormal3f(-0.707 * sens, -0.707, 0);
	glVertex3f(p1[0] - 0.75 * sens, p1[1] - 1, p1[2]); //Doigt Arri�re face de gauche
	glNormal3f(-0.707 * sens, 0.707, 0);
	glVertex3f(p1[0] - 0.75 * sens, p1[1] + 1, p1[2]);

	glEnd();

	//FACE DU DESSOUS

	glBegin(GL_QUAD_STRIP);
	glNormal3f(-0.577 * sens, -0.577, -0.577);
	glVertex3f(p4[0] - largeur[numero] * sens, p4[1] - largeur[numero], p4[2]); //Doigt Avant face du dessous
	glNormal3f(0.577 * sens, -0.577, -0.577);
	glVertex3f(p4[0] + largeur[numero] * sens, p4[1] - largeur[numero], p4[2]);
	glNormal3f(-0.707 * sens, -0.707, 0);
	glVertex3f(p3[0] - largeur[numero] * sens, p3[1] - largeur[numero], p3[2]);
	glNormal3f(0.707 * sens, -0.707, 0);
	glVertex3f(p3[0] + largeur[numero] * sens, p3[1] - largeur[numero], p3[2]);
	glNormal3f(-0.707 * sens, -0.707, 0);
	glVertex3f(p2[0] - largeur[numero] * sens, p2[1] - largeur[numero], p2[2]); //Doigt Arri�re face du dessous
	glNormal3f(0.707 * sens, -0.707, 0);
	glVertex3f(p2[0] + largeur[numero] * sens, p2[1] - largeur[numero], p2[2]);
	glNormal3f(-0.707 * sens, -0.707, 0);
	glVertex3f(p1[0] - 0.75 * sens, p1[1] - 1, p1[2]);
	glNormal3f(0.707 * sens, -0.707, 0);
	glVertex3f(p1[0] + 0.75 * sens, p1[1] - 1, p1[2]);

	glEnd();

	//FACE DE DROITE

	glBegin(GL_QUAD_STRIP);
	glNormal3f(0.577 * sens, -0.577, -0.577);
	glVertex3f(p4[0] + largeur[numero] * sens, p4[1] - largeur[numero], p4[2]); //Doigt Avant face de droite
	glNormal3f(0.577 * sens, 0.577, -0.577);
	glVertex3f(p4[0] + largeur[numero] * sens, p4[1] + largeur[numero], p4[2]);
	glNormal3f(0.707 * sens, -0.707, 0);
	glVertex3f(p3[0] + largeur[numero] * sens, p3[1] - largeur[numero], p3[2]); //Doigt face de droite
	glNormal3f(0.707 * sens, 0.707, 0);
	glVertex3f(p3[0] + largeur[numero] * sens, p3[1] + largeur[numero], p3[2]);
	glNormal3f(0.707 * sens, -0.707, 0);
	glVertex3f(p2[0] + largeur[numero] * sens, p2[1] - largeur[numero], p2[2]); //Doigt Arri�re face de droite
	glNormal3f(0.707 * sens, 0.707, 0);
	glVertex3f(p2[0] + largeur[numero] * sens, p2[1] + largeur[numero], p2[2]);
	glNormal3f(0.707 * sens, -0.707, 0);
	glVertex3f(p1[0] + 0.75 * sens, p1[1] - 1, p1[2]);
	glNormal3f(0.707 * sens, 0.707, 0);
	glVertex3f(p1[0] + 0.75 * sens, p1[1] + 1, p1[2]);

	glEnd();

	//FACE DU DESSUS

	glBegin(GL_QUAD_STRIP);
	glNormal3f(-0.577 * sens, 0.577, -0.577);
	glVertex3f(p4[0] - largeur[numero] * sens, p4[1] + largeur[numero], p4[2]); //Doigt Avant face du dessus
	glNormal3f(0.577 * sens, 0.577, -0.577);
	glVertex3f(p4[0] + largeur[numero] * sens, p4[1] + largeur[numero], p4[2]);
	glNormal3f(-0.707 * sens, 0.707, 0);
	glVertex3f(p3[0] - largeur[numero] * sens, p3[1] + largeur[numero], p3[2]); //Doigt face du dessus
	glNormal3f(0.707 * sens, 0.707, 0);
	glVertex3f(p3[0] + largeur[numero] * sens, p3[1] + largeur[numero], p3[2]);
	glNormal3f(-0.707 * sens, 0.707, 0);
	glVertex3f(p2[0] - largeur[numero] * sens, p2[1] + largeur[numero], p2[2]);
	glNormal3f(0.707 * sens, 0.707, 0);
	glVertex3f(p2[0] + largeur[numero] * sens, p2[1] + largeur[numero], p2[2]); //Doigt Arri�re face du dessus
	glNormal3f(-0.707 * sens, 0.707, 0);
	glVertex3f(p1[0] - 0.75 * sens, p1[1] + 1, p1[2]);
	glNormal3f(0.707 * sens, 0.707, 0);
	glVertex3f(p1[0] + 0.75 * sens, p1[1] + 1, p1[2]);

	glEnd();
}

void output(float x, float y, char *string)
{
	int len, i;
	glColor3f(0, 0, 1);
	glRasterPos2f(x, y);
	len = (int) strlen(string);
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string[i]);
	}
}

void output_texte(GLfloat x, GLfloat y, char *p, int numero, int taille,
		int cmpt)
{
	float MatSpec[4] =
	{ 1.f, .0f, .0f, 1.f };
	float MatDiff[4] =
	{ .0f, .0f, .0f, 1.f };
	glPushMatrix();
	glRotatef(-10, 1, 0, 0);
	glTranslatef(x + 0.2 * (1. - ((float) glutStrokeWidth(
	GLUT_STROKE_ROMAN, *p)) / (glutStrokeWidth(
	GLUT_STROKE_ROMAN, 'W'))), y, 0);
	if (taille)
		glScalef(.02f, .02f, .02f);
	else
		glScalef(0.005F, 0.005F, 0.005F);

	if (*p)
	{
		MatSpec[0] = (float) couleur_texte[numero][cmpt][0];
		MatSpec[1] = (float) couleur_texte[numero][cmpt][1];
		MatSpec[2] = (float) couleur_texte[numero][cmpt++][2];
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, MatSpec);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MatDiff);
		if (*p >= 'A' && *p <= 'Z')
			glutStrokeCharacter(GLUT_STROKE_ROMAN, *(p++));
		else
		{
			glutStrokeCharacter(GLUT_STROKE_ROMAN, ' ');
			p++;
		}
	}
	glPopMatrix();
	if (*p)
		output_texte(x + 0.45, y, p, numero, taille, cmpt);
}

void init_alphabet(void)
{
	int cmpt, cmpt2, ascii;
	unsigned char val;
	char nom[6] =
	{ 'A', '.', 'l', 'e', 't', '\0' };
	FILE *fichier;
	for (cmpt = 'A'; cmpt <= 'Z'; cmpt++)
	{
		nom[0] = cmpt;
		fichier = fopen(nom, "r");
		for (cmpt2 = 0; cmpt2 < 256; cmpt2++)
		{
			ascii = getc(fichier);
			if (ascii < '0' || ascii > '9')
				ascii = getc(fichier);
			switch (ascii)
			{
				case '1':
					val = 0;
					break;
				case '0':
					val = 0xC0;
					break;
			}
			Texture[cmpt - 'A'][4 * cmpt2] = val;
			Texture[cmpt - 'A'][4 * cmpt2 + 1] = val;
			Texture[cmpt - 'A'][4 * cmpt2 + 2] = val;
			Texture[cmpt - 'A'][4 * cmpt2 + 3] = 0xFF;
		}
		fclose(fichier);
	}
	for (cmpt = 0; cmpt < 1024; cmpt++)
	{
		if (cmpt && !((cmpt + 1) % 4))
			Texture[26][cmpt] = 0xFF;
		else
			Texture[26][cmpt] = 0xC0;
	}
}

void Reshape(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (float) width / (float) height, 0.01, 50);
	glMatrixMode(GL_MODELVIEW);
}

void sortir()
{
#ifdef FMOD
	FSOUND_Close();
#endif
	exit(0);
}

void menuPrincipal(int value)
{
	int cmpt, cmpt2, ref, ascii, caract;
	char modes[3][24];
	strcpy(modes[0], "Mode 1 : Aleatoire.");
	strcpy(modes[1], "Mode 2 : Fichier Texte.");
	strcpy(modes[2], "Mode 3 : Frappe Libre.");
	if (value == 3)
	{
		sortir();
	}
	if (value != mode)
	{
		switch (value)
		{
			case 1:
				cmpt = 0;
				cmpt2 = 0;
				numero_texte_actuel = 0;
				position_texte_actuel = 0;
				texte_demande[0][0] = '\0';
				texte_demande[1][0] = '\0';
				texte_demande[2][0] = '\0';
				if ((fichier_entree = fopen("texte.txt", "r")) == NULL)
					value = 2;
				else
				{
					fseek(fichier_entree, offset_ouverture, SEEK_SET);
					do
					{
						caract = getc(fichier_entree);
						offset_ouverture++;
						ascii = caract;
						if (ascii >= 'a' && ascii <= 'z')
							ascii += 'A' - 'a';
						if (ascii < 'A' || ascii > 'Z')
							ascii = ' ';
						if (cmpt == 0 && ascii != ' ' && cmpt2)
						{
							if (texte_demande[cmpt2 - 1][34] != ' ')
							{
								while (texte_demande[cmpt2 - 1][35 - cmpt]
										!= ' ')
									cmpt++;
								ref = cmpt;
								for (cmpt = 0; cmpt < ref; cmpt++)
								{
									texte_demande[cmpt2][cmpt] =
											texte_demande[cmpt2 - 1][35 - ref
													+ cmpt];
									couleur_texte[cmpt2][cmpt][0] = 1;
									couleur_texte[cmpt][cmpt][1] = 0;
									couleur_texte[cmpt2][cmpt][2] = 1;
								}
								texte_demande[cmpt2 - 1][35 - ref] = '\0';
							}
						}
						if (!cmpt
								|| (texte_demande[cmpt2][cmpt - 1] != ' '
										|| ascii != ' '))
						{
							texte_demande[cmpt2][cmpt] = ascii;
							couleur_texte[cmpt2][cmpt][0] = 1;
							couleur_texte[cmpt2][cmpt][1] = 0;
							couleur_texte[cmpt2][cmpt++][2] = 1;
							texte_demande[cmpt2][cmpt] = '\0';
						}
						if (cmpt == 35)
						{
							cmpt = 0;
							cmpt2++;
						}
					}
					while (!(cmpt2 == 3 || caract == EOF));
					if (caract == EOF)
					{
						file_end = 1;
						offset_ouverture = 0;
					}
					else
					{
						if ((caract >= 'a' && caract <= 'z')
								|| (caract >= 'A' && caract <= 'Z'))
						{
							ascii = getc(fichier_entree);
							if ((ascii >= 'a' && ascii <= 'z')
									|| (ascii >= 'A' && ascii <= 'Z'))
							{
								cmpt = 34;
								do
								{
									cmpt--;
									offset_ouverture--;
								}
								while ((texte_demande[2][cmpt] >= 'a'
										&& texte_demande[2][cmpt] <= 'z')
										|| (texte_demande[2][cmpt] >= 'A'
												&& texte_demande[2][cmpt] <= 'Z'));
								texte_demande[2][cmpt] = '\0';
							}
						}
					}
					fclose(fichier_entree);
				}
			case 0:
			case 2:
				for (cmpt = 0; cmpt < 2; cmpt++)
					for (cmpt2 = 0; cmpt2 < 27; cmpt2++)
						positions_y[cmpt][cmpt2] = 0;
				if (value != 2)
					demande = 1;
				else
					demande = 0;
				mode = value;
				if (mode != 1)
				{
					texte_demande[0][0] = '\0';
					texte_demande[1][0] = '\0';
					texte_demande[2][0] = '\0';
				}
				strcpy(affichage_texte, modes[value]);
				temps1 = clock();
				break;
			case 4:
				if (son)
				{
					son = 0;
					glutChangeToMenuEntry(4, "Activer Son", 4);
					strcpy(affichage_texte, "Son Desactive");
					temps1 = clock();
				}
				else
				{
					son = 1;
					glutChangeToMenuEntry(4, "Desactiver Son", 4);
					strcpy(affichage_texte, "Son Active");
					temps1 = clock();
				}
				break;
			case 5:
				if (affichage)
				{
					affichage = 0;
					glutChangeToMenuEntry(5, "Activer Affichage", 5);
					strcpy(affichage_texte, "Affichage Desactive");
					temps1 = clock();
				}
				else
				{
					affichage = 1;
					glutChangeToMenuEntry(5, "Desactiver Affichage", 5);
					strcpy(affichage_texte, "Affichage Active");
					temps1 = clock();
				}
				break;
			case 6:
				save_config();
				strcpy(affichage_texte, "Config Sauvee");
				temps1 = clock();
				break;
		}
	}
}

void InitGL(void)
{
	int cmpt;
	GLfloat position[4] =
	{ 0, 25, 15, 1 };
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glClearColor(0, 0, 0, 0); 	//Change la couleur du fond
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);   //Mode de transparence
	glEnable(GL_DEPTH_TEST); 	//Active le depth test
	glEnable(GL_TEXTURE_2D); 	//Active le texturing
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glGenTextures(27, Texts);

	glutCreateMenu(menuPrincipal);
	glutAddMenuEntry("Mode 0 : Al�atoire", 0);
	glutAddMenuEntry("Mode 1 : Lecture du fichier texte.txt", 1);
	glutAddMenuEntry("Mode 2 : Frappe libre", 2);
	if (son)
		glutAddMenuEntry("Desactiver Son", 4);
	else
		glutAddMenuEntry("Activer Son", 4);
	if (affichage)
		glutAddMenuEntry("Desactiver Affichage", 5);
	else
		glutAddMenuEntry("Activer Affichage", 5);
	glutAddMenuEntry("Sauvegarder Preferences", 6);
	glutAddMenuEntry("Quitter", 3);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

}

void Draw()
{
	int cmpt, touche_frappee = 0;
	float MatSpec_touches[4] =
	{ 1.f, 1.f, 1.f, 1.f };
	float MatSpec_touches_2[4] =
	{ .1f, .1f, 1.f, 1.f };
	float MatSpec_touches_3[4] =
	{ 1.f, .1f, .1f, 1.f };
	float MatSpec_touches_4[4] =
	{ .1f, 1.f, .1f, 1.f };
	float MatDiff_touches[4] =
	{ .1f, .1f, .1f, 1.f };

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 25, 10, 0, 0, 2, 0, 1, 0);
	for (cmpt = 0; cmpt < 26; cmpt++)
	{
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MatDiff_touches);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, MatSpec_touches);
		if (positions_y[1][liste_touches[cmpt] - 'A']
				&& positions_y[0][liste_touches[cmpt] - 'A'] != -1)
		{
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, MatSpec_touches_2);
		}
		if (positions_y[1][liste_touches[cmpt] - 'A']
				&& positions_y[0][liste_touches[cmpt] - 'A'] == -1)
		{
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, MatSpec_touches_4);
			touche_frappee = liste_touches[cmpt];
#ifdef FMOD
			if(son)
			son_lettre(liste_touches[cmpt]);
#endif
		}
		if ((positions_y[0][liste_touches[cmpt] - 'A'] == -1)
				&& (positions_y[1][liste_touches[cmpt] - 'A'] != 1))
		{
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, MatSpec_touches_3);
		}
		dessiner_touche(liste_touches[cmpt],
				((cmpt) % 10 - 5) * 2.6 + (cmpt / 10) * (2.6 / 3),
				positions_y[0][liste_touches[cmpt] - 'A'], ((cmpt) / 10) * 2.6);
	}
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MatDiff_touches);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, MatSpec_touches);
	if (positions_y[1][26] && positions_y[0][26] != -1)
	{
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, MatSpec_touches_2);
	}
	if (positions_y[1][26] && positions_y[0][26] == -1)
	{
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, MatSpec_touches_4);
		touche_frappee = ' ';
#ifdef FMOD
		if(son)
		son_lettre(' ');
#endif
	}
	if ((positions_y[0][26] == -1) && (positions_y[1][26] != 1))
	{
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, MatSpec_touches_3);
	}
	dessiner_touche(liste_touches[cmpt],
			((cmpt) % 10 - 5) * 2.6 + (cmpt / 10) * (2.6 / 3),
			positions_y[0][26], ((cmpt) / 10) * 2.6);

	dessiner_mains(touche_frappee);

	output(-8.2, 12.6, affichage_texte);
	if (affichage)
	{
		if (mode)
		{
			if (texte_demande[0][0] != ' ')
				output_texte(-8.5, 10, texte_demande[0], 0, 0, 0);
			else
				output_texte(-9, 10, texte_demande[0], 0, 0, 0);
			if (texte_demande[1][0] != ' ')
				output_texte(-8.5, 9, texte_demande[1], 1, 0, 0);
			else
				output_texte(-9, 9, texte_demande[1], 1, 0, 0);
			if (texte_demande[2][0] != ' ')
				output_texte(-8.5, 8, texte_demande[2], 2, 0, 0);
			else
				output_texte(-9, 8, texte_demande[2], 2, 0, 0);
		}
		else
			output_texte(-1, 7, texte_demande[0], 0, 1, 0);
	}

	glutSwapBuffers();

}

void idle()
{
	int ascii, ref, ref2, ref3;
	temps2 = clock();
#ifdef FMOD
	if(son_lance && !FSOUND_GetChannelsPlaying())
	{	FSOUND_StopSound(SoundChannel);
		son_lance=0;
	}
#endif
	if (strcmp(affichage_texte, "") && (temps2 - temps1) >= 0.5 * CLOCKS_PER_SEC)
		strcpy(affichage_texte, "");
	if (demande)
	{
		if (mode == 0)
		{
			srand((unsigned int) time(0) % rand());
			ascii = rand() % 27;
			numero_texte_actuel = 0;
			position_texte_actuel = 0;
			texte_demande[0][0] = liste_touches[ascii];
			texte_demande[0][1] = '\0';
		}
		if (mode == 1)
		{
			ascii = texte_demande[numero_texte_actuel][position_texte_actuel];
			ref = ascii;
			ascii = 0;
			while (liste_touches[ascii] != ref && ascii < 27)
				ascii++;
		}
		if (ascii < 26)
			positions_y[1][liste_touches[ascii] - 'A'] = 1;
		else
			positions_y[1][26] = 1;
		demande = 0;
		couleur_texte[numero_texte_actuel][position_texte_actuel][0] = 0;
		couleur_texte[numero_texte_actuel][position_texte_actuel][1] = 0;
		couleur_texte[numero_texte_actuel][position_texte_actuel++][2] = 1;
		if (ref == '\0' && position_texte_actuel < 35)
			menuPrincipal(2);
		if (texte_demande[numero_texte_actuel][position_texte_actuel] == '\0')
		{
			position_texte_actuel = 0;
			numero_texte_actuel++;
		}
	}
	glutPostRedisplay();
}

void Touches(unsigned char key, int x, int y)
{
	int ref[3] =
	{ strlen(texte_demande[0]), strlen(texte_demande[1]), strlen(
			texte_demande[2]) };
	if (key == 27)
	{
		sortir();
	}
	if (key >= 'a' && key <= 'z')
		key += 'A' - 'a';
	if (key == ' ')
		key = 'A' + 26;
	if (key >= 'A' && key <= 'A' + 26)
	{
		positions_y[0][key - 'A'] = -1;
		if (mode == 2)
		{
			positions_y[1][key - 'A'] = 1;
			if (ref[0] == 35)
			{
				if (ref[1] == 35)
				{
					if (ref[2] != 35)
					{
						texte_demande[2][ref[2] + 1] = '\0';
						texte_demande[2][ref[2]] = key;
					}
				}
				else
				{
					texte_demande[1][ref[1] + 1] = '\0';
					texte_demande[1][ref[1]] = key;
				}
			}
			else
			{
				texte_demande[0][ref[0] + 1] = '\0';
				texte_demande[0][ref[0]] = key;
			}
		}
	}
	if (mode == 2)
	{
		if (ref[0] == 35)
		{
			if (ref[1] == 35)
			{
				if (ref[2] != 35)
				{
					couleur_texte[2][ref[2]][0] = 0;
					couleur_texte[2][ref[2]][1] = 1;
					couleur_texte[2][ref[2]][2] = 0;
					if (ref[2])
					{
						couleur_texte[2][ref[2] - 1][0] = 0;
						couleur_texte[2][ref[2] - 1][1] = 0;
						couleur_texte[2][ref[2] - 1][2] = 1;
					}
					else
					{
						couleur_texte[1][34][0] = 0;
						couleur_texte[1][34][1] = 0;
						couleur_texte[1][34][2] = 1;
					}
				}
			}
			else
			{
				couleur_texte[1][ref[1]][0] = 0;
				couleur_texte[1][ref[1]][1] = 1;
				couleur_texte[1][ref[1]][2] = 0;
				if (ref[1])
				{
					couleur_texte[1][ref[1] - 1][0] = 0;
					couleur_texte[1][ref[1] - 1][1] = 0;
					couleur_texte[1][ref[1] - 1][2] = 1;
				}
				else
				{
					couleur_texte[0][34][0] = 0;
					couleur_texte[0][34][1] = 0;
					couleur_texte[0][34][2] = 1;
				}
			}
		}
		else
		{
			couleur_texte[0][ref[0]][0] = 0;
			couleur_texte[0][ref[0]][1] = 1;
			couleur_texte[0][ref[0]][2] = 0;
			if (ref[0])
			{
				couleur_texte[0][ref[0] - 1][0] = 0;
				couleur_texte[0][ref[0] - 1][1] = 0;
				couleur_texte[0][ref[0] - 1][2] = 1;
			}
		}
	}
	else
	{
		if (!mode)
		{
			if (positions_y[1][key - 'A'] == 1)
			{
				if (ref[0] == 35)
				{
					if (ref[1] == 35)
					{
						if (ref[2] != 35)
						{
							couleur_texte[2][ref[2] - 1][0] = 0;
							couleur_texte[2][ref[2] - 1][1] = 1;
							couleur_texte[2][ref[2] - 1][2] = 0;
						}
					}
					else
					{
						couleur_texte[1][ref[1] - 1][0] = 0;
						couleur_texte[1][ref[1] - 1][1] = 1;
						couleur_texte[1][ref[1] - 1][2] = 0;
					}
				}
				else
				{
					couleur_texte[0][ref[0] - 1][0] = 0;
					couleur_texte[0][ref[0] - 1][1] = 1;
					couleur_texte[0][ref[0] - 1][2] = 0;
				}
			}
		}
		else
		{
			if (positions_y[1][key - 'A'] == 1)
			{
				if (position_texte_actuel)
				{
					couleur_texte[numero_texte_actuel][position_texte_actuel - 1][0] =
							0;
					couleur_texte[numero_texte_actuel][position_texte_actuel - 1][1] =
							1;
					couleur_texte[numero_texte_actuel][position_texte_actuel - 1][2] =
							0;
				}
				else
				{
					couleur_texte[numero_texte_actuel - 1][ref[numero_texte_actuel
							- 1] - 1][0] = 0;
					couleur_texte[numero_texte_actuel - 1][ref[numero_texte_actuel
							- 1] - 1][1] = 1;
					couleur_texte[numero_texte_actuel - 1][ref[numero_texte_actuel
							- 1] - 1][2] = 0;
				}
			}
		}
	}
}

void Touches_spe(int key, int x, int y)
{
	char val[10];
	switch (key)
	{
		case GLUT_KEY_F1:
			menuPrincipal(0);
			break;
		case GLUT_KEY_F2:
			menuPrincipal(1);
			break;
		case GLUT_KEY_F3:
			menuPrincipal(2);
			break;
		case GLUT_KEY_F4:
			menuPrincipal(4);
			break;
		case GLUT_KEY_F5:
			menuPrincipal(5);
			break;
		case GLUT_KEY_F6:
			menuPrincipal(6);
			break;
	}
}

void Relache_Touches(unsigned char key, int x, int y)
{
	int ref[3] =
	{ strlen(texte_demande[0]), strlen(texte_demande[1]), strlen(
			texte_demande[2]) };
	if (key >= 'a' && key <= 'z')
		key += 'A' - 'a';
	if (key == ' ')
		key = 'A' + 26;

	if (positions_y[1][key - 'A'] == 1)
	{
		if (mode == 1)
		{
			if (position_texte_actuel)
			{
				couleur_texte[numero_texte_actuel][position_texte_actuel - 1][0] =
						1;
				couleur_texte[numero_texte_actuel][position_texte_actuel - 1][1] =
						0;
				couleur_texte[numero_texte_actuel][position_texte_actuel - 1][2] =
						1;
			}
			else
			{
				couleur_texte[numero_texte_actuel - 1][ref[numero_texte_actuel
						- 1] - 1][0] = 1;
				couleur_texte[numero_texte_actuel - 1][ref[numero_texte_actuel
						- 1] - 1][1] = 0;
				couleur_texte[numero_texte_actuel - 1][ref[numero_texte_actuel
						- 1] - 1][2] = 1;
			}
			if (numero_texte_actuel == 3)
			{
				if (file_end)
					menuPrincipal(2);
				else
				{
					mode = 2;
					menuPrincipal(1);
				}
			}
		}
		else
		{
			if (ref[0] == 35)
			{
				if (ref[1] == 35)
				{
					if (ref[2] != 35)
					{
						couleur_texte[2][ref[2] - 1][0] = 0;
						couleur_texte[2][ref[2] - 1][1] = 0;
						couleur_texte[2][ref[2] - 1][2] = 1;
					}
					else
					{
						strcpy(texte_demande[0], texte_demande[1]);
						strcpy(texte_demande[1], texte_demande[2]);
						texte_demande[2][0] = '\0';
					}
				}
				else
				{
					couleur_texte[1][ref[1] - 1][0] = 0;
					couleur_texte[1][ref[1] - 1][1] = 0;
					couleur_texte[1][ref[1] - 1][2] = 1;
				}
			}
			else
			{
				couleur_texte[0][ref[0] - 1][0] = 0;
				couleur_texte[0][ref[0] - 1][1] = 0;
				couleur_texte[0][ref[0] - 1][2] = 1;
			}
		}
		if (!mode)
			texte_demande[0][0] = '\0';
		if (mode == 2 && ref[2] == 35)
		{
			couleur_texte[2][34][0] = 0;
			couleur_texte[2][34][1] = 0;
			couleur_texte[2][34][2] = 1;
		}
	}

	if (key >= 'A' && key <= 'A' + 26)
	{
		positions_y[0][key - 'A'] = 0;
		if (positions_y[1][key - 'A'])
		{
			positions_y[1][key - 'A'] = 0;
			if (mode != 2)
				demande = 1;
		}
	}
}

void dessiner_touche(unsigned char key, float x, float y, float z)
{
	if (key != ' ')
	{
		glBindTexture(GL_TEXTURE_2D, Texts[key - 'A']);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, 16, 16, 0, GL_RGBA, GL_UNSIGNED_BYTE,
				Texture[key - 'A']);
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, Texts[26]);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, 16, 16, 0, GL_RGBA, GL_UNSIGNED_BYTE,
				Texture[26]);
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	if (key != ' ')
	{
		glBegin(GL_QUADS);
		glNormal3f(0.338, 0.878, -0.338);
		glTexCoord2i(0, 0);
		glVertex3f(-1.0f + x, .5f + y, -1.0f + z);
		glNormal3f(0.338, 0.878, 0.338);
		glTexCoord2i(0, 1);
		glVertex3f(-1.0f + x, .5f + y, 1.0f + z);
		glNormal3f(-0.338, 0.878, 0.338);
		glTexCoord2i(1, 1);
		glVertex3f(1.0f + x, .5f + y, 1.0f + z);
		glNormal3f(-0.338, 0.878, -0.338);
		glTexCoord2i(1, 0);
		glVertex3f(1.0f + x, .5f + y, -1.0f + z);

		glEnd();
	}
	else
	{
		glBegin(GL_QUADS);
		glNormal3f(0.05, 0.9, -0.4);
		glTexCoord2i(0, 0);
		glVertex3f(-6.6f, .5f + y, 6.8f);
		glNormal3f(0.05, 0.9, 0.4);
		glTexCoord2i(0, 1);
		glVertex3f(-6.6f, .5f + y, 8.8f);
		glNormal3f(-0.05, 0.9, 0.4);
		glTexCoord2i(1, 1);
		glVertex3f(6.6f, .5f + y, 8.8f);
		glNormal3f(-0.05, 0.9, -0.4);
		glTexCoord2i(1, 0);
		glVertex3f(6.6f, .5f + y, 6.8f);

		glEnd();
	}

	glBindTexture(GL_TEXTURE_2D, Texts[26]);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, 16, 16, 0, GL_RGBA, GL_UNSIGNED_BYTE,
			Texture[26]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	if (key != ' ')
	{
		glBegin(GL_QUADS);

		glNormal3f(0.338, -0.878, -0.338);
		glTexCoord2i(0, 0);
		glVertex3f(-1.3f + x, -.5f + y, -1.3f + z);
		glNormal3f(0.338, -0.878, 0.338);
		glTexCoord2i(1, 0);
		glVertex3f(-1.3f + x, -.5f + y, 1.3f + z);
		glNormal3f(-0.338, -0.878, 0.338);
		glTexCoord2i(1, 1);
		glVertex3f(1.3f + x, -.5f + y, 1.3f + z);
		glNormal3f(-0.338, -0.878, -0.338);
		glTexCoord2i(0, 1);
		glVertex3f(1.3f + x, -.5f + y, -1.3f + z);

		glNormal3f(-0.338, 0.878, 0.338);
		glTexCoord2i(0, 0);
		glVertex3f(1.0f + x, .5f + y, 1.0f + z);
		glNormal3f(-0.338, -0.878, 0.338);
		glTexCoord2i(1, 0);
		glVertex3f(1.3f + x, -.5f + y, 1.3f + z);
		glNormal3f(0.338, -0.878, 0.338);
		glTexCoord2i(1, 1);
		glVertex3f(-1.3f + x, -.5f + y, 1.3f + z);
		glNormal3f(0.338, 0.878, 0.338);
		glTexCoord2i(0, 1);
		glVertex3f(-1.0f + x, .5f + y, 1.0f + z);

		glNormal3f(-0.338, 0.878, -0.338);
		glTexCoord2i(0, 0);
		glVertex3f(1.0f + x, .5f + y, -1.0f + z);
		glNormal3f(-0.338, -0.878, -0.338);
		glTexCoord2i(1, 0);
		glVertex3f(1.3f + x, -.5f + y, -1.3f + z);
		glNormal3f(0.338, -0.878, -0.338);
		glTexCoord2i(1, 1);
		glVertex3f(-1.3f + x, -.5f + y, -1.3f + z);
		glNormal3f(0.338, 0.878, -0.338);
		glTexCoord2i(0, 1);
		glVertex3f(-1.0f + x, .5f + y, -1.0f + z);

		glNormal3f(-0.338, 0.878, 0.338);
		glTexCoord2i(0, 0);
		glVertex3f(1.0f + x, .5f + y, 1.0f + z);
		glNormal3f(-0.338, -0.878, 0.338);
		glTexCoord2i(1, 0);
		glVertex3f(1.3f + x, -.5f + y, 1.3f + z);
		glNormal3f(0.338, -0.878, -0.338);
		glTexCoord2i(1, 1);
		glVertex3f(1.3f + x, -.5f + y, -1.3f + z);
		glNormal3f(-0.338, 0.878, -0.338);
		glTexCoord2i(0, 1);
		glVertex3f(1.0f + x, .5f + y, -1.0f + z);

		glNormal3f(0.338, 0.878, 0.338);
		glTexCoord2i(0, 0);
		glVertex3f(-1.0f + x, .5f + y, 1.0f + z);
		glNormal3f(0.338, -0.878, 0.338);
		glTexCoord2i(1, 0);
		glVertex3f(-1.3f + x, -.5f + y, 1.3f + z);
		glNormal3f(0.338, -0.878, -0.338);
		glTexCoord2i(1, 1);
		glVertex3f(-1.3f + x, -.5f + y, -1.3f + z);
		glNormal3f(0.338, 0.878, -0.338);
		glTexCoord2i(0, 1);
		glVertex3f(-1.0f + x, .5f + y, -1.0f + z);

		glEnd();
	}
	else
	{
		glBegin(GL_QUADS);

		glNormal3f(0.05, -0.9, -0.4);
		glTexCoord2i(0, 0);
		glVertex3f(-6.9f, -.5f + y, 6.5f);
		glNormal3f(0.05, -0.9, 0.4);
		glTexCoord2i(1, 0);
		glVertex3f(-6.9f, -.5f + y, 9.1f);
		glNormal3f(-0.05, -0.9, 0.4);
		glTexCoord2i(1, 1);
		glVertex3f(6.9f, -.5f + y, 9.1f);
		glNormal3f(0.05, -0.9, -0.4);
		glTexCoord2i(0, 1);
		glVertex3f(6.9f, -.5f + y, 6.5f);

		glNormal3f(-0.05, 0.9, 0.4);
		glTexCoord2i(0, 0);
		glVertex3f(6.6f, .5f + y, 8.8f);
		glNormal3f(-0.05, -0.9, 0.4);
		glTexCoord2i(1, 0);
		glVertex3f(6.9f, -.5f + y, 9.1f);
		glNormal3f(0.05, -0.9, 0.4);
		glTexCoord2i(1, 1);
		glVertex3f(-6.9f, -.5f + y, 9.1f);
		glNormal3f(0.05, 0.9, 0.4);
		glTexCoord2i(0, 1);
		glVertex3f(-6.6f, .5f + y, 8.8f);

		glNormal3f(-0.05, 0.9, -0.4);
		glTexCoord2i(0, 0);
		glVertex3f(6.6f, .5f + y, 6.8f);
		glNormal3f(-0.05, -0.9, -0.4);
		glTexCoord2i(1, 0);
		glVertex3f(6.9f, -.5f + y, 6.5f);
		glNormal3f(0.05, -0.9, -0.4);
		glTexCoord2i(1, 1);
		glVertex3f(-6.9f, -.5f + y, 6.5f);
		glNormal3f(0.05, 0.9, -0.4);
		glTexCoord2i(0, 1);
		glVertex3f(-6.6f, .5f + y, 6.8f);

		glNormal3f(-0.05, 0.9, 0.4);
		glTexCoord2i(0, 0);
		glVertex3f(6.6f, .5f + y, 8.8f);
		glNormal3f(-0.05, -0.9, 0.4);
		glTexCoord2i(1, 0);
		glVertex3f(6.9f, -.5f + y, 9.1f);
		glNormal3f(-0.05, -0.9, -0.4);
		glTexCoord2i(1, 1);
		glVertex3f(6.9f, -.5f + y, 6.5f);
		glNormal3f(-0.05, 0.9, -0.4);
		glTexCoord2i(0, 1);
		glVertex3f(6.6f, .5f + y, 6.8f);

		glNormal3f(0.05, 0.9, 0.4);
		glTexCoord2i(0, 0);
		glVertex3f(-6.6f, .5f + y, 8.8f);
		glNormal3f(0.05, -0.9, 0.4);
		glTexCoord2i(1, 0);
		glVertex3f(-6.9f, -.5f + y, 9.1f);
		glNormal3f(0.05, -0.9, -0.4);
		glTexCoord2i(1, 1);
		glVertex3f(-6.9f, -.5f + y, 6.5f);
		glNormal3f(0.05, 0.9, -0.4);
		glTexCoord2i(0, 1);
		glVertex3f(-6.6f, .5f + y, 6.8f);

		glEnd();
	}
}

void Afficher_curseur(int x, int y)
{
	glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
}

void Cacher_curseur(int x, int y)
{
	glutSetCursor(GLUT_CURSOR_NONE);
}

int main(int argc, char *argv[])
{
	affichage_texte[0] = '\0';
	texte_demande[0][0] = '\0';
	texte_demande[1][0] = '\0';
	texte_demande[2][0] = '\0';
	init_alphabet();
#ifdef FMOD
	init_son();
#endif
	init_config();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(1024, 768); //Optionnel
	glutCreateWindow("Dactylographie");
	glutFullScreen(); //Optionnel
	glutReshapeFunc(Reshape);

	glutKeyboardFunc(Touches);
	glutKeyboardUpFunc(Relache_Touches);
	glutDisplayFunc(Draw);
	glutSpecialFunc(Touches_spe);
	glutMotionFunc(Afficher_curseur);
	glutPassiveMotionFunc(Cacher_curseur);
	InitGL();
	glutIdleFunc(idle);
	glutMainLoop();
	return 0;
}

