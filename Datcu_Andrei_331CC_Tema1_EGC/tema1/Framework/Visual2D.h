#pragma once;
#include "Color.h"
#include "Text.h"
#include "Object2D.h"
#include "ComplexObject2D.h"


class Visual2D
{
protected:	
	float XFm, XFM, YFm, YFM;

private:
	int XPm, XPM, YPm, YPM;
	bool tip_tran, decup;
	float sx,sy,tx,ty;

	//functii
	void calcTran();

public:
	//container obiecte 2D
	vector <Object2D*> visual2D_objects2D;
	vector <ComplexObject2D*> visual2D_complexObjects2D;
	//contatiner text
	vector <Text*> visual2D_texts;


	Visual2D();

	Visual2D(float xf1,float yf1,float xf2,float yf2,int xp1,int yp1,int xp2,int yp2);

	~Visual2D();

	void tipTran(bool tip);

	void decupare(bool dec);

	void fereastra(float x1,float y1,float x2,float y2);

	void poarta(int x1,int y1,int x2,int y2);

	int xDisp(float xf);

	int yDisp(float yf);

	void cadruFereastra(Color color);

	void cadruPoarta(Color color);

	float xLog(int xp);

	float yLog(int yp);

	void addComplexObject2D(ComplexObject2D *co);
	void removeComplexObject2D(ComplexObject2D *co);

	void addText(Text* t);

};
