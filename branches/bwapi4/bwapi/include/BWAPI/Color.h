#pragma once
#include <string>
#include <set>
#include "Type.h"
namespace BWAPI
{
	// TODO: Add color palette image and info about text color
	/** StarCraft uses a 256 color palette to render everything, so the colors we can use to draw shapes using
	 * BWAPI is limited to the colors available in the Palette. */
	class Color : public Type<Color, 255>
	{
		public:
			/** Create a color using the specified index from the Broodwar color palette. */
			Color(int id = 0);

			/** Create a color using the color in the palette that is closest to the RGB color specified. */
			Color(int red, int green, int blue);

			/** Return the red component of the color. */
			int red() const;

			/** Return the green component of the color. */
			int green() const;

			/** Return the blue component of the color. */
			int blue() const;
	};

	/** While any color from the palette can be used, the following colors are available as short cuts. */
	namespace Colors
	{
		void init();

		static const Color Red(111);
		static const Color Blue(165);
		static const Color Teal(159);
		static const Color Purple(164);
		static const Color Orange(179);
		static const Color Brown(19);
		static const Color White(255);
		static const Color Yellow(135);
		static const Color Green(117);
		static const Color Cyan(128);
		
		static const Color Black(0);
		static const Color Grey(74);
	}
	namespace Text
	{
		enum Enum
		{
			Previous		= 1,
			Default			= 2,
			Yellow			= 3,
			White			= 4,
			Grey			= 5,
			DarkRed			= 6,
			Green			= 7,
			Red				= 8,
			Invisible		= 11,
			Blue			= 14,
			Teal			= 15,
			Purple			= 16,
			Orange			= 17,
			Align_Right		= 18,
			Align_Center	= 19,
			Invisible2		= 20,
			Brown			= 21,
			PlayerWhite		= 22,
			PlayerYellow	= 23,
			DarkGreen		= 24,
			LightYellow		= 25,
			Cyan			= 26,
			Tan				= 27,
			GreyBlue		= 28,
			GreyGreen		= 29,
			GreyCyan		= 30,
			Turquoise		= 31
		};
	}
}
