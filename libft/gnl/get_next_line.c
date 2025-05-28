/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: camerico <camerico@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 15:25:50 by camerico          #+#    #+#             */
/*   Updated: 2025/02/21 18:36:05 by camerico         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*get_next_line(int fd)
{
	char		*line;
	static char	buffer[BUFFER_SIZE + 1];

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	line = NULL;
	line = ft_strdup_free(buffer);
	line = read_to_buffer(buffer, fd, line);
	if (line == NULL)
		return (free(line), NULL);
	return (line);
}

char	*read_to_buffer(char *buffer, int fd, char *line)
{
	int	b_read;

	b_read = 1;
	while ((!ft_strchr_gnl(buffer, '\n')) && b_read != 0)
	{
		b_read = read(fd, buffer, BUFFER_SIZE);
		if (b_read < 0)
		{
			buffer[0] = '\0';
			return (free(line), NULL);
		}
		buffer[b_read] = '\0';
		line = ft_strjoin_free(line, buffer);
		if (!line[0])
			return (free(line), NULL);
	}
	fill_stash(line, buffer);
	return (line);
}

void	fill_stash(char *line, char *buffer)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (line[i] != '\n' && line[i])
		i++;
	if (line[i] == '\n')
		i++;
	while (line[i])
	{
		buffer[j] = line[i];
		line[i] = '\0';
		i++;
		j++;
	}
	buffer[j] = '\0';
}

// int	main(void)
// {
// 	int fd;
// 	char *str;

// 	fd = open("test3.txt", O_RDONLY);
// 	if (fd < 0)
// 	{
// 		printf("%s", "une erreur s'est produite dans l'ouverture du fichier");
// 		return (1);
// 	}
// 	str = get_next_line(fd);
// 	printf("%s", str);
// 	free(str);
// 	str = get_next_line(fd);
// 	printf("%s", str);
// 	free(str);
// 	str = get_next_line(fd);
// 	printf("%s", str);
// 	free(str);
// 	// str = get_next_line(fd);
// 	// printf("%s", str);
// 	// free(str);
// 	close(fd);
// 	return (0);
// }

// int	main(void)
// {
// 	int fd;
// 	char *str;

// 	fd = open("test3.txt", O_RDONLY);
// 	if (fd < 0)
// 	{
// 		printf("%s", "une erreur s'est produite dans l'ouverture du fichier");
// 		return (1);
// 	}
// 	while((str = get_next_line(fd)) != 0)
// 	{
// 		printf("%s", str);
// 		free(str);
// 	}
// 	close(fd);
// 	return (0);
// }

// int	main(void)
// {
// 	int fd;
// 	char *str;
// 	int line = 1;

// 	fd = open("test3.txt", O_RDONLY);
// 	if (fd < 0)
// 	{
// 		printf("%s", "une erreur s'est produite dans l'ouverture du fichier");
// 		return (1);
// 	}
// 	while(line <= 3)
// 	{
// 		str = get_next_line(fd);
// 		printf("%s", str);
// 		free(str);
// 		line++;
// 	}
// 	close(fd);
// 	return (0);
// }