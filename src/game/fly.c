#include "doom.h"

void 	reset_jetpack(t_main *s)
{
	t_sprite *sprite;

	sprite = s->sprite;
	while (sprite)
	{
		if (sprite->name == jetpack)
			sprite->destroy = 0;
		sprite = sprite->next;
	}
}

char	*get_jetpack_value(t_main *s)
{
	char *str;
	char *str2;
	char *str3;
	int	pct;

	str = ft_strdup("jetpack:");
	pct = s->time->jetpack_reserve * 100 / JET_TIME;
	if (pct < 2)
		reset_jetpack(s);
	str2 = ft_itoa(pct);
	str3 = ft_strjoin_free(&str, &str2);
	str = ft_strdup("%");
	return (ft_strjoin_free(&str3, &str));
}

void	ft_fly_mode(t_main *s, const Uint8 *keys)
{
	// printf("ft_fly\n");
	if (keys[SDL_SCANCODE_BACKSLASH])
	{
		// printf("%.1f + %.1f + 0.2 < plafond %d\n", s->player.foot_height, s->player.size, s->player.ceiling_height);
		if (s->player.foot_height + s->player.size + 0.2 < s->player.ceiling_height)
			s->player.jump_height += 0.2;
		// printf("fly up\n");
	}
	if (keys[SDL_SCANCODE_DELETE])
	{
		// printf("%.1f - 0.2 > sol %d\n", s->player.foot_height, s->player.floor_height);
		if (s->player.foot_height - 0.2 > s->player.floor_height)
			s->player.jump_height -= 0.2;
		// printf("fly down\n");
	}
}

void	ft_activ_fly(t_main *s)
{
	if (!s->player.fly)
	{
		s->player.fly = 1;
		ft_create_message(s, 0, 1000, "Fly active");
	}
	else
	{
		s->player.fly = 0;
		ft_create_message(s, 0, 1000, "Fly down");
		if (s->player.foot_height > s->player.floor_height)
			s->player.jump = 3;
	}
}

void 	fly(t_main *s)
{
 	if (s->player.jetpack == 1 && s->player.fly == 1)
	{
		if (s->player.foot_height + s->player.size + 0.2 < s->player.ceiling_height)
			s->player.jump_height += 0.2;
		if (s->time->time_ms - s->time->jetpack_ms >= 100)
		{
			s->time->jetpack_reserve -= 100;
			if (s->time->jetpack_reserve <= 0)
			{
				s->player.jetpack = 0;
				s->time->jetpack_reserve = JET_TIME;
			}
			s->time->jetpack_ms = s->time->time_ms;
		}
	}
	else
		s->time->jetpack_ms = s->time->time_ms;
}
