/**
	@File:			fs.c
	@Author:		Ihsoh
	@Date:			2014-7-29
	@Description:
		文件系统的系统调用处理程序。
*/

#include "fs.h"
#include "../types.h"
#include "../sparams.h"
#include "../ifs1fs.h"
#include "../cmos.h"
#include "../ifs1blocks.h"
#include "../tasks.h"
#include "../console.h"
#include <ilib/string.h>

static BOOL lock = FALSE;

static
BOOL
lock_fs(void)
{
	if(lock)
		return FALSE;
	lock = TRUE;
	return TRUE;
}

void
system_call_fs_unlock_fs(void)
{
	lock = FALSE;
}

/**
	@Function:		check_priviledge
	@Access:		Private
	@Description:
		检查任务是否包含有文件指针的所有权。
	@Parameters:
		tid, int32, IN
			任务 ID。
		fptr, FILE *, IN
			文件指针。
	@Return:
		BOOL
			返回TRUE则成功，否则失败。		
*/
static
BOOL
check_priviledge(	IN int32 tid,
					IN FILE * fptr)
{
	struct Task * task = get_task_info_ptr(tid);
	if(task != NULL)
	{
		uint32 ui;
		for(ui = 0; 
				ui < MAX_TASK_OPENED_FILE_COUNT && task->opened_file_ptrs[ui] != fptr;
				ui++);
		if(ui < MAX_TASK_OPENED_FILE_COUNT)
			return TRUE;
	}
	return FALSE;
}

/**
	@Function:		system_call_fs
	@Access:		Public
	@Description:
		文件系统的系统调用处理程序。
	@Parameters:
		func, uint32, IN
			功能号。
		base, uint32, IN
			任务的基地址。该地址为物理地址。
		sparams, struct SParams *, IN OUT
			系统调用参数。
	@Return:	
*/
void
system_call_fs(	IN uint32 func,
				IN uint32 base,
				IN OUT struct SParams * sparams)
{
	switch(func)
	{
		//打开文件
		//
		//参数:
		//	Param0=文件名地址(相对于调用程序空间的偏移地址)
		//	Param1=打开模式
		//返回值:
		//	Param0=文件结构体地址(相对于内核空间的偏移地址)
		case SCALL_FOPEN:
		{
			int32 tid = sparams->tid;
			struct Task * task = get_task_info_ptr(tid);
			if(task != NULL) 
			{
				uint32 ui;
				for(ui = 0;
						ui < MAX_TASK_OPENED_FILE_COUNT && task->opened_file_ptrs[ui] != NULL;
						ui++);
				if(ui < MAX_TASK_OPENED_FILE_COUNT)
				{
					int8 * filename = NULL;
					filename = (int8 *)get_physical_address(tid, 
															VOID_PTR_SPARAM(sparams->param0));
					int32 mode = INT32_SPARAM(sparams->param1);
					FILE * fptr = fopen(filename, mode);
					task->opened_file_ptrs[ui] = fptr;
					sparams->param0 = SPARAM(fptr);
				}
				else
				{
					FILE * fptr = NULL;
					sparams->param0 = SPARAM(fptr);
				}
			}
			else
			{
				FILE * fptr = NULL;
				sparams->param0 = SPARAM(fptr);
			}
			break;
		}
		//关闭文件
		//
		//参数:
		//	Param0=文件结构体地址(相对于内核空间的偏移地址)
		//返回值:
		//	Param0=返回1则成功, 返回0则失败
		case SCALL_FCLOSE:
		{
			int32 tid = sparams->tid;
			struct Task * task = get_task_info_ptr(tid);
			FILE * fptr = (FILE *)(sparams->param0);
			if(task != NULL && fptr != NULL) 
			{
				uint32 ui;
				for(ui = 0;
						ui < MAX_TASK_OPENED_FILE_COUNT && task->opened_file_ptrs[ui] != fptr;
						ui++);
				if(ui < MAX_TASK_OPENED_FILE_COUNT)
				{
					task->opened_file_ptrs[ui] = NULL;
					BOOL r = fclose(fptr);
					sparams->param0 = SPARAM(r);
				}
				else
				{
					BOOL r = FALSE;
					sparams->param0 = SPARAM(r);
				}
			}
			else
			{
				BOOL r = FALSE;
				sparams->param0 = SPARAM(r);
			}
			break;
		}
		//写文件
		//
		//参数:
		//	Param0=文件结构体地址(相对于内核空间的偏移地址)
		//	Param1=缓冲区地址(相对于调用程序空间的偏移地址)
		//	Param2=写入长度
		//返回值:
		//	Param0=返回1则成功, 返回0则失败
		case SCALL_FWRITE:
		{
			int32 tid = sparams->tid;
			FILE * fptr = (FILE *)(sparams->param0);
			if(check_priviledge(tid, fptr))
			{
				uint8 * buffer = NULL;
				buffer = (uint8 *)get_physical_address(	tid, 
														VOID_PTR_SPARAM(sparams->param1));
				uint32 len = UINT32_SPARAM(sparams->param2);
				BOOL r = fwrite(fptr, buffer, len);
				sparams->param0 = SPARAM(r);
			}
			else
			{
				BOOL r = FALSE;
				sparams->param0 = SPARAM(r);
			}
			break;
		}
		//读文件
		//
		//参数:
		//	Param0=文件结构体地址(相对于内核空间的偏移地址)
		//	Param1=缓冲区地址(相对于调用程序空间的偏移地址)
		//	Param2=读入长度
		//返回值:
		//	Param0=真正读入的长度
		case SCALL_FREAD:
		{
			int32 tid = sparams->tid;
			FILE * fptr = (FILE *)(sparams->param0);
			if(check_priviledge(tid, fptr))
			{
				uint8 * buffer = NULL;
				buffer = (uint8 *)get_physical_address(	tid, 
														VOID_PTR_SPARAM(sparams->param1));
				uint32 len = UINT32_SPARAM(sparams->param2);
				uint32 real_len = fread(fptr, buffer, len);
				sparams->param0 = SPARAM(real_len);
			}
			else
			{
				uint32 len = 0;
				sparams->param0 = SPARAM(len);	
			}
			break;
		}
		//追加文件
		//
		//参数:
		//	Param0=文件结构体地址(相对于内核空间的偏移地址)
		//	Param1=缓冲区地址(相对于调用程序空间的偏移地址)
		//	Param2=追加长度
		//返回值:
		//	Param0=返回1则成功, 返回0则失败
		case SCALL_FAPPEND:
		{
			int32 tid = sparams->tid;
			FILE * fptr = (FILE *)(sparams->param0);
			if(check_priviledge(tid, fptr))
			{
				uint8 * buffer = NULL;
				buffer = (uint8 *)get_physical_address(	tid, 
														VOID_PTR_SPARAM(sparams->param1));
				uint32 len = UINT32_SPARAM(sparams->param2);
				BOOL r = fappend(fptr, buffer, len);
				sparams->param0 = SPARAM(r);
			}
			else
			{
				BOOL r = FALSE;
				sparams->param0 = SPARAM(r);
			}
			break;
		}
		//重置文件读取指针
		//
		//参数:
		//	Param0=文件结构体地址(相对于内核空间的偏移地址)
		case SCALL_FRESET:
		{
			int32 tid = sparams->tid;
			FILE * fptr = (FILE *)(sparams->param0);
			if(check_priviledge(tid, fptr))
				freset(fptr);
			break;
		}
		//确认文件或文件夹是否存在
		//
		//参数:
		//	Param0=路径缓冲区地址(相对于调用程序空间的偏移地址)
		//返回值:
		//	Param0=1则存在, 0则不存在
		case SCALL_EXISTS_DF:
		{
			int8 * path = NULL;
			path = (int8 *)get_physical_address(sparams->tid,
												VOID_PTR_SPARAM(sparams->param0));
			BOOL r = exists_df(path);
			sparams->param0 = SPARAM(r);
			break;
		}
		//创建文件夹
		//
		//参数:
		//	Param0=路径缓冲区地址(相对于调用程序空间的偏移地址)
		//	Param1=新文件夹名称缓冲区地址(相对于调用程序空间的偏移地址)
		//返回值:
		//	Param0=1则成功, 0则失败
		case SCALL_CREATE_DIR:
		{
			int8 * path = NULL;
			path = (int8 *)get_physical_address(sparams->tid,
												VOID_PTR_SPARAM(sparams->param0));
			int8 * name = NULL;
			name = (int8 *)get_physical_address(sparams->tid,
												VOID_PTR_SPARAM(sparams->param1));
			BOOL r = create_dir(path, name);
			sparams->param0 = SPARAM(r);
			break;
		}
		//创建文件
		//
		//参数:
		//	Param0=路径缓冲区地址(相对于调用程序空间的偏移地址)
		//	Param1=新文件名称缓冲区地址(相对于调用程序空间的偏移地址)
		//返回值:
		//	Param0=1则成功, 0则失败
		case SCALL_CREATE_FILE:
		{
			int8 * path = NULL;
			path = (int8 *)get_physical_address(sparams->tid,
												VOID_PTR_SPARAM(sparams->param0));
			int8 * name = NULL;
			name = (int8 *)get_physical_address(sparams->tid,
												VOID_PTR_SPARAM(sparams->param1));
			BOOL r = create_file(path, name);
			sparams->param0 = SPARAM(r);
			break;
		}
		//删除文件夹
		//
		//参数:
		//	Param0=路径缓冲区地址(相对于调用程序空间的偏移地址)
		//返回值:
		//	Param0=1则成功, 0则失败
		case SCALL_DEL_DIR:
		{
			int8 * path = NULL;
			path = (int8 *)get_physical_address(sparams->tid,
												VOID_PTR_SPARAM(sparams->param0));
			BOOL r = del_dir(path);
			sparams->param0 = SPARAM(r);
			break;
		}
		//删除文件
		//
		//参数:
		//	Param0=路径缓冲区地址(相对于调用程序空间的偏移地址)
		//返回值:
		//	Param0=1则成功, 0则失败
		case SCALL_DEL_FILE:
		{
			int8 * path = NULL;
			path = (int8 *)get_physical_address(sparams->tid,
												VOID_PTR_SPARAM(sparams->param0));
			BOOL r = del_file(path);
			sparams->param0 = SPARAM(r);
			break;
		}
		//删除文件夹, 包括文件夹内的内容
		//
		//参数:
		//	Param0=路径缓冲区地址(相对于调用程序空间的偏移地址)
		//返回值:
		//	Param0=1则成功, 0则失败
		case SCALL_DEL_DIRS:
		{
			int8 * path = NULL;
			path = (int8 *)get_physical_address(sparams->tid,
												VOID_PTR_SPARAM(sparams->param0));
			BOOL r = del_dirs(path);
			sparams->param0 = SPARAM(r);
			break;
		}
		//重命名文件夹
		//
		//参数:
		//	Param0=路径缓冲区地址(相对于调用程序空间的偏移地址)
		//	Param1=文件夹新名称缓冲区地址(相对于调用程序空间的偏移地址)
		//返回值:
		//	Param0=1则成功, 0则失败
		case SCALL_RENAME_DIR:
		{
			int8 * path = NULL;
			path = (int8 *)get_physical_address(sparams->tid,
												VOID_PTR_SPARAM(sparams->param0));
			int8 * new_name = NULL;
			new_name = (int8 *)get_physical_address(sparams->tid,
													VOID_PTR_SPARAM(sparams->param1));
			BOOL r = rename_dir(path, new_name);
			sparams->param0 = SPARAM(r);
			break;
		}
		//重命名文件
		//
		//参数:
		//	Param0=路径缓冲区地址(相对于调用程序空间的偏移地址)
		//	Param1=文件新名称缓冲区地址(相对于调用程序空间的偏移地址)
		//返回值:
		//	Param0=1则成功, 0则失败
		case SCALL_RENAME_FILE:
		{
			int8 * path = NULL;
			path = (int8 *)get_physical_address(sparams->tid,
												VOID_PTR_SPARAM(sparams->param0));
			int8 * new_name = NULL;
			new_name = (int8 *)get_physical_address(sparams->tid,
													VOID_PTR_SPARAM(sparams->param1));
			BOOL r = rename_file(path, new_name);
			sparams->param0 = SPARAM(r);
			break;
		}
		//文件长度
		//
		//参数:
		//	Param0=文件结构体地址(相对于内核空间的偏移地址)
		//返回值:
		//	Param0=文件长度
		case SCALL_FLEN:
		{
			int32 tid = sparams->tid;
			FILE * fptr = (FILE *)(sparams->param0);
			if(check_priviledge(tid, fptr))
				sparams->param0 = SPARAM(flen(fptr));
			else
			{
				uint32 len = 0;
				sparams->param0 = SPARAM(len);
			}
			break;
		}
		//获取文件创建日期时间
		//
		//参数:
		//	Param0=文件结构体地址(相对于内核空间的偏移地址)
		//	Param1=CMOSDateTime结构体地址(相对于调用程序空间的偏移地址)
		case SCALL_FCREATE_DT:
		{
			int32 tid = sparams->tid;
			FILE * fptr = (FILE *)(sparams->param0);
			if(check_priviledge(tid, fptr))
			{
				struct CMOSDateTime * dt = NULL;
				dt = (struct CMOSDateTime *)get_physical_address(	tid, 
																	VOID_PTR_SPARAM(sparams->param1));
				memcpy(dt, &(fptr->file_block->create), sizeof(struct CMOSDateTime));
			}
			break;
		}
		//获取文件修改日期时间
		//
		//参数:
		//	Param0=文件结构体地址(相对于内核空间的偏移地址)
		//	Param1=CMOSDateTime结构体地址(相对于调用程序空间的偏移地址)
		case SCALL_FCHANGE_DT:
		{
			int32 tid = sparams->tid;
			FILE * fptr = (FILE *)(sparams->param0);
			if(check_priviledge(tid, fptr))
			{
				struct CMOSDateTime * dt = NULL;
				dt = (struct CMOSDateTime *)get_physical_address(	tid, 
																	VOID_PTR_SPARAM(sparams->param1));
				memcpy(dt, &(fptr->file_block->change), sizeof(struct CMOSDateTime));
			}
			break;
		}
		//获取指定目录下的文件以及文件夹的总数
		//
		//参数:
		//	Param0=路径字符串地址(相对于调用程序空间的偏移地址)
		//返回值:
		//	Param0=指定目录下的文件以及文件夹的总数
		case SCALL_DF_COUNT:
		{
			int8 * path = NULL;
			path = (int8 *)get_physical_address(sparams->tid,
												VOID_PTR_SPARAM(sparams->param0));
			int32 count = df_count(path);
			sparams->param0 = SPARAM(count);
			break;
		}
		//获取指定目录下的文件Block以及文件夹Block
		//
		//参数:
		//	Param0=路径字符串地址(相对于调用程序空间的偏移地址)
		//	Param1=用于保存所有Blocks的缓冲区地址(相对于调用程序空间的偏移地址)
		//	Param2=储存块集合的缓冲区最大能储存块的个数。
		//返回值:
		//	Param0=指定目录下的文件以及文件夹的总数
		case SCALL_DF:
		{
			int8 * path = NULL;
			path = (int8 *)get_physical_address(sparams->tid,
												VOID_PTR_SPARAM(sparams->param0));
			struct RawBlock * raw_blocks = NULL;
			raw_blocks = (struct RawBlock *)get_physical_address(	sparams->tid,
																	VOID_PTR_SPARAM(sparams->param1));
			uint32 max = 0;
			max = UINT32_SPARAM(sparams->param2);
			int32 count = df(path, raw_blocks, max);
			sparams->param0 = SPARAM(count);
			break;
		}
		//根据当前目录，产生一个路径的绝对路径。
		//
		//参数:
		//	Param0=路径(相对于调用程序空间的偏移地址)
		//	Param1=绝对路径(相对于调用程序空间的偏移地址)
		//返回值:
		//	Param0=1则成功, 0则失败
		case SCALL_FIX_PATH:
		{
			struct Task * task = get_task_info_ptr(sparams->tid);
			int8 * path = NULL;
			path = (int8 *)get_physical_address(sparams->tid,
												VOID_PTR_SPARAM(sparams->param0));
			int8 * new_path = NULL;
			new_path = (int8 *)get_physical_address(sparams->tid,
													VOID_PTR_SPARAM(sparams->param1));
			BOOL r = fix_path(path, task->working_dir, new_path);
			sparams->param0 = SPARAM(r);
			break;
		}
		//锁住文件系统。
		//
		//返回值:
		//	Param0=如果成功锁住文件系统则返回TRUE，否则返回FALSE。
		case SCALL_LOCK_FS:
		{
			int32 tid = sparams->tid;
			BOOL r = lock_fs();
			if(r)
			{
				struct Task * task = get_task_info_ptr(tid);
				if(task == NULL)
				{
					system_call_fs_unlock_fs();
					r = FALSE;
				}
				else
					task->fs_lock = TRUE;
			}
			sparams->param0 = SPARAM(r);
			break;
		}
		//解锁文件系统。
		//
		case SCALL_UNLOCK_FS:
		{
			system_call_fs_unlock_fs();
			break;
		}
		//获取文件系统的锁的状态。
		//
		//返回值:
		//	Param0=返回TRUE则文件系统被锁住，否则没有被锁住。
		case SCALL_FS_LOCK_STATE:
		{
			sparams->param0 = SPARAM(lock);
			break;
		}
		//检测是否已经到达文件尾
		//
		//参数:
		//	Param0=文件结构体地址(相对于内核空间的偏移地址)
		//返回值:
		//	Param0=返回1则到达文件尾，否则未到达文件尾。
		case SCALL_FEOF:
		{
			int32 tid = sparams->tid;
			FILE * fptr = (FILE *)(sparams->param0);
			BOOL r = FALSE;
			if(check_priviledge(tid, fptr))
				r = feof(fptr);
			sparams->param0 = SPARAM(r);
			break;
		}
	}
}