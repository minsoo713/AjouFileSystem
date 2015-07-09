#define FUSE_USE_VERSION 29

#include <fuse.h>


static struct fuse_operations operations ={
	// set operations
	// look Function Specification in shared folder
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

