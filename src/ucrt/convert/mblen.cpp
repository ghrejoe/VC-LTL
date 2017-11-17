//
// mblen.cpp
//
//      Copyright (c) Microsoft Corporation. All rights reserved.
//
// The mblen() and _mblen_l() functions, which return the number of bytes
// contained in a multibyte character.
//
#include <corecrt_internal.h>
#include <locale.h>


// Computes the number of bytes contained in a multibyte character.  If the string
// is null, zero is returned to indicate that we support only state-independent
// character encodings.  If the next max_count bytes of the string are not a valid
// multibyte character, -1 is returned.  Otherwise, the number of bytes that
// compose the next multibyte character are returned.
#ifdef _ATL_XP_TARGETING
extern "C" int __cdecl _mblen_l(
    char const* const string,
    size_t      const max_count,
    _locale_t   const locale
    )
{
    if (!string || *string == '\0' || max_count == 0)
        return 0;

    //_LocaleUpdate locale_update(locale);

    _ASSERTE(
        locale_update.GetLocaleT()->locinfo->_public._locale_mb_cur_max == 1 ||
        locale_update.GetLocaleT()->locinfo->_public._locale_mb_cur_max == 2);

    if (_isleadbyte_l(static_cast<unsigned char>(*string), locale))
    {
		unsigned int _locale_lc_codepage;
		int			 _locale_mb_cur_max;
		if (locale)
		{
			_locale_lc_codepage = locale->locinfo->_locale_lc_codepage;
			_locale_mb_cur_max = locale->locinfo->_locale_mb_cur_max;

		}
		else
		{
			_locale_lc_codepage = ___lc_codepage_func();
			_locale_mb_cur_max = ___mb_cur_max_func();
		}

        // Multi-byte character; verify that it is valid:
        if (_locale_mb_cur_max <= 1)
            return -1;

        if (max_count > INT_MAX || static_cast<int>(max_count) < _locale_mb_cur_max)
            return -1;

        int const status = MultiByteToWideChar(
            _locale_lc_codepage,
            MB_PRECOMPOSED | MB_ERR_INVALID_CHARS,
            string,
            _locale_mb_cur_max,
            nullptr,
            0);

        if (status == 0)
            return -1;

        return _locale_mb_cur_max;
    }
    else
    {
        // Single byte character; verify that it is valid:
        int const status = MultiByteToWideChar(
			locale ? locale->locinfo->_locale_lc_codepage : ___lc_codepage_func(),
            MB_PRECOMPOSED | MB_ERR_INVALID_CHARS,
            string,
            1,
            nullptr,
            0);

        if (status == 0)
            return -1;

        return sizeof(char);
    }
}
#endif


//extern "C" int __cdecl mblen(
//    char const* const string,
//    size_t      const max_count
//    )
//{
//    if (!__acrt_locale_changed())
//    {
//        return _mblen_l(string, max_count, &__acrt_initial_locale_pointers);
//    }
//    else
//    {
//        return _mblen_l(string, max_count, nullptr);
//    }
//}
