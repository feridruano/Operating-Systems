//////////////////////////////////////////////////////////////////////////
///
/// Copyright (c) 2020 Prof. AJ Bieszczad. All rights reserved.
///
//////////////////////////////////////////////////////////////////////////

#include "disk.h"

disk_t disk;

/***
 *
 * Verifies correctness of the logical block address, and then translates it to a cylinder-head-sector equivalent.
 *
 */
SIM_DEV_RET_CODE lba2chs(lba_t lba, chs_t *chs)
{
    if (lba >= MAX_LOGICAL_BLOCK)
        return SIM_DEV_ADDRESS_ERROR;

// todo: implement

    return SIM_DEV_SUCCESS;
}

/***
 *
 * Verifies correctness of the cylinder-head-sector address, and then translates it to a logical block address.
 *
 */
SIM_DEV_RET_CODE chs2lba(chs_t *chs, lba_t *lba)
{
// todo: implement

    return SIM_DEV_SUCCESS;
}
/***
 *
 * Verifies the parameters, then allocates space for the requested data (using
 * the caller-provided pointer buffer), copies the data from the disk to the buffer,
 * and appends '\0' to terminate the string.
 *
 */
SIM_DEV_RET_CODE readDisk(lba_t lba, unsigned int size, char **buffer)
{
// todo: verify parameters

    chs_t chs;

    *buffer = malloc(11*sizeof(char)); // todo: modify as required
    strcpy(*buffer, "CHANGE ME!");

    SIM_DEV_RET_CODE errCode = SIM_DEV_SUCCESS;

    // todo: implement

    return errCode;
}

/***
 *
 * An auxiliary function to fill a single disk block with '.'
 *
*/
SIM_DEV_RET_CODE clearBlock(lba_t lba)
{
    char *writeBuffer;
#ifdef __DEBUG_DISK
    writeBuffer = malloc(SECT_SIZE);
#else
    writeBuffer = kmalloc(SECT_SIZE, GFP_USER);
#endif
    if (writeBuffer == NULL)
        return SIM_DEV_SPACE_ERROR;

    for (int i = 0; i < SECT_SIZE; i++)
        writeBuffer[i] = '.';

    SIM_DEV_RET_CODE errCode = writeDisk(lba, writeBuffer);

#ifdef __DEBUG_DISK
    free(writeBuffer);
#else
    kfree(writeBuffer);
#endif

    return errCode;
}

/***
 *
 * Validates the parameters, and then writes the caller-provided data to the disk starting at the block pointed
 * to by the logical block address.
 *
 */
SIM_DEV_RET_CODE writeDisk(lba_t lba, char *buffer)
{
// todo: verify the parameters

    SIM_DEV_RET_CODE errCode = SIM_DEV_SUCCESS;

    chs_t chs;

// todo: implement

    return errCode;
}
