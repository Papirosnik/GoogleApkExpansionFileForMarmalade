Last changes by spothieux:

Removed R.java and added the call by the marmalade function

Improved the failed function in order to add the notification Text inside the struct of the callback.
It allows you to draw the text inside the application.

Added StopDownload function. If you close the application during a download, the app will not crash if you call this function before you exit your app.

Added support for several languages (fr, de, es, pt, it).



==================================
GoogleApkExpansionFileForMarmalade
==================================

Google Apk Expansion Files (Main and Patch obb) with Marmalade SDK

The extension uses both Google Play Downloader Library and Google Play License Library
with all their advantages (network connection checking, background downloading, pause/resume downloading and so on...) 

Using:

Add the following lines into your main project mkb file :

	subprojects
	{
		s3eApkExpansionFile
	}

And write something like this somewhere in your code:

	#include <s3eApkExpansionFile.h>
	
	...
	
	int32 downloadingObbCompletedCallback(void *sysInfo, void* userInfo)
	{
		IwTrace(APKEXPANSIONFILE, ("Downloading successfully completed"));	
		// unregister all callbacks
		s3eApkExpansionFileUnRegister(S3E_APKEXPANSIONFILE_DOWNLOADING_COMPLETED, downloadingObbCompletedCallback);
		s3eApkExpansionFileUnRegister(S3E_APKEXPANSIONFILE_DOWNLOADING_FAILED, downloadingObbFailedCallback);
		s3eApkExpansionFileUnRegister(S3E_APKEXPANSIONFILE_DOWNLOADING_UPDATE, downloadingObbUpdateCallback);	
		return S3E_RESULT_SUCCESS;
	}

	int32 downloadingObbFailedCallback(void *sysInfo, void* userInfo)
	{
		FailedInfo* failedInfo = (FailedInfo*)userInfo;
		IwTrace(APKEXPANSIONFILE, ("Downloading failed with error '%s' (code: %d)", failedInfo->mTextError, failedInfo->mNewState));
		return S3E_RESULT_SUCCESS;
	}

	int32 downloadingObbUpdateCallback(void *sysInfo, void* userInfo)
	{
		s3eApkExpansionFileDownloadingInfo* info =  (s3eApkExpansionFileDownloadingInfo*)sysInfo;
		uint percent = (uint)(100.0f * info->mOverallProgress / info->mOverallTotal);
		IwTrace(APKEXPANSIONFILE, ("Downloading: %u%% (remaining: %u)", percent, info->mTimeRemaining));		
		return S3E_RESULT_SUCCESS;
	}


	void main()
	{
		...
		if (s3eApkExpansionFileAvailable())
		{
			s3eApkExpansionFileInitialize(
				"*** INSERT_YOUR_BASE64_ENCODED_RSA_PUBLIC_KEY_HERE ***", // Obtain it from the Google Developer Console
				"1.0.2", // Give apk version for the main expansion file
				NULL);	 // Don't use the patch expansion file
				
			// check if the main obb file exists			
			// probably, you should perform some extra validation here (check its size, crc or something else)
			if (s3eApkExpansionFileNeedDownloadMainObb())
			{
				// register necessary callbacks
				s3eApkExpansionFileRegister(S3E_APKEXPANSIONFILE_DOWNLOADING_COMPLETED, downloadingObbCompletedCallback, NULL);
				s3eApkExpansionFileRegister(S3E_APKEXPANSIONFILE_DOWNLOADING_FAILED, downloadingObbFailedCallback, NULL);
				s3eApkExpansionFileRegister(S3E_APKEXPANSIONFILE_DOWNLOADING_UPDATE, downloadingObbUpdateCallback, NULL);

				// and start downloading obb file(s)
				s3eApkExpansionFileStartDownloading();				
			}
		}
		...
	}
	
That all.

Enjoy;


