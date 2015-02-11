/* Autor: Datcu Andrei Daniel - 331CC
 * Bonus
 * Clasa abstracta care defineste un obiect de tip bonus, pentru care nu 
 * conteaza cadrul, fiindca el va fi mereu static
 */

#pragma once
#include "../Framework/complexobject2d.h"

class Bonus : public ComplexObject2D{
public:
	Bonus(int type);
	const int type;
};

