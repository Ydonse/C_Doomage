#include "doom.h"

void	draw_editor_menu(t_main *s, double perx, short orig_x, short orig_y)
{
	t_pos		coord;
	t_pos		dest;
	double		pery;
	int			pix_tex;
	t_image		*menu;

	menu = s->editor->menu;
	dest.x = WIDTH / 2 + (menu->w / 2) + 1;
	dest.y = menu->h;
	coord.x = orig_x;
	while (coord.x < dest.x)
	{
		coord.y = orig_y;
		perx = percent(coord.x - orig_x, dest.x - orig_x);
		while (coord.y < dest.y)
		{
			coord.y++;
			pery = percent(coord.y - orig_y, dest.y - orig_y);
			pix_tex = (int)(pery * menu->h - 1) * menu->w + (int)(perx * menu->w);
			set_pixel(s->sdl->editor, menu->tex[pix_tex], coord);
		}
		coord.x++;
	}
}

void	change_mode(t_main *s, int key)
{
	if (key == MOVE)
	{
		s->editor->mode = move;
		ft_reset_color_vertex(s);
	}
	else if (key == WALL)
	{
		s->editor->mode = sector;
		s->editor->color_sector = 2;
		ft_reset_color_vertex(s);
		ft_memdel((void **)&s->str_vtx);
	}
	else if (key == VERTEX)
	{
		s->editor->mode = vertex;
		ft_reset_color_vertex(s);
	}
	else if (key == PLAYER)
	{
		s->editor->mode = player;
		ft_reset_color_vertex(s);
	}
	else if (key == SUPP)
	{
		s->editor->mode = supp;
		ft_reset_color_vertex(s);
	}
	// printf("mode = %d\n", s->editor->mode);
	// printf("mode = %d\n", s->editor->mode);
}

int		ft_find_ref(t_editor *edi, int decal)
{
	// printf("Ref : x %d | y : %d | decalx = %d\n", edi->ref.x, edi->ref.y, edi->decal_x);
	if (decal >= 0 && decal < edi->space)
	{
		// printf("ETAT [1] ");
		return (0);
	}
	else if (decal > 0)
	{
		// printf("ETAT [2] ");
		return ((decal / edi->space) * -1);
	}
	else if (decal < 0)
	{
		// printf("ETAT [3] ");
		// printf("\n(%d / (%d) -1) * -1", decal, edi->space);
		if (decal % edi->space == 0)
			return ((decal / (edi->space)) * -1);
		else
			return ((decal / (edi->space) -1) * -1);
	}
	return (0);
}

void	ft_draw_editor(t_editor *edi, t_texture *tex)
{
	t_pos	coord;
	Uint32	color = 0xaca7a7FF;

	// decal_x = decalage total en pixels entre le point 0,0 et le premier point dans l'ecran.
	// coord.x  decalage en pixels entre le pixel 0 de l'ecran et le premier point de la grille affichée a l'ecran.
	coord.x = edi->decal_x % edi->space;
	coord.y = edi->decal_y % edi->space;

	edi->ref.x = ft_find_ref(edi, edi->decal_x);
	edi->ref.y = ft_find_ref(edi, edi->decal_y);
	// printf("\ncoord.x = %d\ndecal_x = %d\nref.x = %d\n",coord.x , edi->decal_x, edi->ref.x);
	// printf("\ncoord.x = %d\ndecal_x = %d\nref.x = %d\n",coord.x , edi->decal_x, edi->ref.x);
	// printf("coord.y = %d\ndecal_y = %d\nref.y = %d\n\n\n",coord.y , edi->decal_y, edi->ref.y);

	//printf("REF   X %d | Y %d\n",edi->ref.x, edi->ref.y);

	// printf("REF   X %d | Y %d\n\n",edi->ref.x, edi->ref.y);
	// printf("coord.x = %d\n", coord.x);
	if (coord.y < 0)
		coord.y += edi->space;
	while (coord.y < HEIGHT)
	{
		if (coord.x < 0)
			coord.x += edi->space;
		while (coord.x < WIDTH)
		{
			set_pixel(tex, color, coord);
			coord.x += edi->space;
		}
		coord.x = edi->decal_x % edi->space;
		coord.y += edi->space;
	}
}

void	draw_wall(t_main *s, t_pos ori)
{
	s->line.x1 = ori.x;
	s->line.y1 = ori.y;
	get_line(s, WHITE);
}
