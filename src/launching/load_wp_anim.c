#include "doom.h"

t_anim				load_gun_anim(t_anim data)
{
	data.image[0] = load_tga("images/weapons/gun/00.tga", 0, 0, 0);
	data.image[1] = load_tga("images/weapons/gun/01.tga", 0, 0, 0);
	data.image[2] = load_tga("images/weapons/gun/02.tga", 0, 0, 0);
	return (data);
}

t_anim				load_shotgun_anim(t_anim data)
{
	data.image[0] = load_tga("images/weapons/shotgun/shotgun1.tga", 0, 0, 0);
	data.image[1] = load_tga("images/weapons/shotgun/shotgun_fire.tga", 0, 0, 0);
	data.image[2] = load_tga("images/weapons/shotgun/shotgun_fire.tga", 0, 0, 0);
	data.image[3] = load_tga("images/weapons/shotgun/shotgun2.tga", 0, 0, 0);
	data.image[4] = load_tga("images/weapons/shotgun/shotgun3.tga", 0, 0, 0);
	data.image[5] = load_tga("images/weapons/shotgun/shotgun4.tga", 0, 0, 0);
	data.image[6] = load_tga("images/weapons/shotgun/shotgun5.tga", 0, 0, 0);
	return (data);
}

t_anim				load_kick_anim(t_anim data)
{
	data.image[0] = NULL;
	data.image[1] = load_tga("images/weapons/kick/01.tga", 0, 0, 0);
	data.image[2] = load_tga("images/weapons/kick/02.tga", 0, 0, 0);
	data.image[3] = data.image[1];
	return (data);
}

t_anim_wp		load_wp_anims(t_main *s)
{
	ft_bzero((void*)(&s->wp_anims), sizeof(t_anim_wp));
	s->wp_anims.gun = load_gun_anim(s->wp_anims.gun);
	s->wp_anims.shotgun = load_shotgun_anim(s->wp_anims.shotgun);
	s->wp_anims.kick = load_kick_anim(s->wp_anims.kick);
	return (s->wp_anims);
}
