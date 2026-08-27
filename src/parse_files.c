/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_files.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldalmass <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/27 18:25:34 by ldalmass          #+#    #+#             */
/*   Updated: 2026/08/27 18:2727:1515 by ldalmass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_nm.h"

void	init_file_name(t_nm *nm)
{
	AUTO_LOG;

	if (nm->files == NULL)
	{
		nm->files = calloc(1, sizeof(t_file_name));
		if (nm->files == NULL)
		{
			LOG(ERROR "Failed to calloc nm->files !" RESET);
			return ;
		}
		nm->files->name = NULL;
		nm->files->next = NULL;
	}
	return ;
}

void	add_file_name(char *name, t_nm *nm)
{
	AUTO_LOG;

	t_file_name	*temp = nm->files;

	if (nm->files == NULL)
	{
		LOG(ERROR "Cannot add a name if nm->files is not initialized !" RESET);
		return ;
	}

	// Got to latest node
	while (temp->next != NULL)
		temp = temp->next;

	// Calloc and set the name
	if (temp->name == NULL)
		temp->name = calloc(strlen(name), sizeof(char));
	else
	{
		LOG(ERROR "Data already exist in temp->name !" RESET);
		return ;
	}
	if (temp->name == NULL)
	{
		LOG(ERROR "Failed to calloc temp->name !" RESET);
		return ;
	}
	temp->name = name;
	temp->next = NULL;
	return ;
}

void	list_file_name(t_nm *nm)
{
	AUTO_LOG;

	t_file_name	*temp = nm->files;
	if (nm->files == NULL)
	{
		LOG(ERROR "Cannot read a name if nm->files is not initialized !" RESET);
		return ;
	}

	// Read each nodes
	while (temp->next)
	{
		LOG(DEBUG "%s" RESET, temp->name);
		temp = temp->next;
	}
	return ;
}

void	free_file_name(t_nm *nm)
{
	AUTO_LOG;

	t_file_name	*temp = nm->files;
	if (nm->files == NULL)
	{
		LOG(INFO "nm->files was already freed or NULL !" RESET);
		return ;
	}

	// Free each node
	while (temp)
	{
		t_file_name	*next = temp->next;
		if (temp->name) free(temp->name);
		free(temp);
		temp = next;
	}
	return ;
}