/***************************************************************************
                          ksimembfont  -  description
                             -------------------
    begin                : Sa Jan 10 2004
    copyright            : (C) 2004 by Rasmus Diekenbrock
    email                : ksimus@gmx.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef KSIMEMBFONT_H
#define KSIMEMBFONT_H

// C-Includes

// QT-Includes
#include <qbitmap.h>
#include <qintdict.h>

// KDE-Includes

// Project-Includes
#include "globals.h"

// Forward declaration
class QPainter;
class QRect;
class QPoint;
class QString;


/**
A class with embedded fonts.

@author Rasmus Diekenbrock
*/
class KSimEmbFont
{
public:
	enum eFont { eFont08, eFont10 };

	KSimEmbFont(enum eFont font);
	~KSimEmbFont();

	int width(const QString & str) const;
	int height() const;

	const QBitmap & getBitmap(const QChar & c) const;
	bool exist(const QChar & c) const;
	void drawText(QPainter * p, int x, int y, const QString & str) const;
	void drawText(QPainter * p, int x, int y, int tf, const QString & str) const;
	void drawText(QPainter * p, const QPoint & pos, int tf, const QString & str) const;
	void drawText(QPainter * p, const QRect & r, int tf, const QString & str) const;
	void drawText(QPainter * p, const QPoint & pos, double rot, int tf, const QString & str) const;

protected:

private:
	QIntDict<QBitmap> m_glyphDict;
	QBitmap m_unknownGlyph;
	int m_height;
	int m_glyphSpace;
};

inline void KSimEmbFont::drawText(QPainter * p, const QPoint & pos, int tf, const QString & str) const
{
	drawText(p, pos.x(), pos.y(), tf, str);
}

extern KSimEmbFont * g_embFont08;
extern KSimEmbFont * g_embFont10;

#endif
