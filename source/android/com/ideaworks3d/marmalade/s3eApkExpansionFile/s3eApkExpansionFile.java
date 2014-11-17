/* 
 * s3eApkExpansionFile
 * Created by Papirosnik on 17-Nov-2014 
 */
package com.ideaworks3d.marmalade.s3eApkExpansionFile;
import com.ideaworks3d.marmalade.s3eApkExpansionFile.R;
import com.ideaworks3d.marmalade.LoaderAPI;
import com.ideaworks3d.marmalade.LoaderActivity;
import android.content.Context;
import android.os.Environment;
import android.os.Messenger;
import android.content.Intent;
import android.app.PendingIntent;
import android.app.Activity;
import android.util.Log; 
import java.io.File;
import android.content.pm.PackageManager.NameNotFoundException;
import com.google.android.vending.expansion.downloader.Helpers;
import com.google.android.vending.expansion.downloader.DownloadProgressInfo;
import com.google.android.vending.expansion.downloader.DownloaderClientMarshaller;
import com.google.android.vending.expansion.downloader.DownloaderServiceMarshaller;
import com.google.android.vending.expansion.downloader.IDownloaderService;
import com.google.android.vending.expansion.downloader.IDownloaderClient;
import com.google.android.vending.expansion.downloader.IStub;

class s3eApkExpansionFile implements IDownloaderClient
{
	//java versions of the native structs which are passed to the native functions
	public class DownloadingInfo
	{
		public long mOverallTotal;
		public long mOverallProgress;
		public long mTimeRemaining; // time remaining
		public float mCurrentSpeed; // speed in KB/S	
	}
	
	public static native void native_DownloadingComplete();
	public static native void native_DownloadingFailed(int err);
	public static native void native_DownloadingUpdate(DownloadingInfo sysInfo);
	
	public static String apiKey;	
	public static int mainFileVerCode;	
	public static int patchFileVerCode;	

	
	private static final String LOG_TAG = "s3eApkExpansionFile";
    private IDownloaderService mRemoteService;
    private IStub mDownloaderClientStub; 		

    public int s3eApkExpansionFileInitialize(String apiKey, String mainFileVersion, String patchFileVersion)
    {
		s3eApkExpansionFile.apiKey = apiKey;
		s3eApkExpansionFile.mainFileVerCode = (mainFileVersion == null)? 0 : Integer.parseInt(mainFileVersion.replace(".", ""));
		Log.d(LOG_TAG, "s3eApkExpansionFileInitialize: mainFileVerCode = " + mainFileVerCode);
		s3eApkExpansionFile.patchFileVerCode = (patchFileVersion == null)? 0 : Integer.parseInt(patchFileVersion.replace(".", ""));
        return 0;
    }
	
	public String s3eApkExpansionFileGetPathToMainObb()
	{
		String fileName = Helpers.getExpansionAPKFileName(getApplicationContext(), true, mainFileVerCode);
		return Environment.getExternalStorageDirectory().toString() + "/Android/obb/" +				
				getApplicationContext().getPackageName().toString() + "/" + 
				fileName;				
	}	
	
	public String s3eApkExpansionFileGetPathToPatchObb()
	{
		String fileName = Helpers.getExpansionAPKFileName(getApplicationContext(), false, patchFileVerCode);
		return Environment.getExternalStorageDirectory().toString() + "/Android/obb/" +				
				getApplicationContext().getPackageName().toString() + "/" + 
				fileName;				
	}		
	
    public boolean s3eApkExpansionFileNeedDownloadMainObb()
    {
		boolean b = new File(s3eApkExpansionFileGetPathToMainObb()).isFile();
		return !b;
    }
	
    public boolean s3eApkExpansionFileNeedDownloadPatchObb()
    {
		boolean b = new File(s3eApkExpansionFileGetPathToPatchObb()).isFile();
		return !b;
    }	
	

	public void s3eApkExpansionFileStartDownloading()
	{
	    try
		{
			Intent launchIntent = getActivity().getIntent();
            Intent intentToLaunchThisActivityFromNotification = new Intent(getActivity(), getActivity().getClass());
            intentToLaunchThisActivityFromNotification.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK | Intent.FLAG_ACTIVITY_CLEAR_TOP);
            intentToLaunchThisActivityFromNotification.setAction(launchIntent.getAction());
			
			if (launchIntent.getCategories() != null)
				for (String category : launchIntent.getCategories())
					intentToLaunchThisActivityFromNotification.addCategory(category);
			
			PendingIntent pendingIntent = PendingIntent.getActivity(
                        getActivity(),
                        0, intentToLaunchThisActivityFromNotification,
                        PendingIntent.FLAG_UPDATE_CURRENT);					
						
			// Request to start the download
            int startResult = DownloaderClientMarshaller.startDownloadServiceIfRequired(
						getApplicationContext(),
                        pendingIntent,
						MyDownloaderService.class);
		
			if (startResult != DownloaderClientMarshaller.NO_DOWNLOAD_REQUIRED)
			{
				mDownloaderClientStub = DownloaderClientMarshaller.CreateStub(this, MyDownloaderService.class);
				if (null != mDownloaderClientStub)
				{
					mDownloaderClientStub.connect(getContext());
				}
			}
		}
		catch (NameNotFoundException e)
		{
            Log.e(LOG_TAG, "Cannot find own package! MAYDAY!");
            e.printStackTrace();
        }
	}
	
	@Override
	public void onDownloadStateChanged(int newState)
	{
        switch (newState)
		{
            case IDownloaderClient.STATE_IDLE:
				Log.d(LOG_TAG, "IDownloaderClient.STATE_IDLE");
                break;
				
            case IDownloaderClient.STATE_CONNECTING:
				Log.d(LOG_TAG, "IDownloaderClient.STATE_CONNECTING");
				break;
				
            case IDownloaderClient.STATE_FETCHING_URL:
				Log.d(LOG_TAG, "IDownloaderClient.STATE_FETCHING_URL");
                break;

            case IDownloaderClient.STATE_DOWNLOADING:
				Log.d(LOG_TAG, "IDownloaderClient.STATE_DOWNLOADING");
                break;

            case IDownloaderClient.STATE_FAILED_CANCELED:
            case IDownloaderClient.STATE_FAILED:
            case IDownloaderClient.STATE_FAILED_FETCHING_URL:
            case IDownloaderClient.STATE_FAILED_UNLICENSED:
				Log.d(LOG_TAG, "IDownloaderClient failed: " + newState);
				s3eApkExpansionFile.native_DownloadingFailed(newState);
                break;
				
            case IDownloaderClient.STATE_PAUSED_NEED_CELLULAR_PERMISSION:
				Log.d(LOG_TAG, "IDownloaderClient.STATE_PAUSED_NEED_CELLULAR_PERMISSION");
				break;
				
            case IDownloaderClient.STATE_PAUSED_WIFI_DISABLED_NEED_CELLULAR_PERMISSION:
				Log.d(LOG_TAG, "IDownloaderClient.STATE_PAUSED_WIFI_DISABLED_NEED_CELLULAR_PERMISSION");
                break;

            case IDownloaderClient.STATE_PAUSED_BY_REQUEST:
				Log.d(LOG_TAG, "IDownloaderClient.STATE_PAUSED_BY_REQUEST");
                break;

			case IDownloaderClient.STATE_PAUSED_ROAMING:
				Log.d(LOG_TAG, "IDownloaderClient.STATE_PAUSED_ROAMING");
				break;
				
            case IDownloaderClient.STATE_PAUSED_SDCARD_UNAVAILABLE:
				Log.d(LOG_TAG, "IDownloaderClient.STATE_PAUSED_SDCARD_UNAVAILABLE");
                break;
				
            case IDownloaderClient.STATE_COMPLETED:
				Log.d(LOG_TAG, "IDownloaderClient.STATE_COMPLETED");
				s3eApkExpansionFile.native_DownloadingComplete();
                break;
				
            default:
				Log.d(LOG_TAG, "IDownloaderClient: Unknown state");
				break;
		}
	}
	
    @Override
    public void onServiceConnected(Messenger m)
	{
        mRemoteService = DownloaderServiceMarshaller.CreateProxy(m);
        mRemoteService.onClientUpdated(mDownloaderClientStub.getMessenger());
    }	
	
    @Override
    public void onDownloadProgress(DownloadProgressInfo progress)
	{
		DownloadingInfo p = new DownloadingInfo();
		p.mOverallTotal = progress.mOverallTotal;
		p.mOverallProgress = progress.mOverallProgress;
		p.mTimeRemaining = progress.mTimeRemaining;
		p.mCurrentSpeed = progress.mCurrentSpeed;
		s3eApkExpansionFile.native_DownloadingUpdate(p);
    }
	
	// auxiliary functions
    private Context getContext()
    {
        return (Context)LoaderActivity.m_Activity;
    }

    private Activity getActivity()
    {
        return LoaderActivity.m_Activity;
    }
    
    private Context getApplicationContext()
    {
        return LoaderActivity.m_Activity.getApplicationContext();
    }	
}
