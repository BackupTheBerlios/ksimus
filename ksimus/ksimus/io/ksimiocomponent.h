/***************************************************************************
                          ksimiocomponent.h  -  description
                             -------------------
    begin                : Sun Apr 6 2003
    copyright            : (C) 2003 by Rasmus Diekenbrock
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

#ifndef KSIMIOCOMPONENT_H
#define KSIMIOCOMPONENT_H

// C/C++ includes

// QT includes

// KDE includes

// Project includes
#include "component.h"
#include "compview.h"
#include "componentinfo.h"

// Forward declaration
class ComponentLayout;
class KSimIoJoin;

/**The IO Component.
  *@author Rasmus Diekenbrock
  */

class KSimIoComponent : public Component
{
	class Private;
	
   Q_OBJECT
public: 
	KSimIoComponent(CompContainer * container, const ComponentInfo * ci);
	~KSimIoComponent();

	/** save component properties */
	virtual void save(KSimData & file) const;
	/** load component properties
	*   copyLoad is true, if the load function is used as a copy function
	*	Returns true if successful */
	virtual bool load(KSimData & file, bool copyLoad);
	/** The sheet view of the component */
	/** Initialize the component popup menu
	  *	Return true, if items are added */
	virtual bool initPopupMenu(QPopupMenu * popup);

	/** Reset all simulation variables */
	virtual void reset();
	/** Executes the simulation of this component */
	virtual void calculate();

	void updateLayout();


	void removeJoin(KSimIoJoin * delJoin);


	static Component * create(CompContainer * container, const ComponentInfo * ci);
	static const ComponentInfo * getStaticInfo();

protected:
	unsigned int getNextSerial();

private:
	Private * m_p;

private slots:
	void slotPinSelection();
};

//###############################################################

/** The view for the IO component.
	*
  * @author Rasmus Diekenbrock
  */
class KSimIoComponentView : public CompView
{
   Q_OBJECT
public:
	KSimIoComponentView(KSimIoComponent * comp, eViewType viewType);

	virtual void draw(QPainter * p);
	void updateLayout();


private:
};

#endif
