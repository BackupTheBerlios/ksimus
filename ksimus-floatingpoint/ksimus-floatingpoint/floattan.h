/***************************************************************************
                          floattan.h  -  description
                             -------------------
    begin                : Thu Feb 21 2002
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

#ifndef FLOATTAN_H
#define FLOATTAN_H

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "float1in1out.h"
#include "float2in1out.h"
#include "ksimus/componentinfo.h"


namespace KSimLibFloatingPoint
{

// Forward declaration

/**A floating point tangential gate.
  * @author Rasmus Diekenbrock
  */

class FloatTan : public Float1In1Out
{
public:
	/** Constructs a floating point tangent */
	FloatTan(CompContainer * container, const ComponentInfo * ci);
	/** Executes the simulation of this component */
	virtual void calculate();

	static Component * create(CompContainer * container, const ComponentInfo * ci);
	static const ComponentInfo * getStaticInfo();
};

//###############################################################

/** The view for the floating point tangent gate.
	*
  * @author Rasmus Diekenbrock
  */
class FloatTanView : public Float1In1OutView
{
public:
	FloatTanView(FloatTan * comp, eViewType viewType)
		: Float1In1OutView(comp, viewType, 5, 5) {};
	
	virtual void draw(QPainter * p);
};

//###############################################################

//###############################################################

/**A floating point inverse tangential gate.
  * @author Rasmus Diekenbrock
  */

class FloatATan : public Float1In1Out
{
public:
	/** Constructs a floating point inverse tangent */
	FloatATan(CompContainer * container, const ComponentInfo * ci);
	/** Executes the simulation of this component */
	virtual void calculate();

	static Component * create(CompContainer * container, const ComponentInfo * ci);
	static const ComponentInfo * getStaticInfo();
};

//###############################################################

/** The view for the floating point inverse tangent gate.
	*
  * @author Rasmus Diekenbrock
  */
class FloatATanView : public Float1In1OutView
{
public:
	FloatATanView(FloatATan * comp, eViewType viewType)
		: Float1In1OutView(comp, viewType, 5, 5) {};
	
	virtual void draw(QPainter * p);
};

//###############################################################

//###############################################################

/**A floating point inverse tangent gate with 2 inputs.
  * @author Rasmus Diekenbrock
  */

class FloatATan2 : public Float2In1Out
{
public:
	/** Constructs a floating point inverse tangent with 2 inputs */
	FloatATan2(CompContainer * container, const ComponentInfo * ci);
	/** Executes the simulation of this component */
	virtual void calculate();

	static Component * create(CompContainer * container, const ComponentInfo * ci);
	static const ComponentInfo * getStaticInfo();
};

//###############################################################

/** The view for the floating point inverse tangent gate with 2 inputs.
	*
  * @author Rasmus Diekenbrock
  */
class FloatATan2View : public Float2In1OutView
{
public:
	FloatATan2View(FloatATan2 * comp, eViewType viewType)
		: Float2In1OutView(comp, viewType) {};

	virtual void draw(QPainter * p);
};

//###############################################################

//###############################################################

/**A floating point hyperbolic tangent gate.
  * @author Rasmus Diekenbrock
  */

class FloatTanh : public Float1In1Out
{
public:
	/** Constructs a floating point tangential */
	FloatTanh(CompContainer * container, const ComponentInfo * ci);
	/** Executes the simulation of this component */
	virtual void calculate();

	static Component * create(CompContainer * container, const ComponentInfo * ci);
	static const ComponentInfo * getStaticInfo();
};

//###############################################################

/** The view for the floating point hyperbolic tangent gate.
	*
  * @author Rasmus Diekenbrock
  */
class FloatTanhView : public Float1In1OutView
{
public:
	FloatTanhView(FloatTanh * comp, eViewType viewType)
		: Float1In1OutView(comp, viewType, 5, 5) {};

	virtual void draw(QPainter * p);
};

//###############################################################

//###############################################################

/**A floating point inverse hyperbolic tangential gate.
  * @author Rasmus Diekenbrock
  */

class FloatATanh : public Float1In1Out
{
public:
	/** Constructs a floating point inverse tangent */
	FloatATanh(CompContainer * container, const ComponentInfo * ci);
	/** Executes the simulation of this component */
	virtual void calculate();

	static Component * create(CompContainer * container, const ComponentInfo * ci);
	static const ComponentInfo * getStaticInfo();
};

//###############################################################

/** The view for the floating point inverse hyperbolic tangent gate.
	*
  * @author Rasmus Diekenbrock
  */
class FloatATanhView : public Float1In1OutView
{
public:
	FloatATanhView(FloatATanh * comp, eViewType viewType)
		: Float1In1OutView(comp, viewType, 5, 5) {};

	virtual void draw(QPainter * p);
};

//###############################################################


};  //namespace KSimLibFloatingPoint
#endif
