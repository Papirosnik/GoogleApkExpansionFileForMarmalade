/*
java implementation of the s3eApkExpansionFile extension.

Add android-specific functionality here.

These functions are called via JNI from native code.
*/
/*
 * NOTE: This file was originally written by the extension builder, but will not
 * be overwritten (unless --force is specified) and is intended to be modified.
 */
import com.ideaworks3d.marmalade.LoaderAPI;
// Comment in the following line if you want to use ResourceUtility
// import com.ideaworks3d.marmalade.ResourceUtility;

class s3eApkExpansionFile
{
    public int s3eApkExpansionFileInitialize(String apiKey, String mainFileVersion, String patchFileVersion)
    {
        return 0;
    }
    public FIXME s3eApkExpansionFileGetPathToMainObb()
    {
        
    }
    public FIXME s3eApkExpansionFileGetPathToPatchObb()
    {
        
    }
    public boolean s3eApkExpansionFileNeedDownloadMainObb()
    {
        return false;
    }
    public boolean s3eApkExpansionFileNeedDownloadPatchObb()
    {
        return false;
    }
    public void s3eApkExpansionFileStartDownloading()
    {
        
    }
	public void s3eApkExpansionFileStopDownloading()
    {
        
    }
}
