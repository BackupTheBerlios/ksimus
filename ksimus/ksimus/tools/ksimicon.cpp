/***************************************************************************
                          ksimicon.cpp  -  description
                             -------------------
    begin                : Wed May 28 2003
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

/** Pixmaps are copied from KDE. Thanks KDE team! */

// C-Includes
#include <string.h>

// QT-Includes
#include <qstring.h>

// KDE-Includes
#include "ksimdebug.h"

// Project-Includes
#include "ksimicon.h"


//#######################################################################################
// Icons
//#######################################################################################

namespace KSimIconDefaults
{

static const char * back_16x16_default[]={
"16 16 8 1",
". c #ffffff",
"# c #000000",
"f c #004040",
"e c #008080",
"d c #00c0c0",
"c c #00ffff",
"a c #c0ffff",
"b c #ffffff",
"................",
".......##.......",
"......#a#.......",
".....#bc#.......",
"....#bcd#.......",
"...#bcdd######..",
"..#bcdddcaaac#..",
".#bcdddddddde#..",
"#acddddddddde#..",
".#fedddddddde#..",
"..#fedddeeeee#..",
"...#fedd######..",
"....#fed#.......",
".....#fe#.......",
"......#f#.......",
".......##......."};

static const char * clear_left_16x16_default[]={
"16 16 16 1",
". c None",
"b c #000000",
"# c #000000",
"a c #000000",
"c c #000000",
"n c #181818",
"m c #2d2d2d",
"l c #404040",
"k c #5a5a5a",
"j c #696969",
"h c #888888",
"g c #939393",
"e c #b7b7b7",
"d c #bdbdbd",
"f c #cfcfcf",
"i c #ffffff",
"................",
".....##########a",
"....bcccccccccc#",
"...bcddeedeedfc#",
"..bcghiihghiihc#",
".bcjjkiiikiiikc#",
"bcllllmiiiiimlc#",
"cnnnnnnciiicnnc#",
"icccccciiiiiccc#",
".icccciiiciiicc#",
"..iccciiccciicc#",
"...iccccccccccc#",
"....icccccccccc#",
".....iiiiiiiiiib",
"................",
"................"};

static const char * fileclose_16x16_default[]={
"16 16 3 1",
". c #5d5d5d",
"# c #000000",
"a c #ffffff",
"................",
"................",
"......#####.....",
"....#########...",
"...###########..",
"...##aa###aa##..",
"..###aaa#aaa###.",
"..####aaaaa####.",
"..#####aaa#####.",
"..####aaaaa####.",
"..###aaa#aaa###.",
"...##aa###aa##..",
"...###########..",
"....#########...",
"......#####.....",
"................"};

static const char * fileopen_16x16_default[]={
"16 16 7 1",
". c #5d5d5d",
"# c #000000",
"e c #a0a0a0",
"d c #c3c3c3",
"c c #dcdcdc",
"a c #ffa858",
"b c #ffdca8",
"......####......",
".....##.####.#..",
"....#.....####..",
"...........###..",
"..........####..",
".####...........",
"#abba#######....",
"#bbbbaaaaaaa#...",
"#bb#############",
"#ba#ccccccccccc#",
"#b#cddddededee#.",
"#a#cdddededeee#.",
"##cdddededeee#..",
"##cddededeeee#..",
"#cddededeeee#...",
"#############..."};

static const char * finish_16x16_default[]={
"16 16 8 1",
". c #ffffff",
"# c #000000",
"f c #004040",
"e c #008080",
"c c #00c0c0",
"b c #00ffff",
"a c #c0ffff",
"d c #ffffff",
"................",
"......##........",
"......#a#.....##",
"......#ba#....##",
"......#cba#...##",
".######ccba#..##",
".#dddabcccba#.##",
".#dccccccccba###",
".#acccccccccef##",
".#accccccccef###",
".#ceeeecccef#.##",
".######ccef#..##",
"......#cef#...##",
"......#ef#....##",
"......#f#.....##",
"......##........"};

static const char *forward_16x16_default[]={
"16 16 8 1",
". c #ffffff",
"# c #000000",
"f c #004040",
"e c #008080",
"c c #00c0c0",
"b c #00ffff",
"a c #c0ffff",
"d c #ffffff",
"................",
".......##.......",
".......#a#......",
".......#ba#.....",
".......#cba#....",
"..######ccba#...",
"..#dddabcccba#..",
"..#dccccccccba#.",
"..#acccccccccef#",
"..#accccccccef#.",
"..#ceeeecccef#..",
"..######ccef#...",
".......#cef#....",
".......#ef#.....",
".......#f#......",
".......##......."};

static const char *player_pause_16x16_default[]={
"16 16 2 1",
". c #ffffff",
"# c #000000",
"................",
"................",
"................",
"................",
"....###..###....",
"....###..###....",
"....###..###....",
"....###..###....",
"....###..###....",
"....###..###....",
"....###..###....",
"....###..###....",
"................",
"................",
"................",
"................"};

static const char * rightarrow1_16x16_default[]={
"16 16 2 1",
". c None",
"# c #000000",
"................",
"................",
".....#..........",
".....##.........",
".....###........",
".....####.......",
".....#####......",
".....######.....",
".....######.....",
".....#####......",
".....####.......",
".....###........",
".....##.........",
".....#..........",
"................",
"................"};

static const char *start_16x16_default[]={
"16 16 8 1",
". c #ffffff",
"# c #000000",
"f c #004040",
"e c #008080",
"d c #00c0c0",
"c c #00ffff",
"a c #c0ffff",
"b c #ffffff",
"................",
"........##......",
"##.....#a#......",
"##....#bc#......",
"##...#bcd#......",
"##..#bcdd######.",
"##.#bcdddcaaac#.",
"###bcdddddddde#.",
"##acddddddddde#.",
"###fedddddddde#.",
"##.#fedddeeeee#.",
"##..#fedd######.",
"##...#fed#......",
"##....#fe#......",
"##.....#f#......",
"........##......"};

static const char *stop_16x16_default[]={
"16 16 6 1",
". c #800000",
"# c #000000",
"c c #800000",
"b c #c00000",
"a c #ffc0c0",
"d c #ffffff",
"....########....",
"...#aaaaaaaa#...",
"..#aabbbbbbbc#..",
".#aabbbbbbbbbc#.",
"#aabddbbbbddbbc#",
"#abbdddbbdddbbc#",
"#abbbddddddbbbc#",
"#abbbbddddbbbbc#",
"#abbbbddddbbbbc#",
"#abbbddddddbbbc#",
"#abbdddbbdddbbc#",
"#abbddbbbbddbbc#",
".#cbbbbbbbbbbc#.",
"..#cbbbbbbbbc#..",
"...#cccccccc#...",
"....########...."};

}; //KSimIconDefaults


//#######################################################################################
// Definitions
//#######################################################################################

KSimIcon::tDefaultIcons KSimIcon::defaultIcons[] = {
//const char * name, KIcon::Group group, KIcon::States state, const char * xpm[]
{"back",             KIcon::Small,       KIcon::DefaultState, KSimIconDefaults::back_16x16_default},
{"clear_left",       KIcon::Small,       KIcon::DefaultState, KSimIconDefaults::clear_left_16x16_default},
{"fileclose",        KIcon::Small,       KIcon::DefaultState, KSimIconDefaults::fileclose_16x16_default},
{"fileopen",         KIcon::Small,       KIcon::DefaultState, KSimIconDefaults::fileopen_16x16_default},
{"finish",           KIcon::Small,       KIcon::DefaultState, KSimIconDefaults::finish_16x16_default},
{"forward",          KIcon::Small,       KIcon::DefaultState, KSimIconDefaults::forward_16x16_default},
{"player_pause",     KIcon::Small,       KIcon::DefaultState, KSimIconDefaults::player_pause_16x16_default},
{"1rightarrow",      KIcon::Small,       KIcon::DefaultState, KSimIconDefaults::rightarrow1_16x16_default},
{"start",            KIcon::Small,       KIcon::DefaultState, KSimIconDefaults::start_16x16_default},
{"stop",             KIcon::Small,       KIcon::DefaultState, KSimIconDefaults::stop_16x16_default},

// Last Element !!!!
{(const char *)0 ,   KIcon::Small,       KIcon::DefaultState, (const char **)0}
};


/*KSimIcon::KSimIcon()
{
}
KSimIcon::~KSimIcon()
{
} */


QPixmap KSimIcon::load(const char * name, KIcon::Group group, KIcon::States state)
{
	const tDefaultIcons * defIcon = defaultIcons;

#if defined(DEBUG) && !defined(NO_KSIMDEBUG)
	while((defIcon->name != 0)
	      && (!(0 == strcmp(defIcon->name, name))
	           && (defIcon->group == group)
	           && (defIcon->state == state)))
	{
		defIcon++;
	}

	if (defIcon->name == 0)
	{
		KSIMDEBUG(QString::fromLatin1("FIXME: Icon not in repository! Name=%1 Group=%2, State=%3")
		                              .arg(name)
		                              .arg((int) group)
		                              .arg((int) state));
	}
#endif //defined(DEBUG) && !defined(NO_KSIMDEBUG)

	QPixmap pix(KGlobal::iconLoader()->loadIcon(QString::fromLatin1(name), group, 0, state, 0, true));

	if (!pix.isNull())
		// found
		return pix;

	// Try embedded pixs
#if !(defined(DEBUG) && !defined(NO_KSIMDEBUG))
	while((defIcon->name != 0)
	      && (!(0 == strcmp(defIcon->name, name))
	           && (defIcon->group == group)
	           && (defIcon->state == state)))
	{
		defIcon++;
	}
#endif //!(defined(DEBUG) && !defined(NO_KSIMDEBUG))
	if (defIcon->name != 0)
	{
		KSIMDEBUG(QString::fromLatin1("Use icon from repository! Name=%1 Group=%2, State=%3")
		                            .arg(name)
		                            .arg((int) group)
		                            .arg((int) state));
		return QPixmap(defIcon->xpm);
	}

	// Use unknown pix
	return KIconLoader::unknown();
}


