/***************************************************************************
                          ksimdata.cpp  -  description
                             -------------------
    begin                : Fri Nov 23 2001
    copyright            : (C) 2001 by Rasmus Diekenbrock
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

// C-Includes

// QT-Includes

// KDE-Includes
#include <ksimpleconfig.h>

// Project-Includes
#include "ksimdata.h"

// Forward declaration

class KSimDataPrivate
{
	public:
	
	KSimDataPrivate(const QString &pFileName, bool bReadOnly = false)
		: simpleFile(pFileName, bReadOnly)
	{
	};

	KSimpleConfig simpleFile;
};



//#########################################################################################

#define data (m_p->simpleFile)


//#########################################################################################



KSimData::KSimData(const QString &pFileName, bool bReadOnly)
{
	m_p = new KSimDataPrivate(pFileName, bReadOnly);
}

KSimData::~KSimData()
{
	delete m_p;
}

void KSimData::setGroup( const QString& pGroup )
{
	data.setGroup( pGroup );	
}

QString KSimData::group() const
{
	return data.group();	
}

bool KSimData::hasGroup(const QString &_pGroup) const
{
	return data.hasGroup(_pGroup);	
}

bool KSimData::hasKey(const char *pKey) const
{
	return data.hasKey(pKey);	
}

QString KSimData::readEntry( const char *pKey, const QString& aDefault) const
{
	return data.readEntry(pKey, aDefault);	
}

int KSimData::readListEntry( const char *pKey, QStrList &list, char sep) const
{
	return data.readListEntry(pKey, list, sep);	
}

QStringList KSimData::readListEntry( const char *pKey, char sep) const
{
	return data.readListEntry(pKey, sep);	
}

QValueList<int> KSimData::readIntListEntry( const char *pKey ) const
{
	return data.readIntListEntry(pKey);	
}

QString KSimData::readPathEntry( const char *pKey, const QString & aDefault ) const
{
	return data.readPathEntry(pKey, aDefault);	
}

int KSimData::readNumEntry( const char *pKey, int nDefault ) const
{
	return data.readNumEntry(pKey, nDefault);	
}

unsigned int KSimData::readUnsignedNumEntry( const char *pKey, unsigned int nDefault ) const
{
	return data.readUnsignedNumEntry(pKey, nDefault);	
}

long KSimData::readLongNumEntry( const char *pKey, long nDefault ) const
{
	return data.readLongNumEntry(pKey, nDefault);	
}

unsigned long KSimData::readUnsignedLongNumEntry( const char *pKey, unsigned long nDefault ) const
{
	return data.readUnsignedLongNumEntry(pKey, nDefault);	
}

double KSimData::readDoubleNumEntry( const char *pKey, double nDefault ) const
{
	return data.readDoubleNumEntry(pKey, nDefault);	
}

QFont KSimData::readFontEntry( const char *pKey, const QFont* pDefault ) const
{
	return data.readFontEntry(pKey, pDefault);	
}

bool KSimData::readBoolEntry( const char *pKey, const bool bDefault ) const
{
	return data.readBoolEntry(pKey, bDefault);	
}

QRect KSimData::readRectEntry( const char *pKey, const QRect* pDefault ) const
{
	return data.readRectEntry(pKey, pDefault);	
}

QPoint KSimData::readPointEntry( const char *pKey, const QPoint* pDefault ) const
{
	return data.readPointEntry(pKey, pDefault);	
}

QSize KSimData::readSizeEntry( const char *pKey, const QSize* pDefault ) const
{
	return data.readSizeEntry(pKey, pDefault);	
}

QColor KSimData::readColorEntry( const char *pKey, const QColor* pDefault ) const
{
	return data.readColorEntry(pKey, pDefault);	
}

QDateTime KSimData::readDateTimeEntry( const char *pKey, const QDateTime* pDefault ) const
{
	return data.readDateTimeEntry(pKey, pDefault);	
}

void KSimData::writeEntry( const char *pKey, const QString& pValue)
{
	data.writeEntry(pKey, pValue);	
}

void KSimData::writeEntry( const char *pKey, const QVariant& rValue)
{
	data.writeEntry(pKey, rValue);	
}

void KSimData::writeEntry( const char *pKey, const QStrList &rValue, char sep )
{
	data.writeEntry(pKey, rValue, sep);	
}

void KSimData::writeEntry( const char *pKey, const QStringList &rValue, char sep )
{
	data.writeEntry(pKey, rValue, sep);	
}

void KSimData::writeEntry( const char *pKey, const QValueList<int>& rValue)
{
	data.writeEntry(pKey, rValue);	
}

void KSimData::writeEntry( const char *pKey, int nValue)
{
	data.writeEntry(pKey, nValue);	
}

void KSimData::writeEntry( const char *pKey, unsigned int nValue)
{
	data.writeEntry(pKey, nValue);	
}

void KSimData::writeEntry( const char *pKey, long nValue)
{
	data.writeEntry(pKey, nValue);	
}

void KSimData::writeEntry( const char *pKey, unsigned long nValue)
{
	data.writeEntry(pKey, nValue);	
}

void KSimData::writeEntry( const char *pKey, double nValue, char format, int precision )
{
	data.writeEntry(pKey, nValue, true, false, format, precision );	
}

void KSimData::writeEntry( const char *pKey, bool bValue )
{
	data.writeEntry(pKey, bValue );	
}

void KSimData::writeEntry( const char *pKey, const QFont& rFont )
{
	data.writeEntry(pKey, rFont);	
}

void KSimData::writeEntry( const char *pKey, const QColor& rColor )
{
	data.writeEntry(pKey, rColor);	
}

void KSimData::writeEntry( const char *pKey, const QDateTime& rDateTime )
{
	data.writeEntry(pKey, rDateTime);	
}

void KSimData::writeEntry( const char *pKey, const QRect& rValue )
{
	data.writeEntry(pKey, rValue);	
}

void KSimData::writeEntry( const char *pKey, const QPoint& rValue )
{
	data.writeEntry(pKey, rValue);	
}

void KSimData::writeEntry( const char *pKey, const QSize& rValue )
{
	data.writeEntry(pKey, rValue);	
}



