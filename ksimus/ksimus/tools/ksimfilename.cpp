/***************************************************************************
                          ksimfilename.cpp  -  description
                             -------------------
    begin                : Sun Mar 24 2002
    copyright            : (C) 2002 by Rasmus Diekenbrock
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
#include <qstring.h>
#include <qstringlist.h>
#include <qdir.h>
#include <qfileinfo.h>

// KDE-Includes
#include <kurl.h>

// Project-Includes
#include "ksimfilename.h"
#include "enumdict.h"
#include "ksimdebug.h"
#include "ksimusdoc.h"
#include "ksimfile.h"
#include "component.h"
#include "ksimdata.h"


// Forward declaration


// Defines


//#########################################################################

static EnumDict<KSimFilename::ePathType> pathTypeDict;

EnumDict<KSimFilename::ePathType>::tData EnumDict<KSimFilename::ePathType>::data[]
			= { {"Absolute",          KSimFilename::PATH_ABSOLUTE},
					{"Relative Document", KSimFilename::PATH_RELATIVE_DOCUMENT},
					{"Relative Module",   KSimFilename::PATH_RELATIVE_MODULE},
					{"Relative Home",     KSimFilename::PATH_RELATIVE_HOME},
          {0,(KSimFilename::ePathType)0}};


const char * KSimFilename::convertPathType(KSimFilename::ePathType pathType)
{
	return pathTypeDict.find(pathType);
}
KSimFilename::ePathType KSimFilename::convertPathType(const char * pathType, KSimFilename::ePathType defaultPathType)
{
	return pathTypeDict.find(pathType, defaultPathType);
}


static const char * sFilename       = "Filename";
static const char * sIsPathType     = "Path Type";
static const char * sShouldPathType = "should Path Type";



//#########################################################################


KSimFilename::KSimFilename(Component * parent, QString filename, ePathType pathType)
	:	m_component(parent),
		m_filename(filename),
		m_pathType(pathType)
{
	CHECK_PTR(m_component);
}
	
KSimFilename::KSimFilename(const KSimFilename & filename)
{
	if (&filename != this)
	{
		m_component = filename.m_component;
		m_filename  = filename.m_filename;
		m_pathType  = filename.m_pathType;
	}
}
	
KSimFilename::~KSimFilename()
{
}
	
bool KSimFilename::operator==(const KSimFilename & filename) const
{
	return (  (getPathType() == filename.getPathType())
	       && (getFilename() == filename.getFilename()));
}

bool KSimFilename::operator!=(const KSimFilename & filename) const
{
	return !(*this == filename);
}

const KSimFilename & KSimFilename::operator=(const KSimFilename & filename)
{
	if (&filename != this)
	{
		setPathType(filename.getPathType());
		setFilename(filename.getFilename());
	}
	
	return filename;
}

void KSimFilename::setFilename(QString newFilename)
{
	if (QDir::isRelativePath(newFilename))
	{
		// Path is relative. Try to make it absolute
		switch(getPathType())
		{
			case PATH_ABSOLUTE:
			{
				KSIMDEBUG_VAR("relative path with PATH_ABSOLUTE. Use current dir",newFilename);
				m_filename = KSimFile::absolutePath(QDir::currentDirPath(), newFilename);
			}
			break;
			
			case PATH_RELATIVE_DOCUMENT:
			{
				if (getComponent()->getDoc()->isNamed())
				{
					KURL docUrl(getComponent()->getDoc()->URL());
					ASSERT(!docUrl.isEmpty());
					m_filename = KSimFile::absolutePath(docUrl.directory(), newFilename);
				}
				else
				{
					// Cant convert it. Store it relative.
					m_filename = newFilename;
				}
			}
			break;
		
			case PATH_RELATIVE_MODULE:
			{
				ASSERT(0);
			}
			break;
		
			case PATH_RELATIVE_HOME:
			{
				m_filename = KSimFile::absolutePath(QDir::homeDirPath(), newFilename);
			}
			break;
		}
	}
	else
	{
		// Filename is absolute. Store and leave
		m_filename = newFilename;
	}
}

QString KSimFilename::getFilename() const
{
	return m_filename;
}

QString KSimFilename::getRelativeFilename() const
{
	// If filename is relative return it unchanged
	if (QDir::isRelativePath(getFilename()))
	{
		return getFilename();
	}
	
	QString filename;
	
	switch(getPathType())
	{
		case PATH_ABSOLUTE:
			filename = getFilename();
			break;
			
		case PATH_RELATIVE_HOME:
			if (QDir::isRelativePath(getFilename()))
			{
				filename = getFilename();	// return unchanged
			}
			else
			{
				filename = KSimFile::relativePath(getFilename(), QDir::homeDirPath());
			}
			break;
			
		case PATH_RELATIVE_DOCUMENT:
			if (QDir::isRelativePath(getFilename()) || !getComponent()->getDoc()->isNamed())
			{
				filename = getFilename();	// return unchanged
			}
			else
			{
				filename = KSimFile::relativePath(getFilename(), getComponent()->getDoc()->URL().directory(false, true));
			}
		break;
		
		case PATH_RELATIVE_MODULE:
		{
			getComponent()->logWarning(QString::fromLatin1("Log file is given relative to module.\n This is not implemented yet! Use home dir"));
			// Always valid
			filename = getFilename();	// return unchanged
		}
		break;
	}
	return filename;
}


void KSimFilename::setPathType(ePathType pathType)
{
	m_pathType = pathType;
}
	
KSimFilename::ePathType KSimFilename::getPathType() const
{
	return m_pathType;
}

bool KSimFilename::isPathValid() const
{
	bool res(true);
	
	switch(getPathType())
	{
		case PATH_ABSOLUTE:
		case PATH_RELATIVE_HOME:
			// Always valid
			break;
			
		case PATH_RELATIVE_DOCUMENT:
		{
			res = !QDir::isRelativePath(getFilename()) || getComponent()->getDoc()->isNamed();
		}
		break;
		
		case PATH_RELATIVE_MODULE:
		{
			getComponent()->logWarning(QString::fromLatin1("Log file is given relative to module.\n This is not implemented yet! Use home dir"));
			// Always valid
		}
		break;
	}
	return res;
}

bool KSimFilename::isAbsolutePath() const
{
	switch(getPathType())
	{
		case PATH_ABSOLUTE:
			return true;
			break;
			
		case PATH_RELATIVE_HOME:
		case PATH_RELATIVE_DOCUMENT:
		case PATH_RELATIVE_MODULE:
			return false;
		break;
	}
	return false; //Dummy
}




Component * KSimFilename::getComponent()
{
	return m_component;
}

Component * KSimFilename::getComponent() const
{
	return m_component;
}

void KSimFilename::save(KSimData & config, const char * pGroup) const
{
	ePathType shouldPathType(getPathType());
	ePathType isPathType(shouldPathType);
	QString filename;
	
	// Set new group
	config.pushGroupRel(pGroup);
	
	// How should the path stored?
	switch(shouldPathType)
	{
		case PATH_ABSOLUTE:
		case PATH_RELATIVE_HOME:
		case PATH_RELATIVE_DOCUMENT:
		{
			isPathType = shouldPathType;
		}
		break;
		
		case PATH_RELATIVE_MODULE:
		{
			getComponent()->logWarning(QString::fromLatin1("Log file is given relative to module.\n This is not implemented yet! Use home dir"));
			isPathType = PATH_RELATIVE_HOME;
		}
		break;
	}
	
	config.writeEntry(sFilename, getRelativeFilename());
	pathTypeDict.save(config, sIsPathType, isPathType);
	if (isPathType != shouldPathType)
	{
		// Save "should type" only if different
		pathTypeDict.save(config, sShouldPathType, shouldPathType);
	}

	// Restore the old group
	config.popGroup();
}

bool KSimFilename::load(KSimData & config, const char * pGroup, const KSimFilename* pDefault )
{
	bool success = true;
	
	if (config.hasGroupRel(pGroup))
	{
		// Set new group
		config.pushGroupRel(pGroup);
		if (config.hasKey(sFilename) && config.hasKey(sIsPathType))
		{
			// Keys are okay
			
			QString filename;
			const ePathType *pPathType;
			
			filename = config.readEntry(sFilename, filename);
			pPathType = pathTypeDict.load(config, sIsPathType);
			if (pPathType)
			{
				setPathType(*pPathType);  // First path type
				setFilename(filename);
				
				KSIMDEBUG(QString::fromLatin1("Resulting filename(%1)").arg(getFilename()));
			}
			else
			{
				// Bad is path type
				KSIMDEBUG(QString::fromLatin1("Bad \"is path type\" (%1)").arg(config.readEntry(sIsPathType)));
				success = false;
			}
		}
		else
		{
			// Bad keys
			KSIMDEBUG("Keys not found!");
			success = false;
		}
	}
	else
	{
		// Bad group
		KSIMDEBUG("Group not found!");
		success = false;
	}
	
	// Try default
	if (!success && pDefault)
	{
		KSIMDEBUG("Load defaults");
		setFilename(pDefault->getFilename());
		setPathType(pDefault->getPathType());
	}
	
	// Restore the old group
	config.popGroup();
	
	return success;
}

//#########################################################################

#include <qlabel.h>
#include <qpushbutton.h>
#include <qcombobox.h>
#include <qwhatsthis.h>
#include <qtooltip.h>
#include <qpixmap.h>
#include <qlayout.h>

#include <klocale.h>
#include <kdialog.h>
#include <kfiledialog.h>
#include <kiconloader.h>

#include "ksimlineedit.h"
		
		

class KSimFilenameWidget::KSimFilenameWidgetPrivate
{
public:
	KSimFilenameWidgetPrivate(KSimFilename & fileInfo)
	:	
		m_filename(fileInfo),
		m_fileFilter(i18n("*|All files"))
	{};
	
	~KSimFilenameWidgetPrivate()
	{};
	
	KSimFilename m_filename;
	QString m_fileFilter;
	int m_mask;
	
	KSimLineEdit * m_filenameEdit;
	QPushButton * m_filenameDialogButton;
	QComboBox * m_pathTypeBox;
	int m_absolueIdx, m_relativeDocIdx, m_relativeModuleIdx, m_relativeHomeIdx;
};


KSimFilenameWidget::KSimFilenameWidget(KSimFilename & fileInfo, int mask, QWidget * parent, const char * name)
	: QWidget(parent, name)
{
	m_p = new KSimFilenameWidgetPrivate(fileInfo);
	CHECK_PTR(m_p);
	
	
	QString tip;
	int idx = 0;
	// #### Create Widget #####
	
	// File line
	m_p->m_filenameEdit = new KSimLineEdit(this, "KSimFilenameWidget m_filenameEdit");
	CHECK_PTR(m_p->m_filenameEdit);	
	tip = i18n("Enter the filename with the absolute or relative path.");
	QToolTip::add(m_p->m_filenameEdit, tip);
	QWhatsThis::add(m_p->m_filenameEdit, tip);
	connect(m_p->m_filenameEdit, SIGNAL(changed()), SLOT(slotFilenameChanged()));
	
	m_p->m_filenameDialogButton = new QPushButton(this, "KSimFilenameWidget m_filenameDialogButton");
	CHECK_PTR(m_p->m_filenameDialogButton);	
	m_p->m_filenameDialogButton->setPixmap(KGlobal::iconLoader()->loadIcon(QString::fromLatin1("fileopen.png"),KIcon::Toolbar));
	connect(m_p->m_filenameDialogButton, SIGNAL(released()), SLOT(slotOpenFileDialog()));
	tip = i18n("Opens a file dialog.");
	QToolTip::add(m_p->m_filenameDialogButton, tip);
	QWhatsThis::add(m_p->m_filenameDialogButton, tip);
	

	// Path Type	
	m_p->m_pathTypeBox = new QComboBox(this, "KSimFilenameWidget m_pathTypeBox");
	CHECK_PTR(m_p->m_pathTypeBox);	
	QToolTip::add(m_p->m_pathTypeBox, getPathTypeBoxToolTip());
	QWhatsThis::add(m_p->m_pathTypeBox, getPathTypeBoxWhatsThis(mask));
	connect(m_p->m_pathTypeBox, SIGNAL(activated(int)), SLOT(slotPathType(int)));
	
//	tip += "\n";
	if (mask & KSimFilename::PATH_ABSOLUTE)
	{
		m_p->m_pathTypeBox->insertItem(i18n("Save absolute path"), idx);
		m_p->m_absolueIdx = idx;
		idx++;
	}
	else
	{
		m_p->m_absolueIdx = -1;
	}
	if (mask & KSimFilename::PATH_RELATIVE_DOCUMENT)
	{
		m_p->m_pathTypeBox->insertItem(i18n("Save path relative to document"), idx);
		m_p->m_relativeDocIdx = idx;
		idx++;
	}
	else
	{
		m_p->m_relativeDocIdx = -1;
	}
	if (mask & KSimFilename::PATH_RELATIVE_MODULE)
	{
		m_p->m_pathTypeBox->insertItem(i18n("Save path relative to module"), idx);
		m_p->m_relativeModuleIdx = idx;
		idx++;
	}
	else
	{
		m_p->m_relativeModuleIdx = -1;
	}
	if (mask & KSimFilename::PATH_RELATIVE_HOME)
	{
		m_p->m_pathTypeBox->insertItem(i18n("Save path relative to home dir"), idx);
		m_p->m_relativeHomeIdx = idx;
		idx++;
	}
	else
	{
		m_p->m_relativeHomeIdx = -1;
	}
		
	// Layout
	QVBoxLayout * mainLayout = new QVBoxLayout(this, 0, -1, "KSimFilenameWidget mainlayout");
	CHECK_PTR(mainLayout);	
//	mainLayout->setMargin(KDialog::marginHint());
//	mainLayout->setSpacing(KDialog::spacingHint());
	
	QHBoxLayout * filenameLayout = new QHBoxLayout(mainLayout, 0, "KSimFilenameWidget filenameLayout");
	CHECK_PTR(filenameLayout);	
	filenameLayout->addWidget(m_p->m_filenameEdit, 1);
	filenameLayout->addWidget(m_p->m_filenameDialogButton, 0);
	
	QHBoxLayout * pathTypeLayout = new QHBoxLayout(mainLayout, 0, "KSimFilenameWidget pathTypeLayout");
	CHECK_PTR(pathTypeLayout);	
	pathTypeLayout->addWidget(m_p->m_pathTypeBox, 0);
	pathTypeLayout->addStretch(1);
	

	// Setup values
	setFilename(fileInfo.getFilename());
	setPathType(fileInfo.getPathType());
}
	


KSimFilenameWidget::~KSimFilenameWidget()
{
	delete m_p;
}

const KSimFilename & KSimFilenameWidget::getFileInfo() const
{
	return m_p->m_filename;
};

KSimFilename & KSimFilenameWidget::getFileInfo()
{
	return m_p->m_filename;
};

KSimLineEdit * KSimFilenameWidget::getFilenameEdit()
{
	return m_p->m_filenameEdit;
};

QPushButton * KSimFilenameWidget::getFilenameDialogButton()
{
	return m_p->m_filenameDialogButton;
};

QComboBox * KSimFilenameWidget::getPathTypeBox()
{
	return m_p->m_pathTypeBox;
};

QString KSimFilenameWidget::getFileFilter() const
{
	return m_p->m_fileFilter;
}

void KSimFilenameWidget::setFileFilter(const QString & filter)
{
	m_p->m_fileFilter = filter;
}


QString KSimFilenameWidget::getPathTypeBoxToolTip()
{
	return i18n("The path of the file can be stored absolute or relative in the document.");
}

QString KSimFilenameWidget::getPathTypeBoxWhatsThis(int mask)
{
	QString tip(i18n("<p>The path of the file can be stored absolute or relative in the document.</p>"));
	if (mask & KSimFilename::PATH_ABSOLUTE)
	{
		tip += i18n("<p><b>Save absolute path:</b> Stores the absolute path.</p>");
	}
	if (mask & KSimFilename::PATH_RELATIVE_DOCUMENT)
	{
		tip += i18n("<p><b>Save path relative to document:</b> Stores the path relative to the document.</p>");
	}
	if (mask & KSimFilename::PATH_RELATIVE_MODULE)
	{
		tip += i18n("<p><b>Save path relative to module:</b> Stores the path relative to the module.</p>");
	}
	if (mask & KSimFilename::PATH_RELATIVE_HOME)
	{
		tip += i18n("<p><b>Save path relative to home dir:</b> Stores the path relative to your home directory.</p>");
	}
	
	return tip;
}


void KSimFilenameWidget::setFilename(QString newFilename)
{
	getFileInfo().setFilename(newFilename);
	
	if (getFileInfo().isAbsolutePath() || !getFileInfo().isPathValid())
	{
		getFilenameEdit()->setText(getFileInfo().getFilename());
	}
	else
	{
		getFilenameEdit()->setText(getFileInfo().getRelativeFilename());
	}
}

QString KSimFilenameWidget::getFilename() const
{
	return getFileInfo().getFilename();
}
	
void KSimFilenameWidget::setPathType(KSimFilename::ePathType pathType)
{
	int idx = -1;
	getFileInfo().setPathType(pathType);
	switch(pathType)
	{
		case KSimFilename::PATH_ABSOLUTE:
			idx = m_p->m_absolueIdx;
			break;

		case KSimFilename::PATH_RELATIVE_DOCUMENT:
			idx = m_p->m_relativeDocIdx;
			break;

		case KSimFilename::PATH_RELATIVE_MODULE:
			idx = m_p->m_relativeModuleIdx;
			break;

		case KSimFilename::PATH_RELATIVE_HOME:
			idx = m_p->m_relativeHomeIdx;
			break;
			
		default:
			idx = -2;
	}
	
	if (idx >= 0)
	{
		getPathTypeBox()->setCurrentItem(idx);
	}
	else if (idx == -1)
	{
		KSIMDEBUG(QString::fromLatin1("Path type is masked (%1)")
		         .arg(QString::fromLatin1(KSimFilename::convertPathType(pathType))));
	}
	else
	{
		KSIMDEBUG_VAR("Unknow path type", pathType);
	}
}
	
KSimFilename::ePathType KSimFilenameWidget::getPathType() const
{
	return getFileInfo().getPathType();
}
	

void KSimFilenameWidget::slotOpenFileDialog()
{
	QString file(getFilename());
	
	if (file.isEmpty())
	{
		file = QDir::current().path();
	}
	
	file=KFileDialog::getOpenFileName(file, getFileFilter(), this, i18n("Open File..."));
	
	if(!file.isEmpty())
	{
		setFilename(file);
	}
}

void KSimFilenameWidget::slotPathType(int idx)
{
	if (idx == m_p->m_absolueIdx)
	{
		setPathType(KSimFilename::PATH_ABSOLUTE);
	}
	else if (idx == m_p->m_relativeDocIdx)
	{
		setPathType(KSimFilename::PATH_RELATIVE_DOCUMENT);
	}
	else if (idx == m_p->m_relativeModuleIdx)
	{
		setPathType(KSimFilename::PATH_RELATIVE_MODULE);
	}
	else if (idx == m_p->m_relativeHomeIdx)
	{
		setPathType(KSimFilename::PATH_RELATIVE_HOME);
	}
	setFilename(getFilename());
}

void KSimFilenameWidget::slotFilenameChanged()
{
	setFilename(getFilenameEdit()->text());
}
