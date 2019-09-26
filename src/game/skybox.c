#include "doom.h"

 void			display_sky(t_main *s)
{
	double		perx;
	double		pery;
	t_pos		coord;
	int			px;
	t_image 	*wp;

	wp = s->skybox;
	coord.x = 0;
	while (coord.x++ < WIDTH)
	{
		coord.y = 0;
		perx = (double)coord.x / (double)WIDTH;
		while (coord.y++ < HEIGHT)
		{
			pery = (double)coord.y / (double)HEIGHT;
			px = (int)(pery * (double)wp->h) * wp->w + (int)(perx * (double)wp->w);
			if (px >= 0 && px < wp->w * wp->h)
				set_pixel(s->sdl->game, wp->tex[px], coord);
		}
	}
}

void 	calcul_angle(t_main *s)
{
	
}
