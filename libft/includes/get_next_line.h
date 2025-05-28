/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: camerico <camerico@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 16:19:26 by camerico          #+#    #+#             */
/*   Updated: 2025/02/21 18:39:15 by camerico         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 5

# endif

/* ************************************************************************** */
/* INCLUDES                                                                   */
/* ************************************************************************** */

# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <strings.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <unistd.h>
# include "libft.h"

/* ************************************************************************** */
/* DEFINES                                                                    */
/* ************************************************************************** */

char	*get_next_line(int fd);
char	*read_to_buffer(char *buffer, int fd, char *line);
void	fill_stash(char *line, char *buffer);
// int		ft_strlen(char *s);
char	*ft_strjoin_free(char *s1, char *s2);
int		ft_strchr_gnl(const char *s, char c);
char	*ft_strdup_free(char *s);

/* ************************************************************************** */
/* STRUCTURES                                                                 */
/* ************************************************************************** */

/* ************************************************************************** */
/* PROTO                                                                      */
/* ************************************************************************** */

#endif