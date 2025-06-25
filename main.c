#define _GNU_SOURCE

#define YEARS_SEC   31536000
#define MONTHS_SEC  2592000
#define DAYS_SEC    86400
#define HOURS_SEC   3600
#define MINUTES_SEC 60
#define STR_LEN     10

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <sys/stat.h>
#include <getopt.h>
#include <unistd.h>

struct Data {
    int number;
    char *text[STR_LEN];
};

int strcmp_three(char str[], char first[], char second[], char three[]) {
    if (strcmp(str, first) == 0 || strcmp(str, second) == 0 || strcmp(str, three) == 0) {
        return 1;
    } else {
        return 0;
    }
}

char process_unique_arg(char optarg[]) {
    if (strcmp_three(optarg, "years", "year", "y") == 1) {
        return 0;
    } else if (strcmp_three(optarg, "months", "month", "m") == 1) {
        return 1;
    } else if (strcmp_three(optarg, "days", "day", "d") == 1) {
        return 2;
    } else if (strcmp_three(optarg, "hours", "hour", "h") == 1) {
        return  3;
    } else if (strcmp_three(optarg, "minutes", "minute", "min") == 1) {
        return 4;
    } else {
        return -1;
    }
}

int main(int argc, char *argv[]) {
    int rest, years;
    struct statx stats;
    time_t current, birth, diff;
    char content[5][25];
    char totalContent[200] = "";
    unsigned char months, weeks, days, hours, minutes, i = 0, isLong = 0, isUnique = 0;
    char idUnique = -1;

    struct option options[] = {
        { "long", no_argument, NULL, 'l' },
        { "lang", required_argument, NULL, 's' },
        { "unique", required_argument, NULL, 'u' }
    };

    // Strings
    char yearStr[STR_LEN] = "years";
    char monthStr[STR_LEN] = "months";
    char dayStr[STR_LEN] = "days";
    char hourStr[STR_LEN] = "hours";
    char minuteStr[STR_LEN] = "minutes";
    char andStr[STR_LEN] = " and ";

    int opt;
    while ((opt = getopt_long(argc, argv, "ls:u:", options, NULL)) != -1) {
        switch (opt) {
            case 'l':
                isLong = 1;
                break;
            case 's':
                if (strcmp(optarg, "fr") == 0 || strcmp(optarg, "french") == 0) {
                    strcpy(yearStr, "ans");
                    strcpy(monthStr, "mois");
                    strcpy(dayStr, "jours");
                    strcpy(hourStr, "heures");
                    strcpy(andStr, " et ");
                }
                break;
            case 'u':
                isUnique = 1;
                idUnique = process_unique_arg(optarg);

                break;
            default:
                return 1;
                break;
        }
    }

    statx(AT_FDCWD, "/", AT_SYMLINK_NOFOLLOW, STATX_BTIME, &stats);

    current = time(NULL);
    birth = (time_t) stats.stx_btime.tv_sec;
    diff = difftime(current, birth);

    if (isUnique == 1) {
        char suffix[STR_LEN];
        int divider;
        switch (idUnique) {
            case 0:
                strcpy(suffix, yearStr);
                divider = YEARS_SEC;
                break;
            case 1:
                strcpy(suffix, monthStr);
                divider = MONTHS_SEC;
                break;
            case 2:
                strcpy(suffix, dayStr);
                divider = DAYS_SEC;
                break;
            case 3:
                strcpy(suffix, hourStr);
                divider = HOURS_SEC;
                break;
            case 4:
                strcpy(suffix, minuteStr);
                divider = MINUTES_SEC;
                break;
            default:
                printf("The unique parameter is invalid.\n");
                return 0;
        }

        int value = diff / divider;

        printf("%d %s\n", value, suffix);
        return 0;
    }

    years = diff / YEARS_SEC;
    rest = diff % YEARS_SEC;

    months = rest / MONTHS_SEC;
    rest = rest % MONTHS_SEC;

    days = rest / DAYS_SEC;
    rest = rest % DAYS_SEC;

    hours = rest / HOURS_SEC;
    rest = rest % HOURS_SEC;

    minutes = rest / MINUTES_SEC;

    struct Data data[5] = { {years, yearStr}, {months, monthStr}, {days, dayStr}, {hours, hourStr}, {minutes, minuteStr} };

    for (int j = 0; j < 5; j++) {
        if (data[j].number != 0 || isLong == 1) {
            snprintf(content[i], sizeof(content[i]), "%d %s", data[j].number, *data[j].text);
            i++;
        }
    }

    for (int j = 0; j < i; j++) {
        if ((i - 1) == j) strcat(totalContent, andStr);
        else if (j != 0) strcat(totalContent, ", ");

        strcat(totalContent, content[j]);
    }

    printf("%s\n", totalContent);

    return 0;
}
