#ifndef LIBSTRATCOM_INCLUDE_GUARD_STRATCOM_H_
#define LIBSTRATCOM_INCLUDE_GUARD_STRATCOM_H_

#ifdef _WIN32
#   ifdef LIBSTRATCOM_EXPORT
#       define LIBSTRATCOM_API __declspec(dllexport)
#   else
#       define LIBSTRATCOM_API __declspec(dllimport)
#   endif
#else
#   define LIBSTRATCOM_API
#endif

#ifdef __cplusplus
extern "C" {
#endif

    void LIBSTRATCOM_API stratcom_init();

#ifdef __cplusplus
}
#endif

#endif
