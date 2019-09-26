#include "doom.h"


void 	display_chainlist(t_main *s)
{
	t_sprite *cur;

	cur = s->sprite;
	while (cur != NULL)
	{
		printf("\n=====SPRITE=====\n");

		printf("id = %d\n",cur->id);
		printf("id_anim = %d\n",cur->anim->id);
		printf("destroy = %d\n",cur->destroy);
		printf("slect = %d\n",cur->select);
		printf("dist= %f\n",cur->dist);
		printf("pos (%d,%d)\n",cur->pos.x,cur->pos.y);
		//printf("player.pos (%f,%f)\n",s->player.pos.x,s->player.pos.y);

		cur = cur->next;
	}
}

int 				check_destroy(t_main *s)
{
		t_sprite *cur;

		cur = s->sprite;
		while (cur != NULL)
		{
			if (cur->destroy == 1)
				return (cur->id);
			cur = cur->next;
		}
		return (-1);
}

void        refresh_sprite_pos(t_main *s)
{
  t_sprite *cur;
	int id;

	while ((id = check_destroy(s)) != -1 )
		remove_sprite_by_id(s,id);
  cur = s->sprite;
  while (cur != NULL)
  {
    cur->pos = get_px_r_pos(s,cur->r_pos);
		cur->dist = calc_sprite_dist(s,cur->pos);
//		printf("space= %d\n",s->editor->space);
	//	printf("old_dist= %f\n",cur->dist);
    cur->r_dist = calc_sprite_r_dist(s,cur->r_pos);
		 //printf("r_dist= %f\n\n",cur->r_dist);
		//cur->dist *=s->editor->space;
		cur->set = 0;
    //cur->angle = 0;
		if (cur->select == 1)
			draw_anchor(s,cur->pos, BLUE);
		else
			draw_anchor(s,cur->pos, YELLOW);
    cur = cur->next;
  }
	 // display_chainlist(s);
}

void             init_sprite(t_main *s)
{
  t_dpos      r_pos;

  r_pos.x = 12.5;
  r_pos.y = 6.5;
  s->sprite = create_sprite_elem(s,0,0,r_pos);
  // r_pos.x = 7.5;
  // r_pos.y = 4.2;
  // add_sprite(s,r_pos,0);
  // r_pos.x = 13;
  // r_pos.y = 10;
  // add_sprite(s,r_pos,0);
}

int 				get_sprite_id(t_main *s)
{
	t_sprite *cur;

	cur = s->sprite;
	while (cur != NULL)
	{
		if (cur->select == 1)
			return (cur->id);
		cur = cur->next;
	}
	return (-1);
}

int 				found_id_sprite(t_main *s, t_pos start, t_pos end)
{
	t_sprite *cur;
	int id;

	id = -1;
	cur = s->sprite;
	while (cur != NULL)
	{
		if (cur->pos.x <= start.x && cur->pos.y <= start.y && cur->pos.x >= end.x && cur->pos.y >= end.y)
			return (cur->id);
		cur = cur->next;
	}
	return (id);
}

void 				remove_sprite(t_main *s, t_sprite *cur, t_sprite *next,t_sprite *prev)
{
	t_sprite *tmp;

	tmp = NULL;
	if (prev != NULL )
	{

		tmp = prev;
		tmp->next = next;
		ft_memdel((void **)&cur);
	}
	else if (prev == NULL)
	{
		tmp = next;
		s->sprite = tmp;
		ft_memdel((void **)&cur);
	}
}

void 				reset_id(t_main *s)
{
	t_sprite *cur;
	int id;

	cur = s->sprite;
	id = 0;
	while (cur != NULL)
	{
		cur->id = id;
		cur = cur->next;
		id++;
	}
}

void 				remove_sprite_by_id(t_main *s, int id)
{
	t_sprite *cur;
	t_sprite *prev;

  cur = s->sprite;
	prev = NULL;
	while (cur != NULL)
	{
		if (cur->id == id)
		{
			remove_sprite(s,cur,cur->next,prev);
		}
		prev = cur;
		cur = cur->next;
	}
	if (s->sprite != NULL)
	{
			reset_id(s);
	}
}

void 				remove_sprite_by_select(t_main *s)
{
	t_sprite *cur;
	t_sprite *prev;

  cur = s->sprite;
	prev = NULL;
	while (cur != NULL)
	{
		if (cur->select == 1)
		{
			remove_sprite(s,cur,cur->next,prev);
			cur = s->sprite;
			prev = NULL;
			continue ;
		}
		prev = cur;
		cur = cur->next;
	}
	if (s->sprite != NULL)
	{
		reset_id(s);
	}

}


t_sprite 		*create_sprite_elem(t_main *s, int id, int idimg, t_dpos pos)
{
	t_sprite	*data;
	t_lsprite *lst;
	int i;

	i = -1;
	lst = s->lsprite;
	data = NULL;
	if (!(data = ft_memalloc(sizeof(t_sprite))))
		handle_error(s, MALLOC_ERROR);
	data->r_pos = pos;
  data->pos = get_px_r_pos(s,pos);
  data->id = id;
	data->set = 0;
	data->life = 100;
	data->current = 0;
	data->select = 0;
	data->destroy = 0;
  data->orientation = 0;
	data->angle = 0;
  data->s_angle = 0;
	data->dist = calc_sprite_r_dist(s,data->r_pos);
	data->dist  = calc_sprite_dist(s,data->pos);
	data->img = NULL;
	data->anim = NULL;
	data->next = NULL;
	while (++i != idimg)
		lst = lst->next;
	if (lst->anim != NULL)
	{
		data->anim = lst->anim;
		data->img = data->anim->image[data->current];
	}
	else
		data->img = lst->img;
	return (data);
}

void	add_sprite(t_main *s, t_dpos pos, int idimg)
{
	t_sprite *tmp;

	if (s->sprite == NULL)
	{
		s->sprite = create_sprite_elem(s,0,idimg,pos);
		return ;
	}
	tmp = s->sprite;
	while (tmp->next != NULL)
	 	tmp = tmp->next;
	tmp->next = create_sprite_elem(s,(tmp->id+1),idimg,pos);
}
