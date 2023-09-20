#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

char *readData(char *file)
{
	int fd=-1;
	char *buff = NULL;
	char *dup = NULL;
	int buffLength;
	int i;
	char tmp = 0;
	int reat;

	fd = open(file, O_RDONLY);
	if (fd == -1)
		goto F_READDATA_END;
	buffLength = 500;
	buff = malloc(sizeof(char) * buffLength);
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
				goto F_READDATA_END;
		}
		reat = read(fd, &tmp, 1);
	}
	if (reat < 0)
		goto F_READDATA_END;
	buff[i] = '\0';
	dup = strdup(buff);
	F_READDATA_END:
	if (buff != NULL)
		free(buff);
	if (fd != -1)
		close(fd);
	return dup;
}
