#include "Carte.hpp"
#include <iostream>
#include <fstream>
#include <random>

Carte::Carte()
{
	this->tabl_ = new Case[256];
	creerCarte();
}

Carte::~Carte()
{
	delete[] this->tabl_;
}

void Carte::creerCarte()
{
	std::ifstream f ("carte");
	char a;

	//petit message d'erreur en cas d'absence de fichier
	if (!f.is_open())
		{
			std::cerr << "Impossible d'ouvrir le fichier en lecture !" << std::endl;
	  	}
	//sinon, on lit
	else
	{
		int cpt = 0;
		//tant qu'il y a à lire & qu'on a pas atteint le bout de la carte
		while(!f.eof() && (cpt <256))
		{
			f >> a;
			this->setTab(cpt, (a-48));
			//à la création de la carte, il n'y a rien dessus mais
			//on initialise l'occupation tout de même
			this->setOccupation(cpt, 0);
			++cpt;
		}

		f.close();
	}
	remplirCarte();
}

void Carte::setTab(int a, int b)
{
	if ((0 <= a) && (a < 256))
	{
		this->tabl_[a].type = b;
	}


}
//avec une petite vérification, si le chiffre n'est pas compris
//dans les bonnes valeurs, -1
int Carte::getTab(int i)
{
	if ((0 <= i) && (i < 256))
	{
		return this->tabl_[i].type;
	}
	return -1;
}

/** renvoit ce que le canard voit
// -17 -16 -15
//  -1  X  +1
// +15 +16 +17
// ou vue est un pointeur vers un tableau de 9 cases créées avant
TODO : Gérer les bords de la carte
*/
int* Carte::getVue(int* vue, int i)
{
	vue[0] = getTab(i-17);
	vue[1] = getTab(i-16);
	vue[2] = getTab(i-15);
	vue[3] = getTab(i-1);
	vue[4] = getTab(i);
	vue[5] = getTab(i+1);
	vue[6] = getTab(i+15);
	vue[7] = getTab(i+16);
	vue[8] = getTab(i+17);

	if (i%16 == 0)
	{
		vue[0] = 0;
		vue[3] = 0;
		vue[6] = 0;
	}
	if (i%16 == 15)
	{
		vue[2] = 0;
		vue[5] = 0;
		vue[8] = 0;
	}
	if (i < 15)
	{
		vue[0] = 0;
		vue[1] = 0;
		vue[2] = 0;
	}
	if (i > 239)
	{
		vue[6] = 0;
		vue[7] = 0;
		vue[8] = 0;
	}
	return vue;
}

void Carte::setOccupation(int a, int b)
{
	if ((0 <= a) && (a < 256))
	{
		this->tabl_[a].occupation = b;
	}
}

int Carte::getOccupation(int a)
{
	return this->tabl_[a].occupation;
}

bool Carte::getObstacleSol(int a)
{
	//1 / 2 / 5 = obstacles (eau, arbre, rocher)
	if ((this->getType(a) == 1) || (this->getType(a) == 2) || (this->getType(a) == 5))
	{
		return true;
	}
	return false;
}

bool Carte::getObstacleVol()
{
	//il n'y a pas d'obstacles pour le vol
	return false;
}

bool Carte::getObstacleEau(int a)
{
	//3 / 5 = obstacles (sol, rocher)
	if ((this->getType(a) == 3) || (this->getType(a) == 5))
	{
		return true;
	}
	return false;
}

int Carte::getType(int i)
{
	return this->tabl_[i].type;
}

void Carte::setType(int i, int a)
{
	this->tabl_[i].type = a;
}

void Carte::remplirCarte()
{
	/*
	On crée l'aléatoire (cf cplusplus.com/reference/random)
	*/
	std::default_random_engine generator;
	std::uniform_int_distribution<int> distribution(0,2);

	/*
	D'abord on remplit la carte	sans prédateur [ de 0  à 2]
	*/

	for (int i = 0 ; i < 256 ; ++i)
	{
        this->tabl_[i].occupation = distribution(generator);
	}

}
