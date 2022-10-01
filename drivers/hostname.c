#include "../headers.h"
#include "hostname.h"

char *hostname(void)
{

    struct utsname name;
    if (uname(&name) == -1)
        return NULL;
    struct passwd *pw = getpwuid(getuid());
    char *hostname = malloc(strlen(pw->pw_name) + strlen(name.nodename) + 1);
    strcpy(hostname, pw->pw_name);
    strcat(hostname, "@");
    strcat(hostname, name.nodename);


    return hostname;
}
