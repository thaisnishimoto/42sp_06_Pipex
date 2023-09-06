/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tester_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmina-ni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 15:06:15 by tmina-ni          #+#    #+#             */
/*   Updated: 2023/09/06 17:20:45 by tmina-ni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"
#include <stdio.h>

/*
FILE *popen(const char *command, const char *type);
opens a process stream with fork and invokes the shell cmd

char *fgets (char *str, int n, FILE *stream);
reads from stream and stores in str, until n-1 chars, \n, EOF
*/

//char	*exec_command(char *cmd)
//{
//	FILE	*pipe;
//
//	pipe = popen(cmd, 'r');
//	pclose(pipe);
//}

int	main(void)
{
	FILE	*stream;
	char	*cmd_output;

	stream = popen("ls", "r");
	fgets(cmd_output, 1000, stream);
	printf("cmd output: %s\n", cmd_output);
	pclose(stream);
	return (0);
}

//char	*read_output(char *outfile)
