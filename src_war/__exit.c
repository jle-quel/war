#include <war.h>

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

void __exit(struct s_host *host, struct s_keychain *keychain)
{
//	decrypt_right(keychain, (char *)injection, (void *)__exit - (void *)injection);

	(void)keychain;
	munmap(host->header, host->filesize);

	exit(0);
}
