#define FUSE_USE_VERSION 29

#include <fuse.h>


static struct fuse_operations operations = {
	.init = afs_init,
	.destroy = afs_destroy,

	.getattr = afs_getattr,
	.fgetattr = afs_fgetattr,

	.create = afs_create,
	.open = afs_open,
	.create = afs_create,
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

	ret_state = fuse_main(argc, argv, operations, NULL);
	// argc : argc
	// argv : argv
	// operations : struct fuse_operation
	// user_data : (void*)
	// set user_data to fuse_get_context()->private_data
	// cast & use.
	return ret_state;
}

