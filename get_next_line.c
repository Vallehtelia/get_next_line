/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvaalant <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 13:30:07 by vvaalant          #+#    #+#             */
/*   Updated: 2023/12/01 13:30:09 by vvaalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*get_next_line(int fd)
{
	char		*line;
	static char	*left_str;
	char		*temp;

	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, &line, 0) < 0)
	{
		free(left_str);
		left_str = NULL;
		return (NULL);
	}
	left_str = read_to_left_str(fd, left_str);
	if (!left_str || !*left_str)
	{
		free(left_str);
		left_str = NULL;
		return (NULL);
	}
	line = fetch_line(left_str);
	temp = new_left_str(left_str);
	if (left_str != temp)
	{
		free(left_str);
		left_str = NULL;
	}
	left_str = temp;
	return (line);
}

char	*read_to_left_str(int fd, char *left_str)
{
	char	*buf;
	int		rd_bytes;

	buf = malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!buf)
	{
		free(buf);
		buf = NULL;
		return (NULL);
	}
	rd_bytes = 1;
	while (!is_newline(left_str) && rd_bytes != 0)
	{
		rd_bytes = read(fd, buf, BUFFER_SIZE);
		if (rd_bytes < 0)
		{
			free(buf);
			buf = NULL;
			return (NULL);
		}
		buf[rd_bytes] = '\0';
		left_str = strjoin(left_str, buf);
	}
	free(buf);
	if (rd_bytes == 0 && (!left_str || !*left_str))
	{
		free(left_str);
		left_str = NULL;
		return (NULL);
	}
	return (left_str);
}

int	line_length(char *left_str)
{
	int	len;

	if (left_str == NULL)
		return (0);
	len = 0;
	while (left_str[len] && left_str[len] != '\n')
		len++;
	if (left_str[len] == '\n')
		return (len + 1);
	else
		return (len);
}
