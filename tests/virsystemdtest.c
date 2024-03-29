/*
 * Copyright (C) 2013 Red Hat, Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Author: Daniel P. Berrange <berrange@redhat.com>
 */

#include <config.h>

#include <stdlib.h>

#include "virsystemd.h"
#include "virlog.h"
#include "testutils.h"

#define VIR_FROM_THIS VIR_FROM_NONE

static int testCreateContainer(const void *opaque ATTRIBUTE_UNUSED)
{
    unsigned char uuid[VIR_UUID_BUFLEN] = {
        1, 1, 1, 1,
        2, 2, 2, 2,
        3, 3, 3, 3,
        4, 4, 4, 4
    };
    if (virSystemdCreateMachine("demo",
                                "lxc",
                                true,
                                uuid,
                                "/proc/123/root",
                                123,
                                true,
                                "highpriority.slice") < 0) {
        fprintf(stderr, "%s", "Failed to create LXC machine\n");
        return -1;
    }

    return 0;
}

static int testCreateMachine(const void *opaque ATTRIBUTE_UNUSED)
{
    unsigned char uuid[VIR_UUID_BUFLEN] = {
        1, 1, 1, 1,
        2, 2, 2, 2,
        3, 3, 3, 3,
        4, 4, 4, 4
    };
    if (virSystemdCreateMachine("demo",
                                "qemu",
                                false,
                                uuid,
                                NULL,
                                123,
                                false,
                                NULL) < 0) {
        fprintf(stderr, "%s", "Failed to create KVM machine\n");
        return -1;
    }

    return 0;
}

static int testCreateNoSystemd(const void *opaque ATTRIBUTE_UNUSED)
{
    unsigned char uuid[VIR_UUID_BUFLEN] = {
        1, 1, 1, 1,
        2, 2, 2, 2,
        3, 3, 3, 3,
        4, 4, 4, 4
    };
    int rv;

    setenv("FAIL_NO_SERVICE", "1", 1);

    if ((rv = virSystemdCreateMachine("demo",
                                      "qemu",
                                      true,
                                      uuid,
                                      NULL,
                                      123,
                                      false,
                                      NULL)) == 0) {
        unsetenv("FAIL_NO_SERVICE");
        fprintf(stderr, "%s", "Unexpected create machine success\n");
        return -1;
    }
    unsetenv("FAIL_NO_SERVICE");

    if (rv != -2) {
        fprintf(stderr, "%s", "Unexpected create machine error\n");
        return -1;
    }

    return 0;
}

static int testCreateBadSystemd(const void *opaque ATTRIBUTE_UNUSED)
{
    unsigned char uuid[VIR_UUID_BUFLEN] = {
        1, 1, 1, 1,
        2, 2, 2, 2,
        3, 3, 3, 3,
        4, 4, 4, 4
    };
    int rv;

    setenv("FAIL_BAD_SERVICE", "1", 1);

    if ((rv = virSystemdCreateMachine("demo",
                                      "qemu",
                                      true,
                                      uuid,
                                      NULL,
                                      123,
                                      false,
                                      NULL)) == 0) {
        unsetenv("FAIL_BAD_SERVICE");
        fprintf(stderr, "%s", "Unexpected create machine success\n");
        return -1;
    }
    unsetenv("FAIL_BAD_SERVICE");

    if (rv != -1) {
        fprintf(stderr, "%s", "Unexpected create machine error\n");
        return -1;
    }

    return 0;
}


struct testScopeData {
    const char *name;
    const char *partition;
    const char *expected;
};

static int
testScopeName(const void *opaque)
{
    const struct testScopeData *data = opaque;
    int ret = -1;
    char *actual = NULL;

    if (!(actual = virSystemdMakeScopeName(data->name,
                                           "lxc",
                                           data->partition)))
        goto cleanup;

    if (STRNEQ(actual, data->expected)) {
        fprintf(stderr, "Expected '%s' but got '%s'\n",
                data->expected, actual);
        goto cleanup;
    }

    ret = 0;

 cleanup:
    VIR_FREE(actual);
    return ret;
}

static int
mymain(void)
{
    int ret = 0;

    if (virtTestRun("Test create container ", 1, testCreateContainer, NULL) < 0)
        ret = -1;
    if (virtTestRun("Test create machine ", 1, testCreateMachine, NULL) < 0)
        ret = -1;
    if (virtTestRun("Test create no systemd ", 1, testCreateNoSystemd, NULL) < 0)
        ret = -1;
    if (virtTestRun("Test create bad systemd ", 1, testCreateBadSystemd, NULL) < 0)
        ret = -1;

#define TEST_SCOPE(name, partition, unitname)                           \
    do {                                                                \
        struct testScopeData data = {                                   \
            name, partition, unitname                                   \
        };                                                              \
        if (virtTestRun("Test scopename", 1, testScopeName, &data) < 0) \
            ret = -1;                                                   \
    } while (0)

    TEST_SCOPE("demo", "/machine", "machine-lxc\\x2ddemo.scope");
    TEST_SCOPE("demo-name", "/machine", "machine-lxc\\x2ddemo\\x2dname.scope");
    TEST_SCOPE("demo!name", "/machine", "machine-lxc\\x2ddemo\\x21name.scope");
    TEST_SCOPE(".demo", "/machine", "machine-lxc\\x2d\\x2edemo.scope");
    TEST_SCOPE("demo", "/machine/eng-dept", "machine-eng\\x2ddept-lxc\\x2ddemo.scope");
    TEST_SCOPE("demo", "/machine/eng-dept/testing!stuff",
               "machine-eng\\x2ddept-testing\\x21stuff-lxc\\x2ddemo.scope");

    return ret==0 ? EXIT_SUCCESS : EXIT_FAILURE;
}

VIRT_TEST_MAIN_PRELOAD(mymain, abs_builddir "/.libs/virsystemdmock.so")
