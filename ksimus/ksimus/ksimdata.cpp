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
#include <qstringlist.h>
#include <qpixmap.h>

// KDE-Includes
#include <ksimpleconfig.h>

// Project-Includes
#include "ksimdata.h"
#include "ksimdebug.h"
#include "enumdict.h"

// Forward declaration

class KSimData::Private
{
	public:

	Private(const QString &filename, KSimData::eVersionType versionType, bool bReadOnly)
		: m_simpleFile(filename, bReadOnly),
			m_versionType(versionType),
			m_filename(filename)
	{
	};

	KSimpleConfig m_simpleFile;
	KSimData::eVersionType m_versionType;
	QStringList m_groupStack;
	QString m_filename;
};

//#########################################################################################

typedef struct
{
	const char * docVersion;
	const char * firstKSimusVersion;
	const char * lastKSimusVersion;
} tVersionList;

static const tVersionList versionList[] =
{
	{ "KSimus <0.3.5", "0.0.0", "0.3.4" },
	{ "KSimus 0.3.5", "0.3.5", (const char *)0 }
};


static const char * const sVersionTypeKey = "Document Version";
static const char * const sKSimusKey =      "Document Type";


//#########################################################################################

EnumDict<KSimData::eVersionType>::tData EnumDict<KSimData::eVersionType>::data[]
      = { {"error",        KSimData::versionError},
          {"as is",        KSimData::versionAsIs},
          {"unknown",      KSimData::versionUnknown},
          {"KSimus 0.3.5", KSimData::version0_3_5},
          {"topical",      KSimData::versionTopical},
          {0,              (KSimData::eVersionType)0}};

static const EnumDict<KSimData::eVersionType> & getVersionTypeDict()
{
	static EnumDict<KSimData::eVersionType> versionTypeDict;
	return versionTypeDict;
}

const char * KSimData::convertVersionType(KSimData::eVersionType versionType)
{
	return getVersionTypeDict().find(versionType);
}

KSimData::eVersionType KSimData::convertVersionType(const char * versionType, KSimData::eVersionType defaultVersionType)
{
	return getVersionTypeDict().find(versionType, defaultVersionType);
}


//#########################################################################################

#define data       (m_p->m_simpleFile)
#define groupStack (m_p->m_groupStack)


//#########################################################################################



KSimData::KSimData(const QString &pFileName, eVersionType versionType, bool bReadOnly)
{
	m_p = new Private(pFileName, versionType, bReadOnly);
	CHECK_PTR(m_p);
	pushGroup("/");
	writeEntry(sKSimusKey, "KSimus"); // Set always
	
	switch(versionType)
	{
		case versionError:
			KSIMDEBUG("Fix me: call KSimData::KSimData with versionType == versionError");
			break;
			
		case versionAsIs:
			m_p->m_versionType = convertVersionType(data.readEntry(sVersionTypeKey), versionUnknown);
			break;
			
		case versionUnknown:
			KSIMDEBUG("Fix me: call KSimData::KSimData with versionType == versionUnknown");
			break;
			
		default:
			m_p->m_versionType = versionType;
			data.writeEntry(sVersionTypeKey, convertVersionType(m_p->m_versionType));
			break;
	}
	
	popGroup();
}

KSimData::~KSimData()
{
	if (groupStack.count() != 0)
	{
		KSIMDEBUG(QString::fromLatin1("close file while stack is not empty (File %1, current group %2").arg(m_p->m_filename).arg(group()));
	}
	
	delete m_p;
}

KSimData::eVersionType KSimData::getVersionType() const
{
	return m_p->m_versionType;
}

void KSimData::setGroup( const QString& pGroup )
{
	data.setGroup( pGroup );	
}

void KSimData::setGroupRel( const QString& groupRel )
{
	data.setGroup(data.group() + groupRel);
}

void KSimData::setGroup( const char * pGroup )
{
	data.setGroup( pGroup );	
}

void KSimData::setGroupRel( const char * groupRel )
{
	data.setGroup(data.group() + groupRel);
}

QString KSimData::group() const
{
	return data.group();	
}

void KSimData::pushGroup( const QString& newGroup )
{
	groupStack.append(group());
	setGroup(newGroup);
}
	
void KSimData::pushGroup( const char * newGroup )
{
	pushGroup(QString::fromLatin1(newGroup));
}

void KSimData::pushGroupRel( const QString& groupRel )
{
	groupStack.append(group());
	setGroupRel(groupRel);
}
	
void KSimData::pushGroupRel( const char * groupRel )
{
	pushGroupRel(QString::fromLatin1(groupRel));
}
	
void KSimData::popGroup(void)
{
	if (groupStack.count())
	{
		QStringList::Iterator it(groupStack.end());
		it--;
		
		setGroup(*it);
		groupStack.remove(it);
	}
	else
	{
		KSIMDEBUG(QString::fromLatin1("pop group from an empty stack (File %1, current group %2").arg(m_p->m_filename).arg(group()));
	}
}

bool KSimData::hasGroup(const QString &group) const
{
	return data.hasGroup(group);	
}

bool KSimData::hasGroup(const char * group) const
{
	return data.hasGroup(group);	
}

bool KSimData::hasGroupRel(const QString &groupRel) const
{
	return data.hasGroup(data.group() + groupRel);	
}

bool KSimData::hasGroupRel(const char * groupRel) const
{
	return data.hasGroup(data.group() + groupRel);	
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

QPixmap KSimData::readPixmapEntry( const char *pKey, const QPixmap* pDefault ) const
{
	if (hasKey(pKey))
	{
		QStringList strList(readListEntry(pKey));
		unsigned int size,i;
		size = strList[0].toUInt(0,16);
		QByteArray array(size);
		
		for (i = 0; i < size; i++)
		{
			array[i] = (char) strList[i+1].toUInt(0,16);
		}
		
		QDataStream stream(array, IO_ReadOnly);
		QPixmap pixmap;
		stream >> pixmap;
		return pixmap;
	}
	else if (pDefault)
	{
		return *pDefault;
	}
	else
	{
		return QPixmap();
	}
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

void KSimData::writeEntry( const char *pKey, const QPixmap& rPixmap )
{
	QByteArray array;
	QDataStream stream(array, IO_WriteOnly);
	QStringList strList;
	unsigned int i;
	
	stream << rPixmap;
	
	strList.append(QString::number(array.size(),16));
	
	for (i = 0; i < array.size(); i++)
	{
		strList.append(QString::number((unsigned char)array[i],16));
	}
	writeEntry(pKey, strList);	
}


