#define FUSE_USE_VERSION 26

struct afs_pdata {
	char *hdddir;
	char *ssddir;
};

#include <fuse.h>

static int afs_getattr(const char *path, struct stat * stbuf)
{
	int res;
	res = lstat(path, stbuf);

	if(res = -1)
	{
		return errno;
	}
	return 0;
}
static int afs_fgetattr(const char *path, struct stat *stbuf, struct fuse_file_info *fi)
{        
	int res;        
	(void) path;        
	res = fstat(fi->fh, stbuf);
        
	if (res == -1)//err
	{
		return -errno;
	}
	return 0;
}
static int afs_access(const char *path, int mask)
{        
	int res;        
	res = access(path, mask);
        
	if (res == -1)//err
	{
		return -errno;
	}
	return 0;
}
static int afs_readlink(const char *path, char *buf, size_t size)
{        
	int res;        
	res = readlink(path, buf, size - 1);

	if (res == -1)//err
	{
		return -errno;
	}
	buf[res] = '\0';        
	return 0;
}
static int afs_mknod(const char *path, mode_t mode, dev_t rdev)
{        
	int res;

	if (S_ISFIFO(mode))
	{
		res = mkfifo(path, mode);
	}
	else
	{
		res = mknod(path, mode, rdev);
	}

	if (res == -1)//err
	{
		return -errno;
	}
	return 0;
}
static int afs_mkdir(const char *path, mode_t mode)
{        
	int res;        
	res = mkdir(path, mode);

	if (res == -1)//err
	{
		return -errno;
	}
	return 0;
}
void* afs_init(struct fuse_conn_info *conn){
	return ((struct afs_pdata*)fuse_get_context()->private_data);
}

void afs_destroy(void * pdata){
	// free(pdata);
}

int afs_create(const char * path, mode_t mode, struct fuse_file_info * fi){
	int res;
	res = open(path, fi->flags, mode);
	if (res < 0){
		//error;
		return res;
	}
	fi->fh = res;
	return 0;
}

int afs_open(const char * path, struct fuse_file_info * fi){
	int res;
	res = open(path, fi->flags);
	if(res < 0){
		//error;
		return res;
	}
	fi->fh = res;
	return 0;
}

int afs_read(const char * path, char * buff, size_t size, off_t offset, struct fuse_file_info * fi){
	
}

static struct fuse_operations operations = {
	.init = afs_init,
	.destroy = afs_destroy,

	.getattr = afs_getattr,
	.fgetattr = afs_fgetattr,

	.create = afs_create,
	.open = afs_open,
	.read = afs_read,
	.wirte = afs_wirte,

	.mkdir = afs_mkdir,
	.rmdir = afs_rmdir,
	.oepndir = afs_opendir,
	.readir = afs_readdir,
	.releasedir = afs_releasedir,
	.access = afs_access,

	.flush = afs_flush,
	.release = afs_releas,

	.unlink = afs_unlink,
	.fsync = afs_fsync
};

int main(int argc, char *argv[]){
	int ret_state;
	struct afs_pdata *afs_data;

	afs_data = malloc(sizeof(struct afs_pdata));
	if(afs_data == NULL){
		// error
		exit(-1);
	}

	afs_data->hdddir = realpath(argv[argc-3], NULL);
	afs_data->ssddir = realpath(argv[argc-2], NULL);
	argv[argc-3] = argv[argc-1];

	ret_state = fuse_main(argc-2, argv, operations, afs_data);
	// argc : argc
	// argv : argv
	// operations : struct fuse_operation
	// user_data : (void*)
	// set user_data to fuse_get_context()->private_data
	// cast & use.
	return ret_state;
}

