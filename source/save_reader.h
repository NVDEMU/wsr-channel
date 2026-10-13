#ifndef RESORT_RECORDS_SAVE_READER_H
#define RESORT_RECORDS_SAVE_READER_H

typedef enum {
    WSR_SAVE_PROBE_NOT_CHECKED = 0,
    WSR_SAVE_PROBE_FOUND_NTSC_U,
    WSR_SAVE_PROBE_FOUND_PAL,
    WSR_SAVE_PROBE_NOT_FOUND,
    WSR_SAVE_PROBE_ACCESS_ERROR
} wsr_save_probe_result_t;

/*
 * Probe known regional Sports2.dat paths using read-only ISFS access.
 * This checks accessibility only; it does not validate or decode file data.
 * path_out may be NULL. When a file is found, it receives a static path.
 */
wsr_save_probe_result_t wsr_probe_save(const char **path_out);

const char *wsr_save_probe_result_name(wsr_save_probe_result_t result);

#endif
