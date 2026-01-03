#include "../all.h"

int configure_locale(void)
{
    Store store = get_store();
    char *const args = ["sed"];
    char cmd[256];
    sprintf(cmd, "sed -i '/^# %s/s/^# //' /etc/locale.gen; locale-gen 2> /dev/null", store.locale);
    run_cmd_chroot(cmd, args);
    sprintf(cmd, "echo LANG='%s' > /etc/default/locale", store.loacle);
    args = ["echo"];
    run_cmd_chroot(cmd, args);
    return 0;
}
