#include "doom.h"

void		ft_transition(t_main *s)
{
	int diff_y_eye;
	double diff_abs_angle;

	// if (s->transition == 1)
	// printf("\n--\nangle au debut %d\n", s->transition_angle);
	// printf("angle player %.1f\n", s->player.angle);
	// printf("angle abs %.1f\n", s->player.abs_angle);

	diff_y_eye = abs(s->transition_y_eye - MOON_Y) / TRANSITION_SPEED;
	diff_abs_angle = (double)(abs(s->transition_angle - 70)) / TRANSITION_SPEED;
	// printf("diff_abs_angle %.2f(%.2f)\n", diff_abs_angle * TRANSITION_SPEED, diff_abs_angle);

	if (s->transition_angle < 70)
	{
		s->player.angle += diff_abs_angle;
		s->player.abs_angle += diff_abs_angle;
		// s->player.angle = fmod(s->player.angle, 360);
		// if (s->player.angle < 0)
		// 	s->player.angle += 360;
		// s->player.abs_angle = fmod(s->player.abs_angle, 360);
		// if (s->player.abs_angle < 0)
		// 	s->player.abs_angle += 360;
	}
	else
	{
		s->player.angle -= diff_abs_angle;
		s->player.abs_angle -= diff_abs_angle;
		// s->player.angle = fmod(s->player.angle, 360);
		// if (s->player.angle < 0)
		// 	s->player.angle += 360;
		// s->player.abs_angle = fmod(s->player.abs_angle, 360);
		// if (s->player.abs_angle < 0)
		// 	s->player.abs_angle += 360;
	}

	if (s->transition_y_eye < MOON_Y)
		s->player.y_eye += diff_y_eye;
	else
		s->player.y_eye -= diff_y_eye;

	if (s->transition++ > TRANSITION_SPEED)
		s->transition = 0;
}

void		ft_zoom(t_main *s, t_pos mouse, int space)
{
	t_pos		ori;
	t_pos		p_ori;
	t_pos		diff;
	t_dpos		r_pos;
	t_editor	*edi;

	edi = s->editor;
	ori.x = arround(s->editor->space, mouse.x
		- (s->editor->decal_x % s->editor->space));
	ori.y = arround(s->editor->space, mouse.y
		- (s->editor->decal_y % s->editor->space));
	p_ori.x = mouse.x - ori.x;
	p_ori.y = mouse.y - ori.y;
	ori = get_abs_pos(s, ori);
	r_pos.x = (double)ori.x + ((double)p_ori.x / edi->space);
	r_pos.y = (double)ori.y + ((double)p_ori.y / edi->space);
	s->player.pos.x -= s->editor->decal_x;
	s->player.pos.y -= s->editor->decal_y;
	s->player.pos.x /= (double)(s->editor->space);
	s->player.pos.y /= (double)(s->editor->space);
	s->editor->space += space;
	s->player.pos.x *= (double)(s->editor->space);
	s->player.pos.y *= (double)(s->editor->space);
	diff.x = r_pos.x * (-space);
	diff.y = r_pos.y * (-space);
	s->editor->decal_x += diff.x;
	s->editor->decal_y += diff.y;
	s->player.pos.x += s->editor->decal_x;
	s->player.pos.y += s->editor->decal_y;
}

void		rotate_mouse(t_main *s)
{
	double	angle;

	angle = s->player.angle + (-s->sdl->event.motion.xrel)
		* ROTATE_SPEED / 50 + 360;
	s->player.angle = (int)angle % 360;
	s->player.abs_angle = (int)(s->player.abs_angle + + (-s->sdl->event.motion.xrel)
		* ROTATE_SPEED / 50 + 360) % 360;
	s->player.y_eye = (s->player.y_eye + (-s->sdl->event.motion.yrel)
		* ROTATE_SPEED / 10);
	if (s->player.y_eye > 1000)
		s->player.y_eye = 1000;
	else if (s->player.y_eye < -1000)
		s->player.y_eye = -1000;
}

void	ft_crouch(t_main *s, const Uint8 *keys)
{
	if (keys[LCTRL])
	{
		if ((s->time->time_ms > s->time->crouch_ms + 25) && s->player.size > 1.05)
		{
			s->player.size -= 0.1;
			s->time->crouch_ms = s->time->time_ms;
		}
	}
	else
	{
		if ((s->time->time_ms > s->time->crouch_ms + 25) && s->player.size < PLAYER_SIZE)
		{
			s->player.size += 0.1;
			s->time->crouch_ms = s->time->time_ms;
		}
	}
}

void	ft_jump(t_main *s, const Uint8 *keys)
{
	//pour player->jump, 0 = pas de jump, 1 = phase montante, 2 = descendante, 3 = chute apres fly
	// printf("jump = %d, jump_height = %f, jump size = %f\ntime = %ld, j_time + 10 = %ld\n",s->player.jump, s->player.jump_height, JUMP_SIZE, s->time->time_ms, s->time->jump_ms + 10);

	if (s->player.fly == 0 && (s->player.jetpack == 0 || s->play_or_editor == 1) && keys[SDL_SCANCODE_SPACE] && s->player.jump == 0 && (s->player.size
		+ JUMP_SIZE - 0.1 <= s->player.ceiling_height - s->player.floor_height))
			s->player.jump = 1;
	if (s->player.jump == 1 && (s->time->time_ms > s->time->jump_ms + 10))
	{
		if (s->player.jump_height < 0.5)
			s->player.jump_height += 0.2;
		else
			s->player.jump_height += 0.1;
		s->time->jump_ms = s->time->time_ms;
		if (s->player.jump_height >= JUMP_SIZE)
			s->player.jump = 3;
	}
	if (s->player.jump == 2 && (s->time->time_ms > s->time->jump_ms + 10)
	&& s->player.jump_height + 0.1 > 0)
	{
		if (s->player.jump_height < 0.7)
			s->player.jump_height -= 0.2;
		else
			s->player.jump_height -= 0.1;
		s->time->jump_ms = s->time->time_ms;
		if (s->player.jump_height <= 0)
		{
			s->player.jump = 0;
			s->player.jump_height = 0;
		}
	}
	if (s->player.jump == 3 && (s->time->time_ms > s->time->jump_ms + 10)
	&& s->player.jump_height > 0)
	{
		s->player.jump_height -= 0.6 * s->player.tumble;
		s->player.tumble += 0.15;
		s->time->jump_ms = s->time->time_ms;
		if (s->player.jump_height <= 0)
		{
			s->player.tumble = 0;
			s->player.jump = 0;
			s->player.jump_height = 0;
		}
	}
}
