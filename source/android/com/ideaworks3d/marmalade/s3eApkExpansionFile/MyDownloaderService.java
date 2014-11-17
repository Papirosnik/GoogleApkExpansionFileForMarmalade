package com.ideaworks3d.marmalade.s3eApkExpansionFile;
import com.google.android.vending.expansion.downloader.impl.DownloaderService;
import android.util.Log; 

public class MyDownloaderService extends DownloaderService
{
    // You should modify this salt
    public static final byte[] SALT = new byte[] { -12, -1, 54, 98, -100, -12, 43, 2, -8, -4, 9, 5, -106, -107, -33, 45, -1, 84, 2, 43 };
	
    @Override
    public String getPublicKey()
	{
        return s3eApkExpansionFile.apiKey;
    }

    @Override
    public byte[] getSALT()
	{
        return SALT;
    }

    @Override
    public String getAlarmReceiverClassName()
	{
        return MyAlarmReceiver.class.getName();
    }
}