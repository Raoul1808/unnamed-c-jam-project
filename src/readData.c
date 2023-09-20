#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

char *readData(char *file)
{
	int fd = -1;
	char *buff = NULL;
	char *dup = NULL;
	int buffLength;
	int i;
	char tmp = 0;
	int reat;

	fd = open(file, O_RDONLY);
	if (fd == -1)
		return NULL;
	buffLength = 500;
	buff = malloc(sizeof(char) * buffLength);
	if (buff == NULL)
	{
		close(fd);
		return NULL;
	}
	i = 0;
	reat = read(fd, &tmp, 1);

	while (reat > 0)
	{
		buff[i] = tmp;
		i++;
		if (i == buffLength-1)
		{
			buffLength += 500;
			buff = realloc(buff, buffLength);
			if (buff == NULL)
			{
				close(fd);
				return NULL;
			}
		}
		reat = read(fd, &tmp, 1);
	}
	if (reat >= 0)
	{
		buff[i] = '\0';
		dup = strdup(buff);
	}
	free(buff);
	close(fd);
	return dup;
}
