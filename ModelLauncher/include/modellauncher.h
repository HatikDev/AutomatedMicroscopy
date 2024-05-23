#ifndef LOADER_F_H
#define LOADER_F_H

extern "C"
{
    namespace loader // TODO: should we have loader?
    {
        //typedef size_t Err;
        //typedef const char* Path;
        //typedef size_t Size;
        //typedef size_t Index;

        //typedef struct {
        //    size_t version;
        //    size_t itemSize; // in bytes
        //    size_t itemCount;
        //    void* item;
        //} Batch;

//#define OK 0

#ifdef DLL_EXPORTS
#define LOADERAPI __declspec(dllexport)
#else
#define LOADERAPI __declspec(dllimport)
#endif

        LOADERAPI void __cdecl loadModel(const char* path);
        LOADERAPI void __cdecl predict(const char* input, const char* output);
        LOADERAPI void __cdecl unloadModel();
    }
}
#endif // LOADER_F_H
