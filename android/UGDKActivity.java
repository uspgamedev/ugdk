package ${UGDK_ANDROIDPROJECT_BUNDLE_NAME};

import org.libsdl.app.SDLActivity;

public class UGDKActivity extends SDLActivity {
    static {
        System.loadLibrary("lua");
        System.loadLibrary("freetype");
        System.loadLibrary("freetype-gl");
        System.loadLibrary("gnustl_shared");
        
        System.loadLibrary("libjson");
        try {
            System.loadLibrary("${UGDK_ANDROIDPROJECT_UGDK_NAME}");
        } catch(java.lang.UnsatisfiedLinkError err) {
            System.loadLibrary("${UGDK_ANDROIDPROJECT_UGDK_NAME_DEBUG}");
        }
        
        System.loadLibrary("${UGDK_ANDROIDPROJECT_TARGET_NAME}");
    }
};