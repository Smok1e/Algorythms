#pragma once

//---------------------

#define GP_DOTS        0x01 // 0000 0001 | Draw graph points as dots
#define GP_RECTS       0x02 // 0000 0010 | Draw graph points as squares
#define GP_COLUMNS     0x04 // 0000 0100 | Draw graph points as columns
#define GP_LINEAR      0x08 // 0000 1000 | Draw lines between graph points
							// ----------+
#define GP_AUTOSCALE_X 0x10 // 0001 0000 | Graph scale by X defines automatically
#define GP_AUTOSCALE_Y 0x20 // 0010 0000 | Graph scale by Y defines automatically
							//	---------+
#define GP_AUTOSCALE   0x30 // 0011 0000 | Includes GP_AUTOSCALE_X and GP_AUTOSCALE_Y

//---------------------

template <typename Type, size_t Size>
void DrawGraph (Context* context, Type data[Size], int x, int y, int size_x, int size_y, double scale_x, double scale_y, Color color, int r = 5, unsigned flags = GP_DOTS | GP_AUTOSCALE);

void DrawPoint (Context* context, Coord2D point, Coord2D next, int r, double scale_x, double size_y, unsigned flags);

//---------------------

template <typename Type, size_t Size>
void DrawGraph (Context* context, Type data[Size], int x, int y, int size_x, int size_y, double scale_x, double scale_y, Color color, int r /*= 5*/, unsigned flags /*= GP_DOTS | GP_AUTOSCALE*/)
{
	const size_t count = (flags & GP_LINEAR) ? Size - 1 : Size;

	if (flags & GP_AUTOSCALE_X) scale_x = (double) size_x / count;

	Type min = 0;
	if (flags & GP_AUTOSCALE_Y) 
	{
		Type max = data[0];
		     min = max;
		for (size_t i = 1; i < Size; i++)
		{
			if (data[i] > max) max = data[i];
			if (data[i] < min) min = data[i];
		}
		scale_y = size_y / (max-min);
	}

	for (size_t i = 0; i < count; i++)
	{
		context -> setColor     (color, (flags & GP_LINEAR) ? r : 0);
		context -> setFillColor (color);

		Coord2D point = Coord2D (x, y) + Coord2D (i*scale_x, size_y - ((data[i]-min)*scale_y));
		Coord2D next;

		if (flags & GP_LINEAR)
			next = Coord2D (x, y) + Coord2D ((i+1)*scale_x, size_y - (data[i+1]-min)*scale_y);

		DrawPoint (context, point, next, r, scale_x, data[i]*scale_y, flags);
	}
}

//---------------------

void DrawPoint (Context* context, Coord2D point, Coord2D next, int r, double scale_x, double size_y, unsigned flags)
{
	if (flags & GP_DOTS)
		txEllipse   (txCoord (point-r), txCoord (point+r), *context);

	else if (flags & GP_RECTS)
		txRectangle (txCoord (point-r), txCoord (point+r), *context);

	else if (flags & GP_COLUMNS)
		txRectangle (txCoord (point), txCoord (point + Coord2D (scale_x, size_y)), *context);

	else if (flags & GP_LINEAR)
		txLine (txCoord (point), txCoord (next), *context);
}