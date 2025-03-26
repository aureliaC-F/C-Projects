/**
* \file DicoSynonymes.cpp
* \brief Le code des opérateurs du DicoSynonymes.
* \author IFT-2008, Matthieu Caron-Fournier
* \version 0.5
* \date 23 avril 2021
*
* Travail pratique numéro 3.
*
*/

#include "DicoSynonymes.h"
using namespace std;
namespace TP3
{

	// Méthode fournie
	void DicoSynonymes::chargerDicoSynonyme(std::ifstream& fichier)
	{
        if (!fichier.is_open())
            throw std::logic_error("DicoSynonymes::chargerDicoSynonyme: Le fichier n'est pas ouvert !");

		std::string ligne;
		std::string buffer;
		std::string radical;
		int cat = 1;

		while (!fichier.eof()) // tant qu'on peut lire dans le fichier
		{
            std::getline(fichier, ligne);

			if (ligne == "$")
			{
				cat = 3;
				std::getline(fichier, ligne);
			}
			if (cat == 1)
			{
				radical = ligne;
				ajouterRadical(radical);
				cat = 2;
			}
			else if (cat == 2)
			{
				std::stringstream ss(ligne);
				while (ss >> buffer)
					ajouterFlexion(radical, buffer);
				cat = 1;
			}
			else
			{
				std::stringstream ss(ligne);
				ss >> radical;
				ss >> buffer;
				int position = -1;
				ajouterSynonyme(radical, buffer, position);
				while (ss >> buffer)
					ajouterSynonyme(radical, buffer, position);
			}
		}
	}


	//Mettez l'implantation des autres méthodes demandées ici.
	
	/**
	 * \brief constructeur par défaut
	 * 		  On construit un objet DicoSynonymes vide
	 */
	DicoSynonymes::DicoSynonymes() :racine(0), nbRadicaux(0), groupesSynonymes({}) { }

	/**
	 * \brief constructeur avec paramêtres
	 * 		  On construit un objet DicoSynonymes à partir d'un fichier formatté comme celui fourni.
	 */
	DicoSynonymes::DicoSynonymes(std::ifstream& fichier):DicoSynonymes()
	{
		try
		{
			chargerDicoSynonyme(fichier);
		}
		catch (const std::bad_alloc& ba)
		{
			throw bad_alloc();
		}
	}

	/**
	 * \brief destructeur
	 */
	DicoSynonymes::~DicoSynonymes()
	{
		_auxDestructionNoeuds(racine);
	}

	/**
	 * \brief ajoute au dictionnaire le radical donné
	 * \param[in] motRadical, le mot à ajouter au dictionnaire
	 */
	void DicoSynonymes::ajouterRadical(const std::string& motRadical)
	{
		try
		{
			_auxAjouterRadical(motRadical, racine);
			NoeudDicoSynonymes* noeud_rad = findNoeux(motRadical);
			// cout << "recherche faite" << endl;
			list<NoeudDicoSynonymes*> nouvSynonymes;
			nouvSynonymes.push_back(noeud_rad);
			groupesSynonymes.push_back(nouvSynonymes);
			int numGroupe = groupesSynonymes.size() - 1;
			noeud_rad->appSynonymes.push_back(numGroupe);
		}
		catch (const std::bad_alloc&)
		{
			throw bad_alloc();
		}
		catch (const std::logic_error& le)
		{
			throw logic_error(le);
		}
	}

	/**
	 * \brief ajoute la flexion donnée au radical donné
	 * \param[in] motRadical, le radical auquel on ajoute une flexion
	 * \param[in] motFlexion, la flexion à ajouter au radical
	 */
	void DicoSynonymes::ajouterFlexion(const std::string& motRadical, const std::string& motFlexion)
	{
		try
		{
			_auxAjouterFlexion(motRadical, motFlexion, racine);
		}
		catch (const std::bad_alloc&)
		{
			throw bad_alloc();
		}
		catch (const std::logic_error& le)
		{
			throw logic_error(le);
		}
	}
	
	/**
	 * \brief  ajoute au dictionnaire le synonyme donné au radical fourni
	 * \param[in] motRadical, le radical auquel on ajoute un synonyme
	 * \param[in] motSynonyme, le synonyme à ajouter au radical
	 * \param[in] numGroupe, le numéro du groupe de synonyme auquel on ajoute motSynonyme. Mettre -1 pour créer un nouveau groupe.
	 */
	void DicoSynonymes::ajouterSynonyme(const std::string& motRadical, const std::string& motSynonyme, int& numGroupe)
	{
		try
		{
			NoeudDicoSynonymes* noeud_rad = findNoeux(motRadical);
			if (numGroupe == -1)
			{
				list<NoeudDicoSynonymes*> nouvSynonymes;
				nouvSynonymes.push_back(noeud_rad);
				groupesSynonymes.push_back(nouvSynonymes);
				numGroupe = groupesSynonymes.size()-1;
				noeud_rad->appSynonymes.push_back(numGroupe);
			}
			if (find(noeud_rad->appSynonymes.begin(), noeud_rad->appSynonymes.end(), numGroupe) != noeud_rad->appSynonymes.end())
			{

				NoeudDicoSynonymes* noeud_syn;
				try
				{
					noeud_syn = findNoeux(motSynonyme);
				}
				catch (const std::exception&)
				{

					ajouterRadical(motSynonyme);
					noeud_syn = findNoeux(motSynonyme);
				}
				groupesSynonymes[numGroupe].push_back(noeud_syn);
				noeud_syn->appSynonymes.push_back(numGroupe);
			}
			else
			{
				throw logic_error("le groupe de synonyme n'a pas été trouvé");
			}
		}
		catch (const std::bad_alloc&)
		{
			throw bad_alloc();
		}
		catch (const std::logic_error& le)
		{
			throw logic_error(le);
		}
	}

	/**
	 * \brief supprime du dictionnaire le radical donné
	 * \param[in] motRadical, le mot à supprimer du dictionnaire
	 */
	void DicoSynonymes::supprimerRadical(const std::string& motRadical)
	{
		try
		{
			_auxSupprimerRadical(motRadical, racine);
		}
		catch (const std::bad_alloc&)
		{
			throw bad_alloc();
		}
		catch (const std::logic_error& le)
		{
			throw logic_error(le);
		}
		
	}

	/**
	 * \brief supprime la flexion donnée du radical donné
	 * \param[in] motRadical, le radical auquel on supprime une flexion
	 * \param[in] motFlexion, la flexion à supprimer du radical
	 */
	void DicoSynonymes::supprimerFlexion(const std::string& motRadical, const std::string& motFlexion)
	{
		if (racine == 0)
		{
			throw logic_error("l'arbre est vide");
		}
		try
		{
			NoeudDicoSynonymes* noeud = findNoeux(motRadical);

			noeud->flexions.remove(motFlexion);
		}
		catch (const std::exception&)
		{
			throw logic_error("la flexion ou le radical fourni n'existe pas");
		}
	}

	/**
	 * \brief supprime le synonyme donnée du dictionnaire
	 * \param[in] motRadical, le radical auquel on supprime un synonyme
	 * \param[in] motSynonyme, le synonyme à supprimer du dictionnaire
	 * \param[in] numGroupe, le numéro du groupe de synonyme duquel on supprime motSynonyme. 
	 */
	void DicoSynonymes::supprimerSynonyme(const std::string& motRadical, const std::string& motSynonyme, int& numGroupe)
	{
		if (numGroupe > groupesSynonymes.size())
		{
			throw logic_error("le numéro de groupe fourni n'existe pas");
		}
		for (auto ite = groupesSynonymes[numGroupe].begin(); ite != groupesSynonymes[numGroupe].end(); ite++)
		{
			auto noeud = *ite;
			if (noeud->radical == motSynonyme)
			{
				groupesSynonymes[numGroupe].remove(noeud);
				supprimerRadical(motSynonyme);
				return;
			}
		}
		throw logic_error("le synonyme à supprimer n'a pas été trouvé");

	}

	/**
	 * \brief vérifie si l'arbre est vide ou non
	 * \return un booléen indiquant si l'arbre est vide ou non
	 */
	bool DicoSynonymes::estVide() const
	{
		if (racine == 0)
		{
			return true;
		}
		return false;
	}

	/**
	 * \brief retourne le nombre de radicaux du dictionnaire
	 * \return le nombre de radicaux du dictionnaire
	 */
	int DicoSynonymes::nombreRadicaux() const
	{
		return nbRadicaux;
	}

	/**
	 * \brief recherche une flexion dans le dictionnaire
	 * \param[in] mot, la flexion a chercher
	 * \return le radical de mot
	 */
	std::string DicoSynonymes::rechercherRadical(const std::string& mot) const
	{
		if (racine == 0)
		{
			throw logic_error("arbre vide");
		}
		try
		{
			return _auxRechercherRadical(mot, racine);
		}
		catch (const std::logic_error&)
		{
			throw logic_error("le radical de la flexion n'a pas été trouvé dans l'arbre");
		}
		catch (const std::exception& e)
		{
			throw exception(e);
		}
	}

	/**
	 * \brief donne un indice de similitude entre deux mots basé sur la longueur de Levenshtein
	 * \param[in] mot1, un mot à comparer
	 * \param[in] mot2, l'autre mot à comparer
	 * \return l'indice de similitude de mot1 et mot2, entre 0 et 1, où 1 est identique et 0 complètement différent
	 */
	float DicoSynonymes::similitude(const std::string& mot1, const std::string& mot2) const
	{
		if (mot1.length() == 0 && mot2.length() == 0)
		{
			return 1.0f;
		}
		else if(mot1.length() == 0 || mot2.length() == 0)
		{
			return 0.0f;
		}
		else
		{
			int distanceLevenshtein = _auxsimilitude(mot1, mot2);

			int longueurs[] = { mot1.length(), mot2.length() };
			int longueur_max = *max_element(longueurs, longueurs + 2);

			float indiceSimilitude = (longueur_max - distanceLevenshtein) / longueur_max;

			return indiceSimilitude;
		}
	}

	/**
	 * \brief retourne le nombre de sens d'un radical
	 * \param[in] radical, le radical en question 
	 * \return le nombre de sens du radical fourni
	 */
	int DicoSynonymes::getNombreSens(std::string radical) const
	{
		return findNoeux(radical)->appSynonymes.size();
	}

	/**
	 * \brief Donne le premier synonyme du groupe de synonyme de l'emplacement entrée en paramètre, excepté radical.
	 * \param[in] radical, le radical en question
	 * \param[in] position, le groupe de synonyme dont on veut le sens
	 * \return le premier sens du groupe de synonyme qui n'est pas radical
	 */
	std::string DicoSynonymes::getSens(std::string radical, int position) const
	{
		string sens = groupesSynonymes[position].front()->radical;
		if (sens != radical)
		{
			return sens;
		}
		else
		{
			sens = groupesSynonymes[position].back()->radical;

			return sens;
		}
	}
	
	/**
	 * \brief Donne tout les synonymes du groupe de synonyme de l'emplacement entrée en paramètre, excepté radical.
	 * \param[in] radical, le radical en question
	 * \param[in] position, le groupe de synonyme en question
	 * \return un vector<string> de tout les synonymes de radical dans le groupe position
	 */
	std::vector<std::string> DicoSynonymes::getSynonymes(std::string radical, int position) const
	{
		list<string> synoList;
		for (auto ite = groupesSynonymes[position].begin(); ite != groupesSynonymes[position].end(); ite++)
		{
			synoList.push_back((*ite)->radical);
		}
		vector<string> synonymes{synoList.begin(), synoList.end()};

		synonymes.erase(find(synonymes.begin(), synonymes.end(), radical));

		return synonymes;
	}

	/**
	 * \brief Donne toutes les flexions du radical donné en paramètre.
	 * \param[in] radical, le radical en question
	 * \return un vector<string> de toutes les flexions de radical
	 */
	std::vector<std::string> DicoSynonymes::getFlexions(std::string radical) const
	{
		NoeudDicoSynonymes* noeud = findNoeux(radical);
		vector<string> flexions{ noeud->flexions.begin(), noeud->flexions.end()};
		return flexions;
	}


	//private

	void DicoSynonymes::_auxAjouterRadical(const std::string& motRadical, NoeudDicoSynonymes*& noeud)
	{
		// cout << "ajout en cours" << endl;
		if (noeud == 0)
		{
			noeud = new NoeudDicoSynonymes(motRadical);
			nbRadicaux += 1;
			// cout << "ajout fait! Nouveau radical: " << noeud->radical << endl;
			return;
		}
		else
		{
			// cout << noeud->radical << " et " << motRadical << endl;
			if (motRadical < noeud->radical)
			{

				// cout << motRadical << " plus petit que " << noeud->radical << endl;
				_auxAjouterRadical(motRadical, noeud->gauche);
			}
			else if (motRadical > noeud->radical)
			{
				// cout << motRadical << " plus grand que " << noeud->radical << endl;
				_auxAjouterRadical(motRadical, noeud->droit);
			}
			else
			{
				throw logic_error("les duplicatats sont interdits");
			}
		}
		_balancer(noeud);

	}

	void DicoSynonymes::_auxAjouterFlexion(const std::string& motRadical, const std::string& motFlexion, NoeudDicoSynonymes*& noeud)
	{
		if (noeud == 0)
		{
			throw logic_error("le radical fourni de la flexion n'a pas pu être trouvé");
		}
		else
		{
			if (motRadical < noeud->radical)
			{
				_auxAjouterFlexion(motRadical, motFlexion, noeud->gauche);
			}
			else if (motRadical > noeud->radical)
			{
				_auxAjouterFlexion(motRadical, motFlexion, noeud->droit);
			}
			else
			{
				noeud->flexions.push_back(motFlexion);
			}
		}

	}

	int DicoSynonymes::_auxsimilitude(const std::string& mot1, const std::string& mot2) const
	{
		if (mot1.length() == 0)
		{
			return mot2.length();
		}
		else if (mot2.length() == 0)
		{
			return mot1.length();
		}

		else if (mot1.front() == mot2.front())
		{
			return _auxsimilitude(mot1.substr(1), mot2.substr(1));
		}


		else
		{
			int cas1 = _auxsimilitude(mot1.substr(1), mot2);
			int cas2 = _auxsimilitude(mot1, mot2.substr(1));
			int cas3 = _auxsimilitude(mot1.substr(1), mot2.substr(1));

			int cas[] = { cas1, cas2, cas3 };

			return 1 + *min_element(cas, cas + 3);
		}
	}

	void DicoSynonymes::_auxSupprimerRadical(const std::string& motRadical, NoeudDicoSynonymes*& noeud)
	{
		if (noeud == 0)
		{
			throw logic_error("Tentative d’enlever une donnée absente");
		}

		if (motRadical < noeud->radical)
		{
			_auxSupprimerRadical(motRadical, noeud->gauche);
		}
		else if(motRadical < noeud->radical)
		{
			_auxSupprimerRadical(motRadical, noeud->droit);
		}
		else
		{
			if (noeud->gauche != 0 && noeud->droit != 0)
			{
				_SupprimerSuccMinDroit(noeud);
			}
			else
			{
				NoeudDicoSynonymes* oldNoeud = noeud;
				noeud = (noeud->gauche != 0) ? noeud->gauche : noeud->droit;
				delete oldNoeud;
				--nbRadicaux;
			}
		}
		_balancer(noeud);
	}

	void DicoSynonymes::_SupprimerSuccMinDroit(NoeudDicoSynonymes* noeud)
	{
		NoeudDicoSynonymes* temp = noeud->gauche;
		NoeudDicoSynonymes* parent = noeud;

		while (temp->gauche != 0)
		{
			parent = temp;
			temp = temp->gauche;
		}

		noeud->radical = temp->radical;

		if (temp == parent->gauche)
		{
			_auxSupprimerRadical(temp->radical, parent->gauche);
		}
		else
		{
			_auxSupprimerRadical(temp->radical, parent->droit);
		}
	}

	void DicoSynonymes::_auxDestructionNoeuds(NoeudDicoSynonymes* noeud)
	{
		if (noeud == 0)
		{
			delete noeud;
			return;
		}
		
		if (noeud->gauche != 0)
		{
			_auxDestructionNoeuds(noeud->gauche);
		}
		if (noeud->droit != 0)
		{
			_auxDestructionNoeuds(noeud->droit);
		}
		delete noeud;
	}

	std::string DicoSynonymes::_auxRechercherRadical(const std::string& mot, NoeudDicoSynonymes* noeud) const
	{
		if (similitude(noeud->radical, mot) > similitude(noeud->gauche->radical, mot) && 
			similitude(noeud->radical, mot) > similitude(noeud->droit->radical, mot))
		{
			for (auto ite = noeud->flexions.begin(); ite != noeud->flexions.end(); ite++)
			{
				if (*ite == mot)
				{
					return noeud->radical;
				}
			}
		}
		if (similitude(noeud->droit->radical, mot) > similitude(noeud->gauche->radical, mot))
		{
			try
			{
				if (noeud->droit == 0)
				{
					throw logic_error("Pas d'enfant droit");
				}
				return _auxRechercherRadical(mot, noeud->droit);
			}
			catch (const std::exception&)
			{
				try
				{
					if (noeud->gauche == 0)
					{
						throw logic_error("Pas d'enfant gauche");
					}
					return _auxRechercherRadical(mot, noeud->gauche);
				}
				catch (const std::exception&)
				{
					throw logic_error("fin de la branche");
				}
			}

		}
		else
		{
			try
			{
				if (noeud->gauche == 0)
				{
					throw logic_error("Pas d'enfant gauche");
				}
				return _auxRechercherRadical(mot, noeud->gauche);
			}
			catch (const std::exception&)
			{
				try
				{
					if (noeud->droit == 0)
					{
						throw logic_error("Pas d'enfant droit");
					}
					return _auxRechercherRadical(mot, noeud->droit);
				}
				catch (const std::exception&)
				{
					throw logic_error("fin de la branche");
				}
			}

		}
	}

	//balancer
	void DicoSynonymes::_balancer(NoeudDicoSynonymes*& noeud)
	{
		if (_debalencementAGauche(noeud))
		{
			if (_sousArbrePencheADroite(noeud->gauche))
			{
				_zigZagGauche(noeud);
			}
			else
			{
				_zigZigGauche(noeud);
			}
		}
		else if (_debalencementADroite(noeud))
		{
			if (_sousArbrePencheAGauche(noeud->droit))
			{
				_zigZagDroit(noeud);
			}
			else
			{
				_zigZigDroit(noeud);
			}
		}
		else
		{
			if (noeud!=0)
			{
				noeud->hauteur = 1 + max(_hauteur(noeud->gauche), _hauteur(noeud->droit));
			}
		}
		// cout << racine->radical << endl;
	}

	int DicoSynonymes::_hauteur(NoeudDicoSynonymes* noeud)
	{
		if (noeud == 0)
		{
			return -1;
		}
		return noeud->hauteur;
	}

	bool DicoSynonymes::_debalencementAGauche(NoeudDicoSynonymes* noeud)
	{
		if (noeud == 0)
		{
			return false;
		}
		/*
		int hauteurGauche;
		if (noeud->gauche == 0)
		{
			hauteurGauche = noeud->hauteur;
		}
		else
		{
			hauteurGauche = noeud->gauche->hauteur;
		}
		int hauteurDroit;
		if (noeud->droit == 0)
		{
			hauteurDroit = noeud->hauteur;
		}
		else
		{
			hauteurDroit = noeud->droit->hauteur;
		}*/
		return 1 < _hauteur(noeud->gauche) - _hauteur(noeud->droit);
	}

	bool DicoSynonymes::_sousArbrePencheADroite(NoeudDicoSynonymes* noeud)
	{
		if (noeud == 0)
			return false;
		/*int hauteurGauche;
		if (noeud->gauche == 0)
		{
			hauteurGauche = noeud->hauteur;
		}
		else
		{
			hauteurGauche = noeud->gauche->hauteur;
		}
		int hauteurDroit;
		if (noeud->droit == 0)
		{
			hauteurDroit = noeud->hauteur;
		}
		else
		{
			hauteurDroit = noeud->droit->hauteur;
		}*/
		return _hauteur(noeud->gauche) < _hauteur(noeud->droit);
	}

	bool DicoSynonymes::_debalencementADroite(NoeudDicoSynonymes* noeud)
	{
		if (noeud == 0)
		{
			return false;
		}
		/*int hauteurGauche;
		if (noeud->gauche == 0)
		{
			hauteurGauche = noeud->hauteur;
		}
		else
		{
			hauteurGauche = noeud->gauche->hauteur;
		}
		int hauteurDroit;
		if (noeud->droit == 0)
		{
			hauteurDroit = noeud->hauteur;
		}
		else
		{
			hauteurDroit = noeud->droit->hauteur;
		}*/
		return 1 < _hauteur(noeud->droit) - _hauteur(noeud->gauche);
	}

	bool DicoSynonymes::_sousArbrePencheAGauche(NoeudDicoSynonymes* noeud)
	{
		if (noeud == 0)
			return false;
		/*int hauteurGauche;
		if (noeud->gauche == 0)
		{
			hauteurGauche = noeud->hauteur;
		}
		else
		{
			hauteurGauche = noeud->gauche->hauteur;
		}
		int hauteurDroit;
		if (noeud->droit == 0)
		{
			hauteurDroit = noeud->hauteur;
		}
		else
		{
			hauteurDroit = noeud->droit->hauteur;
		}*/
		return _hauteur(noeud->gauche) > _hauteur(noeud->droit);
	}

	void DicoSynonymes::_zigZigGauche(NoeudDicoSynonymes*& noeud)
	{
		NoeudDicoSynonymes* nouvTop = noeud->gauche;
		noeud->gauche = nouvTop->droit;
		nouvTop->droit = noeud;
		noeud->hauteur = 1 + max(_hauteur(noeud->gauche), _hauteur(noeud->droit));
		nouvTop->hauteur = 1 + max(_hauteur(nouvTop->gauche), noeud->hauteur);
		noeud = nouvTop;

	}

	void DicoSynonymes::_zigZagGauche(NoeudDicoSynonymes*& noeud)
	{
		_zigZigDroit(noeud->gauche);
		_zigZigGauche(noeud);
	}

	void DicoSynonymes::_zigZigDroit(NoeudDicoSynonymes*& noeud)
	{
		NoeudDicoSynonymes* nouvTop = noeud->droit;
		noeud->droit = nouvTop->gauche;
		nouvTop->gauche = noeud;
		noeud->hauteur = 1 + max(_hauteur(noeud->gauche), _hauteur(noeud->droit));
		nouvTop->hauteur = 1 + max(_hauteur(nouvTop->droit), noeud->hauteur);
		noeud = nouvTop;
	}

	void DicoSynonymes::_zigZagDroit(NoeudDicoSynonymes*& noeud)
	{
		_zigZigGauche(noeud->droit);
		_zigZigDroit(noeud);
	}


	
}//Fin du namespace