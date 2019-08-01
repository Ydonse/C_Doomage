#include "doom.h"

void	ft_draw_column(t_main *s, t_pos coord, int end, Uint32 color)
{
	int i;
	int begin;

	i = 0;
	begin = coord.y;
	coord.y = 0;
	while (i++ < begin)
	{
		set_pixel(s->sdl->game, 0x485050ff, coord);
		coord.y++;
	}
	coord.y--;
	while (coord.y++ < end)
		set_pixel(s->sdl->game, color, coord);
	while (end++ < HEIGHT)
	{
		set_pixel(s->sdl->game, 0xa8b08eff, coord);
		coord.y++;
	}
}

int		ft_draw_wall(t_main *s, int x, int l_height_wall, int r_height_wall, double width_wall)
{
	int		diff_wall;
	double		i;
	int		height_wall;
	t_pos	coord;
	int		bottom;
	double	pct_avcm; //pourcentage avancement

	i = 0;
	diff_wall = abs(l_height_wall - r_height_wall); //calcule de la diff de hauteur entre les deux murs div par deux (pour haut et bas)
	// printf("diff = %d\n",diff_wall);
	height_wall = l_height_wall;
	coord.x = x;
	// printf("Largeur du mur = %f\n", width_wall);
	while (i++ < width_wall)
	{
		coord.y = (HEIGHT / 2) - height_wall / 2; //haut du mur
		bottom = (HEIGHT / 2) + height_wall / 2; //bas du mur
		if (i == 1 || i == width_wall)
			ft_draw_column(s, coord, bottom, 0x000000FF);
		else
			ft_draw_column(s, coord, bottom, 0xb0842fff);
		coord.x++;
		pct_avcm = (100 * i) / width_wall;

		if (l_height_wall < r_height_wall)
			height_wall = l_height_wall + (diff_wall * pct_avcm) / 100;
		else if (l_height_wall > r_height_wall)
			height_wall = l_height_wall - (diff_wall * pct_avcm) / 100;
		else
			height_wall = l_height_wall;
	}
	return (coord.x);
}

int		ft_print_wall(t_main *s, int x, t_dpos player, t_dpos lwall, t_dpos rwall, t_dpos lplan, t_dpos rplan)
{
	double	l_big_dist;
	double	r_big_dist;
	double	l_small_dist;
	double	r_small_dist;
	double	l_pct;
	double	r_pct;
	int		l_height_wall;
	int		r_height_wall;
	double	pct_plan;
	double		width_wall;

	l_big_dist = ft_dist_t_dpos(player, lwall);
	r_big_dist = ft_dist_t_dpos(player, rwall);
	l_small_dist = ft_dist_t_dpos(player, lplan);
	r_small_dist = ft_dist_t_dpos(player, rplan);

	l_pct = (l_big_dist * 100.0) / l_small_dist; //calcule des ratios mur gauche et droit
	r_pct = (r_big_dist * 100.0) / r_small_dist;

	l_height_wall = HEIGHT / ((l_pct * 0.001)*4); //calcule des hauteur des murs gauche et droit
	r_height_wall = HEIGHT / ((r_pct * 0.001)*4);

	pct_plan = (ft_dist_t_dpos(lplan, rplan) * 100.0) / WIDTHPLAN; //calcule de la largeur du mur dans la fenetre
	width_wall = (WIDTH * pct_plan) / 100;

	x = ft_draw_wall(s, x, l_height_wall, r_height_wall, width_wall);
	// printf("Hauteur du mur Gauche (%d)\n",l_height_wall);
	// printf("Hauteur du mur Droit  (%d)\n\n\n",r_height_wall);

	return(x);
}

t_int	*ft_next_vtx(t_int *vtx, t_sector *sct)
{
	vtx = vtx->next;
	if (vtx == NULL)
		vtx = sct->vertex;
	return (vtx);
}

void	ft_draw_visu(t_main *s, t_dpos player, t_visu *vs)
{
	t_dpos		plan_left;
	t_dpos		plan_right;
	int			x;
	t_sector	*sct;
	t_int		*vtx;
	int			id_vtx; //vertex de repere actuel

	x = 0;
	sct = s->sector;
	while (s->player.sector_id != sct->id)
		sct = sct->next;
	vtx = sct->vertex;
	while (vtx->id != vs->begin_wall_id + 1) // trouver le deuxieme vertex du premier mur
	{
		vtx = vtx->next;
		if (vtx == NULL)
		{
			vtx = sct->vertex;
			break ;
		}
	}
	id_vtx = vtx->id;

	if (vs->begin_wall_id != vs->end_wall_id)
	{
		s->visu.tmp_wall.x = vtx->ptr->x * METRE;
		s->visu.tmp_wall.y = vtx->ptr->y * METRE;
	}
	else
	{
		s->visu.tmp_wall.x = vs->end.x;
		s->visu.tmp_wall.y = vs->end.y;
	}

	ft_find_intersection(s, s->visu.tmp_wall, player, s->visu.left_plan, s->visu.right_plan, 1);
	plan_right.x = s->tmp_intersect.x;
	plan_right.y = s->tmp_intersect.y;
	x = ft_print_wall(s, x, player, s->visu.begin, s->visu.tmp_wall, s->visu.left_plan, plan_right);

	if (vs->begin_wall_id == vs->end_wall_id)
		return ;
	// printf("point gauche x = %f, y = %f\n",s->visu.begin.x, s->visu.begin.y);
	// printf("vertex %d, x = %f, y = %f\n", vtx->id, s->visu.tmp_wall.x, s->visu.tmp_wall.y);
	{s->line.x1 = s->visu.tmp_wall.x + s->editor->decal_x;
	s->line.y1 = s->visu.tmp_wall.y + s->editor->decal_y;
	s->line.x2 = plan_right.x + s->editor->decal_x;
	s->line.y2 = plan_right.y + s->editor->decal_y;
	get_line(s, 0xea7cfcff);}

	// printf("ENTREE   id_vtx = %d\nvtx_id = %d\nend_wall_id = %d\n\n",id_vtx, vtx->id, vs->end_wall_id);
	while (id_vtx != vs->end_wall_id)
	{
		s->visu.begin.x = s->visu.tmp_wall.x;
		s->visu.begin.y = s->visu.tmp_wall.y;
		vtx = ft_next_vtx(vtx, sct);
		s->visu.tmp_wall.x = vtx->ptr->x * METRE;
		s->visu.tmp_wall.y = vtx->ptr->y * METRE;


		plan_left.x = plan_right.x;
		plan_left.y = plan_right.y;
		ft_find_intersection(s, s->visu.tmp_wall, player, s->visu.left_plan, s->visu.right_plan, 1);
		plan_right.x = s->tmp_intersect.x;
		plan_right.y = s->tmp_intersect.y;

		{s->line.x1 = s->visu.tmp_wall.x + s->editor->decal_x;
		s->line.y1 = s->visu.tmp_wall.y + s->editor->decal_y;
		s->line.x2 = plan_right.x + s->editor->decal_x;
		s->line.y2 = plan_right.y + s->editor->decal_y;
		get_line(s, 0xea7cfcff);}

		x = ft_print_wall(s, x, player, s->visu.begin, s->visu.tmp_wall, plan_left, plan_right);
		id_vtx = vtx->id;
	}

	s->visu.begin.x = s->visu.tmp_wall.x;
	s->visu.begin.y = s->visu.tmp_wall.y;
	vtx = ft_next_vtx(vtx, sct);
	s->visu.tmp_wall.x = s->visu.end.x;
	s->visu.tmp_wall.y = s->visu.end.y;

	plan_left.x = plan_right.x;
	plan_left.y = plan_right.y;
	ft_find_intersection(s, s->visu.tmp_wall, player, s->visu.left_plan, s->visu.right_plan, 1);
	plan_right.x = s->tmp_intersect.x;
	plan_right.y = s->tmp_intersect.y;

	x = ft_print_wall(s, x, player, s->visu.begin, s->visu.tmp_wall, plan_left, plan_right);

	// printf("wall n%d et vertex du mur = %d\n", vs->begin_wall_id, vtx->id);
	// printf("coordonees du vertex (%.2f, %.2f)\n",s->visu.tmp_wall.x, s->visu.tmp_wall.y);
	// printf("coordonees de l'intersect sur plan (%.2f, %.2f)\n", plan_right.x, plan_right.y);

	// s->line.x1 = s->visu.tmp_wall.x + s->editor->decal_x;
	// s->line.y1 = s->visu.tmp_wall.y + s->editor->decal_y;
	// s->line.x2 = plan_right.x + s->editor->decal_x;
	// s->line.y2 = plan_right.y + s->editor->decal_y;
	// get_line(s, 0xea7cfcff);
	// printf("---------------------------------------------\n\n\n\n");

}