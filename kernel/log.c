/**
	@File:			log.c
	@Author:		Ihsoh
	@Date:			2015-2-18
	@Description:
		日志。
*/

#include "log.h"
#include "types.h"
#include "ifs1fs.h"
#include "cmos.h"
#include "memory.h"
#include "system.h"

static int8 * log_buffer = NULL;

void
init_log(void)
{
	log_buffer = alloc_memory(LOG_BUFFER_SIZE);
	if(log_buffer == NULL)
		return;
	log_buffer[0] = '\0';
}

void
free_log(void)
{
	if(log_buffer == NULL)
		return;
	write_log_to_disk();
	free_memory(log_buffer);
}

void
write_log_to_disk(void)
{
	if(log_buffer == NULL)
		return;
	FILE * fptr = fopen(SYSTEM_LOG_FILE, FILE_MODE_APPEND);
	if(fptr == NULL)
		return;
	fappend(fptr, log_buffer, strlen(log_buffer));
	fclose(fptr);
}

void
clear_log(void)
{
	log_buffer[0] = '\0';
}

void
log(IN const int8 * type,
	IN const int8 * text)
{
	if(log_buffer == NULL)
		return;
	int8 buffer[1024];
	struct CMOSDateTime dt;
	get_cmos_date_time(&dt);
	sprintf_s(	buffer,
				1024,
				"[%s] %d-%d-%d %d:%d:%d %s\n",
				type,
				dt.year, dt.month, dt.day, 
				dt.hour, dt.minute, dt.second,
				text);
	if(strlen(log_buffer) + strlen(buffer) + 1 > LOG_BUFFER_SIZE)
	{
		write_log_to_disk();
		clear_log();
		strcpy(log_buffer, buffer);
	}
	else
		strcat(log_buffer, buffer);
}

int8 *
get_log(void)
{
	return log_buffer;
}