#include "doom.h"

// t_visu		fill_visu_values_2(t_main *s, t_visu fake_vs, double fake_angle)
// {
// 	double	demi_fov;
//
// 	demi_fov = ft_find_angle_plan(ft_dist_t_dpos(fake_vs.player,
// 		fake_vs.left_plan), METRE, WIDTHPLAN / 2);
// 	fake_vs.angle = angle_mod(fake_angle);
// 	fake_vs = get_walls_to_draw(s, fake_vs.player, demi_fov, fake_vs);
// 	return (fake_vs);
// }

t_visu		fill_visu_values(t_main *s, t_visu *vs, t_int *vtx)
{
	double	fake_angle;
	t_visu	fake_vs;
	t_dpos	fake_player;

	fake_angle = 0;
	fake_player = ft_get_fake_player(s, vs->player, vtx, &fake_angle, vs->angle);
	fake_vs = ft_place_view_plan(s, fake_player, fake_angle, 0x4bd9ffff); // #4bd9ff
	fake_vs.player = fake_player;
	fake_vs.prev_sct_id = vtx->sct;
	fake_vs.sct_id = vtx->sct_dest;
	fake_vs.sct = get_sector_by_id(s, vtx->sct_dest);
	fake_vs.angle = angle_mod(fake_angle);
	fake_vs = get_walls_to_draw(s, fake_vs.player, s->player.demi_fov, fake_vs);
	fake_vs.vtx_droite = vtx->vtx_dest;
	if (fake_vs.vtx_droite == NULL)
		handle_error(s, POINTER_ERROR);
	fake_vs.vtx_gauche = vtx->vtx_dest->next;
	return (fake_vs);
}

void		handle_visu_portal(t_main *s, t_int *vtx, t_visu *vs, int swich)
{
	t_visu	fake_vs;
	t_dpos	wall1;
	t_dpos	wall2;

	fake_vs = fill_visu_values(s, vs, vtx);
	wall1 = vtx->vtx_dest->next->ptr->m_pos;
	wall2 = vtx->vtx_dest->ptr->m_pos;
	ft_find_intersection(s, wall1, wall2, fake_vs.left_point, fake_vs.player, 1);
	if ((ft_find_intersection(s, wall1, wall2, fake_vs.left_point,
		fake_vs.player, 1)) == 0)
		fake_vs.begin = fake_vs.vtx_gauche->ptr->m_pos;
	else
		fake_vs.begin = s->tmp_intersect;
	if ((ft_find_intersection(s, wall1, wall2, fake_vs.right_point,
		fake_vs.player, 1)) == 0)
		fake_vs.end = fake_vs.vtx_droite->ptr->m_pos;
	else
		fake_vs.end = s->tmp_intersect;
	ft_limit_ceiling_floor(s, wall1, wall2, &fake_vs, swich);
	ft_create_new_wall(s, vtx, vs, 'p');
	if (s->portal_nb < PORTAL_LIMIT)
		add_portal_to_list(s, fake_vs.player, fake_vs.sct, fake_vs);
}

void		create_all_walls(t_main *s, t_int *vtx, t_visu *vs, int end)
{
	while (end)
	{
		if (vtx->wall_value != -1)
		{
			if (vtx->ptr->id == vs->begin_wall_id)
			{
				if (vtx->ptr->id == vs->end_wall_id)
					handle_visu_portal(s, vtx, vs, 4);
				else
					handle_visu_portal(s, vtx, vs, 1);
			}
			else if (vtx->ptr->id == vs->end_wall_id)
				handle_visu_portal(s, vtx, vs, 3);
			else
				handle_visu_portal(s, vtx, vs, 2);
		}
		else
		{
			vs->begin = vtx->ptr->m_pos;
			vs->tmp_wall = vtx->next->ptr->m_pos;
			ft_create_new_wall(s, vtx, vs, 'w');
			if (vtx->ptr->id == vs->end_wall_id)
				ft_find_intersection(s, vs->begin, vs->player, vs->left_plan, vs->right_plan, 1);
		}
		if ((vtx->ptr->id == vs->begin_wall_id && vs->begin_wall_id
			== vs->end_wall_id) || vtx->ptr->id == vs->end_wall_id)
			end = 0;
		vtx = vtx->next;
	}
}
