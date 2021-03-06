/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   go.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: liabanzh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/18 17:55:40 by liabanzh          #+#    #+#             */
/*   Updated: 2017/10/28 01:11:48 by mponomar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/vm.h"

void			print_map(t_byte *map)
{
	int			i;

	i = 0;
	while (i < MEM_SIZE)
	{
		if (i % 64 == 0)
			printf("\n0x%04x : ", i);
		printf("%02x ", map[i]);
		i++;
	}
	printf("\n\n");
}

void			check_all(t_players *players, t_byte *map,
						t_players **stack, t_flags *flags)
{
	int			i;

	i = 0;
	if (flags->cycles_test >= flags->die)
	{
		if (get_lives(players, *stack) >= NBR_LIVE || flags->max_checks == 10)
		{
			flags->die -= CYCLE_DELTA;
			flags->max_checks = 0;
		}
		else
			flags->max_checks++;
		check_end(players, map, stack, flags);
		flags->cycles_test = 0;
	}
}

void			main_cycle(t_players *players, t_byte *map,
							t_players **stack, t_flags *flags)
{
	int			i;
	t_players	*tmp;

	i = 0;
	if (flags->dump == flags->cycles && flags->visual == 0)
		print_map(map);
	tmp = *stack;
	while (tmp != NULL)
	{
		get_command(tmp, map, stack, players);
		tmp = tmp->next;
	}
	i = flags->amount_players - 1;
	while (i >= 0)
	{
		if (players[i].comands != NULL)
			get_command(&(players[i]), map, stack, players);
		i--;
	}
}

void			go_vm(t_players *players, int count, t_flags *flags)
{
	t_byte		*map;
	t_players	*stack;
	int			i;

	i = 0;
	flags->cycles = 1;
	flags->cycles_test = 1;
	flags->die = CYCLE_TO_DIE;
	flags->max_checks = 0;
	map = get_map(players, count, &(flags->cycles));
	while (players[i].header.prog_name[0] != '\0')
		get_stop(&(players[i++]), map);
	while (flags->die > 0)
	{
		main_cycle(players, map, &stack, flags);
		check_all(players, map, &stack, flags);
		flags->cycles++;
		flags->cycles_test++;
	}
	end_game(players, map, &stack, flags);
}
