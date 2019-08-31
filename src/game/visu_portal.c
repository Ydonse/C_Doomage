#include "doom.h"

int		check_walls_lenght(t_int *wall1, t_int *wall2)
{
	t_dpos beg1;
	t_dpos beg2;
	t_dpos end1;
	t_dpos end2;

	end1 = ft_pos_to_dpos(wall1->next->ptr->pos);
	end2 = ft_pos_to_dpos(wall2->next->ptr->pos);
	beg1 = ft_pos_to_dpos(wall1->ptr->pos);
	beg2 = ft_pos_to_dpos(wall2->ptr->pos);
	if (ft_dist_t_dpos(beg1, end1) == ft_dist_t_dpos(beg2, end2))
		return (1);
	else
		return (0);
}

//fonction qui revoie l'angle du point needed, 1 pour left, 2 pour right, 3 pour third
double		ft_find_angle_portal(t_dpos *left, t_dpos *right, t_dpos *third, int needed)
{
	//third est le troisieme point pour trouver l'angle si besoin;
	double	angle = 0;
	double	left_right_dist;
	double	right_third_dist;
	double	left_third_dist;
	t_dpos	tmp_third;

	if (third == NULL)
	{
		third = &tmp_third;
		third->x = left->x + 100;
		third->y = left->y;
	}
	left_third_dist = ft_dist_t_dpos(*third, *left);
	right_third_dist = ft_dist_t_dpos(*third, *right);
	left_right_dist = ft_dist_t_dpos(*left, *right);
	if (needed == 1)
		angle = ft_find_angle_plan(left_third_dist, left_right_dist, right_third_dist);
	else if (needed == 2)
		angle = ft_find_angle_plan(left_right_dist, right_third_dist, left_third_dist);
	else if (needed == 3)
		angle = ft_find_angle_plan(left_third_dist, right_third_dist, left_right_dist);
	return (angle);
}

// t_visu		ft_get_fake_vs(t_main *s, t_dpos fake_player, double fake_angle)
// {
// 	t_visu	fake_vs;
//
// 	fake_vs = ft_place_view_plan(s, fake_player, fake_angle, 0x4bd9ffff);
// }

double		ft_get_fake_angle(t_main *s, t_dpos player, t_int *vtx)
{
	(void)s;
	double		angle_fake_player;
	double		angle_portal_in;
	double		angle_portal_out;
	t_dpos		l_portal;
	t_dpos		r_portal;
	t_dpos		lwall;
	t_dpos		rwall;

	lwall.x = vtx->ptr->x * METRE;
	lwall.y = vtx->ptr->y * METRE;
	rwall.x = vtx->next->ptr->x * METRE;
	rwall.y = vtx->next->ptr->y * METRE;

	//trouver le sector dans lequel amene le portail et les coordonees des vtx du portal
	l_portal.x = vtx->vtx_dest->ptr->x * METRE;
	l_portal.y = vtx->vtx_dest->ptr->y * METRE;
	r_portal.x = vtx->vtx_dest->next->ptr->x * METRE;
	r_portal.y = vtx->vtx_dest->next->ptr->y * METRE;

	//placer et orienter le fake joueur (modif player)
	angle_fake_player = ft_find_angle_portal(&lwall, &rwall, &player, 1);

	//trouver langle du portail d'entree
	angle_portal_in = ft_find_angle_portal(&lwall, &rwall, NULL, 1);
	if (rwall.y > lwall.y)
		angle_portal_in = 180 + (180 - angle_portal_in);

	//trouver langle du portail de sortie
	angle_portal_out = ft_find_angle_portal(&l_portal, &r_portal, NULL, 1);
	if (r_portal.y > l_portal.y)
		angle_portal_out = 180 + (180 - angle_portal_out);

	// printf("angle PLAYER  (%f)\n",angle_fake_player);
	// printf("angle IN      (%f)\n",angle_portal_in);
	// printf("angle OUT     (%f)\n\n",angle_portal_out);

	//application de la difference d'angle des deux portail sur angle player
	angle_fake_player = angle_portal_in - angle_fake_player - fabs(angle_portal_in - angle_portal_out);
	return (angle_fake_player);
	// modifier angle et fake player
}

void		add_portal_to_list(t_main *s, t_dpos player, t_sector *sct, t_visu vs)
{
	t_walls		*tmp;
	t_dpos		plan_left;
	int			x;
	t_int		*vtx;
	int			new_x;

	x = 0;
	new_x = 0;
	// printf("sct->vertex")
	vtx = sct->vertex;
	printf("vs.begin_wall_id = %d\n", vs.begin_wall_id);
	printf("vs.end_wall_id = %d\n", vs.end_wall_id);
	//On recuper el'angle entre le joueur et le point de gauche, ca donne le mur de gauche.
	//On recuper el'angle entre le joueur et le point de droite, ca donne le mur de droite.
	vtx = get_t_int_by_vertex_id(vtx, vs.begin_wall_id);
	draw_first_wall(s, vtx, &vs);

	plan_left = s->tmp_intersect;
	// printf("end wall id = %d\n", get_t_int_by_vertex_id(sct->vertex, vs.end_wall_id)->id);
	if (vs.begin_wall_id == vs.end_wall_id) // cas 1 seul mur
	{
		tmp = s->walls;
		ft_print_wall(s, tmp->x, player, tmp->left, tmp->right, tmp->l_plan, tmp->r_plan);
		return ;
	}

	vtx = vtx->next;
	vtx = draw_mid_walls(s, vtx, &vs);

	draw_last_wall(vtx, &vs);
	ft_find_intersection(s, vs.begin, vs.player, vs.left_plan, vs.right_plan, 1);
	// plan_left = s->tmp_intersect;
	// x = (ft_dist_t_dpos(vs.left_plan, plan_left) / WIDTHPLAN) * WIDTH;
	ft_create_new_wall(s, vtx, &vs);

	// print_wall_list(s);
}

t_dpos		ft_get_fake_player(t_main *s, t_dpos player, t_int *vtx, double *angle_fake)
{
	(void)s;
	t_dpos		l_portal;
	t_dpos		r_portal;
	t_dpos		fake_player;
	double		fake_angle;
	double		angle_portal_in;
	double		angle_portal_out;
	t_pos		fake_player2; //juste pour affichage du faek player
	double		dist_player;
	t_dpos		lwall;
	t_dpos		rwall;

	lwall.x = vtx->ptr->x * METRE;
	lwall.y = vtx->ptr->y * METRE;
	rwall.x = vtx->next->ptr->x * METRE;
	rwall.y = vtx->next->ptr->y * METRE;

	//trouver le sector dans lequel amene le portail et les coordonees des vtx du portal
	l_portal.x = vtx->vtx_dest->ptr->x * METRE;
	l_portal.y = vtx->vtx_dest->ptr->y * METRE;
	r_portal.x = vtx->vtx_dest->next->ptr->x * METRE;
	r_portal.y = vtx->vtx_dest->next->ptr->y * METRE;

	//trouver la distance entre joueur et le point gauche du portail initial
	dist_player = ft_dist_t_dpos(player, lwall);

	fake_angle = ft_find_angle_portal(&lwall, &rwall, &player, 1);

	//trouver langle du portail d'entree
	angle_portal_in = ft_find_angle_portal(&lwall, &rwall, NULL, 1);
	if (rwall.y > lwall.y)
		angle_portal_in = 180 + (180 - angle_portal_in);

	//trouver langle du portail de sortie
	angle_portal_out = ft_find_angle_portal(&l_portal, &r_portal, NULL, 1);
	if (r_portal.y > l_portal.y)
		angle_portal_out = 180 + (180 - angle_portal_out);


	// printf("angle PLAYER  (%f)\n",angle_fake_player);
	printf("angle IN      (%f)\n",angle_portal_in);
	printf("angle OUT     (%f)\n\n",angle_portal_out);
	*angle_fake = s->player.angle + (angle_portal_in - angle_portal_out);

	//application de la difference d'angle des deux portail sur angle player
	fake_angle = angle_portal_in - fake_angle - fabs(angle_portal_in - angle_portal_out);
	//Tentative de placement du fake player
	fake_player.x = r_portal.x + cos(to_rad(fake_angle)) * dist_player;
	fake_player.y = r_portal.y - sin(to_rad(fake_angle)) * dist_player;
	printf("\n\nPOSITION DU FAKE PLAYER (%.1f, %.1f)\n", fake_player.x, fake_player.y);

	//ATTENTION !!!! jai ajouter le decal_x pour que le point saffiche a lecran pour mes tests.

	// printf("coord r_portal (%.1f,%.1f)\n\n",r_portal.x, r_portal.y);
	// printf("coord player (%d,%d)\n\n",fake_player.x, fake_player.y);
	fake_player2.x = fake_player.x + s->editor->decal_x;
	fake_player2.y = fake_player.y + s->editor->decal_y;
	draw_anchor(s, fake_player2, 0xfa00ffff); //juste pour tester la pos du fake_player

	return (fake_player);
}
