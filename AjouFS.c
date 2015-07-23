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
		return -errno;
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
static int afs_unlink(const char *path)
{        
	int res;        
	res = unlink(path);

	if (res == -1)
	{
		return -errno;
	}
	return 0;

}
static int afs_rmdir(const char *path)
{        
	int res;        
	res = rmdir(path);
        
	if (res == -1)
	{
		return -errno;
	}
	return 0;
}
static int afs_symlink(const char *from, const char *to)
{        
	int res;        
	res = symlink(from, to);
        
	if (res == -1)
	{
		return -errno;
	}
	return 0;
}
struct afs_dirp 
{        
	DIR *dp;        
	struct dirent *entry;        
	off_t offset;
};
static int afs_opendir(const char *path, struct fuse_file_info *fi)
{        
	int res;        
	struct afs_dirp *d = malloc(sizeof(struct afs_dirp));

	if (d == NULL)
	{
		return -ENOMEM;
	}   
	d->dp = opendir(path);
        
	if (d->dp == NULL) 
	{                
		res = -errno;               
		free(d);                
		return res;        
	}

	d->offset = 0;        
	d->entry = NULL;        
	fi->fh = (unsigned long) d;        
	return 0;
}
static inline struct afs_dirp *get_dirp(struct fuse_file_info *fi)
{        
	return (struct afs_dirp *) (uintptr_t) fi->fh;
}
static int afs_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi)
{        
	struct afs_dirp *d = get_dirp(fi);        
	(void) path;        
	if (offset != d->offset) 
	{                
		seekdir(d->dp, offset);                
		d->entry = NULL;                
		d->offset = offset;        
	}       
	while (1) 
	{                
		struct stat st;                
		off_t nextoff;                
		if (!d->entry) 
		{                        
			d->entry = readdir(d->dp);
                        
			if (!d->entry)
			{              
				break;
			}   
		}                
		memset(&st, 0, sizeof(st));                
		st.st_ino = d->entry->d_ino;                
		st.st_mode = d->entry->d_type << 12;                
		nextoff = telldir(d->dp);                
		if (filler(buf, d->entry->d_name, &st, nextoff))
		{                
			break;
		} 
		d->entry = NULL;                
		d->offset = nextoff;
        }        
	return 0;
}
static int afs_releasedir(const char *path, struct fuse_file_info *fi)
{        
	struct afs_dirp *d = get_dirp(fi);        
	(void) path;        
	closedir(d->dp);        
	free(d);        
	return 0;
}
static int afs_rename(const char *from, const char *to)
{        
	int res;        
	res = rename(from, to);   

	if (res == -1)
	{       
		return -errno;
	}   
	return 0;
}
static int afs_link(const char *from, const char *to)
{        
	int res;        
	res = link(from, to);
        
	if (res == -1)
	{            
		return -errno;
	}   
	return 0;
}
static int afs_chmod(const char *path, mode_t mode)
{        
	int res;        
	res = chmod(path, mode);
        
	if (res == -1)
	{
		return -errno;
	}   
	return 0;
}
static int afs_chown(const char *path, uid_t uid, gid_t gid)
{        
	int res;        
	res = lchown(path, uid, gid);
        
	if (res == -1)		
	{                
		return -errno;		
	}        
	return 0;
}
static int afs_truncate(const char *path, off_t size)
{        
	int res;        
	res = truncate(path, size);        
	if (res == -1)
	{           
		return -errno;
	}   
	return 0;
}
static int afs_ftruncate(const char *path, off_t size, struct fuse_file_info *fi)
{        
	int res;        
	(void) path;        
	res = ftruncate(fi->fh, size);
        
	if (res == -1)
	{          
		return -errno;
	}   
	return 0;
}
#ifdef HAVE_UTIMENSAT
static int afs_utimens(const char *path, const struct timespec ts[2])
{        
	int res;        
	/* don't use utime/utimes since they follow symlinks */        
	res = utimensat(0, path, ts, AT_SYMLINK_NOFOLLOW);
        
	if (res == -1)
	{          
		return -errno;
	}   
	return 0;
}
#endif
static int afs_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{        
	int res;        
	(void) path;        
	res = pread(fi->fh, buf, size, offset);
        
	if (res == -1)
	{
		res = -errno;
	}   
	return res;
}
static int afs_read_buf(const char *path, struct fuse_bufvec **bufp, size_t size, off_t offset, struct fuse_file_info *fi)
{        
	struct fuse_bufvec *src;        
	(void) path;        
	src = malloc(sizeof(struct fuse_bufvec));
        
	if (src == NULL)
	{    
		return -ENOMEM;
	}   
	*src = FUSE_BUFVEC_INIT(size);        
	src->buf[0].flags = FUSE_BUF_IS_FD | FUSE_BUF_FD_SEEK;        
	src->buf[0].fd = fi->fh;        
	src->buf[0].pos = offset;        
	*bufp = src;        
	return 0;
}
static int afs_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{        
	int res;        
	(void) path;        
	res = pwrite(fi->fh, buf, size, offset);
        
	if (res == -1)
	{  
		res = -errno;
	}   
	return res;
}
static int afs_write_buf(const char *path, struct fuse_bufvec *buf, off_t offset, struct fuse_file_info *fi)
{        
	struct fuse_bufvec dst = FUSE_BUFVEC_INIT(fuse_buf_size(buf));        
	(void) path;     
	dst.buf[0].flags = FUSE_BUF_IS_FD | FUSE_BUF_FD_SEEK;    
	dst.buf[0].fd = fi->fh;    
	dst.buf[0].pos = offset;    
	return fuse_buf_copy(&dst, buf, FUSE_BUF_SPLICE_NONBLOCK);
}
static int afs_statfs(const char *path, struct statvfs *stbuf)
{        
	int res;        
	res = statvfs(path, stbuf);        
	if (res == -1)
	{    
		return -errno;
	}   
	return 0;
}
static int afs_flush(const char *path, struct fuse_file_info *fi)
{        
	int res;        
	(void) path;        
	/* This is called from every close on an open file, so call the           
    close on the underlying filesystem.  But since flush may be           
	called multiple times for an open file, this must not really
    close the file.  This is important if used on a network
    filesystem like NFS which flush the data/metadata on close() */        
	res = close(dup(fi->fh));

	if (res == -1)
	{        
		return -errno;
	}   
	return 0;
}
static int afs_release(const char *path, struct fuse_file_info *fi)
{       
	(void) path;   
	close(fi->fh);    
	return 0;
}
static int afs_fsync(const char *path, int isdatasync, struct fuse_file_info *fi)
{        
	int res;        
	(void) path;
#ifndef HAVE_FDATASYNC        
	(void) isdatasync;
#else        
	if (isdatasync)
	{   
		res = fdatasync(fi->fh);
	}   
	else
	{
#endif                
		res = fsync(fi->fh);
	}		
	if (res == -1)
	{                
		return -errno;
	}        
	return 0;
}
#ifdef HAVE_POSIX_FALLOCATE
static int afs_fallocate(const char *path, int mode, off_t offset, off_t length, struct fuse_file_info *fi)
{        
	(void) path;
    
	if (mode)
	{
		return -EOPNOTSUPP;
	}   
	return -posix_fallocate(fi->fh, offset, length);
}
#endif
#ifdef HAVE_SETXATTR
/* xattr operations are optional and can safely be left unimplemented */
static int afs_setxattr(const char *path, const char *name, const char *value, size_t size, int flags)
{        
	int res = lsetxattr(path, name, value, size, flags);
        
	if (res == -1)
	{
		return -errno;
	}   
	return 0;
}
static int afs_getxattr(const char *path, const char *name, char *value, size_t size)
{       
	int res = lgetxattr(path, name, value, size);
        
	if (res == -1)
	{            
		return -errno;
	}   
	return res;
}
static int afs_listxattr(const char *path, char *list, size_t size)
{        
	int res = llistxattr(path, list, size);
        
	if (res == -1)
	{     
		return -errno;
	}   
	return res;
}
static int afs_removexattr(const char *path, const char *name)
{        
	int res = lremovexattr(path, name);
    
	if (res == -1)
	{
		return -errno;
	}   
	return 0;
}
#endif /* HAVE_SETXATTR */
static int afs_lock(const char *path, struct fuse_file_info *fi, int cmd, struct flock *lock)
{        
	(void) path;        
	return ulockmgr_op(fi->fh, cmd, lock, &fi->lock_owner,sizeof(fi->lock_owner));
}
static int afs_flock(const char *path, struct fuse_file_info *fi, int op)
{       
	int res;        
	(void) path;        
	res = flock(fi->fh, op);
        
	if (res == -1)
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

