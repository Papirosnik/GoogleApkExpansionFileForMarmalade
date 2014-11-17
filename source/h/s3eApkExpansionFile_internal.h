/*
 * Internal header for the s3eApkExpansionFile extension.
 *
 * This file should be used for any common function definitions etc that need to
 * be shared between the platform-dependent and platform-indepdendent parts of
 * this extension.
 */

/*
 * NOTE: This file was originally written by the extension builder, but will not
 * be overwritten (unless --force is specified) and is intended to be modified.
 */


#ifndef S3EAPKEXPANSIONFILE_INTERNAL_H
#define S3EAPKEXPANSIONFILE_INTERNAL_H

#include "s3eTypes.h"
#include "s3eApkExpansionFile.h"
#include "s3eApkExpansionFile_autodefs.h"


/**
 * Initialise the extension.  This is called once then the extension is first
 * accessed by s3eregister.  If this function returns S3E_RESULT_ERROR the
 * extension will be reported as not-existing on the device.
 */
s3eResult s3eApkExpansionFileInit();

/**
 * Platform-specific initialisation, implemented on each platform
 */
s3eResult s3eApkExpansionFileInit_platform();

/**
 * Terminate the extension.  This is called once on shutdown, but only if the
 * extension was loader and Init() was successful.
 */
void s3eApkExpansionFileTerminate();

/**
 * Platform-specific termination, implemented on each platform
 */
void s3eApkExpansionFileTerminate_platform();
s3eResult s3eApkExpansionFileInitialize_platform(const char* apiKey, const char* mainFileVersion, const char* patchFileVersion);

const char* s3eApkExpansionFileGetPathToMainObb_platform();

const char* s3eApkExpansionFileGetPathToPatchObb_platform();

bool s3eApkExpansionFileNeedDownloadMainObb_platform();

bool s3eApkExpansionFileNeedDownloadPatchObb_platform();

void s3eApkExpansionFileStartDownloading_platform();


#endif /* !S3EAPKEXPANSIONFILE_INTERNAL_H */