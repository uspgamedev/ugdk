
#ifdef ANDROID

/*******************************************************************************
                 Functions called by JNI
*******************************************************************************/
#include <jni.h>

// The main function defined by client code.
int main(int argc, char** argv);

/* Called before SDL_main() to initialize JNI bindings in SDL library */
extern "C" void SDL_Android_Init(JNIEnv* env, jclass cls);

/* SDL_main declarations. */
extern "C" void SDL_SetMainReady(void);
extern "C" char * SDL_strdup(const char *str);

/* Start up the SDL app */
extern "C" void Java_org_libsdl_app_SDLActivity_nativeInit(JNIEnv* env, jclass cls, jobject obj)
{
    /* This interface could expand with ABI negotiation, calbacks, etc. */
    SDL_Android_Init(env, cls);

    SDL_SetMainReady();

    /* Run the application code! */
    char *argv[2];
    argv[0] = SDL_strdup("UGDK_app");
    argv[1] = nullptr;
    
    /* int status = */ main(1, argv);

    /* Do not issue an exit or the whole application will terminate instead of just the SDL thread */
    /* exit(status); */
}

#endif /* ANDROID */
