/*
Generic implementation of the s3eApkExpansionFile extension.
This file should perform any platform-indepedentent functionality
(e.g. error checking) before calling platform-dependent implementations.
*/

/*
 * NOTE: This file was originally written by the extension builder, but will not
 * be overwritten (unless --force is specified) and is intended to be modified.
 */


#include "s3eApkExpansionFile_internal.h"
s3eResult s3eApkExpansionFileInit()
{
    //Add any generic initialisation code here
    return s3eApkExpansionFileInit_platform();
}

void s3eApkExpansionFileTerminate()
{
    //Add any generic termination code here
    s3eApkExpansionFileTerminate_platform();
}

s3eResult s3eApkExpansionFileInitialize(const char* apiKey, const char* mainFileVersion, const char* patchFileVersion)
{
	return s3eApkExpansionFileInitialize_platform(apiKey, mainFileVersion, patchFileVersion);
}

const char* s3eApkExpansionFileGetPathToMainObb()
{
	return s3eApkExpansionFileGetPathToMainObb_platform();
}

const char* s3eApkExpansionFileGetPathToPatchObb()
{
	return s3eApkExpansionFileGetPathToPatchObb_platform();
}

bool s3eApkExpansionFileNeedDownloadMainObb()
{
	return s3eApkExpansionFileNeedDownloadMainObb_platform();
}

bool s3eApkExpansionFileNeedDownloadPatchObb()
{
	return s3eApkExpansionFileNeedDownloadPatchObb_platform();
}

void s3eApkExpansionFileStartDownloading()
{
	s3eApkExpansionFileStartDownloading_platform();
}

void s3eApkExpansionFileStopDownloading()
{
	s3eApkExpansionFileStopDownloading_platform();
}
