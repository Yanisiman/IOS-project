#include <sys/stat.h>

enum class_
{
    CLASS_OWNER,
    CLASS_GROUP,
    CLASS_OTHER
};

enum permission
{
    PERMISSION_READ,
    PERMISSION_WRITE,
    PERMISSION_EXECUTE
};

mode_t perm(enum class_ c, enum permission p);

mode_t mode_contains(mode_t mode, enum class_ c, enum permission p);

mode_t mode_add(mode_t mode, enum class_ c, enum permission p);

mode_t mode_rm(mode_t mode, enum class_ c, enum permission p);

int chmod_command(int argc, char **argv);

