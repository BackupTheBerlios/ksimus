/***************************************************************************
                          integerconditional.cpp  -  description
                             -------------------
    begin                : Sat Aug 16 2003
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
#include <qpainter.h>
#include <qlabel.h>
#include <qcombobox.h>
#include <qpixmap.h>


// KDE-Includes
#include <klocale.h>

// Project-Includes
#include "ksimus/ksimdebug.h"
#include "ksimus/connectorboolout.h"
#include "ksimus/connectorintegerin.h"
#include "ksimus/ksimdata.h"
#include "ksimus/resource.h"
#include "integerconditional.h"

// Forward declaration

namespace KSimLibInteger
{

//###############################################################
//###############################################################

Component * IntegerConditional::create(CompContainer * container, const ComponentInfo * ci)
{
	return new IntegerConditional(container, ci);
}

const ComponentInfo * IntegerConditional::getStaticLesserInfo()
{
	static const ComponentInfo Info(i18n("Component", "Integer Conditional Lesser"),
	                                QString::fromLatin1("Integer/Conditional/Lesser"),
	                                i18n("Component", "Integer/Conditional/Lesser"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create /* TODO ,
	                                QString::null,
	                                QString::fromLatin1("component-float-conditional-lesser")*/);
	return &Info;
}

const ComponentInfo * IntegerConditional::getStaticLesserEqualInfo()
{
	static const ComponentInfo Info(i18n("Component", "Integer Conditional Lesser Equal"),
	                                QString::fromLatin1("Integer/Conditional/Lesser Equal"),
	                                i18n("Component", "Integer/Conditional/Lesser Equal"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create /* TODO ,
	                                QString::null,
	                                QString::fromLatin1("component-float-conditional-lesser-equal")*/);
	return &Info;
}

const ComponentInfo * IntegerConditional::getStaticEqualInfo()
{
	static const ComponentInfo Info(i18n("Component", "Integer Conditional Equal"),
	                                QString::fromLatin1("Integer/Conditional/Equal"),
	                                i18n("Component", "Integer/Conditional/Equal"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create /* TODO ,
	                                QString::null,
	                                QString::fromLatin1("component-float-conditional-equal")*/);
	return &Info;
}

const ComponentInfo * IntegerConditional::getStaticLargerEqualInfo()
{
	static const ComponentInfo Info(i18n("Component", "Integer Conditional Larger Equal"),
	                                QString::fromLatin1("Integer/Conditional/Larger Equal"),
	                                i18n("Component", "Integer/Conditional/Larger Equal"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create /* TODO ,
	                                QString::null,
	                                QString::fromLatin1("component-float-conditional-larger-equal")*/);
	return &Info;
}

const ComponentInfo * IntegerConditional::getStaticLargerInfo()
{
	static const ComponentInfo Info(i18n("Component", "Integer Conditional Larger"),
	                                QString::fromLatin1("Integer/Conditional/Larger"),
	                                i18n("Component", "Integer/Conditional/Larger"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create /* TODO ,
	                                QString::null,
	                                QString::fromLatin1("component-float-conditional-larger")*/);
	return &Info;
}

//###############################################################
//###############################################################

IntegerConditional::IntegerConditional(CompContainer * container, const ComponentInfo * ci)
	: Boolean1Out(container, ci)
{
	
	if      (ci == getStaticLesserInfo())      m_conditionalType = eLesser;
	else if (ci == getStaticLesserEqualInfo()) m_conditionalType = eLesserEqual;
	else if (ci == getStaticEqualInfo())       m_conditionalType = eEqual;
	else if (ci == getStaticLargerEqualInfo()) m_conditionalType = eLargerEqual;
	else /*if (ci == getStaticLargerInfo())*/  m_conditionalType = eLarger;
	
	
	m_inputA = new ConnectorIntegerIn(this,
	                                  QString::fromLatin1("Input A"),
	                                  i18n("Integer-Connector", "Input A"));
	CHECK_PTR(m_inputA);
	
	m_inputB = new ConnectorIntegerIn(this,
	                                  QString::fromLatin1("Input B"),
	                                  i18n("Integer-Connector", "Input B"));
	CHECK_PTR(m_inputB);
	
	// Initializes the sheet view
	if (getSheetMap())
	{
		new View(this, SHEET_VIEW);
	}

	getAction().disable(KSimAction::UPDATEVIEW);
}

/*IntegerConditional::~IntegerConditional()
{
} */

/** load component properties
*   copyLoad is true, if the load function is used as a copy function
*	Returns true if successful */
bool IntegerConditional::load(KSimData & file, bool copyLoad)
{
	bool res = Boolean1Out::load(file, copyLoad);

	QString id = file.readEntry(Component::sType);
	setConditionalType(id);
	
	return res;
}

bool IntegerConditional::isProperReloadType(const QString & type) const
{
	return (type == getStaticLesserInfo()->getLibName())
	    || (type == getStaticLesserEqualInfo()->getLibName())
	    || (type == getStaticEqualInfo()->getLibName())
	    || (type == getStaticLargerEqualInfo()->getLibName())
	    || (type == getStaticLargerInfo()->getLibName());
}

void IntegerConditional::calculate()
{
	Boolean1Out::calculate();

	bool state = false;
	
	switch(getConditionalType())
	{
		case eLesser:      state = getInputA()->getInput() <  getInputB()->getInput(); break;
		case eLesserEqual: state = getInputA()->getInput() <= getInputB()->getInput(); break;
		case eEqual:       state = getInputA()->getInput() == getInputB()->getInput(); break;
		case eLargerEqual: state = getInputA()->getInput() >= getInputB()->getInput(); break;
		case eLarger:      state = getInputA()->getInput() >  getInputB()->getInput(); break;
	}
	setState(state);
}

void IntegerConditional::setConditionalType(eConditionalType newCond)
{
	switch(newCond)
	{
		case eLesser:      setInfo(getStaticLesserInfo());      break;
		case eLesserEqual: setInfo(getStaticLesserEqualInfo()); break;
		case eEqual:       setInfo(getStaticEqualInfo());       break;
		case eLargerEqual: setInfo(getStaticLargerEqualInfo()); break;
		case eLarger:      setInfo(getStaticLargerInfo());      break;
	}
	m_conditionalType = newCond;
	if (hasDefaultName())
	{
		// Set new default name if required
		setName(getDefaultName());
	}
}

void IntegerConditional::setConditionalType(const QString & type)
{
	if      (type == getStaticLesserInfo()->getLibName())       setConditionalType(eLesser);
	else if (type == getStaticLesserEqualInfo()->getLibName())  setConditionalType(eLesserEqual);
	else if (type == getStaticEqualInfo()->getLibName())        setConditionalType(eEqual);
	else if (type == getStaticLargerEqualInfo()->getLibName())  setConditionalType(eLargerEqual);
	else if (type == getStaticLargerInfo()->getLibName())       setConditionalType(eLarger);
	else
	{
		KSIMDEBUG_VAR("Unknown type", type);
	}
}

/** Creates the general property page for the property dialog.
 * Overload this function if you want to use a modified General Propery Page. Use as base class
 * @ref ComponentPropertyGeneralWidget.
 * This function is called by @ref addGeneralProperty*/
ComponentPropertyBaseWidget * IntegerConditional::createGeneralProperty(QWidget *parent)
{
	PropertyGeneralWidget * wid;
	wid = new PropertyGeneralWidget(this, parent);
	CHECK_PTR(wid);
	
	return wid;
}

//###############################################################
//###############################################################


/* XPM */
const char * IntegerConditional::View::AltB_xpm[] = {
"18 8 2 1",
" 	c None",
".	c #000000",
"   .         .... ",
"   .         .   .",
"  . .      . .   .",
"  . .    ..  .... ",
" .   . ..    .   .",
" .....   ..  .   .",
".     .    . .   .",
".     .      .... "};

/* XPM */
const char * IntegerConditional::View::AlteqB_xpm[] = {
"18 8 2 1",
" 	c None",
".	c #000000",
"   .         .... ",
"   .       . .   .",
"  . .    ..  .   .",
"  . .  ..    .... ",
" .   .   ..  .   .",
" ..... ..  . .   .",
".     .  ..  .   .",
".     .    . .... "};

/* XPM */
const char * IntegerConditional::View::AeqB_xpm[] = {
"18 8 2 1",
" 	c None",
".	c #000000",
"   .         .... ",
"   .         .   .",
"  . .        .   .",
"  . .   .... .... ",
" .   .       .   .",
" .....  .... .   .",
".     .      .   .",
".     .      .... "};

/* XPM */
const char * IntegerConditional::View::AgteqB_xpm[] = {
"18 8 2 1",
" 	c None",
".	c #000000",
"   .         .... ",
"   .   .     .   .",
"  . .   ..   .   .",
"  . .     .. .... ",
" .   .  ..   .   .",
" ..... .  .. .   .",
".     . ..   .   .",
".     ..     .... "};

/* XPM */
const char * IntegerConditional::View::AgtB_xpm[] = {
"18 8 2 1",
" 	c None",
".	c #000000",
"   .         .... ",
"   .         .   .",
"  . .  .     .   .",
"  . .   ..   .... ",
" .   .    .. .   .",
" .....  ..   .   .",
".     ..     .   .",
".     .      .... "};



unsigned int IntegerConditional::View::sInstanceCount  = 0;
QPixmap * IntegerConditional::View::sPixmapLesser      = 0;
QPixmap * IntegerConditional::View::sPixmapLesserEqual = 0;
QPixmap * IntegerConditional::View::sPixmapEqual       = 0;
QPixmap * IntegerConditional::View::sPixmapLargerEqual = 0;
QPixmap * IntegerConditional::View::sPixmapLarger      = 0;
int IntegerConditional::View::sPixmapOffsetTop;
int IntegerConditional::View::sPixmapOffsetLeft;


IntegerConditional::View::View(IntegerConditional * comp, eViewType viewType)
	: CompView(comp, viewType)
{
	enableConnectorSpacingTop(false);
	enableConnectorSpacingBottom(false);
//	enableConnectorSpacingLeft(false);
//	enableConnectorSpacingRight(false);
	enableRotation(true);
	
	setPlace(QRect(0, 0, 5*gridX, 5*gridY));
	getIntegerConditional()->getInputA()->setGridPos(0,1);
	getIntegerConditional()->getInputB()->setGridPos(0,3);
	getIntegerConditional()->getOutputConnector()->setGridPos(4,2);

	if (sInstanceCount == 0)
	{
		sPixmapLesser      = new QPixmap(AltB_xpm);   CHECK_PTR(sPixmapLesser);
		sPixmapLesserEqual = new QPixmap(AlteqB_xpm); CHECK_PTR(sPixmapLesserEqual);
		sPixmapEqual       = new QPixmap(AeqB_xpm);   CHECK_PTR(sPixmapEqual);
		sPixmapLargerEqual = new QPixmap(AgteqB_xpm); CHECK_PTR(sPixmapLargerEqual);
		sPixmapLarger      = new QPixmap(AgtB_xpm);   CHECK_PTR(sPixmapLarger);
		
		sPixmapOffsetLeft = (getDrawingPlace().width() - sPixmapLesser->width())/2 + getDrawingPlace().left();
		sPixmapOffsetTop = (getDrawingPlace().height() - sPixmapLesser->height())/2 + getDrawingPlace().top();

#if defined(DEBUG) && !defined(NO_KSIMDEBUG)
		// Assume same size of all pixmaps !!!!
		if ((sPixmapLesser->size() != sPixmapLesserEqual->size())
		 || (sPixmapLesser->size() != sPixmapEqual->size())
		 || (sPixmapLesser->size() != sPixmapLargerEqual->size())
		 || (sPixmapLesser->size() != sPixmapLarger->size()))
		{
			KSIMDEBUG("FIX ME: Assume same size of all pixmaps !!!!");
		}
#endif //defined(DEBUG) && !defined(NO_KSIMDEBUG)

	}
	sInstanceCount++;
}

IntegerConditional::View::~View()
{
	sInstanceCount--;
	if (sInstanceCount == 0)
	{
		delete sPixmapLesser;       sPixmapLesser = 0;
		delete sPixmapLesserEqual;  sPixmapLesserEqual = 0;
		delete sPixmapEqual;        sPixmapEqual = 0;
		delete sPixmapLargerEqual;  sPixmapLargerEqual = 0;
		delete sPixmapLarger;       sPixmapLarger = 0;
	}
}

void IntegerConditional::View::draw(QPainter * p)
{
	drawFrame(p);
	CompView::draw(p);
	
/*	KSIMDEBUG(QString("getDrawingPlace(%1,%2) sPixmapSize(%3,%4) xy(%5,%6)")
	                  .arg(getDrawingPlace().width()).arg(getDrawingPlace().height())
	                  .arg(sPixmapLesser->width()).arg(sPixmapLesser->height())
	                  .arg(sPixmapOffsetLeft).arg(sPixmapOffsetTop));*/

	switch(getIntegerConditional()->getConditionalType())
	{
		case eLesser:      p->drawPixmap(sPixmapOffsetLeft, sPixmapOffsetTop, *sPixmapLesser);      break;
		case eLesserEqual: p->drawPixmap(sPixmapOffsetLeft, sPixmapOffsetTop, *sPixmapLesserEqual); break;
		case eEqual:       p->drawPixmap(sPixmapOffsetLeft, sPixmapOffsetTop, *sPixmapEqual);       break;
		case eLargerEqual: p->drawPixmap(sPixmapOffsetLeft, sPixmapOffsetTop, *sPixmapLargerEqual); break;
		case eLarger:      p->drawPixmap(sPixmapOffsetLeft, sPixmapOffsetTop, *sPixmapLarger);      break;
	}
}


//###############################################################
//###############################################################


IntegerConditional::PropertyGeneralWidget::PropertyGeneralWidget(IntegerConditional * comp, QWidget *parent, const char *name)
	:	Boolean1OutPropertyGeneralWidget(comp, parent, name)
{
	QString tip;
	
	m_conditionalLabel = new QLabel(i18n("Integer", "Conditional: "), this, "m_conditionalLabel");
	CHECK_PTR(m_conditionalLabel);
	
	m_conditional = new QComboBox(this, "m_conditional");
	CHECK_PTR(m_conditional);
	m_conditional->insertItem(i18n("Integer", "A < B"),  (int)eLesser);
	m_conditional->insertItem(i18n("Integer", "A <= B"), (int)eLesserEqual);
	m_conditional->insertItem(i18n("Integer", "A = B"), (int)eEqual);
	m_conditional->insertItem(i18n("Integer", "A >= B"), (int)eLargerEqual);
	m_conditional->insertItem(i18n("Integer", "A > B"),  (int)eLarger);
	
	tip = i18n("Integer", "Sets the conditional.");
	addToolTip(tip, m_conditional, m_conditionalLabel);
	addWhatsThis(tip, m_conditional, m_conditionalLabel);
	m_conditionalLabel->setBuddy(m_conditional);

	// Setup value
	m_conditional->setCurrentItem((int)comp->getConditionalType());
}

/** The function acceptPressed() is called, if changes are accepted.
	You have to reimplement this function, if you add new properties.
	If you do so, then first call function changeData() and than changed data!
  */
void IntegerConditional::PropertyGeneralWidget::acceptPressed()
{
	Boolean1OutPropertyGeneralWidget::acceptPressed();
	
	if (getIntegerConditional()->getConditionalType() != (eConditionalType)m_conditional->currentItem())
	{
		changeData();
		getIntegerConditional()->setConditionalType((eConditionalType)m_conditional->currentItem());
	}
}


/** The function defaultPressed() is called, if user wants to set the default values.
    You have to reimplement this function, if you add new properties.
  */
void IntegerConditional::PropertyGeneralWidget::defaultPressed()
{
	Boolean1OutPropertyGeneralWidget::defaultPressed();

	m_conditional->setCurrentItem((int)getIntegerConditional()->getConditionalType());
}


//###############################################################
//###############################################################

};  //namespace KSimLibInteger

