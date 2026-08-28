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

	if (nm->filenames == NULL)
	{
		nm->filenames = calloc(1, sizeof(t_file_name));
		if (nm->filenames == NULL)
		{
			LOG(ERROR "Failed to calloc nm->files !" RESET);
			return ;
		}
		nm->filenames->name = NULL;
		nm->filenames->next = NULL;
	}
	return ;
}

void	add_file_name(char *name, t_nm *nm)
{
	AUTO_LOG;

	t_file_name	*temp = nm->filenames;

	if (nm->filenames == NULL)
	{
		LOG(ERROR "Cannot add a name if nm->files is not initialized !" RESET);
		return ;
	}

	// Got to latest node
	while (temp->next != NULL)
		temp = temp->next;

	// Fill the name on the latest node if its empty
	if (temp->name == NULL)
	{
		temp->name = calloc(strlen(name) + 1, sizeof(char));
		if (temp->name == NULL)	return ;
		strcpy(temp->name, name);
		return ;
	}
	
	// Create the new node
	t_file_name	*new = calloc(1, sizeof(t_file_name));
	if (new == NULL)
	{
		LOG(ERROR "Failed to calloc new node !" RESET);
		return ;
	}

	// Calloc and set the name on our new node
	new->name = calloc(strlen(name) + 1, sizeof(char));
	if (new->name == NULL)
	{
		LOG(ERROR "Failed to calloc new->name !" RESET);
		free(new);
		return ;
	}
	strcpy(new->name, name);

	// Attach the new node to our linked list
	temp->next = new;
	return ;
}

void	list_file_name(t_nm *nm)
{
	AUTO_LOG;

	t_file_name	*temp = nm->filenames;
	if (nm->filenames == NULL || nm->filenames->name == NULL)
	{
		LOG(ERROR "Cannot read a name if nm->files is not initialized !" RESET);
		return ;
	}

	// Read each nodes
	while (temp)
	{
		if (temp->name != NULL) LOG(DEBUG "%s" RESET, temp->name);
		else LOG(DEBUG "Empty." RESET);
		temp = temp->next;
	}
	LOG(DEBUG YELLOW "Size : %d" RESET, nm->files_count);
	return ;
}

void	free_file_name(t_nm *nm)
{
	AUTO_LOG;

	t_file_name	*temp = nm->filenames;
	if (nm->filenames == NULL)
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

	// No dangling pointers
	nm->filenames = NULL;
	return ;
}