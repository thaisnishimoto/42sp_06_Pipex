/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tester_pipex.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmina-ni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 12:00:56 by tmina-ni          #+#    #+#             */
/*   Updated: 2023/08/31 22:52:44 by tmina-ni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minunit.h"
#include "../pipex.h"

MU_TEST(funtion_should_read_one_line)
{
	int	fd;
	int	i;
	char	*line = NULL;
	char    *expected[] = {"1st line.\n", "2nd line.\n"};

	printf("\n-----------------\n");
	printf(" TEST 1: 2 lines");
	printf("\n-----------------\n");
	i = 0;
	fd = open("./Files/file.txt", O_RDWR);
	while ((line = get_next_line(fd)) != NULL)
	{
		printf("RETURNED LINE: %s", line);
		mu_assert_string_eq(expected[i++], line);
		printf("\n");
		free(line);
	}
	if (close(fd) == 0)
		printf("%s\n", "File closed");
}

MU_TEST(int_no_nl)
{
	int	fd;
	char	*line = NULL;
	char	*expected = {"01234567890123456789012345678901234567890"};

	fd = open("./Files/41_no_nl.txt", O_RDWR | O_CREAT, 0777);
	if (fd < 0)
		printf("%s\n", "File did not open");
	printf("\n-----------------\n");
	printf(" TEST 2: int no nl");
	printf("\n-----------------\n");
	while ((line = get_next_line(fd)) != NULL)
	{
		printf("RETURNED LINE: %s\n\n", line);
		mu_assert_string_eq(expected, line);
		free(line);	
	}
	if (close(fd) == 0)
		printf("%s\n", "File closed");
}

MU_TEST(just_nl)
{
	int	fd;
	int	i;
	char	*line;
	char	*expected[] = {"\n", "\n", "\n", "\n", "\n"};

	fd = open("./Files/just_nl.txt", O_RDWR | O_CREAT, 0777);
	if (fd < 0)
		printf("%s\n", "File did not open");
	printf("\n-----------------\n");
	printf(" TEST 3: just nl");
	printf("\n-----------------\n");
	i = 0;
	while ((line = get_next_line(fd)) != NULL)
	{
		printf("RETURNED LINE: %s", line);
		mu_assert_string_eq(expected[i++], line);
		free(line);	
	}
	if (close(fd) == 0)
		printf("%s\n", "File closed");
}

MU_TEST(multiple_fd)
{
	int	fd3;
	int	fd4;
	int	fd5;
	int	i;
	char	*line = NULL;
	char    *expected[] = {"1st line.\n", "3rd line.\n", "5th line.\n", "2nd line.\n", "4th line.\n", 	"6th line.\n", NULL};

	printf("\n-----------------\n");
	printf(" TEST 4: 3 files");
	printf("\n-----------------\n");
	i = 0;
	fd3 = open("./Files/file.txt", O_RDWR);
	fd4 = open("./Files/file2.txt", O_RDWR);
	fd5 = open("./Files/file3.txt", O_RDWR);
	while (i < 6)
	{
		line = get_next_line(fd3);
		printf("RETURNED LINE: %s", line);
		printf("\n");
		mu_assert_string_eq(expected[i++], line);
		free(line);
		line = get_next_line(fd4);
		printf("RETURNED LINE: %s", line);
		printf("\n");
		mu_assert_string_eq(expected[i++], line);
		free(line);
		line = get_next_line(fd5);
		printf("RETURNED LINE: %s", line);
		printf("\n");
		mu_assert_string_eq(expected[i++], line);
		free(line);
	}
	close(fd3);
	close(fd4);
	close(fd5);
}

MU_TEST(fd_0)
{
	int	fd;
	char	*line = NULL;

	fd = 0;
	printf("\n-----------------\n");
	printf(" TEST 5: fd 0");
	printf("\n-----------------\n");
	while ((line = get_next_line(fd)) != NULL)
	{
		printf("RETURNED LINE: %s", line);
		free(line);	
	}
}

MU_TEST_SUITE(test_suite)
{
//	MU_RUN_TEST(file_manipulation);
	MU_RUN_TEST(funtion_should_read_one_line);
	MU_RUN_TEST(int_no_nl);
	MU_RUN_TEST(just_nl);
	MU_RUN_TEST(multiple_fd);
	MU_RUN_TEST(fd_0);
}

int	main(void)
{
	MU_RUN_SUITE(test_suite);
	MU_REPORT();
	return MU_EXIT_CODE;
}
