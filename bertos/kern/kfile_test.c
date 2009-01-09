/**
 * \file
 * <!--
 * This file is part of BeRTOS.
 *
 * Bertos is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * As a special exception, you may use this file as part of a free software
 * library without restriction.  Specifically, if other files instantiate
 * templates or use macros or inline functions from this file, or you compile
 * this file and link it with other files to produce an executable, this
 * file does not by itself cause the resulting executable to be covered by
 * the GNU General Public License.  This exception does not however
 * invalidate any other reasons why the executable file might be covered by
 * the GNU General Public License.
 *
 * Copyright 2007 Develer S.r.l. (http://www.develer.com/)
 *
 * -->
 *
 * \brief Test suite for virtual KFile I/O interface.
 *
 * This module implements a test for some generic I/O interfaces for kfile.
 *
 * \version $Id$
 * \author Francesco Sacchi <batt@develer.com>
 * \author Daniele Basile <asterix@develer.com>
 */


#include "kfile.h"

#include "cfg/cfg_kfile.h"
#include <cfg/debug.h>
#include <cfg/test.h>
#include <cfg/module.h>

// Define logging setting (for cfg/log.h module).
#define LOG_LEVEL   KFILE_LOG_LEVEL
#define LOG_FORMAT  KFILE_LOG_FORMAT
#include <cfg/log.h>

#include <mware/formatwr.h>

#include <string.h>

MOD_DEFINE(kfile_test);

#define BUF_TEST_LEN     3209

typedef uint8_t fake_t;
fake_t test_buf[BUF_TEST_LEN];
fake_t test_buf_save[BUF_TEST_LEN];
fake_t test_disk[BUF_TEST_LEN];

KFile fd;

static int fake_close(KFile *fd)
{
	(void)fd;
	return 0;
}

static size_t fake_read(KFile *fd, void *buf, size_t size)
{
	fake_t *dest = (fake_t *)buf;
	size_t rd_len;

	rd_len = MIN((kfile_off_t)size, fd->size - fd->seek_pos);

	memcpy(dest, test_disk, size);
	fd->seek_pos += rd_len;

	LOG_INFO("Read: real[%ld] expected[%ld]\n", rd_len, size);

	return rd_len;
}

static size_t fake_write(KFile *fd, const void *buf, size_t size)
{
	fake_t *src = (fake_t *)buf;
	size_t wr_len;

	wr_len = MIN((kfile_off_t)size, fd->size - fd->seek_pos);

	memcpy(test_disk, src, wr_len);
	fd->seek_pos += wr_len;

	LOG_INFO("Write: real[%ld] expected[%ld]\n", wr_len, size);

	return wr_len;
}

int fake_flush(KFile *fd)
{
	(void)fd;

	return 0;
}

void fake_kfileInit(void)
{
	// Setup data flash programming functions.
	fd.reopen = kfile_genericReopen;
	fd.close = fake_close;
	fd.read = fake_read;
	fd.write = fake_write;
	fd.seek = kfile_genericSeek;
	fd.flush = fake_flush;

	fd.seek_pos = 0;
	fd.size = BUF_TEST_LEN;

}

static void init_testBuf(void)
{
	#include <stdlib.h>

	kprintf("Init fake buffer..\n");
	for (int i = 0; i < BUF_TEST_LEN; i++)
	{
		test_disk[i] = random();
		kprintf("%d ", test_disk[i]);
	}
	kprintf("\nend\n");

	memset(test_buf, 0, sizeof(test_buf));
	memset(test_buf_save, 0, sizeof(test_buf_save));
}

/**
 * KFile read/write subtest.
 * Try to write/read in the same \a f file location \a size bytes.
 * \return true if all is ok, false otherwise
 * \note Restore file position at exit (if no error)
 * \note Test buffer \a buf must be filled with
 * the following statement:
 * <pre>
 * buf[i] = i & 0xff
 * </pre>
 */
static bool kfile_rwTest(KFile *f, uint8_t *buf, size_t size)
{
	/*
	 * Write test buffer
	 */
	if (kfile_write(f, buf, size) != size)
		return false;

	kfile_seek(f, -(kfile_off_t)size, KSM_SEEK_CUR);

	/*
	 * Reset test buffer
	 */
	memset(buf, 0, size);

	/*
	 * Read file in test buffer
	 */
	if (kfile_read(f, buf, size) != size)
		return false;

	kfile_seek(f, -(kfile_off_t)size, KSM_SEEK_CUR);

	/*
	 * Check test result
	 */
	for (size_t i = 0; i < size; i++)
		if (buf[i] != (i & 0xff))
			return false;

	return true;
}

/**
 * KFile read/write test.
 * This function write and read \a test_buf long \a size
 * on \a fd handler.
 * \a save_buf can be NULL or a buffer where to save previous file content.
 */
int kfile_testRunGeneric(KFile *fd, uint8_t *test_buf, uint8_t *save_buf, size_t size)
{

	/*
	 * Part of test buf size that you would write.
	 * This var is used in test 3 to check kfile_write
	 * when writing beyond filesize limit.
	 */
	kfile_off_t len = size / 2;


	/* Fill test buffer */
	for (size_t i = 0; i < size; i++)
		test_buf[i] = (i & 0xff);

	/*
	 * If necessary, user can save content,
	 * for later restore.
	 */
	if (save_buf)
	{
		LOG_INFO("Saved content..form [%d] to [%ld]\n", fd->seek_pos, fd->seek_pos + size);
		kfile_read(fd, save_buf, size);
	}

	/* TEST 1 BEGIN. */
	LOG_INFO("Test 1: write from pos 0 to [%ld]\n", (long)size);

	/*
	 * Seek to addr 0.
	 */
	if (kfile_seek(fd, 0, KSM_SEEK_SET) != 0)
		goto kfile_test_end;

	/*
	 * Test read/write to address 0..size
	 */
	if (!kfile_rwTest(fd, test_buf, size))
		goto kfile_test_end;

	LOG_INFO("Test 1: ok!\n");

	/*
	 * Restore previous read content.
	 */
	if (save_buf)
	{
		kfile_seek(fd, 0, KSM_SEEK_SET);

		if (kfile_write(fd, save_buf, size) != size)
			goto kfile_test_end;

		LOG_INFO("Restore content..form [%d] to [%ld]\n", fd->seek_pos, fd->seek_pos + size);
	}
	/* TEST 1 END. */

	/* TEST 2 BEGIN. */
	LOG_INFO("Test 2: write from pos [%d] to [%ld]\n", fd->size/2 , fd->size/2 + size);

	/*
	 * Go to half test size.
	 */
	kfile_seek(fd, (fd->size / 2), KSM_SEEK_SET);

	/*
	 * If necessary, user can save content
	 * for later restore.
	 */
	if (save_buf)
	{
		kfile_read(fd, save_buf, size);
		kfile_seek(fd, -(kfile_off_t)size, KSM_SEEK_CUR);
		LOG_INFO("Saved content..form [%d] to [%ld]\n", fd->seek_pos, fd->seek_pos + size);
	}

	/*
	 * Test read/write to address filesize/2 ... filesize/2 + size
	 */
	if (!kfile_rwTest(fd, test_buf, size))
		goto kfile_test_end;

	LOG_INFO("Test 2: ok!\n");

	/*
	 * Restore previous content.
	 */
	if (save_buf)
	{
		kfile_seek(fd, -(kfile_off_t)size, KSM_SEEK_CUR);

		if (kfile_write(fd, save_buf, size) != size)
			goto kfile_test_end;

		LOG_INFO("Restore content..form [%d] to [%ld]\n", fd->seek_pos, fd->seek_pos + size);
	}

	/* TEST 2 END. */

	/* TEST 3 BEGIN. */
	LOG_INFO("Test 3: write outside of fd->size limit [%d]\n", fd->size);
	LOG_INFO("This test should FAIL!, you must see an assertion fail message.\n");

	/*
	 * Go to the Flash end
	 */
	kfile_seek(fd, -len, KSM_SEEK_END);

	/*
	 * If necessary, user can save content,
	 * for later restore.
	 */
	if (save_buf)
	{
		kfile_read(fd, save_buf, len);
		kfile_seek(fd, -len, KSM_SEEK_CUR);
		LOG_INFO("Saved content..form [%d] to [%d]\n", fd->seek_pos, fd->seek_pos + len);
	}

	/*
	 * Test read/write to address (filesize - size) ... filesize
	 */
	if (kfile_rwTest(fd, test_buf, size))
		goto kfile_test_end;

	kprintf("Test 3: ok!\n");

	/*
	 * Restore previous read content
	 */
	if (save_buf)
	{
		kfile_seek(fd, -len, KSM_SEEK_END);

		if ((kfile_off_t)kfile_write(fd, save_buf, len) != len)
			goto kfile_test_end;

		LOG_INFO("Restore content..form [%d] to [%d]\n", fd->seek_pos, fd->seek_pos + len);
	}

	/* TEST 3 END. */

	kfile_close(fd);
	return 0;

kfile_test_end:
	kfile_close(fd);
	LOG_ERR("One kfile_test failed!\n");
	return EOF;
}




/**
 * Setup all needed for kfile test
 */
int kfile_testSetup(void)
{
        MOD_INIT(kfile_test);
        LOG_INFO("Mod init..ok\n");

		fake_kfileInit();
		init_testBuf();

        return 0;
}

int kfile_testRun(void)
{
	return kfile_testRunGeneric(&fd, test_buf, test_buf_save, BUF_TEST_LEN);
}

/**
 * End a dataflash Test.
 * (Unused)
 */
int kfile_testTearDown(void)
{
	return 0;
}

TEST_MAIN(kfile);
