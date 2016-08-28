#include <util.h>

static HCRYPTPROV prov = 0;

int random()
{
    if (prov == 0)
        if (!CryptAcquireContext(&prov, NULL, NULL, PROV_RSA_FULL, CRYPT_SILENT | CRYPT_VERIFYCONTEXT))
            ExitProcess(1);

    int out;
    CryptGenRandom(prov, sizeof(out), (BYTE *)(&out));
    return out & 0x7fffffff;
}

void reverse_string_w(LPWSTR str)
{
    int len = lstrlenW(str);

    if (len <= 1)
        return;

    WCHAR c;
    int i, j;
    for (i = 0, j = len - 1; i < j; i++, j--) {
        c = str[i];
        str[i] = str[j];
        str[j] = c;
    }

    // Fix Newlines
    for (i = 0; i < len - 1; i++) {
        if (str[i] == L'\n' && str[i + 1] == L'\r') {
            str[i] = L'\r';
            str[i + 1] = L'\n';
        }
    }
}