/* Autor: Datcu Andrei Daniel - 331CC
 * Bonus
 * Clasa abstracta care defineste un obiect de tip bonus, pentru care nu 
 * conteaza cadrul, fiindca el va fi mereu static
 */

#include "Bonus.h"


//Nu ma intereseaza pozitia niciun fel de cadru, fiindca obiectul de tip
//bonus este static

Bonus::Bonus(int type): ComplexObject2D(Point2D(0,0),Point2D(0,0)), type(type){
}

