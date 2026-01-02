#pragma once

/**
 * Executes a shell command, or logs it if dry run mode is enabled.
 *
 * In dry run mode, commands are written to "dry-run.log" instead of
 * being executed, and the function returns 0 (success).
 *
 * @param cmd The shell command to execute.
 *
 * @return The return value of system(), or 0 in dry run mode.
 */
int run_cmd(const char *cmd);

/**
 * Executes a command inside a chroot environment at /mnt.
 *
 * Forks a child process that enters the chroot before executing the command.
 * In dry run mode, logs the command instead of executing.
 *
 * @param cmd_path The command to execute.
 * @param argv     NULL-terminated argument array (argv[0] should be cmd_path).
 *
 * @return 0 on success, non-zero on failure.
 */
int run_cmd_chroot(const char *cmd_path, char *const argv[]);

/**
 * Closes the dry run log file if open.
 *
 * Should be called at the end of installation to flush and close
 * the log file properly.
 */
void close_dry_run_log(void);
