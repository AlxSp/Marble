

#ifdef SANDBOX_ASSETS_PATH
    #define ASSETS_DIR_NAME "/assets/" 
    #define ASSETS(path) SANDBOX_ASSETS_PATH ASSETS_DIR_NAME path
#else 
    #define ASSETS(path)
#endif