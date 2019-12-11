#include "doom.h"


int		check_if_visible(t_main *s, t_sprite *sprite)
{
	t_walls	*wall;
	// double	angle;
	t_dpos	fake_sprite;

	wall = s->walls;
	while (wall)
	{
		// printf("sprite->x = %d, wall->x = %d, fin du mur = %d\n", sprite->x, wall->x, wall->x + wall->screen_width_wall);
		if (wall->wall_or_portal == 'w' && wall->sct_id != sprite->sct_id)
		{
			// printf("sprite->angle = %f\nwall->angle=%f\n", sprite->angle, wall->angle);
			// printf("dernier angle = %f\n", angle_mod(sprite->angle + wall->angle));
			fake_sprite.x = wall->player.x + cos(to_rad(angle_mod(sprite->angle + wall->angle))) * (sprite->r_dist * METRE);
			fake_sprite.y = wall->player.y - sin(to_rad(sprite->angle + wall->angle)) * (sprite->r_dist * METRE);
			draw_anchor(s, ft_dpos_to_pos(to_edi_coord(s, fake_sprite)), S_RED);
			if (ft_find_intersection(s, wall->player, fake_sprite,
				wall->left, wall->right, 1))
				return (0);
		}
		// sprite->x >= wall->x && sprite->x <= wall->x + wall->screen_width_wall
		// 	&& ft_find_intersection(s, wall->player, sprite->m_pos,
		// 	wall->left, wall->right, 1))
		// 		return (0);
		wall = wall->next;
	}
	// printf("\n");
	// exit(-1);
	return (1);
}

void 	get_sprite_x(t_main *s, t_visu *vs, t_sprite *sprite)
{
	float		per;
	t_dpos	inter;

	per = 0;
	ft_find_intersection(s, vs->left_plan, vs->right_plan,
		vs->player, sprite->m_pos, 1);
	inter = s->tmp_intersect;
	per = ft_dist_t_dpos(vs->left_plan, inter) * 100 / ft_dist_t_dpos(vs->left_plan, vs->right_plan);
	sprite->x = per * WIDTH / 100 - sprite->anim.image[sprite->anim.current]->w;

}

void 	unset_sprites(t_main *s)
{
	t_sprite *sprite;

	sprite = s->sprite;
	while (sprite)
	{
		sprite->set = 0;
		sprite = sprite->next;
	}
}

void 	set_visible_sprites(t_main *s, t_visu *vs)
{
	t_lsprite 	*liste;
	t_int		*wall;
	int			inter;
	t_dpos		wall1;
	t_dpos		wall2;


	liste = vs->sct->liste;
	if(!liste)
	{
		return ;
	}

	wall = vs->vtx_gauche;
	if (wall == NULL || vs->vtx_droite == NULL)
		return ;
	while (liste)
	{
		inter = 0;
		while (wall->id != vs->vtx_droite->next->id)
		{
			wall1.x = wall->ptr->x * METRE;
			wall1.y = wall->ptr->y * METRE;
			wall2.x = wall->next->ptr->x * METRE;
			wall2.y = wall->next->ptr->y * METRE;
			if (ft_find_intersection(s, wall1, wall2, vs->player, liste->sprite->m_pos, 1))
			{
				// printf("inter\n");
				inter = 1;;
				break;
			}
			wall = wall->next;
		}
		if (inter == 0 && ft_find_intersection(s, vs->left_plan, vs->right_plan,
			vs->player, liste->sprite->m_pos, 1))
		{
			liste->sprite->r_dist = ft_dist_t_dpos(vs->player, liste->sprite->m_pos) / METRE;
			liste->sprite->l_dist = ft_dist_t_dpos(vs->player, s->tmp_intersect);
			get_sprite_x(s, vs, liste->sprite);
			liste->sprite->angle = ft_find_angle_portal(&vs->player, &liste->sprite->m_pos, NULL, 1);
			if (liste->sprite->m_pos.y > vs->player.y)
				liste->sprite->angle = 180 + (180 - liste->sprite->angle);
			liste->sprite->angle = (angle_mod(liste->sprite->angle - vs->angle));
			liste->sprite->set = 1;
		}

		else
		{
			liste->sprite->set = 0;
			// printf("nop\n");
		}

		liste = liste->next;
	}
}

void 	display_sprites(t_main *s)
{
	t_sprite *sprite;

	sprite = s->sprite;
	if (!sprite)
		return ;
	while (sprite)
	{
		// printf("boucle\n");
		if (sprite->set == 1 && sprite->destroy == 0 && check_if_visible(s, sprite))
		{
			// printf("sprite ok\n");
			play_sprites_anims(s);
			draw_sprite(s, sprite->angle, sprite);
		}
		sprite = sprite->next;
	}
}

void	set_sprite(t_main *s)
{
	t_sprite *tmp;

	tmp = s->sprite;
	while (tmp)
	{
		tmp->pos = get_px_r_pos(s, tmp->r_pos);
		tmp->m_pos.x = tmp->r_pos.x * METRE;
		tmp->m_pos.y = tmp->r_pos.y * METRE;
		tmp = tmp->next;
	}
}

int		ft_get_sprite_height_pxl(t_main *s, t_sprite *sprite, double height)
{
	double		pct_eyesight; //pourcentage vision player
	t_sector 	*sct;

	sct = get_sector_by_id(s, sprite->sct_id);

	s->player.eyesight = s->player.foot_height - sct->floor + s->player.size;
	pct_eyesight = (s->player.eyesight * 100 / sprite->size);
	// printf("pct = %.2f      ", pct_eyesight);
	return ((pct_eyesight * height) / 100);
}

void		draw_sprite(t_main *s, double angle, t_sprite *cur)
{
	double		perx;
	double		pery;
	t_pos		coord;
	int			px;
	t_image		*wp;
	int			i;
	int			j;
	double		height;
	double		width;
	double 		pct;
	double		diff_height_pxl;
	int			bottom;
	int			tmp;
	(void)angle;

	wp = cur->anim.image[cur->current];
	pct = (cur->r_dist * METRE * 100) / cur->l_dist;
	height = HEIGHT / ((pct * 0.001) * 4) * cur->size * HEIGHT_MULT;
	pct = (100 * wp->w) / wp->h;
	width = (pct * height) / 100;
	i = 0;
	coord.x = cur->x;
	while (coord.x < cur->x + width && coord.x >= 0 && coord.x <= WIDTH)
	{
		j = 0;
		// coord.x = cur->x + i;
		perx = (double)((coord.x - cur->x) * 100) / (width);
		// printf("perx = %f\n", perx);

		diff_height_pxl = ft_get_sprite_height_pxl(s, cur, height);
		coord.y = (HEIGHT / 2) - height + s->player.y_eye + diff_height_pxl;
		bottom = (HEIGHT / 2) + s->player.y_eye + diff_height_pxl;
		tmp = coord.y;
		while (coord.y < bottom) //J'ai ajouté la protection pour pas que ca rame mais ça fait disparaitre le sprite quand on ets trop pres
		{
			pery = (double)((coord.y - tmp) * 100) / height;
			if (cur->inverse == 0)
				px = (int)(pery / 100 * (double)wp->h) * wp->w
					+ (int)(perx / 100 * (double)wp->w);
			else
				px = (int)(pery / 100 * (double)wp->h) * wp->w
					- (int)(perx / 100 * (double)wp->w);

			if (px >= 0 && px < wp->w * wp->h && wp->tex[px] != 65280)
				set_pixel(s->sdl->game, wp->tex[px], coord);
			coord.y++;
		}
		coord.x++;
	}
}
